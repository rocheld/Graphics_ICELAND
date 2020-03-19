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
#include "stb_image_aug.h"
using namespace std;

Terrain::~Terrain() {
    
}
Terrain::Terrain(int size) {
    model = glm::mat4(1.0f);
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    maxHeight = 0.0f;
    glm::vec3 center ;
    float cx = (float) size / 2.0f;
    this->width = size-1;
    this->height = size-1;
    this->squareLen = size -1;
    this->size = size;
    // set four corners
    int x = width;
    int y = height;
    cnt = 0;
    vector<float> temp;
    
    for(int i=0; i < size; i++) {
        map.push_back(temp);
        for(int j=0; j < size; j++) {
            map[i].push_back(0.0f);
        }
    }
    
    // Initialize four points
    srandom((unsigned)time(NULL));
    map[0][0] = rand() % 11;
    map[width][0] = rand() % 11;
    map[0][height] = rand() % 11;
    map[height][width] = rand() % 11;
    int half;
   
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

    for(int i=0; i <5; i++)
        filter(0.75f);
    
   
    center = glm::vec3(-cx,-maxHeight,cx);
    model = glm::translate(model,center);
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
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    loadTexture();
    
    // Normal
    // GENERATE A NEW VERTEX BUFFER
    glGenBuffers(1, &vbo2);
    // Bind it as a GL_ARRAY_BUFFER type
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    // Pass in the data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normal.size(),normal.data(),GL_STATIC_DRAW);
    //Enable the next attribute array
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),0);
    
    // Unbind from the VBO.
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
    
}
void Terrain::update() {
    
}
void Terrain::filter(float k) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            
            float h;
            if(i == width)
            h = map[i-1][j] * (1-k) + map[i][j] * k;
            else
            h = map[i+1][j] * (1-k) + map[i][j] * k;
            float z;
            if( j == width)
                z = map[i][j+1] * (1-k) + map[i][j] * k;
            else
                z = map[i][j-1] * (1-k) + map[i][j] * k;
            
            map[i][j] = (h+z)/2.0f;
            
            if(h > maxHeight) {
                maxHeight = h;
            }
        }
            
    }
}
void Terrain::draw() {
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Set point size.
    glPointSize(0.1f);
    // Draw points
    glDrawArrays(GL_POINTS, 0, vertex.size());
    // Draw triangles
    glDrawElements(GL_TRIANGLES, 3*face.size(), GL_UNSIGNED_INT, 0);
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
    float down = 0.0f;
    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float avg = 0.0f;
    int cnt = 0;
    
    if(half < 1)
        return;
    
    if(x-half >= 0) {
        left = map[x-half][y];
        avg += left;
        cnt++;
    }
    
    if(y-half >=0) {
        down = map[x][y-half];
        avg += down;
        cnt++;
    }
    if(x+half < size) {
        right = map[x+half][y];
        avg += right;
        cnt++;
    }
    if(y+half < size) {
        top = map[x][y+half];
        avg += top;
        cnt++;
    }
    if(cnt !=0)
        map[x][y] = avg/cnt + r;

}

void Terrain::mesh() {
    for(float i=0; i < size; i++) {
        for(float j=0; j < size; j++) {
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
        
        glm::vec3 t1 = vertex[i];
        glm::vec3 t2 = vertex[i+1];
        glm::vec3 t3 = vertex[i+1+width];
         
        glm::vec3 v1 = t2-t1;
        glm::vec3 v2 = t3-t1;
               
        glm::vec3 n = normalize(glm::cross(v1,v2));
        normal.push_back(n);
        t1 = vertex[i+1];
        t2 = vertex[i+1+size];
        t3 = vertex[i+size];
        v1 = t2-t1;
        v2 = t3-t1;
        n = normalize(glm::cross(v1,v2));
        normal.push_back(n);
    }
}
void Terrain::loadTexture() {
    int width, height, nrChannels;
    string file = "terrain.png";
    string file2 = "terrain2.png";
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels,0);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    if(data) {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, width,height,0,GL_RGB, GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "TERRAIN TExture error" << endl;
    }
    stbi_image_free(data);

}
void Terrain::generate() {
        vector<glm::vec3> v;
        vector<glm::ivec3> f;
        vector<glm::vec3> nn;
        maxHeight = 0;
    
        for(int i=0; i< size-1; i++) {
            for(int j=0; j < size-1; j++) {
                map[i][j] = 0.0f;
            }
        }
        map[0][0] = rand() % 11;
        map[width][0] = rand() % 11;
        map[0][height] = rand() % 11;
        map[height][width] = rand() % 11;
        int half;
        squareLen = size-1;
        while(squareLen > 1) {
            half = squareLen / 2;
            
            for(int i =0; i < this->height; i += squareLen) {
                for(int j=0; j < this->width; j += squareLen) {
                    int r =rand()%(squareLen+1);
                    d_step(j,i,squareLen,r);
                }
            }
            
            for(int i=0; i < this->height; i += half) {
                for(int j=0; j < this->width; j += half) {
                    s_step(j,i,half,rand()%(squareLen+1));
                }
            }
            squareLen -= half;
        }
        for(int i=0; i < 5; i++)
            filter(0.75f);

        float cx1 = size / 2.0f;
        model = glm::mat4(1.0f);
        glm::vec3 center = glm::vec3(-cx1,-maxHeight,cx1);
        model = glm::translate(model,center);
    
        for(int i=0; i < size; i ++) {
            for(int j=0; j < size; j++) {
                glm::vec3 temp;
                temp = glm::vec3(i,map[i][j],j);
                v.push_back(temp);
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
        temp1 = glm::ivec3(i,i+1,i+1+width);
        temp2 = glm::ivec3(i+1,i+1+size,i+size);
        f.push_back(temp1);
        f.push_back(temp2);
        
        
        glm::vec3 t1 = v[i];
        glm::vec3 t2 = v[i+1];
        glm::vec3 t3 = v[i+1+width];
         
        glm::vec3 v1 = t2-t1;
        glm::vec3 v2 = t3-t1;
               
        glm::vec3 n = normalize(glm::cross(v1,v2));
        nn.push_back(n);
        t1 = v[i+1];
        t2 = v[i+1+size];
        t3 = v[i+size];
        v1 = t2-t1;
        v2 = t3-t1;
        n = normalize(glm::cross(v1,v2));
        nn.push_back(n);
    }
        vertex = v;
        face = f;
        normal = nn;
    
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
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    loadTexture();
    
    // Normal
    // GENERATE A NEW VERTEX BUFFER
    glGenBuffers(1, &vbo2);
    // Bind it as a GL_ARRAY_BUFFER type
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    // Pass in the data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normal.size(),normal.data(),GL_STATIC_DRAW);
    //Enable the next attribute array
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),0);
    
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}


