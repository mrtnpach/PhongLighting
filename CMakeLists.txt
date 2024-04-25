cmake_minimum_required(VERSION 3.15)

#Used to get root dir if CMakeLists is inside the src folder:
#cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PROJECT_PARENT_DIR)
#message(STATUS "Root directory: ${PROJECT_PARENT_DIR}")

project(PhongModel VERSION 1.0)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(glfw)

add_executable(PhongModel 
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
    src/imgui/imgui_draw.cpp 
    src/imgui/imgui_impl_glfw.cpp 
    src/imgui/imgui_impl_opengl3.cpp 
    src/imgui/imgui_tables.cpp 
    src/imgui/imgui_widgets.cpp 
)

#Shaders
configure_file("assets/shaders/CubeLightFrag.frag" "shaders/CubeLightFrag.frag" COPYONLY)
configure_file("assets/shaders/CubeLightVert.vert" "shaders/CubeLightVert.vert"  COPYONLY)
configure_file("assets/shaders/FragShader.frag" "shaders/FragShader.frag" COPYONLY)
configure_file("assets/shaders/VertShader.vert" "shaders/VertShader.vert" COPYONLY)
#Textures
configure_file("assets/textures/container.png" "textures/container.png" COPYONLY)
configure_file("assets/textures/container_specular.png" "textures/container_specular.png" COPYONLY)
configure_file("assets/textures/smudges.jpg" "textures/smudges.jpg" COPYONLY)

target_include_directories(PhongModel PUBLIC
    "include"
    "src/imgui"
)

target_link_libraries(PhongModel OpenGL::GL)
target_link_libraries(PhongModel ${OPENGL_LIBRARIES} glfw)