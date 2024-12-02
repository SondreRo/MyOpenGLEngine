#include "LandscapeGenerator.h"
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

std::queue<Chunk*> LandscapeGenerator::chunkQueueInput;
std::queue<Chunk> LandscapeGenerator::chunkQueueOutput;

void LandscapeGenerator::ReadPointCloudFile(std::filesystem::path filePath)
{

	if (ReadChunksFromFile(filePath))
	{
		std::cout << "Read Chunks from file" << std::endl;
		return;
	}

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

	WriteChunksToFile(filePath);
}

void LandscapeGenerator::GetVerticesFromFile(std::ifstream& file, glm::vec3& min, glm::vec3& max)
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

	std::cout << "Reading Files: \n" << "|--------------------|\n" << "|";

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

	/*	if (LineNumber % 2 == 0)
		{
			continue;
		}*/

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

void LandscapeGenerator::CreateEmptyChunks(glm::vec3& min, glm::vec3& max)
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
			//chunk->MinY = min.y;
			chunk->MinZ = CurZ;
			chunk->MaxX = CurX + ChunkSize;
			//chunk->MaxY = max.y;
			chunk->MaxZ = CurZ + ChunkSize;
			chunk->TriangleSize = TriangleSize;
			chunks.push_back(chunk);

			CurZ += ChunkSize;
		}
		CurZ = min.z;
		CurX += ChunkSize;
	}

	std::cout << "Generated " << chunks.size() << " Chunks" << '\n';
}

void LandscapeGenerator::StartFillChunks()
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

	std::cout << "|";
	for (int i = 0; i < threadCount; ++i)
	{
		std::cout << "-";
	}
	std::cout << "|\n|";

	std::vector<std::thread> threads;
	for (int i = 0; i < threadCount; ++i)
	{
		threads.emplace_back(&LandscapeGenerator::Thread_ChunkFill_worker, this);
	}

	cv.notify_all();

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
	std::cout << "|\n";
	std::cout << "Done Filling Chunks" << '\n';
}

void LandscapeGenerator::StartTriangulateChunks()
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
		threads.emplace_back(&LandscapeGenerator::Thread_ChunkTriangulate_worker, this);
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


	//Delete chunks without vertices
	auto isEmpty = [](Chunk* chunk) {
		return chunk->vertices.empty();
		};

	auto it = std::remove_if(chunks.begin(), chunks.end(), [&](Chunk* chunk) {
		if (isEmpty(chunk))
		{
			delete chunk; // Free memory for the empty chunk
			return true;  // Mark for removal from vector
		}
		return false; // Keep in the vector
		});

	// Erase removed elements from the vector
	chunks.erase(it, chunks.end());
}

void LandscapeGenerator::WriteChunksToFile(std::filesystem::path inpath)
{
	std::string fileName = inpath.filename().string();

	//Create new folder
	std::filesystem::path outputFolder = "LandscapeGenerator";

	if (!std::filesystem::exists(outputFolder))
		std::filesystem::create_directory(outputFolder);

	std::cout << "Writing Chunks to file: " << outputFolder << std::endl;

	// Create new file in the folder
	std::filesystem::path outputFilePath = outputFolder / fileName;
	std::ofstream outputFile(outputFilePath, std::ios::trunc);


	if (!outputFile.is_open())
	{
		std::cerr << "Error: Could not open file for writing.\n";
		return;
	}

	outputFile << chunks.size() << "\n";

	// Write the vertices to the file
	for (auto chunk : chunks)
	{
		outputFile << chunk->MinX << " " << chunk->MinY << " " << chunk->MinZ << " " << chunk->MaxX << " " << chunk->MaxY << " " << chunk->MaxZ << " " << chunk->chunkSize << " " << chunk->TriangleSize << " " << chunk->xCount << " " << chunk->zCount << "\n";
		outputFile << chunk->verticesTriangulated.size() << "\n";
		for (auto& vertex : chunk->verticesTriangulated)
		{
			outputFile << 
				vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << " " << 
				vertex.color.r << " " << vertex.color.g << " " << vertex.color.b << " " << 
				vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z <<"\n";
		}

		outputFile << chunk->indices.size() << "\n";
		for (int i = 0; i < chunk->indices.size(); i+=3)
		{
			outputFile << chunk->indices[i] << " " << chunk->indices[i + 1] << " " << chunk->indices[i + 2] << "\n";
		}
		//outputFile << "\n";
	}

	outputFile.close();
	std::cout << "Done Writing Chunks to file" << '\n';
}

