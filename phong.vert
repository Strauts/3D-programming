#version 330
layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 colAttr;
layout(location = 2) in vec2 uv;
out vec3 norm_cs;
out vec3 fragPos_cs;
out vec3 lightPos_cs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main() {
    mat4 matrix = projection * view * model;
    gl_Position = matrix * vec4(posAttr, 1);

    //Camera space
    vec4 vertex_c = view * model * vec4(posAttr, 1);
    vec4 normal_c = view * model * vec4(colAttr, 0);
    vec4 lightPos_c = view * vec4(lightPos, 1);

    fragPos_cs = vertex_c.xyz;
    norm_cs = normalize(normal_c.xyz);
    lightPos_cs = lightPos_c.xyz;
}
