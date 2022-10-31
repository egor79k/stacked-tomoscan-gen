#ifndef TOMO_P_3D_MODEL_HPP
#define TOMO_P_3D_MODEL_HPP

#include "TomoP3DObject.hpp"


class TomoP3DModel {
public:
    bool read_from_file(char* file_name, const int model_id);
    std::istream& read_from(std::istream& is);
    std::ostream& write_to(std::ostream& os) const noexcept;
};


std::istream& operator>>(std::istream& is, TomoP3DModel& obj);
std::ostream& operator<<(std::ostream& os, const TomoP3DModel& obj) noexcept;


#endif // TOMO_P_3D_MODEL_HPP