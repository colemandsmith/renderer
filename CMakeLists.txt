cmake_minimum_required(VERSION 3.15)
project(Renderer)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# glm
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.1
)
FetchContent_MakeAvailable(glm)

# yaml-cpp
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# assimp
# set variables that are needed
# set(ZLIB_LIBRARIES zlibstatic)
# set(ENABLE_BOOST_WORKAROUND ON)
# set(BUILD_STATIC_LIB ON)
# set(BUILD_ASSIMP_TOOLS  ON)
# set(ASSIMP_BUILD_STATIC_LIB ON)

set(SOURCES
    Renderer/Camera.cpp
    Renderer/DirectionalLight.cpp
    Renderer/GameObject.cpp
    Renderer/Light.cpp
    Renderer/main.cpp
    Renderer/Material.cpp
    Renderer/Mesh.cpp
    Renderer/Model.cpp
    Renderer/OmniShadowMap.cpp
    Renderer/PointLight.cpp
    Renderer/RenderObject.cpp
    Renderer/Scene.cpp
    Renderer/Shader.cpp
    Renderer/ShaderManager.cpp
    Renderer/ShadowMap.cpp
    Renderer/Skybox.cpp
    Renderer/SpotLight.cpp
    Renderer/Texture.cpp
    Renderer/Window.cpp
)

set(INCLUDE_DIRS
    ${INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
   )

include_directories(${INCLUDE_DIRS})

set(LIBS
    ${ASSIMP_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
    glm::glm
    ${OPENGL_LIBRARY}
    yaml-cpp::yaml-cpp
   )

add_executable(Renderer ${SOURCES})
target_link_libraries(Renderer ${LIBS})
