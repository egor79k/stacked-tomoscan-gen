#ifndef TOMO_P_3D_MODEL_HPP
#define TOMO_P_3D_MODEL_HPP


#include <vector>
#include <opencv2/opencv.hpp>
#include "TomoP3DObject.hpp"


/**
 * \brief Representation of 3D model
 * TomoP3DModel represents phantom model as a set of TomoP3DObject.
 */
class TomoP3DModel {
public:

    /**
     * \brief Constructs an empty model
     */
    TomoP3DModel() = default;

    /**
     * \brief Constructs a model from given parameters
     * 
     * \param[in] file_name Phantom library path
     * \param[in] model_id Model identificator from library
     */
    TomoP3DModel(const char* file_name, const int model_id);

    /**
     * \brief Constructs a copy of the given model
     */
    TomoP3DModel(const TomoP3DModel&) = default;

    /**
     * \brief Moves model on given offset
     * Change each object position vector from (x0, y0, z0) to (x0 + x, y0 + y, z0)
     * 
     * \param[in] x X offset (in [-1, 1])
     * \param[in] y Y offset (in [-1, 1])
     */
    void move(float x, float y);

    /**
     * \brief Rotates model on given angles
     * Rotates model around (cor_x, cor_y, cor_z) point on (ang_x, ang_y, ang_z) Euler angles
     * 
     * \param[in] ang_x X rotation (in degrees)
     * \param[in] ang_y Y rotation (in degrees)
     * \param[in] ang_z Z rotation (in degrees)
     * \param[in] cor_x X center of rotation point (in [-1, 1])
     * \param[in] cor_y Y center of rotation point (in [-1, 1])
     * \param[in] cor_z Z center of rotation point (in [-1, 1])
     */
    void rotate(float ang_x, float ang_y, float ang_z, float cor_x, float cor_y, float cor_z);

    /**
     * \brief Builds model projections
     * Registrates projections of model part from z1 to z2 from specified angles and writes it to data sequentially
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

    /**
     * \brief Reads model from library
     * Reads model from library, where models are described in format used in TomoPhantom
     * 
     * \param[in] file_name Phantom library path
     * \param[in] model_id Model identificator from library
     * \return true - success, false - error
     */
    bool read_from_file(const char* file_name, const int model_id);

    // std::istream& read_from(std::istream& is);

    /**
     * \brief Writes model params to ostream
     * 
     * \param[in] os Out stream where to write
     * \return Out stream
     */
    std::ostream& write_to(std::ostream& os) const noexcept;

private:
    std::vector<TomoP3DObject> objects;
};


// std::istream& operator>>(std::istream& is, TomoP3DModel& obj);
std::ostream& operator<<(std::ostream& os, const TomoP3DModel& obj) noexcept;


#endif // TOMO_P_3D_MODEL_HPP