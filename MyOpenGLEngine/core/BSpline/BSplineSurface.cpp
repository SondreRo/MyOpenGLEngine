#include "BSplineSurface.h"

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

// B-spline basis function for degree 2 (bi-quadratic)
float BSplineSurface::bSplineBasis2(int i, float t, int m) {
    if (i == 0)
        return (1 - t) * (1 - t) / 2.0f;
    else if (i == 1)
        return (-2 * t * t + 2 * t + 1) / 2.0f;
    else if (i == 2)
        return t * t / 2.0f;
    return 0.0f;
}

// Function to evaluate the B-spline surface point at (u, v)
glm::vec3 BSplineSurface::evaluateBSplineSurface(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPoints) {
    int m = controlPoints.size();       // Number of control points in u direction
    int n = controlPoints[0].size();    // Number of control points in v direction

    glm::vec3 point(0.0f);  // Initialize the surface point at (u, v)

    // Bi-quadratic B-spline basis function uses three control points
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            float Bu = bSplineBasis2(i, u, m);  // Basis function for u
            float Bv = bSplineBasis2(j, v, n);  // Basis function for v
            point += Bu * Bv * controlPoints[i][j];  // Weighted sum of control points
        }
    }

    return point;
}

// Generate the mesh vertices
std::vector<glm::vec3> BSplineSurface::generateBSplineMesh(const std::vector<std::vector<glm::vec3>>& controlPoints, int resolutionU, int resolutionV) {
    std::vector<glm::vec3> vertices;  // Store the vertices of the mesh

    for (int i = 0; i <= resolutionU; ++i) {
        float u = static_cast<float>(i) / resolutionU;
        for (int j = 0; j <= resolutionV; ++j) {
            float v = static_cast<float>(j) / resolutionV;
            glm::vec3 vertex = evaluateBSplineSurface(u, v, controlPoints);  // Compute surface point
            vertices.push_back(vertex);  // Add vertex to the list
        }
    }

    return vertices;
}

// Triangulate the mesh
std::vector<unsigned int> BSplineSurface::triangulate(int resolutionU, int resolutionV) {
    std::vector<unsigned int> indices;  // Store the indices for triangles

    for (int i = 0; i < resolutionU; ++i) {
        for (int j = 0; j < resolutionV; ++j) {
            int row1 = i * (resolutionV + 1);
            int row2 = (i + 1) * (resolutionV + 1);

            // First triangle
            indices.push_back(row1 + j);
            indices.push_back(row2 + j);
            indices.push_back(row1 + j + 1);

            // Second triangle
            indices.push_back(row2 + j);
            indices.push_back(row2 + j + 1);
            indices.push_back(row1 + j + 1);
        }
    }

    return indices;
}