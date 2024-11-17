#include "Application.h"
#include "PunktSky.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "MeshGenerator.h"

std::queue<Mesh*> PunktSky::outputQueue;
std::queue<Mesh*> PunktSky::inputQueue;

float PunktSky::ChunkSize = 100;
float PunktSky::MeshResolution = 1;

void PunktSky::ReadFileMesh(std::filesystem::path path, Mesh* mesh, std::vector<Mesh*>& finalchunks, float inChunkSize, float inMeshResolution, bool useVertexColors)
{
	// Exit Early if file does not exist
	if (!std::filesystem::exists(path))
	{
		std::cout << "File does not exist at: " << path << "\n";
		return;
	}
	ChunkSize = inChunkSize;
	MeshResolution = inMeshResolution;


	glm::vec3 min = glm::vec3(FLT_MAX);
	glm::vec3 max = glm::vec3(FLT_MIN);

	std::ifstream file(path); // Open file
	std::string line;

	if (!file.is_open())
	{
		std::cout << "Could not open file: " << path << "\n";
		return;
	}

	mesh->vertices.reserve(100000);
	mesh->renderDots = true;

	std::getline(file, line);
	std::cout << "Reading file with: " << line << " lines\n";

	int Line = 0;
	while (std::getline(file, line))
	{
		Line++;	

		if (Line % 100000 == 0)
		{
			std::cout << "Reading line: " << Line << "\n";
		}

		std::stringstream ss(line);

		glm::vec3 position;
		glm::vec3 color = glm::vec3(1.f);
		glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);

		//ss >> position.x >> position.z >> position.y;
		ss >> position.x >> position.z >> position.y >> color.x >> color.y >> color.z;

		min = glm::min(min, position);
		max = glm::max(max, position);

		mesh->vertices.emplace_back(position,normal,glm::vec2(0), color);
	}

	std::cout << "//------------------//\n";
	std::cout << "Read " << Line << " lines\n";


	glm::vec3 mid = (min + max) / 2.f;
	for (auto& vertex : mesh->vertices)
	{
		vertex.position -= mid;
		vertex.position.x *= -1.f;

	}

	min -= mid;
	max -= mid;


	// start timer
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Creating Chunks\n";
	std::vector<Mesh*> chunks;
	CreateChunks(mesh, min, max, chunks);

	// Output time
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	std::cout << "Created " << chunks.size() << " chunks in " << duration.count() << " seconds\n";



	for (auto chunk : chunks)
	{
		if (chunk->vertices.size() == 0)
		{
			continue;
		}
		std::string ChunkName = "Chunk: " + std::to_string(chunk->vertices[0].position.x) + " " + std::to_string(chunk->vertices[0].position.z);
		chunk->name = ChunkName;
		chunk->VertexColorAsColor = true;
		inputQueue.push(chunk);
	}



	std::cout << "Threads Started" << "\n";
	std::vector<std::thread> threads;
	unsigned int threadCount = chunks.size();


	if (threadCount > 100)
	{
		threadCount = 100;
	}

	if (threadCount > std::thread::hardware_concurrency())
	{
		threadCount = std::thread::hardware_concurrency();
	}
	std::cout << "Thread Count: " << threadCount << "\n";
	//threadCount = 1;

	//threadCount = 1;
	
	for (int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&PunktSky::worker, this);
	}
	cv.notify_all();

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	while (!outputQueue.empty())
	{
		if (useVertexColors)
		{
			outputQueue.front()->VertexColorAsColor = true;
		}
		else
		{
			outputQueue.front()->VertexColorAsColor = false;
		}
		finalchunks.push_back(outputQueue.front());
		outputQueue.pop();
	}

	//for (auto chunk : chunks)
	//{
	//	if (chunk->vertices.size() == 0)
	//	{
	//		continue;
	//	}
	//	std::string ChunkName = "Chunk: " + std::to_string(chunk->vertices[0].position.x) + " " + std::to_string(chunk->vertices[0].position.z);
	//	Mesh* TriangulatedChunk = new Mesh(ChunkName);


	//	TriangulateMesh(chunk, 1, TriangulatedChunk);
	//	TriangulatedChunk->VertexColorAsColor = true;
	//	finalchunks.push_back(TriangulatedChunk);
	//	
	//}


}

