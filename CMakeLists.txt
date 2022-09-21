cmake_minimum_required(VERSION 3.15.0)
project(PhysicsEngine VERSION 0.1.0)

include(CTest)
enable_testing()

# Set C++ Standard to C++20
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")

set(IMGUI_PATH "lib/imgui")
set(GLM_PATH "lib/glm")
set(GLFW_PATH "lib/glfw")
set(GLAD_PATH "lib/glad")
set(STB_PATH "lib/stb")
set(OpenCV_DIR "C:/opencv")

find_package(OpenCV REQUIRED)

add_subdirectory(${GLM_PATH})
add_subdirectory(${GLFW_PATH})

file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)

file(GLOB_RECURSE _PROJECT_SOURCES src/*.cpp)
file(GLOB_RECURSE _SANDBOX_TEST_SOURCES test/sandbox/*.cpp)


add_executable(PhysicsEngine main.cpp ${_PROJECT_SOURCES} ${IMGUI_SOURCES} ${GLAD_PATH}/src/glad.c)
target_include_directories(PhysicsEngine PUBLIC ${STB_PATH} ${IMGUI_PATH} ${GLM_PATH} ${GLFW_PATH} ${GLFW_PATH}/include ${GLAD_PATH}/include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(PhysicsEngine PRIVATE glm glfw ${OpenCV_LIBS})

add_executable(SandboxTest test/sandbox/sandbox.cpp ${_PROJECT_SOURCES} ${_SANDBOX_TEST_SOURCES} ${IMGUI_SOURCES} ${GLAD_PATH}/src/glad.c)
target_include_directories(SandboxTest PUBLIC ${STB_PATH} ${IMGUI_PATH} ${GLM_PATH} ${GLFW_PATH} ${GLFW_PATH}/include ${GLAD_PATH}/include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(SandboxTest glm glfw ${OpenCV_LIBS})

file(COPY src/graphics/assets DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY test/sandbox/assets DESTINATION ${EXECUTABLE_OUTPUT_PATH})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
