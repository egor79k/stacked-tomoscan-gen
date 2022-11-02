#include <iostream>
#include <opencv2/opencv.hpp>
#include "Generator.hpp"


void Generator::run() {
    std::vector<std::pair<int, int>> partition;
    build_partition(partition);

    std::vector<float> angles(param.angles_num);

    for (int i = 0; i < param.angles_num; ++i) {
        angles[i] = i * param.angles_step;
    }

    int v_det = param.height;
    int h_det = param.height;

    model.read_from_file(param.models_lib.data(), param.model);

    for (auto part : partition) {
        printf("%s %i %s %i\n", "Generating slices from", part.first, "to", part.second);

        int proj_mat_shape[] = {part.second - part.first, param.angles_num, h_det};
        cv::Mat_<float> projections(3, proj_mat_shape);
        model.sinogram(reinterpret_cast<float*>(projections.data), h_det, v_det, part.first, part.second, param.height, angles);
        cv::normalize(projections, projections, 0, 1, cv::NORM_MINMAX);

        int angle = 0;
        int key = 0;

        cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );

        while (key != 'q') {
            cv::Mat_<float> slice(proj_mat_shape[0], proj_mat_shape[2], reinterpret_cast<float*>(projections.data) + proj_mat_shape[2] * proj_mat_shape[0] * angle);
            cv::imshow("Display Image", slice);
            key = cv::waitKeyEx(20);

            ++angle;
            if (angle == param.angles_num) {
                angle = 0;
            }
        }
    }
}


void Generator::build_partition(std::vector<std::pair<int, int>>& partition) {
    partition.resize(param.parts_num);

    if (param.parts_num > 1) {
        int scan_height = param.height / param.parts_num;
        int half_overlay = param.overlay / 2;
        
        partition[0] = {0, scan_height + param.overlay};

        for (int scan_id = 1; scan_id < param.parts_num - 1; ++scan_id) {
            int offset = scan_id * scan_height;
            partition[scan_id] = {offset - half_overlay, offset + scan_height + half_overlay};
        }

        partition[param.parts_num - 1] = {param.height - scan_height - param.overlay, param.height};
    }
    else {
        partition[0] = {0, param.height};
    }
}


void Generator::apply_offset() {

}


void Generator::apply_noise() {

}

