#include "Material.h"

#include "glm/gtc/type_ptr.hpp"

void Material::BindMaterial(ShaderProgram* shader)
{
    // uniform vec3 diffuse;
    // uniform vec3 specular;
    // uniform float shininess;
    //glUniformMatrix3fv(glGetUniformLocation(shader.shaderProgram, "diffuse"), 1, GL_FALSE, glm::value_ptr(diffuse));
    //glUniformMatrix3fv(glGetUniformLocation(shader.shaderProgram, "specular"), 1, GL_FALSE, glm::value_ptr(specular));

    glUniform3fv(glGetUniformLocation(shader->shaderProgram, "diffuse"), 1, glm::value_ptr(diffuse));
    glUniform3fv(glGetUniformLocation(shader->shaderProgram, "specular"), 1, glm::value_ptr(specular));
    glUniform1f(glGetUniformLocation(shader->shaderProgram, "shininess"), shininess);
    glUniform1f(glGetUniformLocation(shader->shaderProgram, "transparency"), transparency);

}