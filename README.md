# Phong Lighting

The [Phong lighting model](https://en.wikipedia.org/wiki/Phong_reflection_model) (designed by Bui Tuong Phong) is used to approximate lighting in computer graphics. It defines three components to do so: ambient, diffuse and specular. 

<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/6/6b/Phong_components_version_4.png/655px-Phong_components_version_4.png" alt="Phong lighting visual representation"  width=70% height=70%>
</p>

This is just a simple implementation using C++, OpenGL 4.60 Core and:

* [GLFW](https://www.glfw.org/)
* [Glad](https://glad.dav1d.de/)
* [OpenGL Mathematics](https://glm.g-truc.net/0.9.8/index.html)
* [stb](https://github.com/nothings/stb)
* [Dear ImGui](https://github.com/ocornut/imgui)


Three different light casters were implemented:

* Point light
* Directional light
* Spotlight

Textures and shaders can be found inside the `assets` folder. It is possible to replace the cube's textures as long as the original filenames are kept (extensions included). Model importing is planned for a future version so as to finally get rid of that boring cube.

At over 500 lines inside Main.cpp, it is clear that the project needs some serious redesign and restructuring. I plan to learn about game engine architectures and how to better abstract things such as vertex array objects, element buffers objects, renderers, input processing and so on.

## Keybindings and controls

>Use the mouse and keyboard to look and move around the scene. 

| Key | Action |
|---|---|
|`Escape`| Close application |
|`W`| Move forwards|
|`S`| Move backwards|
|`A`| Move left|
|`D`| Move right|
|`E`| Move up|
|`Q`| Move down|
|`F`| Toggle flashlight*|
|`Left shift`| Toggle edit mode|

*Toggling the flashlight on **disables the point and directional light** (makes it look nicer, I guess)

## Building the project using CMake

CMake 3.15 or newer is required to build the project, so make sure to download at least  the specified version. Follow these steps inside the project's directory in order to build it using your desired generator:

1. `mkdir build`
2. `cd build`
3. `cmake ..` (you can specify the generator using the **-G** flag, check the available ones usign `cmake --help`)
4. `cmake --build .`