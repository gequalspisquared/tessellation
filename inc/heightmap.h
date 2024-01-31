#pragma once

#include <vector>

class Heightmap {
public:
    Heightmap(const char* heigthmap_path, float scale, float shift);

    void update_vao();
    void draw();

private:
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int m_vao;
    unsigned int m_width;
    unsigned int m_height;
};