#include "PunktSky.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


void PunktSky::ReadFile(std::filesystem::path path)
{
	if (!std::filesystem::exists(path))
	{
		std::cout << "File does not exist at: " << path << "\n";
		return;
	}

	std::ifstream file(path); // Open file
	//FILE* file = fopen("data.txt", "r");

	std::string line;

	std::getline(file, line);
	std::cout << "Reading file with: " << line << " lines\n";

	glm::vec3 min(FLT_MAX), max(FLT_MIN);


	//// Start timer
	auto start = std::chrono::high_resolution_clock::now();
	vertices.reserve(1000000);




	//// Read the file line by line
	int Line = 0;
	while (std::getline(file, line))
	{
		Line++;

		if (Line % 100000 == 0)
		{
			std::cout << "Reading line: " << Line << "\n";
		}

		if ((Line & 2) == 0)
		{
			continue;
		}

		Vertex vertex;
		std::stringstream ss(line);
		//ss >> vertex.position.x >> vertex.position.z >> vertex.position.y;
		ss >> vertex.position.x >> vertex.position.z >> vertex.position.y >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;


		min = glm::min(min, vertex.position);
		max = glm::max(max, vertex.position);

		vertices.push_back(vertex);
	}


	// Read using comma

	//while (std::getline(file, line))
	//{
	//	Vertex vertex;
	//	//std::stringstream ss(line);
	//	//std::string value;

	//	char* token;
	//	char* lineCStr = &line[0];  // Convert string to C-style string for strtok

	//	//// Read the x, y, z positions separated by commas
	//	//std::getline(ss, value, ',');
	//	//vertex.position.x = std::stof(value);

	//	//std::getline(ss, value, ',');
	//	//vertex.position.z = std::stof(value);  // Assuming z comes second

	//	//std::getline(ss, value, ',');
	//	//vertex.position.y = std::stof(value);  // Assuming y comes last


	//	token = std::strtok(lineCStr, ",");
	//	vertex.position.x = std::stof(token);  // X-coordinate

	//	token = std::strtok(nullptr, ",");
	//	vertex.position.z = std::stof(token);  // Z-coordinate

	//	token = std::strtok(nullptr, ",");
	//	vertex.position.y = std::stof(token);  // Y-coordinate


	//	// Update min and max using glm
	//	min = glm::min(min, vertex.position);
	//	max = glm::max(max, vertex.position);

	//	// Store the vertex
	//	vertices.push_back(vertex);
	//}

	
	// TEST

	//glm::vec3 min(FLT_MAX), max(FLT_MIN);

	//const size_t BUFFER_SIZE = 1024 * (1024 * 12);  // Read 1 MB at a time
	//char* buffer = (char*)malloc(BUFFER_SIZE);

	//FILE* file = fopen(path.string().c_str(), "r");
	//if (!file) {
	//	std::cerr << "Error opening file!" << std::endl;
	//	free(buffer);
	//	return;
	//}

	//vertices.reserve(30000000);  // Preallocate for 30 million vertices

	//size_t bytesRead;
	//std::string lineBuffer;  // To handle lines split across buffer chunks

	//while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
	//	// Process the buffer
	//	size_t i = 0;
	//	while (i < bytesRead) {
	//		if (buffer[i] == '\n') {
	//			// End of line, process the lineBuffer with the current line
	//			Vertex vertex;
	//			char* lineCStr = &lineBuffer[0];
	//			char* token = std::strtok(lineCStr, ",");
	//			vertex.position.x = std::stof(token);
	//			token = std::strtok(nullptr, ",");
	//			vertex.position.z = std::stof(token);
	//			token = std::strtok(nullptr, ",");
	//			vertex.position.y = std::stof(token);

	//			// Update min and max
	//			min = glm::min(min, vertex.position);
	//			max = glm::max(max, vertex.position);

	//			// Store the vertex
	//			vertices.push_back(vertex);

	//			// Clear lineBuffer for the next line
	//			lineBuffer.clear();
	//		}
	//		else {
	//			// Accumulate characters in lineBuffer
	//			lineBuffer += buffer[i];
	//		}
	//		++i;
	//	}
	//}

	//fclose(file);
	//free(buffer);


	//





	// End timer
	auto end = std::chrono::high_resolution_clock::now();

	// Print timer
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Reading file took: " << elapsed.count() << " seconds\n";

	glm::vec3 mid = (min + max) / 2.f;


	float heighestPoint = max.y;
	float lowestPoint = min.y;

	srand(time(nullptr));

	for (auto& vertex : vertices)
	{
		vertex.position -= mid;

		float DiffDown = vertex.position.y - lowestPoint;

		float New = DiffDown / (heighestPoint - lowestPoint);

		float percentage = New/8;


		// Create vector for blue to red
		glm::vec3 color = glm::mix(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), percentage);

		//vertex.normal = vertex.position;
		//vertex.normal = glm::vec3(0, 1, 0);
		//std::cout << color.x << " " << color.y << " " << color.z << "\n";

	}


	std::cout << "Read " << vertices.size() << " vertices\n";
}

void PunktSky::Bind()
{
	// VAO
	glGenBuffers(1, &VBO);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	Vertex::BindAttributes();

	isBound = true;


}

void PunktSky::Draw()
{
	if (!isBound) {
		Bind();
	}

	material.BindMaterial(shaderProgram);
	glPointSize(1.f);
	shaderProgram->UseProgram();
	shaderProgram->SetUniformMat4("model", glm::mat4(1));

	glBindVertexArray(VAO);

	glDrawArrays(GL_POINTS, 0, vertices.size());
}
