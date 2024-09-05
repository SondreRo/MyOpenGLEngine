#include "Scene.h"

#include <iostream>
#include <glad/glad.h>
#include "Application.h"
#include "Ball.h"
#include "imgui.h"
#include "Vertex.h"
#include "glm/gtc/type_ptr.hpp"
#include "MeshGenerator.h"
#include "Material.h"

Scene::Scene(const std::string& name)
{
}

Scene::~Scene()
{
}

void Scene::LoadContent()
{

	camera = new Camera();


	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->ReadShaderFile("../../../core/shaders/VertexShader.vert", "../../../core/shaders/FragmentShader.frag");
	//shaderProgram->ReadShaderFile("D:/OpenGL/MyEngine/MyOpenGLEngine/core/shaders/VertexShader.vert", "D:/OpenGL/MyEngine/MyOpenGLEngine/core/shaders/FragmentShader.frag");
	shaderProgram->CompileShaders();
	shaderProgram->CreateProgram();

	RootMesh = new Mesh("RootMesh");
	RootMesh->shaderProgram = shaderProgram;


	Material* material = new Material();
	material->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	material->specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material->shininess = 32.0f;


	SunMesh = new Mesh("SunMesh");
	Meshes["SunMesh"] = SunMesh;
	MeshGenerator::GenerateCube(SunMesh, glm::vec3(0.1f));
	SunMesh->shaderProgram = shaderProgram;
	SunMesh->transform.SetRotation(glm::vec3(33.35f, 0.f, -20.5f));
	RootMesh->Children.push_back(SunMesh);



	Mesh* TriangleMesh = new Mesh("TriangelMesh");
	Meshes["TriangleMesh"] = TriangleMesh;
	MeshGenerator::GenerateCubeWithHardEdges(TriangleMesh, glm::vec3(1.0f, 1.0f, 1.0f));
	TriangleMesh->shaderProgram = shaderProgram;
	RootMesh->Children.push_back(TriangleMesh);

	Mesh* BoxMesh = new Mesh("BoxMesh");
	Meshes["BoxMesh"] = BoxMesh;
	MeshGenerator::GenerateCubeWithHardEdges(BoxMesh, glm::vec3(0.5f, 0.5f, 0.5f));
	
	BoxMesh->transform.SetLocation(glm::vec3(2.0f, 0.0f, 0.0f));
	BoxMesh->shaderProgram = shaderProgram;
	RootMesh->Children.push_back(BoxMesh);

	Mesh* LongBox = new Mesh("LongBox");
	Meshes["LongBox"] = LongBox;
	MeshGenerator::GenerateCubeWithHardEdges(LongBox, glm::vec3(1.5f, 0.5f, 0.5f));

	LongBox->transform.SetLocation(glm::vec3(-2.0f, 0.0f, 0.0f));
	LongBox->shaderProgram = shaderProgram;
	TriangleMesh->Children.push_back(LongBox);

	Ball* SphereMesh = new Ball("SphereMesh");
	SphereMesh->velocity = glm::vec3(.1f, 0.0f, 0.0f);
	Meshes["SphereMesh"] = SphereMesh;
	MeshGenerator::GenerateIcosahedron(SphereMesh, 2);
	SphereMesh->transform.SetLocation(glm::vec3(0.0f, 2.0f, 0.0f));
	SphereMesh->shaderProgram = shaderProgram;
	RootMesh->Children.push_back(SphereMesh);
	

	Mesh* FloorMesh = new Mesh("FloorMesh");
	Meshes["FloorMesh"] = FloorMesh;
	FloorMesh->shaderProgram = shaderProgram;
	FloorMesh->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);
	FloorMesh->material.shininess = 16.f;

	Mesh* RightWall = new Mesh("RightWall");
	Meshes["RightWall"] = RightWall;
	RightWall->shaderProgram = shaderProgram;

	Mesh* LeftWall = new Mesh("LeftWall");
	Meshes["LeftWall"] = LeftWall;
	LeftWall->shaderProgram = shaderProgram;

	Mesh* TopWall = new Mesh("TopWall");
	Meshes["TopWall"] = TopWall;
	TopWall->shaderProgram = shaderProgram;

	Mesh* BottomWall = new Mesh("BottomWall");
	Meshes["BottomWall"] = BottomWall;
	BottomWall->shaderProgram = shaderProgram;

	MeshGenerator::GeneratePoolTable(FloorMesh, TopWall, BottomWall, RightWall, LeftWall, glm::vec3(10.0f, 0.5f, 5.0f));

	FloorMesh->Children.push_back(RightWall);
	FloorMesh->Children.push_back(LeftWall);
	FloorMesh->Children.push_back(TopWall);
	FloorMesh->Children.push_back(BottomWall);

	RootMesh->Children.push_back(FloorMesh);







}

void Scene::UnloadContent()
{

}

void Scene::Bind()
{
	BindSceneGraph(RootMesh);
}

