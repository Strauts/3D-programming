#version 330 core

layout(location = 0) in vec3 squareVertices;

out vec2 UV;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VPmatrix;
uniform vec3 BillboardPos;
uniform vec2 BillboardSize;



void main(void)
{
    vec3 particleCenter_worldspace = BillboardPos;

    vec3 vertexPosition_worldspace =
            particleCenter_worldspace
            + CameraRight_worldspace * squareVertices * BillboardSize.x;
            + CameraUp_worldspace * squareVertices * BillboardSize.y;

    gl_Position = VPmatrix * vec4(vertexPosition_worldspace, 1.0f);
}
