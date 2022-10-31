// extern "C" {
//     extern float TomoP3DModel_core(float *A, int ModelSelected, long N1, long N2, long N3, long Z1, long Z2, char *ModelParametersFilename);
//     extern float TomoP3DModelSino_core(float *A, int ModelSelected, long Horiz_det, long Vert_det, long Z1, long Z2, long N, float *Theta_proj, int AngTot, char* ModelParametersFilename);
// }

#include <iostream>
#include <fstream>
#include <vector>

extern "C" {
    // #include <tomophantom/TomoP3DModel_core.h>
    #include <tomophantom/TomoP3DModelSino_core.h>
}

// #include <astra/FilteredBackProjectionAlgorithm.h>

#include <opencv2/opencv.hpp>
#include "TomoP3DObject.hpp"
#include "TomoP3DModel.hpp"


void show_demo_animation(int model_no, int N);


int main(int argc, char** argv) {
    // if (argc != 3)
    // {
    //     printf("Usage: ./strg size model_id\n");
    //     return 1;
    // }
    // show_demo_animation(atoi(argv[2]), atoi(argv[1]));

    if (argc != 3)
    {
        printf("Usage: ./strg models_file model_id\n");
        return 1;
    }
    TomoP3DModel model;
    model.read_from_file(argv[1], atoi(argv[2]));

    return 0;
}


void show_demo_animation(int model_no, int N) {
    int h_det = N;
    int v_det = N;
    int angles_num = 360;

    char lib[] = "../models/Phantom3DLibrary.dat";

    std::vector<float> angles(angles_num);

    for (int i = 0; i < angles_num; ++i) {
        angles[i] = i;
    }

    int proj_mat_shape[] = {v_det, angles_num, h_det};
    cv::Mat_<float> projections(3, proj_mat_shape);

    TomoP3DModelSino_core(reinterpret_cast<float*>(projections.data), model_no, h_det, v_det, 0, N, N, angles.data(), angles_num, lib);

    int angle = 0;
    int key = 0;

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );

    while (key != 'q') {
        cv::Mat_<float> slice(v_det, h_det, reinterpret_cast<float*>(projections.data) + h_det * v_det * angle);
        cv::Mat img;
        cv::normalize(slice, img, 0, 1, cv::NORM_MINMAX);
        cv::imshow("Display Image", img);
        key = cv::waitKeyEx(20);

        ++angle;
        if (angle == angles_num) {
            angle = 0;
        }
    }
}
