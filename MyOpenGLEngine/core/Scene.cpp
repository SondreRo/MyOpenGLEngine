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
#include <queue>
#include <chrono>
#include "actor.h"

Scene::Scene(const std::string& name)
{
}

Scene::~Scene()
{
}

void Scene::LoadContent()
{
	


	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->ReadShaderFile("../../../core/shaders/VertexShader.vert", "../../../core/shaders/FragmentShader.frag");

	ShaderProgram* lineShaderProgram = new ShaderProgram();
	lineShaderProgram->ReadShaderFile("../../../core/shaders/lineShaders/LineVertexShader.vert", "../../../core/shaders/lineShaders/LineFragmentShader.frag");

	//shaderProgram->ReadShaderFile("D:/OpenGL/MyEngine/MyOpenGLEngine/core/shaders/VertexShader.vert", "D:/OpenGL/MyEngine/MyOpenGLEngine/core/shaders/FragmentShader.frag");
	shaderProgram->CompileShaders();
	shaderProgram->CreateProgram();

	lineShaderProgram->CompileShaders();
	lineShaderProgram->CreateProgram();

	Shaders["DefaultShader"] = shaderProgram;
	Shaders["LineShader"] = lineShaderProgram;

	RootMesh = new Mesh("RootMesh");
	RootMesh->shaderProgram = shaderProgram;

	SunMesh = new Mesh("SunMesh");
	Meshes["SunMesh"] = SunMesh;
	MeshGenerator::GenerateCube(SunMesh, glm::vec3(0.1f));
	SunMesh->shaderProgram = shaderProgram;
	SunMesh->transform.SetRotation(glm::vec3(-33.35f, 0.f, -20.5f));
	RootMesh->Children.push_back(SunMesh);

	lineMesh = new LineMesh("LineMesh");
	lineMesh->shaderProgram = shaderProgram;
	RootMesh->Children.push_back(lineMesh);
	lineMesh->AddLine(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));

	Mesh* NodeTreeMesh = new Mesh("NodeTreeMesh");
	Meshes["NodeTreeMesh"] = NodeTreeMesh;
	MeshGenerator::GenerateCube(NodeTreeMesh, glm::vec3(1.f));
	NodeTreeMesh->shaderProgram = shaderProgram;
	NodeTreeMesh->material.diffuse = glm::vec3(1.f, 0.0f, 0.0f);
	NodeTreeMesh->material.transparency = 0.1f;
	NodeTreeMesh->Bind();






	//Mesh* BigBoxMesh = CreateAndRegisterMesh<Mesh>("BigBoxMesh", glm::vec3(-7.f, 7, 0), glm::vec3(0.f), glm::vec3(1.f), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateCubeWithHardEdges(BigBoxMesh, glm::vec3(1.0f, 1.f, 1.0f));


	Mesh* FloorMesh = CreateAndRegisterMesh<Mesh>("FloorMesh", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, nullptr, true);
	FloorMesh->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);
	FloorMesh->material.shininess = 8.f;

	Mesh* RighWall = CreateAndRegisterMesh<Mesh>("RighWall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	RighWall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	Mesh* Leftwall = CreateAndRegisterMesh<Mesh>("Leftwall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	Leftwall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	Mesh* TopWall = CreateAndRegisterMesh<Mesh>("TopWall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	TopWall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	Mesh* BottomWall = CreateAndRegisterMesh<Mesh>("BottomWall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	BottomWall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	MeshGenerator::GeneratePoolTable(FloorMesh, TopWall, BottomWall, RighWall, Leftwall, glm::vec3(200.f, 10.f, 200.f));
	//MeshGenerator::GeneratePoolTable(FloorMesh, TopWall, BottomWall, RighWall, Leftwall, glm::vec3(20, 2, 40));

	FloorMesh->Static = true;
	RighWall->Static = true;
	Leftwall->Static = true;
	TopWall->Static = true;
	BottomWall->Static = true;

	//for (int i = 0; i < 10; ++i)
	//{

	//}
	//Ball* BallMesh = CreateAndRegisterMesh<Ball>("BallMesh", glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh, 3);

	//Ball* BallMesh2 = CreateAndRegisterMesh<Ball>("BallMesh2", glm::vec3(4, 1, 0), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh2, 3);

	//Ball* BallMesh3 = CreateAndRegisterMesh<Ball>("BallMesh3", glm::vec3(6, 1, 0), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh3, 3);

	// Create balls in a pooltable

	//     *
	//    * *
	//   * * *
	//  * * * *
	// * * * * *

	// generate this strucutre

	//Ball* MainBall = CreateAndRegisterMesh<Ball>("MainBall", glm::vec3(0, 1, 4), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(MainBall, 3);
	//MainBall->material.diffuse = glm::vec3(1.f, 1.f, 1.f);


	//Ball* BallMesh = CreateAndRegisterMesh<Ball>("BallMesh", glm::vec3(0, 1, -5), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh, 3);
	//// Yellow
	//BallMesh->material.diffuse = glm::vec3(1.f, 1.f, 0.f);

	//Ball* BallMesh2 = CreateAndRegisterMesh<Ball>("BallMesh2", glm::vec3(1, 1, -6), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh2, 3);
	//// Blue
	//BallMesh2->material.diffuse = glm::vec3(0.f, 0.f, 1.f);

	//Ball* BallMesh3 = CreateAndRegisterMesh<Ball>("BallMesh3", glm::vec3(-1, 1, -6), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh3, 3);
	//// Red
	//BallMesh3->material.diffuse = glm::vec3(1.f, 0.f, 0.f);

	//Ball* BallMesh4 = CreateAndRegisterMesh<Ball>("BallMesh4", glm::vec3(2, 1, -7), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh4, 3);
	//// Purple
	//BallMesh4->material.diffuse = glm::vec3(1.f, 0.f, 1.f);

	//Ball* BallMesh5 = CreateAndRegisterMesh<Ball>("BallMesh5", glm::vec3(0, 1, -7), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh5, 3);
	//// Orange
	//BallMesh5->material.diffuse = glm::vec3(1.f, 0.5f, 0.f);

	//Ball* BallMesh6 = CreateAndRegisterMesh<Ball>("BallMesh6", glm::vec3(-2, 1, -7), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh6, 3);
	//// Green
	//BallMesh6->material.diffuse = glm::vec3(0.f, 1.f, 0.f);

	//Ball* BallMesh7 = CreateAndRegisterMesh<Ball>("BallMesh7", glm::vec3(3, 1, -8), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh7, 3);
	//// Brown
	//BallMesh7->material.diffuse = glm::vec3(0.5f, 0.25f, 0.f);

	//Ball* BallMesh8 = CreateAndRegisterMesh<Ball>("BallMesh8", glm::vec3(1, 1, -8), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh8, 3);
	//// Black
	//BallMesh8->material.diffuse = glm::vec3(0.f, 0.f, 0.f);

	//Ball* BallMesh9 = CreateAndRegisterMesh<Ball>("BallMesh9", glm::vec3(-1, 1, -8), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh9, 3);
	//// Pin
	//BallMesh9->material.diffuse = glm::vec3(1.f, 0.5f, 1.f);

	//Ball* BallMesh10 = CreateAndRegisterMesh<Ball>("BallMesh9", glm::vec3(-3, 1, -8), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh10, 3);
	//// Light Blue
	//BallMesh10->material.diffuse = glm::vec3(0.f, 1.f, 1.f);


	//Ball* BallMesh11 = CreateAndRegisterMesh<Ball>("BallMesh10", glm::vec3(2, 1, -9), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh11, 3);
	//// Light Blue
	//BallMesh11->material.diffuse = glm::vec3(0.f, 1.f, 1.f);

	//Ball* BallMesh12 = CreateAndRegisterMesh<Ball>("BallMesh11", glm::vec3(0, 1, -9), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh12, 3);
	//// Light Green
	//BallMesh12->material.diffuse = glm::vec3(0.5f, 1.f, 0.5f);

	//Ball* BallMesh13 = CreateAndRegisterMesh<Ball>("BallMesh12", glm::vec3(-2, 1, -9), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateIcosahedron(BallMesh13, 3);
	//// Light Yellow
	//BallMesh13->material.diffuse = glm::vec3(1.f, 1.f, 0.5f);

	//int BallCount = 0;

	//for (int i = 0; i < 10; i++)
	//{
	//	for (int y = 0; y < 10; y++)
	//	{
	//		glm::vec3 randomvelocity = glm::vec3(rand() % 20, 0, rand() % 20) - 10.f;
	//		randomvelocity.y = 0;
	//		std::string BallName = "BallMesh" + std::to_string(i) + std::to_string(y);
	//		Ball* BallMesh = CreateAndRegisterMesh<Ball>(BallName, glm::vec3(2 * i, 2, 2 * y), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	//		MeshGenerator::GenerateIcosahedron(BallMesh, 3);
	//		BallMesh->velocity = randomvelocity;
	//		BallMesh->material.diffuse = glm::vec3(rand() % 100 / 100.f, rand() % 100 / 100.f, rand() % 100 / 100.f);
	//		BallCount++;
	//	}
	//	
	//}
	//std::cout << "Created " << BallCount << " Balls" << std::endl;


	/*Ball* BallMesh2 = CreateAndRegisterMesh<Ball>("BallMesh2", glm::vec3(5, 5, 0), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	MeshGenerator::GenerateIcosahedron(BallMesh2, 3);*/

	//Mesh* LongBox = new Mesh("LongBox");
	//Meshes["LongBox"] = LongBox;
	//MeshGenerator::GenerateCubeWithHardEdges(LongBox, glm::vec3(1.5f, 0.5f, 0.5f));

	//LongBox->transform.SetLocation(glm::vec3(-2.0f, 0.0f, 0.0f));
	//LongBox->shaderProgram = shaderProgram;
	//TriangleMesh->AddChild.push_back(LongBox);

	
	//

	//Mesh* FloorMesh = new Mesh("FloorMesh");
	//Meshes["FloorMesh"] = FloorMesh;
	//FloorMesh->shaderProgram = shaderProgram;
	//FloorMesh->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);
	//FloorMesh->material.shininess = 16.f;

	//Mesh* RightWall = new Mesh("RightWall");
	//Meshes["RightWall"] = RightWall;
	//RightWall->shaderProgram = shaderProgram;

	//Mesh* LeftWall = new Mesh("LeftWall");
	//Meshes["LeftWall"] = LeftWall;
	//LeftWall->shaderProgram = shaderProgram;

	//Mesh* TopWall = new Mesh("TopWall");
	//Meshes["TopWall"] = TopWall;
	//TopWall->shaderProgram = shaderProgram;

	//Mesh* BottomWall = new Mesh("BottomWall");
	//Meshes["BottomWall"] = BottomWall;
	//BottomWall->shaderProgram = shaderProgram;

	//MeshGenerator::GeneratePoolTable(FloorMesh, TopWall, BottomWall, RightWall, LeftWall, glm::vec3(10.0f, 0.5f, 5.0f));

	//FloorMesh->AddChild.push_back(RightWall);
	//FloorMesh->AddChild.push_back(LeftWall);
	//FloorMesh->AddChild.push_back(TopWall);
	//FloorMesh->AddChild.push_back(BottomWall);

	//RootMesh->AddChild.push_back(FloorMesh);

	ecs_manager.Setup();
	ecs_manager.SystemSetup();
}

void Scene::UnloadContent()
{

}

void Scene::Bind()
{
	std::cout << "Binding Scene" << std::endl;
	BindSceneGraph(RootMesh);
}

void Scene::BindSceneGraph(Mesh* mesh)
{
	std::cout << "Binding Mesh: " << mesh->name << std::endl;
	mesh->Bind();
	for (auto child : mesh->Children )
	{
		BindSceneGraph(child);
	}
}

void Scene::Init()
{
	std::cout << "Initializing Scene" << std::endl;
	collision_manager = new CollisionManager();
	camera = new Camera();
	camera->cameraPos.y = 50;
	camera->CameraSpeed = 100;
}

void Scene::Update(float DeltaTime)
{
	//start timer
	//StartTimer("Update");


	//std::cout << "Updating Scene" << std::endl;
	UpdateSceneGraph(DeltaTime, RootMesh, glm::mat4(1));
	collision_manager->Update(DeltaTime);
	ecs_manager.Update(DeltaTime);
		//end timer
	//EndTimer("Update");
}

void Scene::UpdateSceneGraph(float DeltaTime, Mesh* mesh, glm::mat4 ParentTransform)
{

	mesh->Update(DeltaTime);
	mesh->ParentMatrix = ParentTransform;

	if (mesh->Children.size() == 0)
	{
		return;
	}
	for (auto child : mesh->Children)
	{
		UpdateSceneGraph(DeltaTime, child, ParentTransform * mesh->GetTransformMat());
	}
}

void Scene::Render(float DeltaTime, int width, int height)
{
	//start timer
	//StartTimer("Render");


	//std::cout << "Rendering Scene" << std::endl;
	// SceneGraph
	RenderSceneGraph(RootMesh, DeltaTime, width, height, glm::mat4(1));
	RenderSceneGraphVisuals();
	collision_manager->Render(lineMesh);
	//lineMesh->AddLine(glm::vec3(0, 0, 0), glm::vec3(10, 5, 2));

	ecs_manager.Render();

	ImGui::Begin("Mesh Properties");
	if (SelectedMesh != nullptr)
	{
		SelectedMesh->RenderProperties();

	}
	ImGui::End();

	//end timer
	//EndTimer("Render");


	RenderTimer(DeltaTime);
}

void Scene::RenderSceneGraph(Mesh* mesh, float DeltaTime, int width, int height, glm::mat4 ParentTransform)
{
	//StartTimer(mesh->name);
	this->width = width;
	this->height = height;
	//std::cout << "Rendering Mesh: " << mesh->name << std::endl;
	ShaderProgram* CurrentShader = mesh->shaderProgram;
	if (CurrentShader->shaderProgram != LastShader)
	{
		CurrentShader->UseProgram();
	}
	LastShader = CurrentShader->shaderProgram;




	ReadyRenderData(CurrentShader, mesh, ParentTransform, width, height);


	mesh->Draw();

	//for (auto corner : mesh->MeshCorners)
	//{
	//	glm::vec3 newCorner = mesh->GetGlobalTransform() * glm::vec4(corner, 1);
	//	lineMesh->AddBox(newCorner, glm::vec3(0.1f));
	//}


	// Drawing Lines
	//mesh->aabb;
	//if (mesh->UseCollision)
	//{

	//	AxisAlignedBoundingBox AABB = mesh->GetAABB();

	//	glm::vec3 Center = AABB.min + (AABB.max - AABB.min) * 0.5f;
	//	//glm::vec3 Size = max - min;
	//	float SizeX = AABB.max.x - AABB.min.x;
	//	float SizeY = AABB.max.y - AABB.min.y;
	//	float SizeZ = AABB.max.z - AABB.min.z;


	//	//size *= 2.f;
	//	//lineMesh->AddBox(Center, Size);
	//	//lineMesh->AddBox(AABB.min, glm::vec3(0.1f));
	//	//lineMesh->AddBox(AABB.max, glm::vec3(0.1f));
	//	//lineMesh->AddBox(Center, glm::vec3(SizeX, SizeY, SizeZ));
	//	lineMesh->AddBoxMinMax(AABB.min, AABB.max);
	//}




	/*ImGui::Begin(mesh->name.c_str());
	glm::vec3 position = mesh->transform.GetLocation();
	ImGui::SliderFloat3("Position", glm::value_ptr(position), -10.0f, 10.0f);
	TriangleMesh->transform.SetLocation(position);

	glm::vec3 rotation = mesh->transform.GetRotation();
	ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), -360.0f, 360.0f);
	mesh->transform.SetRotation(rotation);
	ImGui::End();*/

	//EndTimer(mesh->name);
	if (mesh->Children.size() == 0)
	{
		return;
	}
	for (auto child : mesh->Children)
	{
		RenderSceneGraph(child, DeltaTime, width, height, ParentTransform * mesh->GetTransformMat());
	}
}

void Scene::ReadyRenderData(ShaderProgram* shader, Mesh* mesh, glm::mat4 parentTransform, int width,int height)
{
	//StartTimer("ReadyRenderData");
	//std::cout << "Ready Render Data" << std::endl;
	// Camera stuff
	float Aspect = (float)width / (float)height;
	glm::mat4 projection = glm::perspective(glm::radians(90.f), Aspect, 0.1f, 300.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	// Mesh location
	glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(parentTransform * mesh->GetTransformMat()));


	// Lights
	unsigned int test3 = glGetUniformLocation(shader->shaderProgram, "lightDirection");
	glUniform3fv(test3, 1, glm::value_ptr((-SunMesh->transform.GetUpVector())));


	// Camera For Material
	glUniform3fv(glGetUniformLocation(shader->shaderProgram, "viewPos"), 1, glm::value_ptr(camera->cameraPos));

	// Materials
	glUniform3fv(glGetUniformLocation(shader->shaderProgram, "diffuse"), 1, glm::value_ptr(mesh->material.diffuse));
	glUniform3fv(glGetUniformLocation(shader->shaderProgram, "specular"), 1, glm::value_ptr(mesh->material.specular));
	glUniform1f(glGetUniformLocation(shader->shaderProgram, "shininess"), mesh->material.shininess);
	glUniform1f(glGetUniformLocation(shader->shaderProgram, "transparency"), mesh->material.transparency);
	//EndTimer("ReadyRenderData");
}

template <typename T>
T* Scene::CreateAndRegisterMesh(std::string name, glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale,
	ShaderProgram* shader_program, T* parent, bool UseCollision)
{
	T* mesh = new T(name);
	Meshes[name] = mesh;
	mesh->transform.SetLocation(Location);
	mesh->transform.SetRotation(Rotation);
	mesh->transform.SetScale(Scale);

	mesh->shaderProgram = shader_program;

	if (UseCollision)
	{
		collision_manager->RegisterMeshForCollision(mesh);
	}

	if (parent != nullptr)
	{
		parent->AddChild(mesh);
	}
	else
	{
		RootMesh->AddChild(mesh);
	}

	return mesh;

}

void Scene::RenderSceneGraphVisuals()
{
	//StartTimer("RenderSceneGraphVisuals");
	ImGui::Begin("SceneGraph");

	if (ImGui::Button("Root"))
	{
		SelectMesh(RootMesh);
	}
	for (auto child : RootMesh->Children)
	{
		RenderSceneGraphVisualsChldren(child, 1);
	}

	ImGui::End();
	//EndTimer("RenderSceneGraphVisuals");
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
	this->width = width;
	this->height = height;
}

void Scene::MouseMoveCallback(Window* window, double xpos, double ypos)
{
    camera->MouseMoveCallback(window->GetGLFWWindow(), xpos, ypos);
	ecs_manager.MouseMoveCallback(window, xpos, ypos);
}

void Scene::MouseButtonCallback(Window* window, int button, int action, int mods)
{
	camera->MouseButtonCallback(window->GetGLFWWindow(), button, action, mods);
	ecs_manager.MouseButtonCallback(window, button, action, mods);
}

void Scene::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{
	camera->MouseScrollCallback(window->GetGLFWWindow(), xoffset, yoffset);
	ecs_manager.MouseScrollCallback(window, xoffset, yoffset);
}

void Scene::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{
	camera->KeyCallback(window->GetGLFWWindow(), key, scancode, action, mods);
	ecs_manager.KeyCallback(window, key, scancode, action, mods);
}

void Scene::ProcessInput(Window* window, float DeltaTime)
{
	camera->ProcessInput(window->GetGLFWWindow(), DeltaTime);
	ecs_manager.ProcessInput(window, DeltaTime);
}

void Scene::StartTimer(std::string TimerName)
{
	Timers[TimerName] = static_cast<float>(glfwGetTime());
	
}

void Scene::EndTimer(std::string TimerName)
{

	Timers[TimerName] = static_cast<float>(glfwGetTime()) - Timers[TimerName];
}

void Scene::RenderTimer(float dt)
{
	//ImGui::Begin("Timer");

	////Plot all values of Timer as bars
	//for (auto timer : Timers)
	//{
	//	//float time = static_cast<float>(glfwGetTime()) - timer.second;
	//	float adjustedTime = timer.second / dt;


	//	std::string Name = timer.first + " : " + std::to_string(timer.second);
	//	ImGui::Text(Name.c_str());

	//	ImGui::ProgressBar(adjustedTime);
	//}
	//ImGui::End();
}
