#version 330 core
//uniform sampler2D qt_Texture0;
//varying vec4 qt_TexCoord0;
in vec2 UV;
out vec4 color;
uniform sampler2D myTextureSampler;
uniform float LifeLevel;

void main(void)
{
    color = texture(myTextureSampler, UV);
}
