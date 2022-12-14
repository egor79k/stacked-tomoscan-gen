extern "C" {
    #include <tomophantom/TomoP3DModelSino_core.h>
}

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


void TomoP3DObject::move(float x, float y, float z) {
    x0 += x;
    y0 += y;
    z0 += z;
}


void TomoP3DObject::transform(cv::Mat T) {
    cv::Mat_<float> pos = (cv::Mat_<float>(3, 1) << x0, y0, z0);
    pos = T * pos;
    x0 = pos(0, 0);
    y0 = pos(1, 0);
    z0 = pos(2, 0);
}


void TomoP3DObject::rotate(float ang_x, float ang_y, float ang_z) {
    psi_gr1 += ang_x;
    psi_gr2 += ang_y;
    psi_gr3 += ang_z;
}


void TomoP3DObject::variate_intensity(float c) {
    c0 += c;
}


void TomoP3DObject::sinogram(float *data, long horiz_det, long vert_det, long z1, long z2, long size, std::vector<float>& angles) {
    if (("gaussian" == object) || ("paraboloid" == object) || ("ellipsoid" == object)) {
        TomoP3DObjectSino_core(data, horiz_det, vert_det, z1, z2, size, angles.data(), angles.size(), &object[0], c0, y0, -z0, -x0, b, a, c, psi_gr3, psi_gr2,  psi_gr1, 0l);
    }
    else if ("elliptical_cylinder" == object) {
        TomoP3DObjectSino_core(data, horiz_det, vert_det, z1, z2, size, angles.data(), angles.size(), &object[0], c0, x0, -y0,  z0, b, a, c, psi_gr3, psi_gr2,  psi_gr1, 0l);
    }
    else {
        TomoP3DObjectSino_core(data, horiz_det, vert_det, z1, z2, size, angles.data(), angles.size(), &object[0], c0, x0,  y0,  z0, a, b, c, psi_gr3, psi_gr2, -psi_gr1, 0l);
    }
}


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
