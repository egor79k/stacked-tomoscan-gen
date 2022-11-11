#include <iostream>
#include <utility>
#include <vector>
// #include <astra/CudaBackProjectionAlgorithm3D.h>
#include <opencv2/opencv.hpp>
#include "Generator.hpp"
#include "Parameters.hpp"
#include "TomoP3DModel.hpp"


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./strg <config_file>\n");
        return 1;
    }

    Generator g;
    g.run(argv[1]);

    return 0;
}
