//
//  RobotArmy.cpp
//  HW3
//
//  Created by HyunChoi on 2020/02/27.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "RobotArmy.hpp"

RobotArmy::RobotArmy() {
    
    army = new Transform();
    army_size = 25;
    
    for(int i=0; i < 5; i++) {
        for(int j=0; j < 5; j++) {
        Robot* robot = new Robot();
        Transform* r = robot->getRobot();
        army->addChild(r);
        glm::vec3 pos = glm::vec3(4.0*i-8,0.0f,-4.0f*j+8);
        r->setTranslate(pos);
        if( i==2 && j==2) {
            centerRobot = r;
            center = pos;
        }
        }
    }
    
}
glm::vec3 RobotArmy::getCenter() {
    return center;
}

RobotArmy::~RobotArmy() {
    delete army;
    for(int i=0; i <army->children.size(); i++)
        delete army->children[i];
}

void RobotArmy::draw(glm::mat4 m) {
    m = glm::scale(m, glm::vec3(0.45f));
    army->draw(m);
}

void RobotArmy::setCurve(std::vector<glm::vec3> cc) {
    cPoints = cc;
    //army->setTranslate(cPoints[150]);
}

void RobotArmy::rotate(float v, glm::vec3 r) {
    army->setRotation(center,v,r);
}

void RobotArmy::scaleUp(){
    this->army->setScale(1.1f);
}
void RobotArmy::scaleDown() {
    army->setScale(0.9f);
}
void RobotArmy::update() {
    
    
    army->update();
    
    curr++;
    if(curr == cPoints.size())
        curr =0;
}
