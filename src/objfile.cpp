//
//  objfile.cpp
//  base
//
//  Created by Peta Douglas on 15/07/19.
//

#include "objfile.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <glm/glm.hpp>
#include <opengl.hpp>
#include <algorithm>
#include <stdio.h>
using namespace std;

void ObjFile::loadOBJ(const char *filename){
 
    //used this for help with coding http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    
    int indicesCount = 0;
    
    FILE * file = fopen(filename, "r");
    if( file == NULL ){
        printf("Can't open file \n");
    }
    
    while( 1 ){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        
        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
           // cout << "v: " << vertex.x << " " << vertex.y << " " << vertex.z << " " << endl;
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
            //cout << "vt: " << uv.x << " " << uv.y << endl;
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
            //cout << "vn: " << normal.x << " " << normal.y << " " << normal.z << " " << endl;
        }
        
        else if ( strcmp( lineHeader, "f" ) == 0 ){
            unsigned int vertexIndex[3];
            unsigned int uvIndex[3];
            unsigned int normalIndex[3];
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
           // std::cout << "f: " << vertexIndex[0] << " " << uvIndex[0] << " " << normalIndex[0] << " " << vertexIndex[1] << " " << uvIndex[1] << " " << normalIndex[1] << " " << vertexIndex[2] << " " << uvIndex[2] << " " << normalIndex[2] << endl;
        }
        
    }
    
    for(int i = 0; i < vertexIndices.size(); i++ ){
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 v = temp_vertices[vertexIndex-1];
        m_indices.push_back(indicesCount);
        out_vertices.push_back(v);
        indicesCount++;
    }
    for(int i = 0; i < normalIndices.size(); i++ ){
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 n = temp_normals[normalIndex-1];
        m_indices.push_back(indicesCount);
        out_normals.push_back(n);
        indicesCount++;
    }
    for(int i = 0; i < uvIndices.size(); i++ ){
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[uvIndex-1];
        out_uvs.push_back(uv);
    }
}

    void ObjFile::build() {
        if (m_vao == 0) {
            // generate buffers
            glGenVertexArrays(1, &m_vao); // VAO stores information about how the buffers are set up
            glGenBuffers(1, &m_vbo_pos); // VBO stores the vertex data
            glGenBuffers(1, &m_vbo_norm);
            glGenBuffers(1, &m_ibo); // IBO stores the indices that make up primitives
            
            
            // VAO
            //
            glBindVertexArray(m_vao);
            
            
            // VBO
            //
            // upload Positions to this buffer
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * out_vertices.size(), out_vertices.data(), GL_STATIC_DRAW);
            // this buffer will use location=0 when we use our VAO
            glEnableVertexAttribArray(0);
            // tell opengl how to treat data in location=0 - the data is treated in lots of 3 (3 floats = vec3)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            
            // do the same thing for Normals but bind it to location=1
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * out_normals.size(), out_normals.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            
            
            // IBO
            //
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            // upload the indices for drawing primitives
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
            
            // clean up by binding VAO 0 (good practice)
            glBindVertexArray(0);
        }
    }
    
    
    void ObjFile::draw() {
        if (m_vao == 0) return;
        // bind our VAO which sets up all our buffers and data for us
        glBindVertexArray(m_vao);
        // tell opengl to draw our VAO using the draw mode and how many verticies to render
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }
    
    void ObjFile::printMeshData() {
      
        for (int i = 0; i < out_vertices.size(); i++) {
            printf("v:\t%f\t%f\t%f\n", out_vertices[i].x, out_vertices[i].y, out_vertices[i].z);
        }
        
        for (int i = 0; i < out_uvs.size(); i++) {
            printf("vt:\t%f\t%f\n", out_uvs[i].x, out_uvs[i].y);
        }
        
        for (int i = 0; i < out_normals.size(); i++) {
            printf("vn:\t%f\t%f\t%f\n", out_normals[i].x, out_normals[i].y, out_normals[i].z);
        }
        
         for (int i = 0; i < vertexIndices.size(); i = i+3) {
         printf("f:\t%u/%u/%u\t", vertexIndices[0+i], uvIndices[0+i], normalIndices[0+i]);
         printf("%u/%u/%u\t", vertexIndices[1+i], uvIndices[1+i], normalIndices[1+i]);
         printf("%u/%u/%u\n", vertexIndices[2+i], uvIndices[2+i], normalIndices[2+i]);
         }
    
    }

    void ObjFile::destroy() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo_pos);
        glDeleteBuffers(1, &m_vbo_norm);
        glDeleteBuffers(1, &m_ibo);
        m_vao = 0;
    }
    
