#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application* Application::Get() {
    static Application* app = new Application;
    return app;
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

    while (!mWindow.IsClosed())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime > 0.2f)
        {
            deltaTime = 0.2f;
        }

        mWindow.StartFrame();
        mWindow.Update(deltaTime);
        mWindow.Render(deltaTime);
        mWindow.EndFrame();
    }

    //Mesh::ClearCache();
    //Material::ClearCache();
    //Texture::ClearCache();

    glfwTerminate();



}
