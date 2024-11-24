#pragma once
#include <vector>

#include "Vertex.h"

class BSpline
{
public:


    static float BlendingFunction(int i, int degree, float t, const std::vector<float>& knots) {
        if (degree == 0) {
            return (knots[i] <= t && t < knots[i + 1]) ? 1.0f : 0.0f;
        }
        else {
            float a = 0.0f;
            if (i + degree < knots.size() && knots[i + degree] != knots[i]) {
                a = (t - knots[i]) / (knots[i + degree] - knots[i]);
            }

            float b = 0.0f;
            if (i + degree + 1 < knots.size() && knots[i + degree + 1] != knots[i + 1]) {
                b = (knots[i + degree + 1] - t) / (knots[i + degree + 1] - knots[i + 1]);
            }

            return a * BlendingFunction(i, degree - 1, t, knots) + b * BlendingFunction(i + 1, degree - 1, t, knots);
        }
    }

    static glm::vec3 calculateBSpline(const std::vector<glm::vec3>& control_points, float t, const std::vector<float>& knots) {
        glm::vec3 point(0.0f);
        int n = control_points.size() - 1;
        int degree = 3; // For cubic B-Spline

        for (int i = 0; i <= n; i++) {
            float blendingFunctionValue = BlendingFunction(i, degree, t, knots);
            point += blendingFunctionValue * control_points[i];
        }

        return point;
    }

    static std::vector<float> GenerateKnotVector(int num_control_points, int degree) {
        int num_knots = num_control_points + degree + 1;
        std::vector<float> knots(num_knots);

        // Set the first and last 'degree+1' knots
        for (int i = 0; i <= degree; i++) {
            knots[i] = 0.0f;
            knots[num_knots - 1 - i] = 1.0f;
        }

        // Set the remaining knots
        for (int i = degree + 1; i < num_knots - degree - 1; i++) {
            knots[i] = 1.0f * (i - degree) / (num_control_points - degree);
        }

        return knots;
    }

    static std::vector<glm::vec3> GeneratePoints(std::vector<glm::vec3> control_points, std::vector<float>& knots, int num_points) {

		if (control_points.size() < 4)
		{
			return {};
		}

        if (knots.empty())
        {
            knots = GenerateKnotVector(control_points.size(), 3);

        }
        std::vector<glm::vec3> curve_points;
        float increment = 1.0f / (num_points - 1);
        for (float t = 0; t <= 1; t += increment) {
            glm::vec3 point_on_curve = calculateBSpline(control_points, t, knots);
            curve_points.push_back(point_on_curve);
        }
        return curve_points;
    }

};