bool LandscapeGenerator::ReadChunksFromFile(std::filesystem::path inpath)
{
	//Get last part of inpath
	std::string fileName = inpath.filename().string();

	//OpenFile
	std::filesystem::path outputFolder = "LandscapeGenerator";
	std::filesystem::path outputFilePath = outputFolder / fileName;
	std::ifstream outputFile(outputFilePath);

	if (!outputFile.is_open())
	{
		std::cout << "Error: File not found" << std::endl;
		return false;
	}
	std::cout << "Reading Chunks from file: " << outputFilePath << std::endl;

	std::string currentLine;

	// Loop for checking comments and empty lines
	std::getline(outputFile, currentLine);
	std::cout << "I AM GOING TO READ : " << currentLine << "\n";
	int ChunkCount;

	std::stringstream ss(currentLine);
	ss >> ChunkCount;


	for (int i = 0; i < ChunkCount; i++)
	{
        Chunk* NewChunk = new Chunk();
		std::getline(outputFile, currentLine);

        std::stringstream ss(currentLine);
		ss >> NewChunk->MinX >> NewChunk->MinY >> NewChunk->MinZ >> NewChunk->MaxX >> NewChunk->MaxY >> NewChunk->MaxZ >> NewChunk->chunkSize >> NewChunk->TriangleSize >> NewChunk->xCount >> NewChunk->zCount;

        int amountOfVertices = 0;
        std::getline(outputFile, currentLine);
        ss = std::stringstream(currentLine);
        ss >> amountOfVertices;

        for (int y = 0; y < amountOfVertices; y++)
        {
	        std::getline(outputFile, currentLine);
	        ss = std::stringstream(currentLine);
	        Vertex NewVertex;
	        ss >> NewVertex.position.x >> NewVertex.position.y >> NewVertex.position.z >> NewVertex.color.r >> NewVertex.color.g >> NewVertex.color.b >> NewVertex.normal.x >> NewVertex.normal.y >> NewVertex.normal.z;
	        NewChunk->verticesTriangulated.push_back(NewVertex);
        }

        int amountOfIndices = 0;
        std::getline(outputFile, currentLine);
        ss = std::stringstream(currentLine);
        ss >> amountOfIndices;
		amountOfIndices /= 3;

        for (int y = 0; y < amountOfIndices; y++)
        {
	        std::getline(outputFile, currentLine);
	        ss = std::stringstream(currentLine);
	        unsigned int index1, index2, index3;
	        ss >> index1 >> index2 >> index3;
	        NewChunk->indices.push_back(index1);
	        NewChunk->indices.push_back(index2);
	        NewChunk->indices.push_back(index3);
        }

		chunks.push_back(NewChunk);
	}

	std::cout << "Read " << chunks.size() << " Chunks from file" << std::endl;
	return true;
}

