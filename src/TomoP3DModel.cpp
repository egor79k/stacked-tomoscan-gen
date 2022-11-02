#include <iostream>
#include <cstring>
#include <math.h>
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
    for (auto object : objects) {
        object.move(x, y);
    }
}


void TomoP3DModel::rotate(float ang1, float ang2, float ang3) {
    for (auto object : objects) {
        object.rotate(ang1, ang2, ang3);
    }
}


void TomoP3DModel::sinogram(float *data, long horiz_det, long vert_det, long z1, long z2, long size, std::vector<float>& angles) {
    for (auto object : objects) {
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
