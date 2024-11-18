#include "LandscapeMesh.h"

#include "Application.h"
#include "imgui.h"

LandscapeMesh::LandscapeMesh(std::string inName)
{
	name = inName;
}

void LandscapeMesh::FillchunkMap()
{
	
	//Sort the queue based on the min x and min z values
	for (auto chunk: chunks)
	{
		int XBase = chunk->MinX / chunk->chunkSize;
		int ZBase = chunk->MinZ / chunk->chunkSize;

		//std::cout << "XBase: " << XBase << " ZBase: " << ZBase << std::endl;
		chunkMap[XBase][ZBase] = chunk;
	}
}

void LandscapeMesh::Bind()
{
	for (auto chunk : chunks)
	{
		// VAO
		glGenBuffers(1, &chunk->VBO);

		// VAO
		glGenVertexArrays(1, &chunk->VAO);
		glBindVertexArray(chunk->VAO);

		// VBO
		glGenBuffers(1, &chunk->EBO);


		glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
		glBufferData(GL_ARRAY_BUFFER, chunk->verticesTriangulated.size() * sizeof(Vertex), chunk->verticesTriangulated.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->indices.size() * sizeof(unsigned int), chunk->indices.data(), GL_STATIC_DRAW);

		Vertex::BindAttributes();

		chunk->isBound = true;
	}
	isBound = true;
}

