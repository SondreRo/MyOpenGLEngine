#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "FrameBuffer.h"
#include "Window.h"


Window::Window(std::string name, Scene* scene, int width, int height)
{
	mName = name;
	mScene = scene;
	mWidth = width;
    mHeight = height;
}

Window::~Window()
{
    glfwDestroyWindow(mGLFWWindow);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Window::Init() 
{
    mGLFWWindow = glfwCreateWindow(mWidth, mHeight, "LearnOpenGL", NULL, NULL);
    if (mGLFWWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(mGLFWWindow);
    glfwSetWindowUserPointer(mGLFWWindow, this);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, mWidth, mHeight);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mGLFWWindow, false);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    mFramebuffer = new FrameBuffer(mWidth, mHeight);

    //Vsync
	glfwSwapInterval(0);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Draw one side of triangle
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    if (mScene)
    {
        mScene->Init();
    }
}

void Window::RegisterWindowCallback()
{
    glfwSetFramebufferSizeCallback(mGLFWWindow, [](GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->FramebufferSizeCallback(window, width, height);
            //app->SetWidth(width);
            //app->SetHeight(height);
        }
        });

    glfwSetCursorPosCallback(mGLFWWindow, [](GLFWwindow* window, double xpos, double ypos) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        if (app) app->MouseMoveCallback(window, xpos, ypos);
        });
    glfwSetMouseButtonCallback(mGLFWWindow, [](GLFWwindow* window, int button, int action, int mods) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        if (app) app->MouseButtonCallback(window, button, action, mods);
        });

    glfwSetScrollCallback(mGLFWWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        if (app) app->MouseScrollCallback(window, xoffset, yoffset);
        });

    glfwSetKeyCallback(mGLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        if (app) app->KeyCallback(window, key, scancode, action, mods);
        });

    glfwSetCharCallback(mGLFWWindow, [](GLFWwindow* window, unsigned int c) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        if (app) app->CharCallback(window, c);
        });

}

bool Window::LoadContent()
{
    	if (mScene)
    	{
            mScene->LoadContent();
            mScene->Bind();
    	}
        return true;
}

void Window::StartFrame()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    mFramebuffer->Bind();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update(float DeltaTime)
{
    // scene stuff
    if (mScene)
        mScene->Update(DeltaTime);

	ProcessInput(mGLFWWindow, DeltaTime);
}

void Window::Render(float DeltaTime)
{
    // Imgui stuff
    bool Open = true;
    OpenDockSpace(&Open);

    ImGui::Begin("Properties");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::ShowDemoWindow(); // Show demo window! :)

    ImGui::Begin("Scene");
    {
        ImGui::BeginChild("GameRender");

        float width = ImGui::GetContentRegionAvail().x;
        float height = ImGui::GetContentRegionAvail().y;

        mWidth = width;
        mHeight = height;
        ImGui::Image(
            (ImTextureID)mFramebuffer->getFrameTexture(),
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    }
    ImGui::EndChild();
    ImGui::End();

    // scene stuff
    if (mScene)
        mScene->Render(DeltaTime, mWidth, mHeight);
}

void Window::EndFrame()
{
    mFramebuffer->Unbind();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwPollEvents();
    glfwSwapBuffers(mGLFWWindow);

}

void Window::OpenDockSpace(bool* p_open)
{
    // Variables to configure the Dockspace example.
    static bool opt_fullscreen = true; // Is the Dockspace full-screen?
    static bool opt_padding = false; // Is there padding (a blank space) between the window edge and the Dockspace?
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; // Config flags for the Dockspace

    // In this example, we're embedding the Dockspace into an invisible parent window to make it more configurable.
    // We set ImGuiWindowFlags_NoDocking to make sure the parent isn't dockable into because this is handled by the Dockspace.
    //
    // ImGuiWindowFlags_MenuBar is to show a menu bar with config options. This isn't necessary to the functionality of a
    // Dockspace, but it is here to provide a way to change the configuration flags interactively.
    // You can remove the MenuBar flag if you don't want it in your app, but also remember to remove the code which actually
    // renders the menu bar, found at the end of this function.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    // Is the example in Fullscreen mode?
    if (opt_fullscreen)
    {
        // If so, get the main viewport:
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Set the parent window's position, size, and viewport to match that of the main viewport. This is so the parent window
        // completely covers the main viewport, giving it a "full-screen" feel.
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Set the parent window's styles to match that of the main viewport:
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

        // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        // The example is not in Fullscreen mode (the parent window can be dragged around and resized), disable the
        // ImGuiDockNodeFlags_PassthruCentralNode flag.
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so the parent window should not have its own background:
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // If the padding option is disabled, set the parent window's padding size to 0 to effectively hide said padding.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("DockSpace Demo", p_open, window_flags);

    // Remove the padding configuration - we pushed it, now we pop it:
    if (!opt_padding)
        ImGui::PopStyleVar();

    // Pop the two style rules set in Fullscreen mode - the corner rounding and the border size.
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Check if Docking is enabled:
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        // If it is, draw the Dockspace with the DockSpace() function.
        // The GetID() function is to give a unique identifier to the Dockspace - here, it's "MyDockSpace".
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        // Docking is DISABLED - Show a warning message
       // ShowDockingDisabledMessage();
    }

    // This is to show the menu bar that will change the config settings at runtime.
    // If you copied this demo function into your own code and removed ImGuiWindowFlags_MenuBar at the top of the function,
    // you should remove the below if-statement as well.
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            // Display a menu item for each Dockspace flag, clicking on one will toggle its assigned flag.
            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            // Display a menu item to close this example.
            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                if (p_open != NULL) // Remove MSVC warning C6011 (NULL dereference) - the `p_open != NULL` in MenuItem() does prevent NULL derefs, but IntelliSense doesn't analyze that deep so we need to add this in ourselves.
                    *p_open = false; // Changing this variable to false will close the parent window, therefore closing the Dockspace as well.
            ImGui::EndMenu();
        }

        // Show a help marker that gives an overview of what this example is and does.

        ImGui::EndMenuBar();
    }

    // End the parent window that contains the Dockspace:
    ImGui::End();
}

bool Window::IsClosed()
{
    return glfwWindowShouldClose(mGLFWWindow);
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    //mWidth = width;
    //mHeight = height;
    mFramebuffer->RescaleFrameBuffer(width, height);
	std::cout << "Width: " << width << " Height: " << height << std::endl;
    if (mScene)
        mScene->FramebufferSizeCallback(this, width, height);
}

void Window::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    //if (ImGui::GetIO().WantCaptureMouse) return;

    if (mScene)
        mScene->MouseMoveCallback(this, xpos, ypos);
}

void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    //if (ImGui::GetIO().WantCaptureMouse) return;

    if (mScene)
        mScene->MouseButtonCallback(this, button, action, mods);
}

void Window::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    //if (ImGui::GetIO().WantCaptureMouse) return;

    if (mScene)
        mScene->MouseScrollCallback(this, xoffset, yoffset);
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
		// Wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		// Fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

    if (mScene)
        mScene->KeyCallback(this, key, scancode, action, mods);
}

void Window::CharCallback(GLFWwindow* window, unsigned int c)
{
    ImGui_ImplGlfw_CharCallback(window, c);
}

void Window::ProcessInput(GLFWwindow* window, float DeltaTime)
{
	if (mScene)
		mScene->ProcessInput(this, DeltaTime);
}

void Window::SetWidth(int width)
{
    mWidth = width;
}

void Window::SetHeight(int height)
{
	mHeight = height;
}

int Window::GetWidth() const
{
    return mWidth;
}

int Window::GetHeight() const
{
    return mHeight;
}
