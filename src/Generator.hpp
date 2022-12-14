#ifndef GENERATOR_HPP
#define GENERATOR_HPP


#include <vector>
#include <utility>
#include "Parameters.hpp"
#include "TomoP3DModel.hpp"


/**
 * \brief Interface class for generation management
 */
class Generator {
public:

    /**
     * \brief Starts generation pipeline
     * 
     * \param[in] config_file Configuration file path
     */
    void run(const char* config_file);

private:
    void build_partition();
    void build_angles();
    void build_offsets();
    void build_tilts();
    void build_intensity_variations();
    void build_projections();
    void apply_noise();
    void reconstruct();

    Parameters param;
    TomoP3DModel model;
    std::vector<std::pair<int, int>> partition;
    std::vector<float> angles;
    cv::Mat_<float> offsets;
    cv::Mat_<float> tilts;
    cv::Mat_<float> intensity_variations;
    std::vector<cv::Mat_<float>> projections;
};


#endif // GENERATOR_HPP