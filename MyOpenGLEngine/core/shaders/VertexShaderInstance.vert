#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;
layout (location = 4) in mat4 instanceMatrix;


uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 VerColor;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    //FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));
    //Normal = normalize(mat3(transpose(inverse(instanceMatrix))) * aNormal);
    
    FragPos = aPos;
    Normal = aNormal;

    TexCoord = aTexCoord;
    VerColor = aColor;
}