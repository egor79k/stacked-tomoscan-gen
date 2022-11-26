#ifndef TOMO_P_3D_OBJECT_HPP
#define TOMO_P_3D_OBJECT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>


/**
 * \brief Representation of 3D geometric object
 * 
 * TomoP3DObject represents one of the following 3D geometric oblects:
 *   - gaussian
 *   - paraboloid
 *   - ellipsoid
 *   - cone
 *   - cuboid
 *   - elliptical cylinder
 * 
 * It contains the following parameters:
 *   - object       - object type
 *   - c0           - intensivety
 *   - (x0, y0, z0) - position
 *   - (a, b, c)    - size
 *   - (p1, p2, p3) - rotation angles
 */
class TomoP3DObject {
public:

    /**
     * \brief Constructs an empty object
     */
    TomoP3DObject() = default;

    /**
     * \brief Constructs an object from given parameters
     * 
     * \param[in] object Geometric shape
     * \param[in] c0 Intensivity
     * \param[in] x0 X position in [-1, 1]
     * \param[in] y0 Y position in [-1, 1]
     * \param[in] z0 Z position in [-1, 1]
     * \param[in] a X size in [-1, 1]
     * \param[in] b Y size in [-1, 1]
     * \param[in] c Z size in [-1, 1]
     * \param[in] psi_gr1 XY Euler angle
     * \param[in] psi_gr2 XZ Euler angle
     * \param[in] psi_gr3 YZ Euler angle
     */
    TomoP3DObject(char *object,
        float c0,
        float x0,
        float y0,
        float z0,
        float a,
        float b,
        float c,
        float psi_gr1,
        float psi_gr2,
        float psi_gr3);

    /**
     * \brief Constructs a copy of the given object
     */
    TomoP3DObject(const TomoP3DObject&) = default;

    /**
     * \brief Moves object on given offset
     * Change object position vector from (x0, y0, z0) to (x0 + x, y0 + y, z0)
     * 
     * \param[in] x X offset (in [-1, 1])
     * \param[in] y Y offset (in [-1, 1])
     * \param[in] z Z offset (in [-1, 1])
     */
    void move(float x, float y, float z);

    /**
     * \brief Transforms object position using given matrix
     * Change object position vector from (x, y, z) to T * (x, y, z)^T
     * 
     * \param[in] T transformation matrix
     */
    void transform(cv::Mat T);

    /**
     * \brief Rotates object on given angles
     * Change object rotation vector from (psi_gr1, psi_gr2, psi_gr3) to (psi_gr1 + ang1, psi_gr2 + ang2, psi_gr3 + ang3)
     * 
     * \param[in] ang_x X rotation (in degrees)
     * \param[in] ang_y Y rotation (in degrees)
     * \param[in] ang_z Z rotation (in degrees)
     */
    void rotate(float ang_x, float ang_y, float ang_z);

    /**
     * \brief Builds object projections
     * Registrates projections of object part from z1 to z2 from specified angles and writes it to data sequentially
     * 
     * \param[out] data Target memory with size (part.second - part.first) * param.angles_num * h_det * sizeof(float) bytes
     * \param[in] horiz_det Horizontal detectors count
     * \param[in] vert_det Vertical detectors count
     * \param[in] z1 Lower bound of scanning
     * \param[in] z2 Upper bound of scanning
     * \param[in] size Object size
     * \param[in] angles Vector of angles of registration of projections
     */
    void sinogram(float *data, long horiz_det, long vert_det, long z1, long z2, long size, std::vector<float>& angles);

    // std::istream& read_from(std::istream& is);

    /**
     * \brief Writes object params to ostream
     * 
     * \param[in] os Out stream where to write
     * \return Out stream
     */
    std::ostream& write_to(std::ostream& os) const noexcept;

private:
    std::string object;  // Analytical Model selection
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
};


// std::istream& operator>>(std::istream& is, TomoP3DObject& obj);
std::ostream& operator<<(std::ostream& os, const TomoP3DObject& obj) noexcept;


#endif // TOMO_P_3D_OBJECT_HPP