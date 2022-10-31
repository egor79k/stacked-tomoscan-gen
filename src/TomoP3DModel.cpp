#include <iostream>
#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "TomoP3DModel.hpp"


bool TomoP3DModel::read_from_file(char* file_name, const int model_id) {
    const int MAXCHAR = 1000;
    int Model = 0, Components = 0, steps = 0, counter = 0, ii;
    float C0 = 0.0f, x0 = 0.0f, y0 = 0.0f, z0 = 0.0f, a = 0.0f, b = 0.0f, c = 0.0f, psi_gr1 = 0.0f, psi_gr2 = 0.0f, psi_gr3 = 0.0f;

    FILE *fp = fopen(file_name, "r"); // read parameters file
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
        /* work with non-# commented lines */
        if (str[0] == '#') {
            continue;
        }

        sscanf(str, "%15s : %21[^;];", tmpstr1, tmpstr2);

        if (strcmp(tmpstr1, "Model") != 0) {
            continue;
        }

        Model = atoi(tmpstr2);
        if ((model_id == Model) && (counter == 0)) {
            /* check if we have a right model */
            if (fgets(str, MAXCHAR, fp) != NULL) sscanf(str, "%15s : %21[^;];", tmpstr1, tmpstr2);
            else {
                break;
            }
            if (strcmp(tmpstr1, "Components") == 0) Components = atoi(tmpstr2);
            //printf("%s %i\n", "Components:", Components);
            if (Components <= 0) {
                printf("%s %i\n", "Components cannot be negative, the given value is", Components);
                break;
            }
            if (fgets(str, MAXCHAR, fp) != NULL) sscanf(str, "%15s : %21[^;];", tmpstr1, tmpstr2);
            else {
                break;
            }
            if (strcmp(tmpstr1, "TimeSteps") == 0) steps = atoi(tmpstr2);
            if (steps <= 0) {
                printf("%s %i\n", "TimeSteps cannot be negative, the given value is", steps);
                break;
            }
            //printf("%s %i\n", "TimeSteps:", steps);
            if (steps != 1) {
                printf("Only 1 TimeSteps supported, the given value is %d", steps);
                return false;
            }
            /**************************************************/
            //printf("\n %s %i %s \n", "Stationary 3D model", model_id, " is selected");
            /* loop over all components */
            for (ii = 0; ii<Components; ii++) {
                if (fgets(str, MAXCHAR, fp) != NULL) sscanf(str, "%15s : %21s %15s %15s %15s %15s %15s %15s %15s %15s %15s %15[^;];", tmpstr1, tmpstr2, tmpstr3, tmpstr4, tmpstr5, tmpstr6, tmpstr7, tmpstr8, tmpstr9, tmpstr10, tmpstr11, tmpstr12);
                else {
                    break;
                }

                if (strcmp(tmpstr1, "Object") == 0) {
                    C0 = (float)atof(tmpstr3); /* intensity */
                    x0 = (float)atof(tmpstr4); /* x0 position */
                    y0 = (float)atof(tmpstr5); /* y0 position */
                    z0 = (float)atof(tmpstr6); /* z0 position */
                    a = (float)atof(tmpstr7); /* a - size object */
                    b = (float)atof(tmpstr8); /* b - size object */
                    c = (float)atof(tmpstr9); /* c - size object */
                    psi_gr1 = (float)atof(tmpstr10); /* rotation angle 1*/
                    psi_gr2 = (float)atof(tmpstr11); /* rotation angle 2*/
                    psi_gr3 = (float)atof(tmpstr12); /* rotation angle 3*/
                }
                else {
                    break;
                }
                // printf("\nObject : %s \nC0 : %f \nx0 : %f \ny0 : %f \nz0 : %f \na : %f \nb : %f \nc : %f \n", tmpstr2, C0, x0, y0, z0, a, b, c);                                                          

                // TomoP3DObject_core(A, N1, N2, N3, Z1, Z2, tmpstr2, C0, y0, x0, z0, a, b, c, psi_gr1, psi_gr2, psi_gr3, 0l); /* python */
                TomoP3DObject obj(tmpstr2, C0, x0, y0, z0, a, b, c, psi_gr1, psi_gr2, psi_gr3);
                std::cout << obj;
            }
            counter++;
        }
    }

    fclose(fp);
    return true;
}


// std::istream& TomoP3DModel::read_from(std::istream& is) {}


// std::ostream& TomoP3DModel::write_to(std::ostream& os) const noexcept {}


// std::istream& operator>>(std::istream& is, TomoP3DModel& obj) {}


// std::ostream& operator<<(std::ostream& os, const TomoP3DModel& obj) noexcept {}
