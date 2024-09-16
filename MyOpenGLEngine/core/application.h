#pragma once
#include <iostream>

#include "Scene.h"
#include "Window.h"

class Application {
public:
    static Application& get();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Init();
    void InitGLFW();
    void LoadContent();
    void Run();

    Scene mScene{ "Scene" };

private:
    inline static Application* instance = nullptr;

    Application() = default;


    Window mWindow{ "LearnOpenGL", &mScene, 1280, 720 };
};
