#version 330 core
out vec4 FragColor;
  
in vec3 myColor;

void main()
{
    vec3 finalColor = myColor;

    FragColor = vec4(myColor, 1);//texture(ourTexture, TexCoord);  
}