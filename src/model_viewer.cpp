#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "TomoP3DModel.hpp"


void show_demo_animation(int size, char* models_file, int model_id);


int main(int argc, char** argv) {
    if (argc != 4)
    {
        printf("Usage: ./strg <size> <models_lib> <model_id>\n");
        return 1;
    }

    show_demo_animation(atoi(argv[1]), argv[2], atoi(argv[3]));

    return 0;
}


void show_demo_animation(int size, char* models_file, int model_id) {
    int h_det = size;
    int v_det = size;
    int angles_num = 360;

    std::vector<float> angles(angles_num);

    for (int i = 0; i < angles_num; ++i) {
        angles[i] = i;
    }

    int proj_mat_shape[] = {v_det, angles_num, h_det};
    cv::Mat_<float> projections(3, proj_mat_shape);

    TomoP3DModel model(models_file, model_id);
    model.sinogram(reinterpret_cast<float*>(projections.data), h_det, v_det, 0, size, size, angles);
    cv::normalize(projections, projections, 0, 1, cv::NORM_MINMAX);

    int angle = 0;
    int key = 0;

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );

    while (key != 'q') {
        cv::Mat_<float> slice(v_det, h_det, reinterpret_cast<float*>(projections.data) + h_det * v_det * angle);
        cv::imshow("Display Image", slice);
        key = cv::waitKeyEx(20);

        ++angle;
        if (angle == angles_num) {
            angle = 0;
        }
    }
}
