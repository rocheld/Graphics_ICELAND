//
//  robot.hpp
//  HW3
//
//  Created by HyunChoi on 2020/02/26.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef robot_hpp
#define robot_hpp
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

class Robot : Transform
{
    public:
    std::vector<Node*> children;
    glm::mat4 model = glm::mat4(1.0f);
    
    Transform* robot_body;
    Transform* robot_left_arm;
    Transform* robot_right_arm;
    Transform* robot_right_leg;
    Transform* robot_left_leg;
    Transform* robot_head;
    Transform* robot_left_eye;
    Transform* robot_right_eye;
    
    Geometry* body;
    Geometry* limb;
    Geometry* head;
    Geometry* eye;
    Transform* robot;
    
    bool isLimb;
    bool right;
    bool left;
    bool arm;
    bool leg;
    
    Robot();
    ~Robot();
    Transform* getRobot();
    virtual void draw(glm::mat4 m);
    virtual void update();
    
};
#endif /* robot_hpp */
