//
//  robot.cpp
//  HW3
//
//  Created by HyunChoi on 2020/02/26.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "Robot.hpp"
Robot::Robot() {
    
    robot = new Transform();

    robot_body = new Transform();
    robot_left_arm = new Transform();
    robot_right_arm = new Transform();
    robot_right_leg = new Transform();
    robot_left_leg = new Transform();
    robot_head = new Transform();
    robot_left_eye = new Transform();
    robot_right_eye = new Transform();

    robot->addChild(robot_left_arm);
    robot->addChild(robot_right_arm);
    robot->addChild(robot_left_leg);
    robot->addChild(robot_right_leg);
    robot->addChild(robot_head);
    robot->addChild(robot_body);
    robot->addChild(robot_left_eye);
    robot->addChild(robot_right_eye);

    body = new Geometry("/Users/Hyun/Desktop/cse/HW3/HW3/body_s.obj");
    head = new Geometry("/Users/Hyun/Desktop/cse/HW3/HW3/head_s.obj");
    limb = new Geometry("/Users/Hyun/Desktop/cse/HW3/HW3/limb_s.obj");
    eye = new Geometry("/Users/Hyun/Desktop/cse/HW3/HW3/eyeball_s.obj");
    
    robot_body->addChild(body);
    robot_head->addChild(head);
    robot_left_eye->addChild(eye);
    robot_right_eye->addChild(eye);
    
    robot_left_arm->addChild(limb);
    robot_right_arm->addChild(limb);
    robot_left_leg->addChild(limb);
    robot_right_leg->addChild(limb);
   
    isLimb = true;
    arm = true;
    leg = false;
    left = false;
    right = true;
    
    robot_left_arm->setPart(isLimb,arm,left);
    robot_right_arm->setPart(isLimb,arm,right);
    robot_left_leg->setPart(isLimb,leg,left);
    robot_right_leg->setPart(isLimb,leg,right);
    
    robot_head->setTranslate(glm::vec3(0.0f,1.1f,0.0f));
    robot_left_arm->setTranslate(glm::vec3(-1.2f,0.0f,0.0f));
    robot_right_arm->setTranslate(glm::vec3(1.2f,0.0f,0.0f));
    robot_right_leg->setTranslate(glm::vec3(-0.2f,-1.2f,0.0f));
    robot_left_leg->setTranslate(glm::vec3(0.2f,-1.2f,0.0f));
    robot_right_eye->setTranslate(glm::vec3(0.3f,1.4f,1.0f));
    robot_left_eye->setTranslate(glm::vec3(-0.3f,1.4f,1.0f));

}
void Robot::update() {
    
    for(int i=0; i < robot->children.size(); i++) {
        robot->children[i]->update();
    }
}

void Robot::draw(glm::mat4 m) {
    glm::mat4 new_model = m * model;
    for(int i=0; i < robot->children.size(); i++) {
        robot->children[i]->draw(new_model);
    }
}
Transform* Robot::getRobot() {
    return robot;
}
Robot::~Robot() {
    
    for(int i=0; i <robot->children.size(); i++)
        delete robot->children[i];
    
}
