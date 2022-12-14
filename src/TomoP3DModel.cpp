#include <iostream>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "TomoP3DModel.hpp"


TomoP3DModel::TomoP3DModel(const char* file_name, const int model_id) {
    if (!read_from_file(file_name, model_id)) {
        objects.clear();
    }
}


void TomoP3DModel::move(float x, float y) {
    for (TomoP3DObject& object : objects) {
        object.move(x, y, 0);
    }
}


void TomoP3DModel::rotate(float ang_x, float ang_y, float ang_z, float cor_x, float cor_y, float cor_z) {
    // Angles in radians
    const double rad_ang = std::acos(-1) / 180;
    const float rad_x = ang_x * rad_ang;
    const float rad_y = ang_y * rad_ang;
    const float rad_z = ang_z * rad_ang;

    // X rotation matrix
    cv::Mat Rx = (cv::Mat_<float>(3, 3) <<
        1, 0,               0,
        0, std::cos(rad_x), -std::sin(rad_x),
        0, std::sin(rad_x), std::cos(rad_x));

    // Y rotation matrix
    cv::Mat Ry = (cv::Mat_<float>(3, 3) <<
        std::cos(rad_y),  0, std::sin(rad_y),
        0,                1, 0,
        -std::sin(rad_y), 0, std::cos(rad_y));
    
    // Z rotation matrix
    cv::Mat Rz = (cv::Mat_<float>(3, 3) <<
        std::cos(rad_z), -std::sin(rad_z), 0,
        std::sin(rad_z), std::cos(rad_z),  0,
        0,               0,                1);
    
    // Full rotation matrix
    cv::Mat R = Rz * Ry * Rx;

    for (TomoP3DObject& object : objects) {
        // Move object to the center of rotation
        object.move(-cor_x, -cor_y, -cor_z);
        // Transform object position according to rotation
        object.transform(R);
        // Move object to it's new origin
        object.move(cor_x, cor_y, cor_z);
        // Rotate object on given angles
        object.rotate(-ang_z, -ang_y, -ang_x);
    }
}


void TomoP3DModel::variate_intensity(float c) {
    for (TomoP3DObject& object : objects) {
        object.variate_intensity(c);
    }
}


void TomoP3DModel::sinogram(float *data, long horiz_det, long vert_det, long z1, long z2, long size, std::vector<float>& angles) {
    for (TomoP3DObject& object : objects) {
        object.sinogram(data, horiz_det, vert_det, z1, z2, size, angles);
    }
}


bool TomoP3DModel::read_from_file(const char* file_name, const int model_id) {
    const int MAXCHAR = 1000;

    int model = 0;
    int components = 0;
    int steps = 0;
    int counter = 0;

    float c0 = 0.0f;
    float x0 = 0.0f;
    float y0 = 0.0f;
    float z0 = 0.0f;
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    float psi_gr1 = 0.0f;
    float psi_gr2 = 0.0f;
    float psi_gr3 = 0.0f;

    // Read parameters file
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("%s \n", "Cannot open the file");
        return false;
    }

    char str[MAXCHAR];
    char tmpstr1[16];
    char tmpstr2[22];
    char tmpstr3[16];
    char tmpstr4[16];
    char tmpstr5[16];
    char tmpstr6[16];
    char tmpstr7[16];
    char tmpstr8[16];
    char tmpstr9[16];
    char tmpstr10[16];
    char tmpstr11[16];
    char tmpstr12[16];

    while (fgets(str, MAXCHAR, fp) != NULL) {
        // Work with non-# commented lines
        if (str[0] == '#') {
            continue;
        }

        sscanf(str, "%15s : %21[^;];", tmpstr1, tmpstr2);

        // Find model with specified id
        if (strcmp(tmpstr1, "Model") != 0) {
            continue;
        }

        model = atoi(tmpstr2);
        if (model_id != model) {
            continue;
        }

        // Check if we have a right model
        if (fgets(str, MAXCHAR, fp) == NULL) {
            printf("Wrong Model format");
            return false;
        }

        sscanf(str, "%15s : %21[^;];", tmpstr1, tmpstr2);

        if (strcmp(tmpstr1, "Components") == 0) {
            components = atoi(tmpstr2);
        }

        if (components <= 0) {
            printf("%s %i\n", "Components cannot be negative, the given value is", components);
            break;
        }

        if (fgets(str, MAXCHAR, fp) == NULL) {
            printf("Wrong Model format");
            return false;
        }

        sscanf(str, "%15s : %21[^;];", tmpstr1, tmpstr2);

        if (strcmp(tmpstr1, "TimeSteps") == 0) {
            steps = atoi(tmpstr2);
        }

        if (steps <= 0) {
            printf("%s %i\n", "TimeSteps cannot be negative, the given value is", steps);
            return false;
        }

        if (steps != 1) {
            printf("%s %i\n", "Only 1 TimeSteps supported, the given value is", steps);
            return false;
        }

        // Loop over all components
        for (int i = 0; i < components; i++) {
            if (fgets(str, MAXCHAR, fp) == NULL) {
                printf("%s %i %s %i\n", "Not enougth components, given", i, "expected", components);
                return false;
            }
            
            sscanf(str, "%15s : %21s %15s %15s %15s %15s %15s %15s %15s %15s %15s %15[^;];", tmpstr1, tmpstr2, tmpstr3, tmpstr4, tmpstr5, tmpstr6, tmpstr7, tmpstr8, tmpstr9, tmpstr10, tmpstr11, tmpstr12);

            if (strcmp(tmpstr1, "Object") != 0) {
                printf("Wrong Object format");
                return false;
            }

            c0 = (float)atof(tmpstr3);       // intensity
            x0 = (float)atof(tmpstr4);       // x0 position
            y0 = (float)atof(tmpstr5);       // y0 position
            z0 = (float)atof(tmpstr6);       // z0 position
            a = (float)atof(tmpstr7);        // a - size object
            b = (float)atof(tmpstr8);        // b - size object
            c = (float)atof(tmpstr9);        // c - size object
            psi_gr1 = (float)atof(tmpstr10); // rotation angle 1
            psi_gr2 = (float)atof(tmpstr11); // rotation angle 2
            psi_gr3 = (float)atof(tmpstr12); // rotation angle 3

            objects.emplace_back(tmpstr2, c0, x0, y0, z0, a, b, c, psi_gr1, psi_gr2, psi_gr3);
        }
    }

    fclose(fp);
    return true;
}


// std::istream& TomoP3DModel::read_from(std::istream& is) {}


std::ostream& TomoP3DModel::write_to(std::ostream& os) const noexcept {
    for (auto object : objects) {
        os << object;
    }

    return os;
}


// std::istream& operator>>(std::istream& is, TomoP3DModel& obj) {}


std::ostream& operator<<(std::ostream& os, const TomoP3DModel& obj) noexcept {
    return obj.write_to(os);
}
