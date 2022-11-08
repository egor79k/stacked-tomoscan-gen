#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP


#include <string>


class Parameters {
public:
    // void read_from_file(const char* config_file);
    
    // Model parameters
    std::string models_lib = "../models/Phantom3DLibrary.dat";
    int model = 13;

    // Partition parameters
    int height = 128;
    int depth = 128;
    int width = 128;
    int parts_num = 2;
    int overlay = 0;

    // Scanning parameters
    int angles_num = 180;
    float angles_step = 1.0f;

    // Distortion parameters
    // seed = 0
    bool is_noisy = false;
    // noise_params =  {'noise_type' : 'Poisson',
                        // 'noise_amplitude' : 10000,
                        // 'noise_seed' : seed}
    bool is_offset = false;
    int max_offset = 5;

    // Output parameters
    std::string save_path = "img/demo";
    std::string format = ".tiff";
    // type = 'float32'
};


#endif // PARAMETERS_HPP