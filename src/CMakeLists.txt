find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES TomoP3DObject.cpp TomoP3DModel.cpp)

add_executable(model_viewer model_viewer.cpp ${SOURCE_FILES})

target_link_libraries(model_viewer ctomophantom ${OpenCV_LIBS})

add_executable(strg main.cpp ${SOURCE_FILES})

target_link_libraries(strg ctomophantom astra ${OpenCV_LIBS})
