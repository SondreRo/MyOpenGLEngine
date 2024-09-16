#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ShaderProgram.h>


struct Material {

    void BindMaterial(ShaderProgram& shader);


    glm::vec3 diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
    glm::vec3 specular = glm::vec3(0.5f);
    float shininess = 64.f;
    float transparency = 1.f;
};