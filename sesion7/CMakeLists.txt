cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME proyectoFinal)
set(CODE_PATH src)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH ../binary)

project(${PROJECT_NAME})
set(GLEW_PATH      ../libraries/glew)
set(GLFW_PATH      ../libraries/glfw)
set(GLM_PATH       ../libraries/glm)
set(ASSIMP_PATH    ../libraries/assimp)
set(FREEIMAGE_PATH ../libraries/freeimage)
include_directories(${CODE_PATH} ${GLEW_PATH}/include ${GLFW_PATH}/include ${GLM_PATH}/include ${ASSIMP_PATH}/include ${FREEIMAGE_PATH}/include)
link_directories(${GLEW_PATH}/lib ${GLFW_PATH}/lib ${GLM_PATH}/lib ${ASSIMP_PATH}/lib ${FREEIMAGE_PATH}/lib)
file(GLOB_RECURSE CODE_FILES ${CODE_PATH}/*.*)
add_executable(${PROJECT_NAME} ${CODE_FILES})
target_link_libraries(${PROJECT_NAME} opengl32 glu32 glew32 glfw3 assimp freeimage)