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
#include "Object.h"
#include <vector>

class terrain :  public Object
{
    public:
    int width;
    int height;
    float** map;
    terrain(int size);
    int c_x;
    int c_y;
    void d_step(int x, int y, int cnt, int r);
    void s_step(int x, int y, int cnt, int r);
    
};
#endif /* terrain_hpp */
