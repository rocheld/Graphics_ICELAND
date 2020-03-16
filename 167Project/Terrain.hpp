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

using namespace std;

class Terrain
{
    private:
    int width;
    int height;
    int squareLen;
    int c_x;
    int c_y;
    vector<vector<float> > h_map;
    
    
    public:
    Terrain(int size);
    void d_step(int x, int y, int size, int r);
    void s_step(int x, int y, int size, int r);
    
};
#endif /* terrain_hpp */
