#include "TomoP3DObject.hpp"


TomoP3DObject::TomoP3DObject(char *object_,
    float c0_,
    float x0_,
    float y0_,
    float z0_,
    float a_,
    float b_,
    float c_,
    float psi_gr1_,
    float psi_gr2_,
    float psi_gr3_) :
    object(object_),
    c0(c0_),
    x0(x0_),
    y0(y0_),
    z0(z0_),
    a(a_),
    b(b_),
    c(c_),
    psi_gr1(psi_gr1_),
    psi_gr2(psi_gr2_),
    psi_gr3(psi_gr3_) {}


std::ostream& TomoP3DObject::write_to(std::ostream& os) const noexcept {
    return os << "Object : "
        << object << ' '
        << c0 << ' '
        << x0 << ' '
        << y0 << ' '
        << z0 << ' '
        << a << ' '
        << b << ' '
        << c << ' '
        << psi_gr1 << ' '
        << psi_gr2 << ' '
        << psi_gr3 << ' '
        << std::endl;
}


std::ostream& operator<<(std::ostream& os, const TomoP3DObject& obj) noexcept {
    return obj.write_to(os);
}
