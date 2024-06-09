cmake_minimum_required(VERSION 3.29)
project(Renderer)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(OpenGL REQUIRED)

include(FetchContent)

set(EXT_INCLUDE_DIRS
    ${OPENGL_INCLUDE_DIRS}
   )

set(LIBS
    ${OPENGL_LIBRARY}
   )

FetchContent_Declare(
  glm
  GIT_REPOSITORY  https://github.com/g-truc/glm.git
  GIT_TAG   1.0.1
  FIND_PACKAGE_ARGS NAMES glm
)
FetchContent_MakeAvailable(glm)
list(APPEND LIBS glm::glm)

set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "Disable parse tools")

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)
list(APPEND LIBS yaml-cpp::yaml-cpp)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.4.0
  FIND_PACKAGE_ARGS NAMES assimp
)
FetchContent_MakeAvailable(assimp)

list(APPEND EXT_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIRS})
list(APPEND LIBS assimp)

FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG glew-cmake-2.2.0
  FIND_PACKAGE_ARGS NAMES GLEW
)
FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
  FetchContent_Populate(glew)
  add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
list(APPEND LIBS libglew_static)

FetchContent_Declare(
  glfw3
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  FIND_PACKAGE_ARGS NAMES glfw3
)
FetchContent_MakeAvailable(glfw3)

list(APPEND LIBS glfw)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SOURCES
    src/main_renderer.cpp
    src/rendering/Camera.cpp
    src/rendering/OpenGLRenderer.cpp
    src/rendering/RenderEngine.cpp
    src/rendering/Renderer.cpp
    src/rendering/Shader.cpp
    src/rendering/ShaderManager.cpp
    src/rendering/Window.cpp
    src/resources/DirectionalLight.cpp
    src/resources/Light.cpp
    src/resources/Material.cpp
    src/resources/Mesh.cpp
    # src/resources/Model.cpp
    src/resources/OmniShadowMap.cpp
    src/resources/PointLight.cpp
    # src/resources/RenderObject.cpp
    #src/resources/Scene.cpp
    src/resources/ShadowMap.cpp
    # src/resources/Skybox.cpp
    src/resources/SpotLight.cpp
    src/resources/Texture.cpp
)
list(APPEND INCLUDE_DIRS ${EXT_INCLUDE_DIRS})
# Gives nice relative include paths from the root src/ folder
list(APPEND INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src")

# for IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${SOURCES})

include_directories(${INCLUDE_DIRS})

add_executable(Renderer ${SOURCES})
target_link_libraries(Renderer PRIVATE ${LIBS})
