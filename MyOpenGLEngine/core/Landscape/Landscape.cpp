#include "Landscape.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>

#include "MeshGenerator.h"

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& p) const {
		return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
	}
};

std::queue<Chunk*> Landscape::chunkQueueInput;
std::queue<Chunk> Landscape::chunkQueueOutput;

void Landscape::ReadPointCloudFile(std::filesystem::path filePath)
{
	std::cout << "\n";
	std::cout << "Reading file: " << filePath << std::endl;

	//StartTimer
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Check if file exists
	if (!std::filesystem::exists(filePath))
	{
		std::cout << "Error: File not found" << std::endl;
		return;
	}

	// Open the file
	std::ifstream file(filePath);


	// Check if file is open
	if (!file.is_open())
	{
		std::cout << "Error: File not found" << std::endl;
		return;
	}



	// Read all the lines from the file
	glm::vec3 min, max;
	GetVerticesFromFile(file, min, max);

	CreateEmptyChunks(min, max);

	StartFillChunks();
	
	StartTriangulateChunks();
}

void Landscape::GetVerticesFromFile(std::ifstream& file, glm::vec3& min, glm::vec3& max)
{
	// Variable to store the current line
	std::string currentLine;
	int amountOfVertices = 0;

	// Loop for checking comments and empty lines
	while (true)
	{
		std::getline(file, currentLine);
		if (currentLine.empty()) continue;
		if (currentLine[0] == '#')
		{
			std::cout << "Comment: " << currentLine << std::endl;
		}
		if (IsNumber(currentLine))
		{

			amountOfVertices = std::stoi(currentLine);
			vertices.reserve(amountOfVertices);
			std::cout << "Header: " << currentLine << "\n";

		}
		else break;
	}



	min = glm::vec3(FLT_MAX);
	max = glm::vec3(FLT_MIN);

	unsigned int LineNumber = 0;

	std::cout << "Reding Files: \n" << "|--------------------|\n" << "|";

	int LineTestCheck = amountOfVertices / 20;

	//Start timer
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (std::getline(file, currentLine))
	{
		std::stringstream ss(currentLine);
		LineNumber++;

		if (LineNumber % LineTestCheck == 0)
		{
			std::cout << "-";
		}

		if (LineNumber % 2 == 0)
		{
			continue;
		}

		//if (LineNumber >= 100000)
		//{
		//	break;
		//}

		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec3 color = glm::vec3(1.f);
		glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);

		//ss >> position.x >> position.z >> position.y;
		ss >> position.x >> position.z >> position.y >> color.x >> color.y >> color.z;

		min = glm::min(min, position);
		max = glm::max(max, position);

		vertices.emplace_back(position, normal, glm::vec2(0), color);
	}
	std::cout << "|\n";

	//End timer
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Read " << LineNumber << " lines in " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << '\n';


	glm::vec3 mid = (min + max) / 2.f;

	for (auto& vertex : vertices)
	{
		vertex.position -= mid;
		vertex.position.x *= -1.f;
	}
	min -= mid;
	max -= mid;
	std::cout << "Min: " << min.x << " " << min.y << " " << min.z << '\n';
	std::cout << "Max: " << max.x << " " << max.y << " " << max.z << '\n';
}

void Landscape::CreateEmptyChunks(glm::vec3& min, glm::vec3& max)
{
	std::cout << "\n";
	std::cout << "Generating Chunks: " << std::endl;
	float CurX = min.x;
	float CurZ = min.z;

	while (CurX < max.x)
	{
		while (CurZ < max.z)
		{
			Chunk* chunk = new Chunk;
			chunk->chunkSize = ChunkSize;
			chunk->MinX = CurX;
			chunk->MinZ = CurZ;
			chunk->MaxX = CurX + ChunkSize;
			chunk->MaxZ = CurZ + ChunkSize;

			chunks.push_back(chunk);

			CurZ += ChunkSize;
		}
		CurZ = min.z;
		CurX += ChunkSize;
	}

	std::cout << "Generated " << chunks.size() << " Chunks" << '\n';
}

void Landscape::StartFillChunks()
{
	for (auto chunk : chunks)
	{
		chunkQueueInput.push(chunk);
	}

	unsigned int threadCount = chunks.size();
	if (threadCount > 100)
		threadCount = 100;
	
	if (threadCount > std::thread::hardware_concurrency())
		threadCount = std::thread::hardware_concurrency();

	std::cout << "Thread Count: " << threadCount << "\n";

	std::vector<std::thread> threads;
	for (int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&Landscape::Thread_ChunkFill_worker, this);
	}

	cv.notify_all();

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	std::cout << "Done Filling Chunks" << '\n';
}

void Landscape::StartTriangulateChunks()
{
	for (auto chunk : chunks)
	{
		chunkQueueInput.push(chunk);
	}

	unsigned int threadCount = chunks.size();
	if (threadCount > 100)
		threadCount = 100;

	if (threadCount > std::thread::hardware_concurrency())
		threadCount = std::thread::hardware_concurrency();

	std::cout << "Thread Count: " << threadCount << "\n";

	std::vector<std::thread> threads;
	for (int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&Landscape::Thread_ChunkTriangulate_worker, this);
	}

	cv.notify_all();

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	std::cout << "Done Triangulating Chunks" << '\n';

}

void Landscape::Thread_ChunkFill_worker()
{
	while (true)
	{
		if (chunkQueueInput.empty())
		{
			break;
		}
		Chunk* ThisChunk;
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			cv.wait(lock, [] { return !chunkQueueInput.empty(); }); // Wait until inputQueue has elements

			// Get a pointer from inputQueue
			ThisChunk = chunkQueueInput.front();
			chunkQueueInput.pop();
		}
		
	
		//Fill the chunk
		FillChunksWithVertex(ThisChunk);

		//{
		//	std::unique_lock<std::mutex> lock(queueMutex);
		//	chunkQueueOutput.push(ThisChunk);
		//}
	}
	std::cout << "|";
}