void LandscapeGenerator::Thread_ChunkFill_worker()
{
	//while (!chunkQueueInput.empty())
	//{
	//	Chunk* ThisChunk;
	//	{
	//		std::unique_lock<std::mutex> lock(queueMutex);

	//		//cv.wait(lock, [] { return !chunkQueueInput.empty(); }); // Wait until inputQueue has elements

	//		// Get a pointer from inputQueue
	//		ThisChunk = chunkQueueInput.front();
	//		chunkQueueInput.pop();
	//	}
	//	
	//
	//	//Fill the chunk
	//	FillChunksWithVertex(ThisChunk);

	//	//{
	//	//	std::unique_lock<std::mutex> lock(queueMutex);
	//	//	chunkQueueOutput.push(ThisChunk);
	//	//}
	//}
	//std::cout << "-";
	while (true)
	{
		Chunk* ThisChunk = nullptr;

		{
			std::unique_lock<std::mutex> lock(queueMutex);

			// Wait until there are elements in the queue or stop signal
			//cv.wait(lock, [this] { return !chunkQueueInput.empty(); });

			// Break the loop if queue is empty and processing is done
			if (chunkQueueInput.empty())
				break;

			// Fetch the next chunk
			ThisChunk = chunkQueueInput.front();
			chunkQueueInput.pop();
		}

		// Process the chunk outside the lock to avoid blocking other threads
		if (ThisChunk)
		{
			FillChunksWithVertex(ThisChunk);
		}
	}

	std::cout << "-";
}

void LandscapeGenerator::FillChunksWithVertex(Chunk* inChunk)
{
	float Padding = TriangleSize * 1.5f;
	for (auto vertex : vertices)
	{
		if (vertex.position.x >= inChunk->MinX - Padding && vertex.position.x <= inChunk->MaxX + Padding &&
			vertex.position.z >= inChunk->MinZ - Padding && vertex.position.z <= inChunk->MaxZ + Padding)
		{
			assert(inChunk != nullptr);
			inChunk->vertices.emplace_back(vertex);
		}
	}
	
}

void LandscapeGenerator::Thread_ChunkTriangulate_worker()
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

void LandscapeGenerator::TriangulateChunk(Chunk* inChunk)
{
	float CurX = inChunk->MinX;
	float CurZ = inChunk->MinZ;
	float MeshResolution = TriangleSize;

	int xCount = 0;
	int zCount = 0;
	int LineCount = 0;

	std::vector<Vertex> newVertices;

	if (vertices.empty())
	{
		std::cout << "No vertices in mesh\n";
		return;
	}

	while (CurX <= inChunk->MaxX )
	{
		xCount++;
		while (CurZ <= inChunk->MaxZ)
		{
			zCount++;
			LineCount++;

			float Height = 0;
			glm::vec3 Color = glm::vec3(0, 0, 0);
			unsigned int count = 0;

			for (const auto& vertex : inChunk->vertices)
			{
				if (vertex.position.x > CurX - MeshResolution && vertex.position.x < CurX + MeshResolution &&
					vertex.position.z > CurZ - MeshResolution && vertex.position.z < CurZ + MeshResolution)
				{
					Color += vertex.color;
					Height += vertex.position.y;
					count++;
				}

			}


			if (count > 0)
			{
				Height /= count;
				Color /= count;

				inChunk->MinY = glm::min(inChunk->MinY, Height);
				inChunk->MaxY = glm::max(inChunk->MaxY, Height);
			}
			else
			{
				//std::cout << "|";
				Height = -200;
				inChunk->MinY = glm::min(inChunk->MinY, Height);
				inChunk->MaxY = glm::max(inChunk->MaxY, Height);
			}
			
			inChunk->verticesTriangulated.emplace_back(glm::vec3(CurX, Height, CurZ), glm::vec3(0, 1, 0), glm::vec2(0), Color);

			CurZ += MeshResolution;
		}
		CurZ = inChunk->MinZ;// +(MeshResolution);
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
	
	inChunk->xCount = xCount;
	inChunk->zCount = zCount;

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

bool LandscapeGenerator::IsNumber(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void LandscapeGenerator::Update()
{
	// TODO if needed
}

void LandscapeGenerator::Render()
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

void LandscapeGenerator::RenderChunk(Chunk* chunk)
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

void LandscapeGenerator::BindChunk(Chunk* chunk)
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
