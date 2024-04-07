#version 460 core
out vec4 FragColor;

uniform vec3 fLightColor;

void main()
{
    FragColor = vec4(fLightColor, 1.0);
    //FragColor = vec4(1.0); // set all 4 vector values to 1.0
}