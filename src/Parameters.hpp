#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP


#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


/**
 * \brief Configuration parameters for generator
 * Configuration file must contain all parameters specified in Public Attributes
 */
class Parameters {
public:

    /**
     * \brief Read parameters from specified config file
     * 
     * \param[in] file_name Configuration file path
     * \return true - success, false - error
     */
    bool read_from_file(const char* config_file);
    
    // Model parameters
    std::string models_lib;
    int model;

    // Partition parameters
    int height;
    int depth;
    int width;
    int parts_num;
    int overlay;

    // Scanning parameters
    int angles_num;
    float angles_step;

    // Distortion parameters
    int seed;
    bool is_noisy;
    float noise_amplitude;
    bool is_offset;
    float max_offset;
    bool is_tilted;
    float max_tilt;

    // Output parameters
    std::string save_path;
    // std::string format;
    // type;

private:
    void serialize(const std::unordered_map<std::string, std::string>& dict);

    std::string config_dir;
};


#endif // PARAMETERS_HPP