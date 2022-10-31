#ifndef TOMO_P_3D_OBJECT_HPP
#define TOMO_P_3D_OBJECT_HPP

#include <iostream>


class TomoP3DObject {
public:
    TomoP3DObject() = default;
    TomoP3DObject(char *object_,
        float c0_,
        float x0_,
        float y0_,
        float z0_,
        float a_,
        float b_,
        float c_,
        float psi_gr1_,
        float psi_gr2_,
        float psi_gr3_);

    // std::istream& read_from(std::istream& is);
    std::ostream& write_to(std::ostream& os) const noexcept;

private:
    char *object;  // Analytical Model selection
    float c0;      // intensity
    float x0;      // x0 position
    float y0;      // y0 position
    float z0;      // z0 position
    float a;       // a - size object
    float b;       // b - size object
    float c;       // c - size object
    float psi_gr1; // rotation angle1
    float psi_gr2; // rotation angle2
    float psi_gr3; // rotation angle3
    // long tt;       // temporal index, 0 - for stationary
};


// std::istream& operator>>(std::istream& is, TomoP3DObject& obj);
std::ostream& operator<<(std::ostream& os, const TomoP3DObject& obj) noexcept;


#endif // TOMO_P_3D_OBJECT_HPP