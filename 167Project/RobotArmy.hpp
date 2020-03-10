//
//  RobotArmy.hpp
//  HW3
//
//  Created by HyunChoi on 2020/02/27.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef RobotArmy_hpp
#define RobotArmy_hpp
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <stdio.h>
#include "Node.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"
#include "Robot.hpp"

class RobotArmy : Node
{
    public:
    Transform* army;
    Transform* centerRobot;
    int army_size;
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 center;
    std::vector<glm::vec3> cPoints;
    int curr =0;
    RobotArmy();
    ~RobotArmy();
    void rotate(float v,glm::vec3 r);
    void scaleUp();
    void scaleDown();
    void setCurve(std::vector<glm::vec3> cc);
    glm::vec3 getCenter();
    virtual void draw(glm::mat4 m);
    virtual void update();
    
};
#endif /* RobotArmy_hpp */
