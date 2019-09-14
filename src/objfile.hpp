//
//  objfile.hpp
//  base
//
//  Created by Peta Douglas on 15/07/19.
//

#ifndef objfile_hpp
#define objfile_hpp

#include <cstdio>
#pragma once

// std
#include <vector>

// glm
#include <glm/glm.hpp>

// project
#include "opengl.hpp"


class ObjFile {
private:
    
    // CPU-side data
    
    std::vector<unsigned int> m_indices;
    
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    
    std::vector < glm::vec3 > out_vertices;
    std::vector < glm::vec2 > out_uvs;
    std::vector < glm::vec3 > out_normals;
    
    
    // GPU-side data
    GLuint m_vao = 0;
    GLuint m_vbo_pos = 0;
    GLuint m_vbo_norm = 0;
    GLuint m_ibo = 0;
    
public:
    
    void loadOBJ(const char *filename);
    void build();
    void draw();
    void destroy();
    void printMeshData();
    
};
#endif /* objfile_hpp */
