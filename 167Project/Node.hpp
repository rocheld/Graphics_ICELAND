//
//  graph.hpp
//  HW3
//
//  Created by HyunChoi on 2020/02/26.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef node_hpp
#define node_hpp

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

class Node
{
    public:
    Node();
    ~Node();
    virtual void draw(glm::mat4 c);
    virtual void update();
};

#endif /* graph_hpp */
