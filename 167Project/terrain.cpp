//
//  terrain.cpp
//  HW3
//
//  Created by HyunChoi on 2020/03/09.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "Terrain.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

Terrain::Terrain(int size) {
    model = glm::mat4(1.0f);
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    glm::vec3 center ;
    float cx = (float) size / 2.0f;
    center = glm::vec3(-cx,-50.0f,-cx);
    model = glm::translate(model,center);
    
    this->width = size-1;
    this->height = size-1;
    this->squareLen = size -1;
    this->size = size;
    // set four corners
    int x = width;
    int y = height;
    
    vector<float> temp;
    
    for(int i=0; i < size; i++) {
        map.push_back(temp);
        for(int j=0; j < size; j++) {
            map[i].push_back(0.0f);
        }
    }
    
    // Initialize four points
    map[0][0] = 10.0f;
    map[width][0] = 5.0f;
    map[0][height] = 3.0f;
    map[height][width] = 10.0f;
    int half;
    srandom((unsigned)time(NULL));
    while(squareLen > 1) {
        half = squareLen / 2;
        
        for(int i =0; i < this->height; i += squareLen) {
            for(int j=0; j < this->width; j += squareLen) {
                d_step(j,i,squareLen,rand()%(squareLen));
            }
        }
        
        for(int i=0; i < this->height; i += half) {
            for(int j=0; j < this->width; j += half) {
                s_step(j,i,half,rand()%(squareLen));
            }
        }
        squareLen -= half;
    }
    /*
    int cnt = 0;
    for(int i=0; i < size; i ++) {
        for(int j=0; j < size; j++) {
            cout << cnt << " " << i << j<< " " << map[i][j] << endl;
            cnt++;
        }
    }
    */
    mesh();
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    // This tells OpenGL which data it should be paying attention to
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertex.size(),
        vertex.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Triangle
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)*face.size(), face.data(), GL_STATIC_DRAW);
    

    
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
    
}
void Terrain::update() {
    
}
void Terrain::draw() {
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Set point size.
    glPointSize(0.1f);
    // Draw points
    glDrawArrays(GL_POINTS, 0, vertex.size());
    // Draw triangles
    glDrawElements(GL_LINES, 3*face.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
void Terrain::d_step(int x, int y, int size, int r) {
    
    float topLeft,topRight,botLeft,botRight;
    float avg;
    int cnt = 0;
    
    botLeft = map[x][y];
    topLeft = map[x][y+size];
    topRight = map[x+size][y+size];
    botRight = map[x+size][y];
    
    avg = topRight + topLeft + botLeft + botRight;
    avg /= 4.0f;
    
    int half = size/2;
    map[x+half][y+half] = avg;
    
}

void Terrain::s_step(int x, int y, int half, int r) {
    float down;
    float left;
    float right;
    float top;
    
    int whole = half+half;
    float center = map[x+half][y+half];
    if(half >= 1 && x+whole < size && y+whole < size) {
        down = (map[x][y] + center + map[x+half+half][y])/3;
        left = (map[x][y] + center + map[x][y+whole])/3;
        top = (map[x][y+half] + center + map[x+whole][y+whole])/3;
        right = (map[x+whole][y+whole] + center + map[x+whole][y])/3;
        
        map[x+half][y] = down + r; // down
        map[x][y+half] = left + r; // left
        map[x+half][y+whole] = top +r; // top
        map[x+whole][y+half] = right + r; // right
    }
}

void Terrain::mesh() {
    for(int i=0; i < size; i ++) {
        for(int j=0; j < size; j++) {
            glm::vec3 temp;
            temp = glm::vec3(i,map[i][j],j);
            vertex.push_back(temp);
        }
    }
    
    int looper = size*(size-1);
    int count = 0;
    for(int i=0; i <looper ; i++) {
        if(count == size-1) {
            count = 0;
            continue;
        }
        count++;
        glm::ivec3 temp1;
        glm::ivec3 temp2;
        //cout << i+1 << i+1+size << i+size << endl;
        temp1 = glm::ivec3(i,i+1,i+1+width);
        temp2 = glm::ivec3(i+1,i+1+size,i+size);
        face.push_back(temp1);
        face.push_back(temp2);
    }
}
