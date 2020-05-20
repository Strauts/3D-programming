#version 330
out vec3 fragColor;
in vec3 norm_cs;
in vec3 fragPos_cs;
in vec3 lightPos_cs;

uniform vec3 objectCol;
uniform vec3 ambientCol;
uniform float lightPwr;
uniform float specularPwr;
uniform vec3 lightColor;
uniform vec3 specularColor;


void main() {
    //cs = camera space
    vec3 l_dir_cs = normalize(lightPos_cs - fragPos_cs);
    vec3 view_dir_cs = normalize(-fragPos_cs);
    vec3 reflected = reflect(-l_dir_cs, norm_cs);

    float cosAlpha = clamp(dot(view_dir_cs, reflected), 0, 1);
    float cosTheta = clamp(dot(norm_cs, l_dir_cs), 0, 1);
    float distance = length(lightPos_cs - fragPos_cs);
    float mul = 1.0/distance;

    fragColor = ambientCol
            + objectCol * lightPwr * lightColor * cosTheta * mul
            + specularColor * specularPwr * pow(cosAlpha, 5) * mul;


}