void PunktSky::TriangulateMesh(Mesh* mesh, Mesh* triangulatedMesh)
{

	//float x = mesh->minX;
	//float z = mesh->minY;
	float MinX = FLT_MAX;
	float MinZ = FLT_MAX;

	float MaxX = -FLT_MAX;
	float MaxZ = -FLT_MAX;
	for (auto element : mesh->vertices)
	{
		MinX = glm::min(MinX, element.position.x);
		MinZ = glm::min(MinZ, element.position.z);

		MaxX = glm::max(MaxX, element.position.x);
		MaxZ = glm::max(MaxZ, element.position.z);
	}


	/*MinX += (MeshResolution * 0.5f * 1.f);
	MinZ += (MeshResolution * 0.5f * 1.f);

	MaxX -= (MeshResolution * 0.5f * 1.f);
	MaxZ -= (MeshResolution * 0.5f * 1.f);*/

	/*float x = min.x;
	float z = min.z;*/
	float x = MinX + (MeshResolution);
	float z = MinZ + (MeshResolution);


	unsigned int LineCount = 0;
	unsigned int xCount = 0;
	unsigned int zCount = 0;

	//Starttimer
	auto start = std::chrono::high_resolution_clock::now();

	while (x <= MaxX - (MeshResolution))
	{
		xCount++;
		while (z <= MaxZ - (MeshResolution))
		{
			zCount++;
			LineCount++;


	/*		if (LineCount % 1000 == 0)
			{
				std::cout << "Triangulating Line: " << LineCount << "\n";
			}*/
			
			// Create a square
			// 0 1
			// 2 3

			float Height = 0;
			glm::vec3 Color = glm::vec3(0, 0, 0);
			unsigned int count = 0;

			// Get the height of the terrain
			for (auto& vertex : mesh->vertices)
			{
				if (vertex.position.x > x - MeshResolution && vertex.position.x < x + MeshResolution &&
					vertex.position.z > z - MeshResolution && vertex.position.z < z + MeshResolution)
				{
					Color += vertex.color;
					count++;
					Height += vertex.position.y;
					//break;

				}

			}


			if (count > 0)
			{
				Height /= count;
				Color /= count;
			}
			else
			{
				Height = -50;
			}

			triangulatedMesh->vertices.emplace_back(glm::vec3(x, Height, z), glm::vec3(0, 1, 0), glm::vec2(0), Color);
			//if (LineCount == 1)
			//{
			//	triangulatedMesh->vertices.emplace_back(glm::vec3(x, Height + 1, z), glm::vec3(0, 1, 0), glm::vec2(0), glm::vec3(1, 0, 0));
			//	triangulatedMesh->vertices.emplace_back(glm::vec3(x, Height + 2, z), glm::vec3(0, 1, 0), glm::vec2(0), glm::vec3(1, 0, 0));
			//	triangulatedMesh->vertices.emplace_back(glm::vec3(x, Height + 3, z), glm::vec3(0, 1, 0), glm::vec2(0), glm::vec3(1, 0, 0));

			//}
			//if (LineCount == 2)
			//{
			//	triangulatedMesh->vertices.emplace_back(glm::vec3(x, Height + 1, z), glm::vec3(0, 1, 0), glm::vec2(0), glm::vec3(1, 0, 0));
			//	triangulatedMesh->vertices.emplace_back(glm::vec3(x, Height + 2, z), glm::vec3(0, 1, 0), glm::vec2(0), glm::vec3(1, 0, 0));
			//	

			//}

			z += MeshResolution;
		}
		z = MinZ + (MeshResolution);
		x += MeshResolution;
	}

	if (xCount == 0)
	{
		zCount = 0;
	}
	else
	{
		zCount /= xCount;
	}


    // set up indices
    triangulatedMesh->indices.reserve((xCount - 1) * (zCount - 1) * 6);

	if (xCount > 0 && zCount > 0)
	{
		unsigned int Index = 0;
		for (int NewX = 0; NewX < xCount - 1; NewX++)
		{
			for (int NewZ = 0; NewZ < zCount - 1; NewZ++)
			{

				unsigned int TopLeft = (NewX * zCount) + NewZ;
				unsigned int TopRight = (NewX * zCount) + (NewZ + 1);
				unsigned int BottomLeft = ((NewX + 1) * zCount) + NewZ;
				unsigned int BottomRight = ((NewX + 1) * zCount) + (NewZ + 1);

				triangulatedMesh->indices.push_back(TopLeft);
				// 2
				triangulatedMesh->indices.push_back(TopRight);
				// 1
				triangulatedMesh->indices.push_back(BottomLeft);
				// 3
				triangulatedMesh->indices.push_back(BottomLeft);
				// 3
				triangulatedMesh->indices.push_back(TopRight);
				// 1
				triangulatedMesh->indices.push_back(BottomRight);



			}
			//std::cout << "Done XLINE " << NewX << "\n";
		}
	}
	else
	{
		std::cout << "No vertices in mesh\n";
	}

	

	//Output time
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	//std::cout << "Triangulated " << LineCount << " lines in " << duration.count() << " seconds\n";
	std::cout << "Triangulated "<< mesh->name << " " << xCount << " x " << zCount << " in " << duration.count() << " seconds\n";
	

	MeshGenerator::GenerateNormals(triangulatedMesh);

}


