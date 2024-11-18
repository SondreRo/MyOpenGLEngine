#pragma once

struct Triangle {
	Triangle(unsigned int a, unsigned int b, unsigned int c) : iA(a), iB(b), iC(c) {}
	Triangle(Vertex a, Vertex b, Vertex c) : vA(a), vB(b), vC(c) {}

	float Friction = 0.0f;

	unsigned int iA, iB, iC;
	Vertex vA, vB, vC;
};