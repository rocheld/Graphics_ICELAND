//
//  CubeMap.cpp
//  HW3
//
//  Created by HyunChoi on 2020/02/24.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "CubeMap.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "stb_image_aug.h"
CubeMap::CubeMap() {
   model = glm::mat4(1.0f);

   color = glm::vec3(1.0f, 0.95f, 0.1f);

   /*
    * Cube indices used below.
    *    4----7
    *   /|   /|
    *  0-+--3 |
    *  | 5--+-6
    *  |/   |/
    *  1----2
    *
    */

    // The 8 vertices of a cube.
   std::vector<glm::vec3> vertices
   {
       glm::vec3(-500, 500, 500),
       glm::vec3(-500, -500, 500),
       glm::vec3(500, -500, 500),
       glm::vec3(500, 500, 500),
       glm::vec3(-500, 500, -500),
       glm::vec3(-500, -500, -500),
       glm::vec3(500, -500, -500),
       glm::vec3(500, 500, -500)
   };

   // Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2
   // and v3 in counter-clockwise order.
   std::vector<glm::ivec3> indices
   {
       // Front face.
       glm::ivec3(0, 1, 2),
       glm::ivec3(2, 3, 0),
       // Back face.
       glm::ivec3(7, 6, 5),
       glm::ivec3(5, 4, 7),
       // Right face.
       glm::ivec3(3, 2, 6),
       glm::ivec3(6, 7, 3),
       // Left face.
       glm::ivec3(4, 5, 1),
       glm::ivec3(1, 0, 4),
       // Top face.
       glm::ivec3(4, 0, 3),
       glm::ivec3(3, 7, 4),
       // Bottom face.
       glm::ivec3(1, 5, 6),
       glm::ivec3(6, 2, 1),
   };

   // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
   glGenVertexArrays(1, &vao);
   glGenBuffers(2, vbos);

   // Bind to the VAO.
   glBindVertexArray(vao);

   // Bind to the first VBO. We will use it to store the vertices.
   glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
   // Pass in the data.
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
       vertices.data(), GL_STATIC_DRAW);
   // Enable vertex attribute 0.
   // We will be able to access vertices through it.
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

   // Bind to the second VBO. We will use it to store the indices.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
   // Pass in the data.
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
       indices.data(), GL_STATIC_DRAW);

   // Unbind from the VBOs.
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   // Unbind from the VAO.
   glBindVertexArray(0);
   myTexture = loadTexture();
    //loadTexture();
}
void CubeMap::draw() {
    
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, myTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    
}
void CubeMap::update(){
    
}

CubeMap::~CubeMap() {
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}


unsigned char* CubeMap::loadPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;

    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }

    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);

    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);

    // Read maxval:
    do
    {
      retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');

    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }

    return rawData;
}

// load image file into texture object
void CubeMap::loadTexture2()
{
  GLuint texture[1];     // storage for one texture
  int twidth, theight;   // texture width/height [pixels]
  unsigned char* tdata;  // texture pixel data
  // Create ID for texture
  glGenTextures(1, &texture[0]);

  // Set this texture to be the one we are working with
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);
  
  // Load image file
  for(int i=0; i < faces.size(); i++) {
      tdata = loadPPM(faces[i].c_str(), twidth, theight);
      if (tdata==NULL) return;
      // Generate the texture
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0,GL_RGB,twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  myTexture = texture[0];
}


unsigned int CubeMap::loadTexture()
{
  unsigned int textureID;     // storage for one texture
  int twidth, theight,nrch;   // texture width/height [pixels]
  unsigned char* tdata = 0;  // texture pixel data
  // Create ID for texture
  glGenTextures(1, &textureID);
  // Set this texture to be the one we are working with
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Load image file
  for(int i=0; i < mapFile.size(); i++) {
      std::string path = mapFile[i];
      tdata = stbi_load(path.c_str(), &twidth, &theight, &nrch,0);
      if (tdata) {
          // Generate the texture
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0,GL_RGB,twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
          stbi_image_free(tdata);
    }
  }
  // Set bi-linear filtering for both minification and magnification
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  
  return textureID;
}
