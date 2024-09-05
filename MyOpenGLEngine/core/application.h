#pragma once
#include <iostream>

#include "Scene.h"
#include "Window.h"

class Application {
public:
    Application(const Application&) = delete;
    Application(Application&&) = delete;

    static Application* Get();

    void Init();
    void InitGLFW();
    void LoadContent();
    void Run();
private:
    Application() = default;

    Scene mScene{ "Scene" };
    Window mWindow{ "LearnOpenGL", &mScene, 1280, 720 };
};
