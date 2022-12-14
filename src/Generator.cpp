#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
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

    cv::theRNG().state = param.seed;

    build_partition();
    build_angles();
    build_projections();
    apply_noise();
    reconstruct();
}


void Generator::build_partition() {
    printf("%s %i %s %i %s %i\n", "Building partition of height", param.height, "into", param.parts_num, "parts with overlay", param.overlay);
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


void Generator::build_offsets() {
    offsets = cv::Mat_<float>(partition.size(), 2);
    cv::randu(offsets, -param.max_offset, param.max_offset);
}


void Generator::build_tilts() {
    tilts = cv::Mat_<float>(partition.size(), 2);
    cv::randu(tilts, -param.max_tilt, param.max_tilt);
}


void Generator::build_intensity_variations() {
    intensity_variations = cv::Mat_<float>(partition.size(), 1);
    cv::randu(intensity_variations, -param.max_intensity_variation, param.max_intensity_variation);
}


void Generator::build_projections() {
    int v_det = param.height;
    int h_det = param.width * std::sqrt(2);

    printf("%s %i %s %s\n", "Reading model", param.model, "from", param.models_lib.data());
    model.read_from_file(param.models_lib.data(), param.model);
    projections.resize(partition.size());

    if (param.is_offset) {
        build_offsets();
    }

    if (param.is_tilted) {
        build_tilts();
    }

    if (param.is_intensity_vary) {
        build_intensity_variations();
    }

    for (int part_id = 0; part_id < partition.size(); ++ part_id) {
        auto part = partition[part_id];
        int proj_mat_shape[] = {part.second - part.first, param.angles_num, h_det};

        printf("%s %i %s %i %s %i %i %i\n", "Building sinograms from", part.first, "to", part.second, "with shape", proj_mat_shape[0], proj_mat_shape[1], proj_mat_shape[2]);
        TomoP3DModel curr_model = model;

        if (param.is_offset) {
            printf("%s (%f, %f)\n", "Applying offset on", offsets[part_id][0], offsets[part_id][1]);
            curr_model.move(offsets[part_id][0], offsets[part_id][1]);
        }

        if (param.is_tilted) {
            printf("%s (%f, %f)\n", "Applying tilt on", tilts[part_id][0], tilts[part_id][1]);

            float cor_x = 0.0f;
            float cor_y = 0.0f;
            float cor_z = (part.first + part.second - v_det) / static_cast<float>(v_det);

            if (param.is_offset) {
                cor_x = offsets[part_id][0];
                cor_y = offsets[part_id][1];
            }
            
            curr_model.rotate(tilts[part_id][0], tilts[part_id][1], 0.0f, cor_x, cor_y, cor_z);
        }

        if (param.is_intensity_vary) {
            printf("%s %f\n", "Applying intensity variation on", intensity_variations[part_id][0]);
            curr_model.variate_intensity(intensity_variations[part_id][0]);
        }

        projections[part_id] = cv::Mat_<float>(3, proj_mat_shape);
        curr_model.sinogram(reinterpret_cast<float*>(projections[part_id].data), h_det, v_det, part.first, part.second, param.height, angles);
    }
}


void Generator::apply_noise() {
    if (!param.is_noisy) {
        return;
    }

    const float mean = 10000;
    std::mt19937 mrand(param.seed);
    std::poisson_distribution<> noise(mean);

    printf("Applying noise\n");

    for (cv::Mat_<float>& part_projs : projections) {
        float* data = reinterpret_cast<float*>(part_projs.data);

        for (int i = 0; i < part_projs.size[0] * part_projs.size[1] * part_projs.size[2]; ++i) {
            float n = (noise(mrand) - mean) * param.noise_amplitude;
            data[i] += n;
        }
    }
}


void Generator::reconstruct() {
    std::string proj_dir = param.save_path + "/projections/";

    std::system(("rm -r " + param.save_path).data());

    if (mkdir(param.save_path.data(), S_IRWXU) == -1) {
        printf("%s %s\n", "Unable to create directory", param.save_path.data());
        return;
    }
    
    mkdir(proj_dir.data(), S_IRWXU);

    printf("%s %s\n", "Writing projections to", proj_dir.data());
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

    printf("Running reconstruction script\n");
    std::system(cmd.data());

    // std::system(("rm -r " + proj_dir).data());
}
