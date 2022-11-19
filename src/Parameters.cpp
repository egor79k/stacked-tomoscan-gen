#include "Parameters.hpp"


bool Parameters::read_from_file(const char* config_file) {
    std::ifstream file(config_file);
    
    if (!file.is_open()) {
        std::cerr << "Couldn't open config file for reading\n";
        return false;
    }

    std::unordered_map<std::string, std::string> dict;
    std::string line;
    
    while (getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        if(line[0] == '#' || line.empty())
            continue;

        auto sep_pos = line.find("=");
        auto name = line.substr(0, sep_pos);
        auto value = line.substr(sep_pos + 1);
        dict.insert({name, value});
    }

    std::string cf(config_file);
    config_dir = cf.substr(0, cf.find_last_of('/') + 1);

    serialize(dict);
        
    return true;
}


void Parameters::serialize(const std::unordered_map<std::string, std::string>& dict) {
    // Model parameters
    models_lib = config_dir + dict.at("models_lib");
    model = std::stoi(dict.at("model"));

    // Partition parameters
    height = std::stoi(dict.at("height"));
    depth = std::stoi(dict.at("depth"));
    width = std::stoi(dict.at("width"));
    parts_num = std::stoi(dict.at("parts_num"));
    overlay = std::stoi(dict.at("overlay"));

    // Scanning parameters
    angles_num = std::stoi(dict.at("angles_num"));
    angles_step = std::stof(dict.at("angles_step"));

    // Distortion parameters
    is_noisy = (dict.at("is_noisy") == "1");
    is_offset = (dict.at("is_offset") == "1");
    max_offset = std::stof(dict.at("max_offset"));

    // Output parameters
    save_path = config_dir + dict.at("save_path");
}