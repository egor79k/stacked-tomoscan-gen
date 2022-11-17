#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "Generator.hpp"


void Generator::run(const char* config_file) {
    if (!param.read_from_file(config_file)) {
        printf("%s %s", "Error: Unable to read config file", config_file);
        return;
    }

    build_partition();
    build_angles();
    build_projections();
    apply_noise();
    reconstruct();
}


void Generator::build_partition() {
    partition.resize(param.parts_num);

    if (param.parts_num > 1) {
        int scan_height = param.height / param.parts_num;
        int half_overlay = param.overlay / 2;
        
        partition[0] = {0, scan_height + half_overlay};

        for (int scan_id = 1; scan_id < param.parts_num - 1; ++scan_id) {
            int offset = scan_id * scan_height;
            partition[scan_id] = {offset - half_overlay, offset + scan_height + half_overlay};
        }

        partition[param.parts_num - 1] = {param.height - scan_height - half_overlay, param.height};
    }
    else {
        partition[0] = {0, param.height};
    }
}


void Generator::build_angles() {
    angles.resize(param.angles_num);

    for (int i = 0; i < param.angles_num; ++i) {
        angles[i] = i * param.angles_step;
    }
}


void Generator::build_projections() {
    int v_det = param.height;
    int h_det = param.width * std::sqrt(2);

    model.read_from_file(param.models_lib.data(), param.model);
    projections.resize(partition.size());

    for (int part_id = 0; part_id < partition.size(); ++ part_id) {
        auto part = partition[part_id];
        printf("%s %i %s %i\n", "Generating slices from", part.first, "to", part.second);

        // TomoP3DModel transformed_model = model.transformed();

        int proj_mat_shape[] = {part.second - part.first, param.angles_num, h_det};
        projections[part_id] = cv::Mat_<float>(3, proj_mat_shape);
        printf("%s %i %i %i\n", "Size:", proj_mat_shape[0], proj_mat_shape[1], proj_mat_shape[2]);
        model.sinogram(reinterpret_cast<float*>(projections[part_id].data), h_det, v_det, part.first, part.second, param.height, angles);
    }
}


void Generator::apply_noise() {
    if (!param.is_noisy) {
        return;
    }

    printf("Applying noise\n");

    for (cv::Mat_<float>& part_projs : projections) {
        int noise_size[] = {part_projs.size[0], part_projs.size[1], part_projs.size[2]};
        cv::Mat_<float> noise(3, noise_size);
        cv::randn(noise, 6, 1);
        part_projs += noise;
    }
}


void Generator::reconstruct() {

    std::string proj_dir = param.save_path + "/projections/";

    std::system(("rm -r " + param.save_path).data());

    mkdir(param.save_path.data(), S_IRWXU);
    mkdir(proj_dir.data(), S_IRWXU);

    for (int part_id = 0; part_id < projections.size(); ++ part_id) {
        cv::Mat_<float>& part_projs = projections[part_id];

        std::string save_dir(proj_dir + std::to_string(part_id) + "/");
        mkdir(save_dir.data(), S_IRWXU);

        for (int angle_id = 0; angle_id < param.angles_num; ++angle_id) {
            cv::Mat_<float> slice(part_projs.size[0], part_projs.size[2], reinterpret_cast<float*>(part_projs.data) + part_projs.size[2] * part_projs.size[0] * angle_id);
            cv::imwrite(save_dir + std::to_string(angle_id) + ".tiff", slice);
        }
    }

    std::string cmd = "python3 ../src/reconstructor.py " +
        proj_dir + " " +
        std::to_string(param.height) + " " +
        std::to_string(param.width) + " " +
        std::to_string(param.parts_num) + " " +
        std::to_string(param.angles_num) + " " +
        std::to_string(param.angles_step) + " " +
        param.save_path;

    std::system(cmd.data());

    // std::system(("rm -r " + proj_dir).data());
}