void PunktSky::worker()
{

	while (true)
	{
		Mesh* mesh = nullptr;

		if (inputQueue.empty())
		{
			break;
		}

		{
			std::unique_lock<std::mutex> lock(queueMutex);
			cv.wait(lock, [] { return !inputQueue.empty(); }); // Wait until inputQueue has elements

			// Get a pointer from inputQueue
			mesh = inputQueue.front();
			inputQueue.pop();
		}

		Mesh* triangulatedMesh = new Mesh(mesh->name + " Triangulated");
		TriangulateMesh(mesh, triangulatedMesh);

		{
			std::unique_lock<std::mutex> lock(queueMutex);
			outputQueue.push(triangulatedMesh);

		}
	}
}
//
//void PunktSky::ReadFile(std::filesystem::path path, Mesh* mesh)
//{
//	/*std::thread t1(&PunktSky::ReadFileThreaded, this, path, mesh);
//	t1.detach();*/
//	//t1.join();
//
//	//if (!std::filesystem::exists(path))
//	//{
//	//	std::cout << "File does not exist at: " << path << "\n";
//	//	return;
//	//}
//
//	//std::ifstream file(path); // Open file
//	////FILE* file = fopen("data.txt", "r");
//
//	//std::string line;
//
//	//std::getline(file, line);
//	//std::cout << "Reading file with: " << line << " lines\n";
//
//	//glm::vec3 min(FLT_MAX), max(FLT_MIN);
//
//
//	////// Start timer
//	//auto start = std::chrono::high_resolution_clock::now();
//	//vertices.reserve(1000000);
//
//
//
//
//	////// Read the file line by line
//	//int Line = 0;
//	//while (std::getline(file, line))
//	//{
//	//	Line++;
//
//	//	if (Line % 100000 == 0)
//	//	{
//	//		std::cout << "Reading line: " << Line << "\n";
//	//	}
//
//	//	//if ((Line & 2) == 0)
//	//	//{
//	//	//	continue;
//	//	//}
//
//	//	Vertex vertex;
//	//	std::stringstream ss(line);
//	//	//ss >> vertex.position.x >> vertex.position.z >> vertex.position.y;
//	//	ss >> vertex.position.x >> vertex.position.z >> vertex.position.y >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;
//
//
//	//	min = glm::min(min, vertex.position);
//	//	max = glm::max(max, vertex.position);
//
//	//	vertices.push_back(vertex);
//	//}
//
//
//	// Read using comma
//
//	//while (std::getline(file, line))
//	//{
//	//	Vertex vertex;
//	//	//std::stringstream ss(line);
//	//	//std::string value;
//
//	//	char* token;
//	//	char* lineCStr = &line[0];  // Convert string to C-style string for strtok
//
//	//	//// Read the x, y, z positions separated by commas
//	//	//std::getline(ss, value, ',');
//	//	//vertex.position.x = std::stof(value);
//
//	//	//std::getline(ss, value, ',');
//	//	//vertex.position.z = std::stof(value);  // Assuming z comes second
//
//	//	//std::getline(ss, value, ',');
//	//	//vertex.position.y = std::stof(value);  // Assuming y comes last
//
//
//	//	token = std::strtok(lineCStr, ",");
//	//	vertex.position.x = std::stof(token);  // X-coordinate
//
//	//	token = std::strtok(nullptr, ",");
//	//	vertex.position.z = std::stof(token);  // Z-coordinate
//
//	//	token = std::strtok(nullptr, ",");
//	//	vertex.position.y = std::stof(token);  // Y-coordinate
//
//
//	//	// Update min and max using glm
//	//	min = glm::min(min, vertex.position);
//	//	max = glm::max(max, vertex.position);
//
//	//	// Store the vertex
//	//	vertices.push_back(vertex);
//	//}
//
//	
//	// TEST
//
//	//glm::vec3 min(FLT_MAX), max(FLT_MIN);
//
//	//const size_t BUFFER_SIZE = 1024 * (1024 * 12);  // Read 1 MB at a time
//	//char* buffer = (char*)malloc(BUFFER_SIZE);
//
//	//FILE* file = fopen(path.string().c_str(), "r");
//	//if (!file) {
//	//	std::cerr << "Error opening file!" << std::endl;
//	//	free(buffer);
//	//	return;
//	//}
//
//	//vertices.reserve(30000000);  // Preallocate for 30 million vertices
//
//	//size_t bytesRead;
//	//std::string lineBuffer;  // To handle lines split across buffer chunks
//
//	//while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
//	//	// Process the buffer
//	//	size_t i = 0;
//	//	while (i < bytesRead) {
//	//		if (buffer[i] == '\n') {
//	//			// End of line, process the lineBuffer with the current line
//	//			Vertex vertex;
//	//			char* lineCStr = &lineBuffer[0];
//	//			char* token = std::strtok(lineCStr, ",");
//	//			vertex.position.x = std::stof(token);
//	//			token = std::strtok(nullptr, ",");
//	//			vertex.position.z = std::stof(token);
//	//			token = std::strtok(nullptr, ",");
//	//			vertex.position.y = std::stof(token);
//
//	//			// Update min and max
//	//			min = glm::min(min, vertex.position);
//	//			max = glm::max(max, vertex.position);
//
//	//			// Store the vertex
//	//			vertices.push_back(vertex);
//
//	//			// Clear lineBuffer for the next line
//	//			lineBuffer.clear();
//	//		}
//	//		else {
//	//			// Accumulate characters in lineBuffer
//	//			lineBuffer += buffer[i];
//	//		}
//	//		++i;
//	//	}
//	//}
//
//	//fclose(file);
//	//free(buffer);
//
//
//	//
//
//
//	
//	//t1.join();
//
//	//glm::vec3 mid = (min + max) / 2.f;
//
//	//while (!verticesQueue.empty())
//	//{
//	//	vertices.push_back(verticesQueue.front());
//	//	verticesQueue.pop();
//	//}
//
//	//float heighestPoint = max.y;
//	//float lowestPoint = min.y;
//
//	//srand(time(nullptr));
//
//	//for (auto& vertex : vertices)
//	//{
//	//	vertex.position -= mid;
//
//	//	float DiffDown = vertex.position.y - lowestPoint;
//
//	//	float New = DiffDown / (heighestPoint - lowestPoint);
//
//	//	float percentage = New/8;
//
//	//	vertex.position.x *= -1.f;
//	//	// Create vector for blue to red
//	//	glm::vec3 color = glm::mix(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), percentage);
//
//	//	//vertex.normal = vertex.position;
//	//	//vertex.normal = glm::vec3(0, 1, 0);
//	//	//std::cout << color.x << " " << color.y << " " << color.z << "\n";
//
//	//}
//
//
//	//std::cout << "Read " << vertices.size() << " vertices\n";
//}

void PunktSky::CreateChunks(Mesh* mesh, glm::vec3 min, glm::vec3 max, std::vector<Mesh*>& chunks)
{
	float X = min.x;
	float Z = min.z;

	float maxX = max.x;
	float maxZ = max.z;

	float size = ChunkSize;

	float Padding = MeshResolution;



	std::cout << "Creating Chunks with size: " << size << "\n";
	
	while (X < maxX)
	{
		while (Z < maxZ)
		{

			std::string ChunkName = "Chunk: " + std::to_string(X) + " " + std::to_string(Z);
			Mesh* NewChunk = new Mesh(ChunkName);
			NewChunk->vertices.reserve(1000);


			for (auto& vertex : mesh->vertices)
			{
				if (vertex.position.x >= X - Padding && vertex.position.x <= X + size + Padding &&
					vertex.position.z >= Z - Padding && vertex.position.z <= Z + size + Padding)
				{
					NewChunk->vertices.push_back(vertex);
				}
			}
			NewChunk->minX = X;
			NewChunk->minZ = Z;

			NewChunk->maxX = X + size;
			NewChunk->maxZ = Z + size;
			chunks.push_back(NewChunk);

			Z += size;
		}
		Z = min.z;
		X += size;
	}


}
