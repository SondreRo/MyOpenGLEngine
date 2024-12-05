#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application& Application::get()
{
    if (nullptr == instance)
    {
        instance = new Application;
    }

    return *instance;
}

void Application::Init()
{
    InitGLFW();
    mWindow.Init();
    mWindow.RegisterWindowCallback();

}

void Application::InitGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::LoadContent()
{
    mWindow.LoadContent();
}

void Application::Run()
{
    Init();
    LoadContent();
    float lastFrame = static_cast<float>(glfwGetTime());

    float SpeedMultiply = 1;

    while (!mWindow.IsClosed())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime > 0.2f)
        {
            deltaTime = 0.2f;
        }

      

		deltaTime *= SpeedMultiply;

        mWindow.StartFrame();

        ImGui::Begin("Speed");

        ImGui::SliderFloat("Speed", &SpeedMultiply, 0.1f, 10.0f);

        ImGui::End();

        mWindow.Update(deltaTime);
        mWindow.Render(deltaTime);
        mWindow.EndFrame();
        //std::cout << "DeltaTime: " << 1/deltaTime << std::endl;
    }

    //Mesh::ClearCache();
    //Material::ClearCache();
    //Texture::ClearCache();

    glfwTerminate();



}