void Scene::BindSceneGraph(Mesh* mesh)
{
	mesh->Bind();
	for (auto child : mesh->Children )
	{
		BindSceneGraph(child);
	}
}

void Scene::Init()
{
}

void Scene::Update(float DeltaTime)
{
	UpdateSceneGraph(DeltaTime, RootMesh);
}

void Scene::UpdateSceneGraph(float DeltaTime, Mesh* mesh)
{
	mesh->Update(DeltaTime);
	if (mesh->Children.size() == 0)
	{
		return;
	}
	for (auto child : mesh->Children)
	{
		UpdateSceneGraph(DeltaTime, child);
	}
}

void Scene::Render(float DeltaTime, int width, int height)
{
	//std::cout << "Rendering Scene" << std::endl;

	//shaderProgram->UseProgram();
	// Camera
   


	// SceneGraph
	RenderSceneGraph(RootMesh, DeltaTime, width, height, glm::mat4(1));

	RenderSceneGraphVisuals();


	ImGui::Begin("Mesh Properties");
	if (SelectedMesh != nullptr)
	{
		SelectedMesh->RenderProperties();

	}
	ImGui::End();




}

void Scene::RenderSceneGraph(Mesh* mesh, float DeltaTime, int width, int height, glm::mat4 ParentTransform)
{
	//std::cout << "Rendering Mesh: " << mesh->name << std::endl;
	ShaderProgram* CurrentShader = mesh->shaderProgram;
	CurrentShader->UseProgram();

	// Camera
	float Aspect = (float)width / (float)height;
	glm::mat4 projection = glm::perspective(glm::radians(90.f), Aspect, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(CurrentShader->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(CurrentShader->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	// Mesh
	glUniformMatrix4fv(glGetUniformLocation(CurrentShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(ParentTransform * mesh->transform.GetMatrix()));


	unsigned int test3 = glGetUniformLocation(CurrentShader->shaderProgram, "lightDirection");
	glUniform3fv(test3, 1, glm::value_ptr((-SunMesh->transform.GetUpVector())));

	// Material

	//mesh->material->BindMaterial(*CurrentShader);

	glUniform3fv(glGetUniformLocation(CurrentShader->shaderProgram, "viewPos"), 1, glm::value_ptr(camera->cameraPos));


	glUniform3fv(glGetUniformLocation(CurrentShader->shaderProgram, "diffuse"), 1, glm::value_ptr(mesh->material.diffuse));
	glUniform3fv(glGetUniformLocation(CurrentShader->shaderProgram, "specular"), 1, glm::value_ptr(mesh->material.specular));
	glUniform1f(glGetUniformLocation(CurrentShader->shaderProgram, "shininess"), mesh->material.shininess);





	mesh->Draw();


	/*ImGui::Begin(mesh->name.c_str());
	glm::vec3 position = mesh->transform.GetLocation();
	ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f);
	TriangleMesh->transform.SetLocation(position);

	glm::vec3 rotation = mesh->transform.GetRotation();
	ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), -360.0f, 360.0f);
	mesh->transform.SetRotation(rotation);
	ImGui::End();*/

	if (mesh->Children.size() == 0)
	{
		return;
	}
	for (auto child : mesh->Children)
	{
		RenderSceneGraph(child, DeltaTime, width, height, ParentTransform * mesh->transform.GetMatrix());
	}

}

void Scene::RenderSceneGraphVisuals()
{
	ImGui::Begin("SceneGraph");

	if (ImGui::Button("Root"))
	{
		SelectMesh(RootMesh);
	};
	for (auto child : RootMesh->Children)
	{
		RenderSceneGraphVisualsChldren(child, 1);
	}

	ImGui::End();

}

void Scene::RenderSceneGraphVisualsChldren(Mesh* mesh, int Depth)
{
	std::string DepthString = "";
	for (int i = 0; i < Depth; i++)
	{
		DepthString += "  ";
	}
	DepthString += mesh->name;
	if (ImGui::Button(DepthString.c_str()))
	{
		SelectMesh(mesh);
	}

	for (auto child : mesh->Children)
	{
		RenderSceneGraphVisualsChldren(child, Depth + 1);
	}

}

void Scene::SelectMesh(Mesh* mesh)
{
	SelectedMesh = mesh;
}

void Scene::FramebufferSizeCallback(Window* window, int width, int height)
{
}

void Scene::MouseMoveCallback(Window* window, double xpos, double ypos)
{
    camera->MouseMoveCallback(window->GetGLFWWindow(), xpos, ypos);
}

void Scene::MouseButtonCallback(Window* window, int button, int action, int mods)
{
	camera->MouseButtonCallback(window->GetGLFWWindow(), button, action, mods);
}

void Scene::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{
	camera->MouseScrollCallback(window->GetGLFWWindow(), xoffset, yoffset);
}

void Scene::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{
	camera->KeyCallback(window->GetGLFWWindow(), key, scancode, action, mods);
}

void Scene::ProcessInput(Window* window, float DeltaTime)
{
	camera->ProcessInput(window->GetGLFWWindow(), DeltaTime);
}
