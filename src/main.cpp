#include <iostream>
#include <utility>
#include <vector>
// #include <astra/CudaBackProjectionAlgorithm3D.h>
#include <opencv2/opencv.hpp>
#include "Generator.hpp"
#include "Parameters.hpp"
#include "TomoP3DModel.hpp"


int main(int argc, char** argv) {
    Generator g;
    g.run();

    return 0;
}
