//
//  terrain.hpp
//  HW3
//
//  Created by HyunChoi on 2020/03/09.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef terrain_hpp
#define terrain_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <vector>

#include <vector>
#include "Object.h"
using namespace std;

class Terrain : public Object
{
    private:
    int width;
    int height;
    int squareLen;
    int c_x;
    int c_y;
    vector<vector<float> > map;
    vector<glm::ivec3> face;
    vector<glm::vec3> vertex;
    vector<glm::vec3> normal;
    unsigned int texture, texture2;
    GLuint vao, vao2, vbo, vbo2, ebo; // Added vbo2
    int size;
    int cnt;
    public:
    Terrain(int size);
    ~Terrain();
    int maxHeight;
    void d_step(int x, int y, int size, int r);
    void s_step(int x, int y, int size, int r);
    void mesh();

    void draw();
    void update();
    void loadTexture();
    void filter(float k);
    void generate();
};
#endif /* terrain_hpp */
