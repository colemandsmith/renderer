# renderer
C++ OpenGL renderer

As it stands now, this is undergoing active development as a Visual Studio
project. I may attempt to support linux builds in the future.

This currently targets 32-bit builds for portability, but I will likely
shift to just supporting 64-bit builds. These should be largely invisible
to anyone using this since I do not host my built dependencies in this project
(aside from `stb_image.h`, which I use for image I/O).

### Dependencies

This project depends on a few libraries (subject to change):

* [GLEW](http://glew.sourceforge.net/)
  * Might shift to SDL for the audio and improved controller input support.
  * I include the built dll alongside my code
* [GLFW](https://www.glfw.org/download.html)
* [GLM](https://glm.g-truc.net/0.9.9/index.html)
* [ASSIMP](https://www.assimp.org/index.php/downloads)
  * Recommend building from source
  * I include the built dll alongside my code
* [YAML-CPP](https://github.com/jbeder/yaml-cpp)
  * Using latest release, as of now requires C++11
