#include <vector>

#include <stb/stb_image.h>
#include <glad/glad.h>

#include "heightmap.h"

typedef struct ImageData {
    unsigned char* data;
    int width;
    int height;
    int num_channels;
} ImageData;

std::vector<float> create_vertices_from_data(const ImageData& image_data, float scale, float shift);
std::vector<unsigned int> create_indices_from_data(const ImageData& image_data);

Heightmap::Heightmap(const char* heightmap_path, float scale, float shift) {
    int width, height, num_channels;
    unsigned char* data = stbi_load(heightmap_path, 
                                    &width, &height, &num_channels, 0);
    
    ImageData image_data {data, width, height, num_channels};
    m_width = width;
    m_height = height;
    
    m_vertices = create_vertices_from_data(image_data, scale, shift);
    m_indices = create_indices_from_data(image_data);
    stbi_image_free(data);

    update_vao();
}

void Heightmap::update_vao() {
    // static bool ebo_created = false;
    static unsigned int ebo;

    unsigned int vbo;
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices[0]), &m_vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // if (!ebo_created) {
    // ebo_created = true;

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(m_indices[0]), &m_indices[0], GL_STATIC_DRAW);

    // }
}

void Heightmap::draw() {
    const unsigned int NUM_STRIPS = m_height - 1;
    const unsigned int NUM_VERTS_PER_STRIP = m_width * 2;
    glBindVertexArray(m_vao);
    for (unsigned int strip = 0; strip < NUM_STRIPS; strip++) {
        glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT, 
                       (void*)(sizeof(unsigned int) * NUM_VERTS_PER_STRIP * strip));
    }
}

std::vector<float> create_vertices_from_data(const ImageData& image_data, float scale, float shift) {
    std::vector<float> vertices;
    for (unsigned int i = 0; i < image_data.height; i++) {
        for (unsigned int j = 0; j < image_data.width; j++) {
            unsigned char* texel = image_data.data + (j + image_data.width * i) * image_data.num_channels;
            unsigned char y = texel[0];

            vertices.push_back(-image_data.height / 2.0f + i); // x
            vertices.push_back( (int)y * scale - shift);       // y
            vertices.push_back(-image_data.width / 2.0f + j);  // z
        }
    }

    return vertices;
}

std::vector<unsigned int> create_indices_from_data(const ImageData& image_data) {
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < image_data.height - 1; i++) {
        for (unsigned int j = 0; j < image_data.width; j++) {
            for (unsigned int k = 0; k < 2; k++) {
                indices.push_back(j + image_data.width * (i + k));
            }
        }
    }

    return indices;
}