#include "test.h"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "framebuffer.h"
#include "ShaderProgram.h"

#include "mesh.h"
#include <glm/gtc/type_ptr.hpp>

int Test::MyTest()
{
	std::cout << "Hello from test class" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   

    GLFWwindow* window = glfwCreateWindow(Width, Height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	//glEnable(GL_DEPTH_TEST);

    camera = new Camera();

    glViewport(0, 0, Width, Height);

	Mesh* TriangleMesh = new Mesh();
	std::vector<Vertex> vertices;
    
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f))); // Bottom-right
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 0.0f))); // Top-left

    // Back face (2 triangles)
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 1.0f))); // Bottom-right
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.5f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.5f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 1.0f, 0.5f))); // Top-left

    // Left face (2 triangles)
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 1.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.5f, 0.5f))); // Bottom-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 1.0f, 0.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 1.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 1.0f, 0.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.5f))); // Top-left

    // Right face (2 triangles)
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.5f, 0.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 1.0f))); // Bottom-right
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.5f, 0.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.0f, 1.0f))); // Top-left

    // Top face (2 triangles)
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f))); // Bottom-right
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.5f))); // Top-left

    // Bottom face (2 triangles)
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.5f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f))); // Bottom-right
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.5f))); // Bottom-left
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f))); // Top-right
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 0.0f))); // Top-left



    TriangleMesh->vertices = vertices;
  

    TriangleMesh->Bind();

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Test*>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->WindowFramebufferSizeCallback(width, height);
        }
        });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        auto app = reinterpret_cast<Test*>(glfwGetWindowUserPointer(window));
        if (app) app->MouseMoveCallback(window, xpos, ypos);
        });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        auto app = reinterpret_cast<Test*>(glfwGetWindowUserPointer(window));
        if (app) app->MouseButtonCallback(window, button, action, mods);
        });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto app = reinterpret_cast<Test*>(glfwGetWindowUserPointer(window));
        if (app) app->MouseScrollCallback(window, xoffset, yoffset);
        });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto app = reinterpret_cast<Test*>(glfwGetWindowUserPointer(window));
        if (app) app->KeyCallback(window, key, scancode, action, mods);
        });





    // Imgui
// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Imgui

    framebuffer = new FrameBuffer(Width, Height);

   
    ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->ReadShaderFile("D:/OpenGL/MyEngine/MyOpenGLEngine/core/shaders/VertexShader.vert", "D:/OpenGL/MyEngine/MyOpenGLEngine/core/shaders/FragmentShader.frag");
	shaderProgram->CompileShaders();
    shaderProgram->CreateProgram();
    //TriangleMesh->transform.AddLocation(glm::vec3(0.0f, 0.0f, -3.f));
    float RotSpeedX = 0;
    float RotSpeedY = 0;
    float RotSpeedZ = 0;

    glEnable(GL_DEPTH_TEST);

    

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        framebuffer->Bind();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bool yes = true;
        OpenDockSpace(&yes);

        
        //ImGui::ShowDemoWindow(); // Show demo window! :)


		ImGui::Begin("Properties");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::SliderFloat("Rotation Speed X", &RotSpeedX, -180.0f, 180.0f);
		ImGui::SliderFloat("Rotation Speed Y", &RotSpeedY, -180.0f, 180.0f);
		ImGui::SliderFloat("Rotation Speed Z", &RotSpeedZ, -180.0f, 180.0f);

		ImGui::Value("XRotation", TriangleMesh->transform.GetRotation().x);
		ImGui::Value("YRotation", TriangleMesh->transform.GetRotation().y);
		ImGui::Value("ZRotation", TriangleMesh->transform.GetRotation().z);

        if (ImGui::Button("Reset Rotation")) {
			TriangleMesh->transform.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
            RotSpeedX = 0;
			RotSpeedY = 0;
			RotSpeedZ = 0;
        }


		ImGui::End();

        TriangleMesh->transform.AddRotation(glm::vec3(RotSpeedX * deltaTime, RotSpeedY * deltaTime, RotSpeedZ * deltaTime));



        shaderProgram->UseProgram();

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(TriangleMesh->GetTransformMat()));
        //TriangleMesh->Draw();
        

		float Aspect = (float)Width / (float)Height;
        glm::mat4 projection = glm::perspective(glm::radians(90.f), Aspect, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));


        glfwPollEvents();



        ImGui::Begin("Scene");
        {
            ImGui::BeginChild("GameRender");

            float width = ImGui::GetContentRegionAvail().x;
            float height = ImGui::GetContentRegionAvail().y;

            Width = width;
            Height = height;
            ImGui::Image(
                (ImTextureID)framebuffer->getFrameTexture(),
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        

        }
        ImGui::EndChild();
        ImGui::End();
       

        ImGui::Begin("Camera");
		ImGui::Value("Pos X", camera->cameraPos.x);
		ImGui::Value("Pos Y", camera->cameraPos.y);
		ImGui::Value("Pos Z", camera->cameraPos.z);
		ImGui::Value("Speed", camera->CameraSpeed);

        ImGui::End();



        framebuffer->Unbind();

        glfwSwapBuffers(window);






        //glClear(GL_COLOR_BUFFER_BIT);
       


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete TriangleMesh;
    delete camera;

    glfwTerminate();
}

void Test::OpenDockSpace(bool* p_open)
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

void Test::WindowFramebufferSizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
    Width = width;
	Height = height;
	framebuffer->RescaleFrameBuffer(width, height);
	std::cout << "Width: " << width << " Height: " << height << std::endl;
}

void Test::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	camera->ProcessInput(window, deltaTime);
}

void Test::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << "Mouse X: " << xpos << " Mouse Y: " << ypos << std::endl;
	camera->MouseMoveCallback(window, xpos, ypos);
}

void Test::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << std::endl;
	camera->MouseButtonCallback(window, button, action, mods);
}

void Test::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << "XOffset: " << xoffset << " YOffset: " << yoffset << std::endl;
	camera->MouseScrollCallback(window, xoffset, yoffset);
}

void Test::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << std::endl;
	camera->KeyCallback(window, key, scancode, action, mods);
}
