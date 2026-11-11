#pragma once

#include <vector>
#include <cmath>

#include "glfw.cc"

const float point_step = 0.001f;
const float grid_step = 0.0001f;

inline std::vector<float> hyperbole_mesh() {

    float a2 = A * A;
    float b2 = B * B;

    std::vector<float> points;

    if (A == 0 || B == 0) {
        return points;
    }

    float R = Zoom;

    float step = point_step * Zoom * 0.01;

    int total = (int)((2.0f * R) / step);

    for (int i = 0; i < total; i++) {

        float x = (i * step) - R;

        // sqrt(b^2 * ((x - dx)^2 / a^2 + 1)) = y + dy

        float tmpx = x - DX;

        float tmpy = b2 * (((tmpx * tmpx) / a2) + 1);

        if (tmpy < 0.0f) {
            continue;
        }

        float y = std::sqrt(tmpy);

        points.push_back(x);
        points.push_back(y + DY);
        points.push_back(0.0f);

        points.push_back(x);
        points.push_back(-y + DY);
        points.push_back(0.0f);
    }
    return points;
}

inline std::vector<float> grid_mesh() {

    std::vector<float> mesh = {
        0.0f, 1, 0.0f,
        0.0f, -1, 0.0f,
        1, 0.0f, 0.0f,
        -1, 0.0f, 0.0f
    };

    float R = Zoom;

    int total = (int)((2.0f * R) / point_step);

    for (int i = 0; i < total; i++) {

    }

    return mesh;
}
