//
//  Transform.hpp
//  HW3
//
//  Created by HyunChoi on 2020/02/26.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <vector>
#include "Node.hpp"

class Transform : public Node
{   public:
    
    std::vector<Node*> children;
    glm::mat4 model;
    glm::mat4 new_model;
    glm::vec3 posFromOrigin;
    
    float rotateFactor;
    bool right, arm;
    bool move_back;
    bool isLimb;
    Transform();
    ~Transform();
    
    void addChild(Node* n);
    void removeChild();
    void setTranslate(glm::vec3 t);
    void setRotation(glm::vec3 p,float v, glm::vec3 r);
    void setScale(float s);
    void setPart(bool isLimb,bool right, bool arm);
    // abstract
    virtual void draw(glm::mat4 c);
    virtual void update();
};
#endif /* Transform_hpp */
