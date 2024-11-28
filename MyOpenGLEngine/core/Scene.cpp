#include "Scene.h"

#include "actor.h"
#include "Application.h"
#include "Ball.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "Material.h"
#include "MeshGenerator.h"
#include "PunktSky/PunktSky.h"
#include "Vertex.h"
#include <chrono>
#include <glad/glad.h>
#include <iostream>
#include <queue>

#include "BSpline/BSpline.h"
#include "BSpline/BSplineSurface.h"
#include "Landscape/Landscapegenerator.h"
#include "Landscape/LandscapeMesh.h"

Scene::Scene(const std::string& name)
{
}

Scene::~Scene()
{
}

void Scene::LoadContent()
{
	



	ShaderProgram* shaderProgram = new ShaderProgram();
	
	if (shaderProgram->ReadShaderFile("../../../core/shaders/VertexShader.vert", "../../../core/shaders/FragmentShader.frag") == 0)
	{
		shaderProgram->ReadShaderFile("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
	}

	ShaderProgram* lineShaderProgram = new ShaderProgram();
	if (lineShaderProgram->ReadShaderFile("../../../core/shaders/lineShaders/LineVertexShader.vert", "../../../core/shaders/lineShaders/LineFragmentShader.frag") == 0)
	{
		lineShaderProgram->ReadShaderFile("shaders/lineShaders/LineVertexShader.vert", "shaders/lineShaders/LineFragmentShader.frag");
	

	}

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
	lineMesh->shaderProgram = lineShaderProgram;
	RootMesh->Children.push_back(lineMesh);
	lineMesh->AddLine(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0));
	lineMesh->material.diffuse = glm::vec3(1.f, 0.f, 0.f);

	Mesh* NodeTreeMesh = new Mesh("NodeTreeMesh");
	Meshes["NodeTreeMesh"] = NodeTreeMesh;
	MeshGenerator::GenerateCube(NodeTreeMesh, glm::vec3(1.f));
	NodeTreeMesh->shaderProgram = shaderProgram;
	NodeTreeMesh->material.diffuse = glm::vec3(1.f, 0.0f, 0.0f);
	NodeTreeMesh->material.transparency = 0.1f;
	NodeTreeMesh->Bind();






	//Mesh* BigBoxMesh = CreateAndRegisterMesh<Mesh>("BigBoxMesh", glm::vec3(-7.f, 7, 0), glm::vec3(0.f), glm::vec3(1.f), shaderProgram, nullptr, true);
	//MeshGenerator::GenerateCubeWithHardEdges(BigBoxMesh, glm::vec3(1.0f, 1.f, 1.0f));


	//Mesh* FloorMesh = CreateAndRegisterMesh<Mesh>("FloorMesh", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, nullptr, true);
	//FloorMesh->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);
	//FloorMesh->material.shininess = 8.f;

	//Mesh* RighWall = CreateAndRegisterMesh<Mesh>("RighWall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	//RighWall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	//Mesh* Leftwall = CreateAndRegisterMesh<Mesh>("Leftwall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	//Leftwall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	//Mesh* TopWall = CreateAndRegisterMesh<Mesh>("TopWall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	//TopWall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	//Mesh* BottomWall = CreateAndRegisterMesh<Mesh>("BottomWall", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, FloorMesh, true);
	//BottomWall->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);

	//MeshGenerator::GeneratePoolTable(FloorMesh, TopWall, BottomWall, RighWall, Leftwall, glm::vec3(200.f, 10.f, 200.f));
	////MeshGenerator::GeneratePoolTable(FloorMesh, TopWall, BottomWall, RighWall, Leftwall, glm::vec3(20, 2, 40));

	//FloorMesh->Static = true;
	//RighWall->Static = true;
	//Leftwall->Static = true;
	//TopWall->Static = true;
	//BottomWall->Static = true;



	//Mesh* Landscape = CreateAndRegisterMesh<Mesh>("Landscape", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, nullptr, true);
	////Mesh* LandscapeTriangulated = CreateAndRegisterMesh<Mesh>("LandscapeTriangulated", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, Landscape, true);
	//Landscape->Hide = true;

	//std::filesystem::path path = "../../../core/pointCloud.txt";
	//std::filesystem::path path = "../../../core/vsim_las.txt";
	//std::filesystem::path path = "C:/Users/soroe/Downloads/CroppedCloud.txt";
	std::filesystem::path path = "C:/Users/soroe/Downloads/Leira.txt";
	//std::filesystem::path path = "C:/Users/soroe/Downloads/anders.txt";


	/*std::vector<Mesh*> triangulatedChunks;
	PunktSky* PunktSkyReader = new PunktSky();*/
	//PunktSkyReader->ReadFileMesh(path, Landscape, triangulatedChunks, 20, 0.5f, true);

	LandscapeGenerator newLandscape;
	newLandscape.ReadPointCloudFile(path);

	LandscapeMesh* myLandscapeMesh = CreateAndRegisterMesh<LandscapeMesh>("MyLandscape", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, nullptr, false);
	myLandscapeMesh->chunks = newLandscape.chunks;
	myLandscapeMesh->VertexColorAsColor = true;
	myLandscapeMesh->FillchunkMap();

	//LandscapeGenerator newLandscape2;
	//newLandscape2.ReadPointCloudFile(path2);

	//LandscapeMesh* myLandscapeMesh2 = CreateAndRegisterMesh<LandscapeMesh>("MyLandscape2", glm::vec3(300.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, nullptr, false);
	//myLandscapeMesh2->chunks = newLandscape2.chunks;
	//myLandscapeMesh2->VertexColorAsColor = true;
	//myLandscapeMesh2->FillchunkMap();
	//int Counter = 0;
	//for (auto chu : newLandscape.chunks)
	//{
	//	Counter++;
	//	std::string Name = "Chunk" + std::to_string(Counter);
	//	Mesh* NewMesh = CreateAndRegisterMesh<Mesh>(Name, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0, 0, 0), glm::vec3(1.f), shaderProgram, nullptr, true);
	//	NewMesh->vertices = chu->verticesTriangulated;
	//	//NewMesh->vertices = chu->vertices;
	//	NewMesh->indices = chu->indices;
	//	NewMesh->VertexColorAsColor = true;
	//	//NewMesh->renderDots = true;
	//}


	//for (auto triangulated_chunk : triangulatedChunks)
	//{
	//	triangulated_chunk->shaderProgram = shaderProgram;
	//	triangulated_chunk->material.diffuse = glm::vec3(0.2f, 0.5f, 0.2f);
	//	triangulated_chunk->material.specular = glm::vec3(0.2f, 0.2f, 0.2f);
	//	triangulated_chunk->material.shininess = 64.f;
	//	triangulated_chunk->useShading = false;
	//	triangulated_chunk->Parent = Landscape;
	//	Landscape->AddChild(triangulated_chunk);
	//}
	//Landscape->vertices = punktSky.vertices;
	//Landscape->renderDots = true;
	//Landscape->VertexColorAsColor = true;


	// -------------------  BSPLINE SURFACE ------------------- //
	Mesh* BSplineMesh = CreateAndRegisterMesh<Mesh>("BSplineMesh", glm::vec3(0.f, 5.f, 0.f), glm::vec3(0,180,0), glm::vec3(1.f), shaderProgram, nullptr, false);
	Mesh* BSplineControlMesh = CreateAndRegisterMesh<Mesh>("BSplineControlMesh", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, BSplineMesh, false);
	BSplineControlMesh->renderDots = true;

	std::vector<std::vector<glm::vec3>> controlPoints = {
	   { glm::vec3(0, 0, 0), glm::vec3(1, 0, 1), glm::vec3(2, 0, 0) },
	   { glm::vec3(0, 1, 1), glm::vec3(1, 1, 2), glm::vec3(2, 1, 1) },
	   { glm::vec3(0, 2, 0), glm::vec3(1, 2, 1), glm::vec3(2, 2, 0) }
	};

	//controlPoints = {
	//	{glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(2.0, 0.0, 0.0), glm::vec3(3.0, 0.0, 0.0)},
	//	{glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 1.0, 2.0), glm::vec3(2.0, 1.0, 2.0), glm::vec3(3.0, 1.0, 0.0)},
	//	{glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0, 2.0, 0.0), glm::vec3(2.0, 2.0, 0.0), glm::vec3(3.0, 2.0, 0.0)},
	//};

	//std::vector<std::vector<glm::vec3>> controlPoints = {
	//{ glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(2, 0, 0), glm::vec3(3, 0, 0) },
	//{ glm::vec3(0, 1, 0), glm::vec3(1, 1, 2), glm::vec3(2, 1, 2), glm::vec3(3, 1, 0) },
	//{ glm::vec3(0, 2, 0), glm::vec3(1, 2, 0), glm::vec3(2, 2, 0), glm::vec3(3, 2, 0) }
	//};

	//std::vector<std::vector<glm::vec3>> controlPoints = {
	//{ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 3.0f, 3.0f), glm::vec3(3.0f, 0.0f, 1.0f) },
	//{ glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(2.0f, 3.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
	//{ glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(3.0f, 2.0f, 1.0f) }
	//};


	for (auto& control_point : controlPoints)
	{
		for (auto& point : control_point)
		{
			point *= 10.f;
		}
	}


	for (auto & row : controlPoints)
	{
		for (auto& point : row)
		{
			BSplineControlMesh->vertices.emplace_back(point);
		}
	}


	int resolutionU = 10;  // Resolution along the u direction
	int resolutionV = 10;  // Resolution along the v direction

	// Generate vertices for the B-spline surface
	std::vector<glm::vec3> vertices = BSplineSurface::generateBSplineMesh(controlPoints, resolutionU, resolutionV);

	// Generate the triangle indices for the surface
	std::vector<unsigned int> indices = BSplineSurface::triangulate(resolutionU, resolutionV);

	for (auto& vertex : vertices)
	{
		BSplineMesh->vertices.emplace_back(vertex);
	}

	BSplineMesh->indices = indices;

	MeshGenerator::GenerateNormals(BSplineMesh);


	Mesh* NewMesh = CreateAndRegisterMesh<Mesh>("NewMesh", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f), shaderProgram, nullptr, false);

	MeshGenerator::GenerateSphere(NewMesh, 10, 10, 10);

	//for (int i = 0; i < 10; ++i)
	//{

	//}
	Ball* BallMesh = CreateAndRegisterMesh<Ball>("BallMesh", glm::vec3(-150, 25, 90), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	MeshGenerator::GenerateIcosahedron(BallMesh, 3);
	
	//Ball* BallMesh2 = CreateAndRegisterMesh<Ball>("BallMesh2", glm::vec3(-148, 25, 44), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, false);
	//MeshGenerator::GenerateIcosahedron(BallMesh2, 3);

	//Ball* BallMesh3 = CreateAndRegisterMesh<Ball>("BallMesh3", glm::vec3(-128, 25, 94), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, false);
	//MeshGenerator::GenerateIcosahedron(BallMesh3, 3);

	//Ball* BallMesh4 = CreateAndRegisterMesh<Ball>("BallMesh4", glm::vec3(-138, 25, 74), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, false);
	//MeshGenerator::GenerateIcosahedron(BallMesh4, 3);

	//Mesh* BallMesh = CreateAndRegisterMesh<Mesh>("BallMesh", glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, false);
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

	//ecs_manager.Setup();
	//ecs_manager.SystemSetup();

	int Degree = 3;
	//std::vector<float> knots = { 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f };
	std::vector<float> knots = {};

	std::vector<glm::vec3> control_points = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 5.0f, 0.0f),
	glm::vec3(2.0f, 4.0f, 5.0f),
	glm::vec3(3.0f, 0.0f, 0.0f),
	glm::vec3(4.0f, 1.0f, 0.0f),
	glm::vec3(5.0f, -1.0f, 0.0f),
	glm::vec3(6.0f, 0.0f, 0.0f),
	glm::vec3(10.0f, 5.0f, -5.0f)
	};

	std::vector<glm::vec3> Points = BSpline::GeneratePoints(control_points, knots, 1000);
	Mesh* PointMesh = CreateAndRegisterMesh<Mesh>("PointMesh", glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1), shaderProgram, nullptr, true);
	Mesh* PointMesh2 = CreateAndRegisterMesh<Mesh>("PointMesh2", glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1), shaderProgram, PointMesh, true);
	for (auto paths : control_points)
	{
		PointMesh2->vertices.emplace_back(paths);
	}
	PointMesh->renderDots = true;
	PointMesh2->renderDots = true;
	PointMesh->DotsSize = 10;
	PointMesh2->DotsSize = 10;
	PointMesh2->material.diffuse = glm::vec3(1.f, 0.f, 0.f);

	for (auto point : Points)
	{
		//std::cout << point.x << " " << point.y << " " << point.z << std::endl;
		PointMesh->vertices.emplace_back(point);
	}

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

	//LandscapeMesh* myLandscape = (LandscapeMesh*)Meshes["MyLandscape"];
	//Chunk* myChunk = myLandscape->GetChunkFromPosition(camera->cameraPos);

	//if(myChunk)
	//{
	//	glm::vec3 ChunkCenter = myChunk->GetCenter();
	//	glm::vec3 Size = glm::vec3(myChunk->MinX - myChunk->MaxX);
	//	
	//	lineMesh->AddBoxMinMax({ myChunk->MinX, myChunk->MinY, myChunk->MinZ }, { myChunk->MaxX, myChunk->MaxY, myChunk->MaxZ });
	//	Ball* BallMesh = (Ball*)Meshes["BallMesh"];
	//	glm::vec3 BallPos = BallMesh->transform.GetLocation();


	//	std::pair<bool, Triangle> Triangle = myLandscape->GetTriangleFromPosition(BallPos);
	//	if (Triangle.first)
	//	{
	//		glm::vec3 RealPos = CollisionManager::BarycentricCheck(Triangle.second, BallPos);
	//		lineMesh->AddLine(Triangle.second.vA.position, Triangle.second.vB.position);
	//		lineMesh->AddLine(Triangle.second.vB.position, Triangle.second.vC.position);
	//		lineMesh->AddLine(Triangle.second.vC.position, Triangle.second.vA.position);
	//		//lineMesh->AddLine(RealPos, RealPos + glm::vec3(0, 10, 0));

	//		glm::vec3 Normal = glm::normalize(glm::cross(Triangle.second.vB.position - Triangle.second.vA.position, Triangle.second.vC.position - Triangle.second.vA.position));
	//		lineMesh->AddLine(RealPos, RealPos + Normal * 10.f);

	//		Normal = glm::vec3(Normal.x, Normal.z, Normal.y);


	//		glm::vec3 AccelerasjonsVector = 9.81f * glm::vec3(Normal.x * Normal.z, Normal.y * Normal.z, (Normal.z * Normal.z) - 1);
	//		AccelerasjonsVector = glm::vec3(AccelerasjonsVector.x, AccelerasjonsVector.z, AccelerasjonsVector.y);
	//		//AccelerasjonsVector.y = 0;
	//		BallMesh->velocity = BallMesh->velocity + (AccelerasjonsVector * DeltaTime);
	//		glm::vec3 NyPosition = BallMesh->transform.GetLocation() + (BallMesh->velocity * DeltaTime);
	//		NyPosition.y = RealPos.y;
	//		BallMesh->transform.SetLocation(NyPosition);
	//		//std::cout << RealPos.x << " " << RealPos.y << " " << RealPos.z << "\n";
	//		
	//		//BallMesh->transform.SetLocation({ BallPos.x, RealPos.y, BallPos.z });
	//		
	//		//lineMesh->AddLine(Triangle.se, myLandscape->GetTriangleFromPosition(camera->cameraPos) + glm::vec3(0, 10, 0));

	//	}
	//	//lineMesh->AddLine(ChunkCenter, camera->cameraPos);
	//	//lineMesh->AddLine(ChunkCenter, ChunkCenter + glm::vec3(0, 10, 0));
	//	//lineMesh->AddLine({ myChunk->MinX, myChunk->MinY, myChunk->MinZ }, { myChunk->MaxX, myChunk->MinY, myChunk->MinZ });
	//}
	LandscapeMesh* myLandscape = (LandscapeMesh*)Meshes["MyLandscape"];

	std::vector<Ball*> Balls;

	for (auto mesh : Meshes)
	{
		if (mesh.second->name.find("BallMesh") != std::string::npos)
		{
			Ball* ball = (Ball*)mesh.second;
			Balls.push_back(ball);
		}
	}

	collision_manager->PhysicsUpdate(DeltaTime, myLandscape, Balls);

	//lineMesh->AddLine(camera->cameraPos, Meshes["Landscape"]->transform.GetLocation());

	while (MeshQueue.size() > 0)
	{
		Mesh* mesh = MeshQueue.front();
		mesh->Bind();
		Meshes[mesh->name] = mesh;
		MeshQueue.pop();
	}

	//start timer
	//StartTimer("Update");


	//std::cout << "Updating Scene" << std::endl;
	UpdateSceneGraph(DeltaTime, RootMesh, glm::mat4(1));
	collision_manager->Update(DeltaTime);
	//ecs_manager.Update(DeltaTime);
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



	//collision_manager->Render(lineMesh);
	//lineMesh->AddLine(glm::vec3(0, 0, 0), glm::vec3(10, 5, 2));

	//ecs_manager.Render();
	//punktSky.Draw();


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
	glm::mat4 projection = glm::perspective(glm::radians(90.f), Aspect, 0.1f, 30000.0f);
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

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		int size = Meshes.size();
		std::string name = "BallMesh" + std::to_string(size);
		Ball* BallMesh2 = CreateAndRegisterMesh<Ball>(name, camera->cameraPos, glm::vec3(0), glm::vec3(1), Shaders["DefaultShader"], nullptr, true);
		MeshGenerator::GenerateIcosahedron(BallMesh2, 3);
		BallMesh2->Bind();
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		float OfsettSize = 10;
		for (int i = 0; i < 10; ++i)
		{
			for (int y = 0; y < 10; ++y)
			{
				int size = Meshes.size();
				std::string name = "BallMesh" + std::to_string(size);
				Ball* BallMesh2 = CreateAndRegisterMesh<Ball>(name, camera->cameraPos + glm::vec3((OfsettSize * i) - (OfsettSize * 10)/2, 0, (OfsettSize * y) - (OfsettSize * 10)/2), glm::vec3(0), glm::vec3(1), Shaders["DefaultShader"], nullptr, true);
				MeshGenerator::GenerateIcosahedron(BallMesh2, 3);
				BallMesh2->Bind();
			}
		}


	
	}
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
