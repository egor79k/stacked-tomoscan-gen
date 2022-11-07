#ifndef GENERATOR_HPP
#define GENERATOR_HPP


#include <vector>
#include <utility>
#include "Parameters.hpp"
#include "TomoP3DModel.hpp"


class Generator {
public:
    // Generator(const Parameters& p);
    void run();

private:
    void build_partition();
    void build_angles();
    void build_projections();
    void apply_noise();
    void reconstruct();

    Parameters param;
    TomoP3DModel model;
    std::vector<std::pair<int, int>> partition;
    std::vector<float> angles;
    std::vector<cv::Mat_<float>> projections;
};


#endif // GENERATOR_HPP