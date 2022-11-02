#ifndef TOMO_P_3D_MODEL_HPP
#define TOMO_P_3D_MODEL_HPP


#include <vector>
#include "TomoP3DObject.hpp"


class TomoP3DModel {
public:
    TomoP3DModel() = default;
    TomoP3DModel(char* file_name, const int model_id);
    TomoP3DModel(const TomoP3DModel&) = default;

    void sinogram(float *data, long horiz_det, long vert_det, long z1, long z2, long size, std::vector<float>& angles);

    bool read_from_file(char* file_name, const int model_id);
    // std::istream& read_from(std::istream& is);
    std::ostream& write_to(std::ostream& os) const noexcept;

private:
    std::vector<TomoP3DObject> objects;
};


// std::istream& operator>>(std::istream& is, TomoP3DModel& obj);
std::ostream& operator<<(std::ostream& os, const TomoP3DModel& obj) noexcept;


#endif // TOMO_P_3D_MODEL_HPP