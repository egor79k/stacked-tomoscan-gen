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
    void build_partition(std::vector<std::pair<int, int>>& partition);
    void apply_offset();
    void apply_noise();

    Parameters param;
    TomoP3DModel model;
};


#endif // GENERATOR_HPP