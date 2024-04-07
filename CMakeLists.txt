cmake_minimum_required(VERSION 3.15)

#Used to get root dir if CMakeList is inside the src folder:
#cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PROJECT_PARENT_DIR)
#message(STATUS "Root directory: ${PROJECT_PARENT_DIR}")

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

project(OpenGLLighting VERSION 1.0)

add_subdirectory(glfw)

add_executable(OpenGLLighting
    src/Main.cpp
    src/Camera.cpp
    src/ElementBuffer.cpp
    src/glad.c
    src/LabDebug.cpp
    src/Shader.cpp
    src/stb_image.cpp
    src/STBLoading.cpp
    src/VertexBuffer.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp 
    src/imgui/imgui_draw.cpp 
    src/imgui/imgui_impl_glfw.cpp 
    src/imgui/imgui_impl_opengl3.cpp 
    src/imgui/imgui_tables.cpp 
    src/imgui/imgui_widgets.cpp 
)

#Shaders
configure_file("assets/shaders/CubeLightFrag.txt" "shaders/CubeLightFrag.txt")
configure_file("assets/shaders/CubeLightVert.txt" "shaders/CubeLightVert.txt")
configure_file("assets/shaders/FragShader.txt" "shaders/FragShader.txt")
configure_file("assets/shaders/VertShader.txt" "shaders/VertShader.txt")
#Textures
configure_file("assets/textures/container.png" "textures/container.png")
configure_file("assets/textures/container_specular.png" "textures/container_specular.png")
configure_file("assets/textures/smudges.jpg" "textures/smudges.jpg")

target_include_directories(OpenGLLighting PUBLIC
    "include"
    "src/imgui"
)

target_link_libraries(OpenGLLighting OpenGL::GL)
target_link_libraries(OpenGLLighting ${OPENGL_LIBRARIES} glfw)