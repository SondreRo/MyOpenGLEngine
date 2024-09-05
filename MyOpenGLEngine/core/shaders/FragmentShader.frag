#version 330 core
out vec4 FragColor;
  

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform int useTexture;

uniform vec3 viewPos;

uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 lightDirection;


struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    vec3 ambient;
};

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

void main()
{
    DirectionalLight dl;
    dl.direction = normalize(lightDirection);
    //dl.direction = normalize(vec3(.4, -1, 0));
    dl.color = vec3(1, 1, 1);
    dl.ambient = vec3(0.1);

    Material mat;
    //mat.diffuse = vec3(0,1,0);
    //mat.diffuse = vec3(.6,.6,0);
    //mat.specular = vec3(.5);
    //mat.shininess = 64;

    if (useTexture == 0)
    {
        mat.diffuse = diffuse;
    }
    else
    {
        mat.diffuse = vec3(texture(ourTexture, TexCoord)) * diffuse;
    }

    mat.diffuse = diffuse;

    mat.specular = specular;
    mat.shininess = shininess;

    // diffuse angle
    float NdL = max(dot(Normal, -dl.direction), 0);

    // variables to calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(dl.direction, Normal);
    float VdR = pow(max(dot(viewDir, reflectDir), 0), mat.shininess); 

    // specular light
    vec3 specularContribution = VdR * mat.specular * dl.color;

    // diffuse light
    vec3 diffuseContribution = NdL * mat.diffuse * dl.color;

    // ambient light
    vec3 ambientContribution = dl.ambient * mat.diffuse;

    // final color
    vec3 finalColor = ambientContribution + diffuseContribution + specularContribution;

    FragColor = vec4(finalColor, 1);
    //FragColor = texture(ourTexture, TexCoord) * vec4(finalColor, 1);  
}