void LandscapeMesh::Draw()
{
	if (Hide) return;

	if (isBound == false)
	{
		Bind();
	}

	if (VertexColorAsColor)
	{
		shaderProgram->SetUniform1i("useColorNormal", 1);
	}
	else {
		shaderProgram->SetUniform1i("useColorNormal", 0);
	}

	if (useShading)
	{
		shaderProgram->SetUniform1i("useShading", 1);
	}
	else
	{
		shaderProgram->SetUniform1i("useShading", 0);
	}

	


	for (auto chunk : chunks)
	{
		if (chunk->hide)
			continue;


		if (RenderFrictionColor)
		{
			glm::vec3 noFriction = glm::vec3(0, 1, 0);
			glm::vec3 fullFriction = glm::vec3(1, 0, 0);
			float frictionPercentage = chunk->Friction / 2.0f;
			glm::vec3 color = glm::mix(noFriction, fullFriction, frictionPercentage);
			material.diffuse = color;
		}
		material.BindMaterial(shaderProgram);
		glBindVertexArray(chunk->VAO);
		unsigned int RenderMode;
		if (renderDots)
		{
			RenderMode = GL_POINTS;
			glPointSize(DotsSize);
		}
		else
		{
			RenderMode = GL_TRIANGLES;
		}

		if (chunk->indices.size() > 0)
		{
			glDrawElements(RenderMode, chunk->indices.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(RenderMode, 0, chunk->verticesTriangulated.size());
		}
	}
}

void LandscapeMesh::RenderProperties()
{
	Mesh::RenderProperties();

	ImGui::Spacing();
	ImGui::Text("ChunkSelect");


	ImGui::DragFloat("OverallFriction", &Friction, 0.01f, 0, 2);
	if (ImGui::Button("ApplyFrictionToAllChunks"))
	{
		for (auto value : chunks)
		{
			value->Friction = Friction;
		}
	}
	ImGui::Checkbox("RenderFrictionColor", &RenderFrictionColor);

	//Array of two ints
	int XZ[2] = { XSelect, ZSelect };
	
	//ImGui::DragInt("Select ChunkX", &XSelect, 1.0f);
	//ImGui::DragInt("Select ChunkZ", &ZSelect, 1.0f);
	ImGui::DragInt2("Select Chunk", XZ, 0.1f);

	XSelect = XZ[0];
	ZSelect = XZ[1];

	if (chunkMap.count(XSelect) && chunkMap[XSelect].count(ZSelect))
	{
		SelectedChunk = chunkMap[XSelect][ZSelect];
	}
	else
	{
		SelectedChunk = nullptr;
	}



	/*if (SelectedChunk)
		std::cout << "Selected Chunk: " << SelectedChunk->MinX << " " << SelectedChunk->MinZ << std::endl;*/

	if(SelectedChunk)
	{
		Application::get().mScene.lineMesh->AddBoxMinMax({ SelectedChunk->MinX, SelectedChunk->MinY, SelectedChunk->MinZ }, { SelectedChunk->MaxX, SelectedChunk->MaxY, SelectedChunk->MaxZ });
		ImGui::Checkbox("Hidden", &SelectedChunk->hide);
		ImGui::DragFloat("Friction", &SelectedChunk->Friction, 0.01f, 0.0f, 2.0f);
	}

}

Chunk* LandscapeMesh::GetChunkFromPosition(glm::vec3 inPosition)
{
	for (auto chunk : chunks)
	{
		if (chunk->MinX <= inPosition.x && chunk->MaxX >= inPosition.x)
		{
			if (chunk->MinZ <= inPosition.z && chunk->MaxZ >= inPosition.z)
			{
				return chunk;
			}
		}
	}
	return nullptr;
}
std::pair<bool, Triangle> LandscapeMesh::GetTriangleFromPosition(glm::vec3 inPosition)
{
	Chunk* inChunk = GetChunkFromPosition(inPosition);
	float TriangleSize = inChunk->TriangleSize;

	// Find the grid cell that the position lies in
	int NewX = static_cast<int>((inPosition.x - inChunk->MinX) / TriangleSize);  // X position in the grid
	int NewZ = static_cast<int>((inPosition.z - inChunk->MinZ) / TriangleSize);  // Z position in the grid

	// Ensure the position is within bounds of the grid
	if (NewX < 0 || NewX >= inChunk->xCount - 1 || NewZ < 0 || NewZ >= inChunk->zCount - 1)
	{
		//std::cout << "Position out of bounds for the chunk" << std::endl;
		return { false,{0,0,0} }; // Return an empty array if the position is out of bounds
	}

	// Calculate the indices for the triangle at (NewX, NewZ)
	unsigned int TopLeft = (NewX * inChunk->zCount) + NewZ;
	unsigned int TopRight = (NewX * inChunk->zCount) + (NewZ + 1);
	unsigned int BottomLeft = ((NewX + 1) * inChunk->zCount) + NewZ;
	unsigned int BottomRight = ((NewX + 1) * inChunk->zCount) + (NewZ + 1);

	// Check if the position is closer to the top-left triangle or the bottom-right triangle
	glm::vec3 topLeftVertex = inChunk->verticesTriangulated[TopLeft].position;
	glm::vec3 topRightVertex = inChunk->verticesTriangulated[TopRight].position;
	glm::vec3 bottomLeftVertex = inChunk->verticesTriangulated[BottomLeft].position;
	glm::vec3 bottomRightVertex = inChunk->verticesTriangulated[BottomRight].position;

	// Use simple distance checks to determine which triangle the point is in
	glm::vec3 centerOfTriangle1 = (topLeftVertex + topRightVertex + bottomLeftVertex) / 3.0f;
	glm::vec3 centerOfTriangle2 = (bottomLeftVertex + topRightVertex + bottomRightVertex) / 3.0f;

	// Calculate the distances to the centroids of the two triangles
	float distanceToTriangle1 = glm::distance(inPosition, centerOfTriangle1);
	float distanceToTriangle2 = glm::distance(inPosition, centerOfTriangle2);

	if (distanceToTriangle1 < distanceToTriangle2)
	{
		//return { true, {TopLeft, TopRight, BottomLeft} }; // Triangle 1
		Triangle NewTriangle = Triangle(inChunk->verticesTriangulated[TopLeft], inChunk->verticesTriangulated[TopRight], inChunk->verticesTriangulated[BottomLeft]);
		NewTriangle.Friction = inChunk->Friction;
		return { true, NewTriangle }; // Triangle 1
		
	}
	else
	{
		//return{ true, {BottomLeft, TopRight, BottomRight} }; // Triangle 2
		Triangle NewTriangle = Triangle(inChunk->verticesTriangulated[BottomLeft], inChunk->verticesTriangulated[TopRight], inChunk->verticesTriangulated[BottomRight]);
		NewTriangle.Friction = inChunk->Friction;
		return { true, NewTriangle }; // Triangle 2
	}
}
