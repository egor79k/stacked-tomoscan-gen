find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(model_viewer
            model_viewer.cpp
            TomoP3DObject.cpp
            TomoP3DModel.cpp)

target_link_libraries(model_viewer ctomophantom ${OpenCV_LIBS})

add_executable(strg
            main.cpp
            TomoP3DObject.cpp
            TomoP3DModel.cpp
            Generator.cpp)
            
target_link_libraries(strg ctomophantom astra ${OpenCV_LIBS})
