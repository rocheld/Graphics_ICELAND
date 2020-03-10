//
//  Curve.cpp
//  HW3
//
//  Created by HyunChoi on 2020/02/28.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "Curve.hpp"
namespace {
GLuint programLine;
GLuint vao,vbo;

glm::mat4 B;
glm::vec4 T;
std::vector<glm::vec4> xt;
std::vector<glm::vec3> c_points;
std::vector<glm::vec3> ctrl_points{
    glm::vec3(3,3,3), //p0
    glm::vec3(3,1,1), // p1
    glm::vec3(-1,5,2), // p2
    glm::vec3(-2,4,0), // p3
    
    glm::vec3(-2,4,0), // p3
    glm::vec3(-4,0,-6), // p4
    glm::vec3(-5,3,5), // p5
    glm::vec3(-3,4,5), // p6
    
    glm::vec3(-3,4,5), // p6
    glm::vec3(-1,5,5), //p7
    glm::vec3(-4,-3,6), //p8
    glm::vec3(-6,-2,5), //p9
    
    glm::vec3(-6,-2,5), //p9
    glm::vec3(-8,-7,4), //p10
    glm::vec3(2,1,1), //p 11
    glm::vec3(2,-3,-3), // p12
    
    glm::vec3(2,-3,-3), // p12
    glm::vec3(6,7,7), // 13
    glm::vec3(-2,2,5), //14
    glm::vec3(3,3,3) //15
    
};
};
Curve::Curve() {

    float t = 0.0f;
    float inc = 1.0f/150.0f;
    B[0] = glm::vec4(-1.0f,3.0f,-3.0f,1.0f);
    B[1] = glm::vec4(3.0f,-6.0f,3.0f,0.0f);
    B[2] = glm::vec4(-3.0f,3.0f,0.0f,0.0f);
    B[3] = glm::vec4(1.0f,0.0f,0.0f,0.0f);
    
    for(int i=0; i < ctrl_points.size(); i += 4) {
        c_points.push_back(ctrl_points[i]);
        float t = 0.0f;
        while(t < 1.0f) {
            float t_2 = t*t;
            T = glm::vec4(t_2*t,t_2,t,1.0f);
            t+= inc;
            
            glm::vec4 p_0 = glm::vec4(ctrl_points[i],1.0f);
            glm::vec4 p_1 = glm::vec4(ctrl_points[i+1],1.0f);
            glm::vec4 p_2 = glm::vec4(ctrl_points[i+2],1.0f);
            glm::vec4 p_3 = glm::vec4(ctrl_points[i+3],1.0f);
            
            glm::mat4 G;
            G[0] = p_0;
            G[1] = p_1;
            G[2] = p_2;
            G[3] = p_3;
            
            glm::vec4 res = G*B*T;
            glm::vec3 points = glm::vec3(res.x/res.w, res.y/res.w, res.z/res.w);
            c_points.push_back(points);
        }
        c_points.push_back(ctrl_points[i+3]);
    }
    
    programLine = LoadShaders("shaders/line.vert", "shaders/line.frag");
    glUseProgram(programLine);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c_points.size(),
                 c_points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),0);
    
}

std::vector<glm::vec3> Curve::getPoints() {
    return c_points;
}
Curve::~Curve() {
    
}
void Curve::draw() {
    
    glBindVertexArray(vao);
    // Draw points
    glPointSize(1.2f);
    glDrawArrays(GL_LINE_STRIP, 0,c_points.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Curve::update() {
    
}
