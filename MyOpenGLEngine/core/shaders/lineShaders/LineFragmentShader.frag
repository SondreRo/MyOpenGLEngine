#version 330 core
out vec4 FragColor;
  
in vec3 myColor;
in vec3 myNormal;
uniform int useColorNormal = 0;
uniform vec3 diffuse;

void main()
{
    vec3 finalColor = diffuse;

    if (useColorNormal == 1)
    {
    finalColor = myNormal;
    }

    FragColor = vec4(finalColor, 1);//texture(ourTexture, TexCoord);  
}