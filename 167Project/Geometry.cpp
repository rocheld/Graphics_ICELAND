//
//  Geometry.cpp
//  HW3
//
//  Created by HyunChoi on 2020/02/26.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "Geometry.hpp"
#include "shader.h"
#include <iostream>

namespace
    {
    GLuint program;
    GLuint modelLoc;
    };

Geometry::Geometry(std::string filename) {
        FILE* fp;
        int c1,c2;
        float x,y,z, x_n,y_n,z_n;
        float v1,v2,v3,v4,v5,v6,v7,v8,v9;
        
        fp = fopen(filename.c_str(),"rb");
        if (fp==NULL) {std::cerr << "GEOMETRY error loading file" <<std::endl; exit(-1);}
        
        do {
            c1 = fgetc(fp);
            
            if(c1=='v') {
                c1 = fgetc(fp);
                if( c1 == ' ') {
                    fscanf(fp, "%f %f %f\n", &x, &y, &z);
                    points_input.push_back(glm::vec3(x,y,z));
                }
                
                else if(c1 == 'n') {
                    fscanf(fp, "%f %f %f\n", &x_n, &y_n, &z_n);
                    glm::vec3 input = glm::vec3(x_n,y_n,z_n);
                    glm::vec3 norm = 0.5f*normalize(input) + 0.5f;
                    normals_input.push_back(norm);
                }
            }
            else if(c1=='f') {
                c1 = fgetc(fp);
                if( c1 == ' ') {
                    fscanf(fp, "%f/%f/%f %f/%f/%f %f/%f/%f\n",&v1,&v2,&v3,&v4,&v5,&v6,&v7,&v8,&v9);
                    vertices_indices.push_back(v1-1);
                    vertices_indices.push_back(v4-1);
                    vertices_indices.push_back(v7-1);
                    normal_indices.push_back(v3-1);
                    normal_indices.push_back(v6-1);
                    normal_indices.push_back(v9-1);
                }
            }
            else if(c1 == '#')  {
                char c;
                do {
                    c = fgetc(fp);
                } while(c != '\n');
            }
        } while(c1 != EOF);
        for(int i=0; i < vertices_indices.size(); i++) {
            vertices__.push_back(points_input[vertices_indices[i]]);
            normals__.push_back(normals_input[normal_indices[i]]);
            if(i%3 ==0)
                indices.push_back(glm::ivec3(i,i+1,i+2));
            
        }
        
        // end of parsing
        program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
        glUseProgram(program);
        modelLoc = glGetUniformLocation(program, "model");
        
        // Vertices
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        // Bind to the first VBO. We will use it to store the points.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Pass in the data.
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices__.size(),
                     vertices__.data(), GL_STATIC_DRAW);
        
        // Triangle
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)*indices.size(), indices.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        // Normals
        glGenBuffers(1, &vbo2);
        glBindBuffer(GL_ARRAY_BUFFER, vbo2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals__.size(),normals__.data(),GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),0);
        
        // Unbind from the VBO.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbind from the VAO.
        glBindVertexArray(0);
    //other initialization
    model = glm::mat4(1.0f);
    
}

Geometry::~Geometry() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo2);
        glDeleteProgram(program);
}

void Geometry::draw(glm::mat4 m) {
    
    model = m;
        //redering
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Bind to the VAO.
        glBindVertexArray(vao);
        // Set point size.
        glPointSize(1.0f);
        // Draw points
        glDrawArrays(GL_POINTS, 0, vertices__.size());
        glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_INT, 0);
        // Unbind from the VAO.
        glBindVertexArray(0);

}

void Geometry::update() {
}
