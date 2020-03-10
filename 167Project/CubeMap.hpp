//
//  CubeMap.hpp
//  HW3
//
//  Created by HyunChoi on 2020/02/24.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef CubeMap_hpp
#define CubeMap_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include "Object.h"
#include <stdio.h>
#include "SOIL.h"
class CubeMap: public Object
{
    private:
        GLuint texture[1];  
        GLuint vao, vbos[2];
        unsigned int myTexture;
        std::vector<std::string> mapFile
        {
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "base.jpg",
            "front.jpg",
            "back.jpg"
        };
        std::vector<std::string> faces
        {
            "right.ppm",
            "left.ppm",
            "top.ppm",
            "base.ppm",
            "front.ppm",
            "back.ppm"
        };
    public:
        CubeMap();
        ~CubeMap();
        void draw();
        void update();
        //unsigned char* loadPPM(const char* filename, int& width, int& height);
        GLuint loadTexture();
        void loadTexture2();
        unsigned char* loadPPM(const char* filename, int& width, int& height);
};
#endif /* CubeMap_hpp */
