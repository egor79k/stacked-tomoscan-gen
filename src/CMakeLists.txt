find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp TomoP3DObject.cpp TomoP3DModel.cpp)

add_executable(strg ${SOURCE_FILES})

target_link_libraries(strg ctomophantom astra ${OpenCV_LIBS})