void Landscape::FillChunksWithVertex(Chunk* inChunk)
{
	std::vector<Vertex> newVertices;
	float Padding = TriangleSize;
	//inChunk->vertices.reserve(vertices.size());
	for (auto vertex : vertices)
	{
		//std::cout << "Vertex: " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << "\n";
		if (vertex.position.x >= inChunk->MinX - Padding && vertex.position.x <= inChunk->MaxX + Padding &&
			vertex.position.z >= inChunk->MinZ - Padding && vertex.position.z <= inChunk->MaxZ + Padding)
		{
			assert(inChunk != nullptr);

			inChunk->vertices.emplace_back(vertex);
			/*Vertex newVertex = vertex;
			inChunk->vertices.emplace_back(newVertex);*/
		}
	}
	
}

void Landscape::Thread_ChunkTriangulate_worker()
{
	while (true)
	{
		if (chunkQueueInput.empty())
		{
			break;
		}
		Chunk* ThisChunk;
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			cv.wait(lock, [] { return !chunkQueueInput.empty(); }); // Wait until inputQueue has elements

			// Get a pointer from inputQueue
			ThisChunk = chunkQueueInput.front();
			chunkQueueInput.pop();
		}

		//Fill the chunk
		TriangulateChunk(ThisChunk);

		//{
		//	std::unique_lock<std::mutex> lock(queueMutex);
		//	chunkQueueOutput.push(ThisChunk);
		//}
	}
	std::cout << "|";
}

void Landscape::TriangulateChunk(Chunk* inChunk)
{
	float CurX = inChunk->MinX;
	float CurZ = inChunk->MinZ;
	float MeshResolution = TriangleSize;

	int xCount = 0;
	int zCount = 0;
	int LineCount = 0;

	std::vector<Vertex> newVertices;

	while (CurX <= inChunk->MaxX)
	{
		xCount++;
		while (CurZ <= inChunk->MaxZ)
		{
			zCount++;
			LineCount++;

			float Height = 0;
			glm::vec3 Color = glm::vec3(0, 0, 0);
			unsigned int count = 0;

			// Get the height of the terrain
			for (auto& vertex : inChunk->vertices)
			{
				/*if (vertex.position.x > CurX - MeshResolution && vertex.position.x < CurX + MeshResolution &&
					vertex.position.z > CurZ - MeshResolution && vertex.position.z < CurZ + MeshResolution)*/
				if (vertex.position.x > CurX && vertex.position.x < CurX &&
					vertex.position.z > CurZ && vertex.position.z < CurZ )
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
			
			inChunk->verticesTriangulated.push_back(Vertex(glm::vec3(CurX, Height, CurZ), glm::vec3(0, 1, 0), glm::vec2(0), Color));
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

			CurZ += MeshResolution;
		}
		CurZ = inChunk->MinZ + (MeshResolution);
		CurX += MeshResolution;
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
	inChunk->indices.reserve((xCount - 1) * (zCount - 1) * 6);

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

				inChunk->indices.push_back(TopLeft);
				// 2
				inChunk->indices.push_back(TopRight);
				// 1
				inChunk->indices.push_back(BottomLeft);
				// 3
				inChunk->indices.push_back(BottomLeft);
				// 3
				inChunk->indices.push_back(TopRight);
				// 1
				inChunk->indices.push_back(BottomRight);



			}
			//std::cout << "Done XLINE " << NewX << "\n";
		}
	}
	else
	{
		std::cout << "No vertices in mesh\n";
	}



	//Output time
	/*auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;*/
	//std::cout << "Triangulated " << LineCount << " lines in " << duration.count() << " seconds\n";
	//std::cout << "Triangulated " << mesh->name << " " << xCount << " x " << zCount << " in " << duration.count() << " seconds\n";

	Mesh* tempMesh = new Mesh();
	tempMesh->vertices = inChunk->verticesTriangulated;
	tempMesh->indices = inChunk->indices;
	MeshGenerator::GenerateNormals(tempMesh);

	inChunk->verticesTriangulated = tempMesh->vertices;
	inChunk->indices = tempMesh->indices;
	delete tempMesh;
}

bool Landscape::IsNumber(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void Landscape::Update()
{
	// TODO if needed
}

void Landscape::Render()
{
	for (auto chunk : chunks)
	{
		if (chunk->hide) 
			continue;

		if (!chunk->isBound)
			BindChunk(chunk);

		RenderChunk(chunk);
	}
}

void Landscape::RenderChunk(Chunk* chunk)
{
	glBindVertexArray(chunk->VAO);

	unsigned int RenderMode = GL_TRIANGLES;

	if (chunk->indices.size() > 0)
	{
		glDrawElements(RenderMode, chunk->indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(RenderMode, 0, chunk->vertices.size());
	}
}

void Landscape::BindChunk(Chunk* chunk)
{
	// VAO
	glGenBuffers(1, &chunk->VBO);

	// VAO
	glGenVertexArrays(1, &chunk->VAO);
	glBindVertexArray(chunk->VAO);

	// VBO
	glGenBuffers(1, &chunk->EBO);


	glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
	glBufferData(GL_ARRAY_BUFFER, chunk->vertices.size() * sizeof(Vertex), chunk->vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->indices.size() * sizeof(unsigned int), chunk->indices.data(), GL_STATIC_DRAW);

	Vertex::BindAttributes();

	chunk->isBound = true;
}
