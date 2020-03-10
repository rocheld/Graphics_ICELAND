//
//  Transform.cpp
//  HW3
//
//  Created by HyunChoi on 2020/02/26.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#include "Transform.hpp"
#include <iostream>
Transform::~Transform() {
    
    for(int i=0; i< children.size(); i++)
        delete children[i];
}
Transform::Transform() {
    model = glm::mat4(1.0f);
    this->isLimb = false;
    this->move_back = false;
    this->posFromOrigin = glm::vec3(0.0f);
    this->rotateFactor = 0.0f;
}
void Transform::addChild(Node* n) {
    this->children.push_back(n);
}
void Transform::removeChild() {
    
}

void Transform::setRotation(glm::vec3 p,float v, glm::vec3 r) {
    this->model = glm::translate(model,-p);
    this->model = glm::rotate(model,v,r);
    this->model = glm::translate(model,p);
}

void Transform::setTranslate(glm::vec3 t) {
    this->posFromOrigin += t;
    this->model = glm::translate(model,t);
}
void Transform::draw(glm::mat4 m) {
    new_model = m * this->model;
    for(int i=0; i < this->children.size(); i++) {
        this->children[i]->draw(new_model);
    }
}
void Transform::setScale(float s) {
    this->model = glm::scale(model, glm::vec3(s));

}
void Transform::update() {
    
    for(int i=0; i < this->children.size(); i++)
        this->children[i]->update();
    
    float deg = 0.0f;
    if(this->isLimb) {
        // arm
        if(this->arm) {
            // right arm
            if(this->right)
                deg = 0.2f;
            // left arm
            else
                deg = -0.2f;
        }
        // leg
        else {
            //right leg
            if(this->right)
                deg = -0.2f;
            
            // left leg
            else
                deg = 0.2f;
        }
        
        if(this->move_back)
            deg *= -1.0f;
        
        model = glm::translate(model,-posFromOrigin);
        model = glm::rotate(model,glm::radians(deg), glm::vec3(1.0f,0.0f,0.0f));
        model = glm::translate(model,posFromOrigin);
    }
    
    this->rotateFactor += deg;
    if(this->rotateFactor > 39.9f || this->rotateFactor < -39.9f)
        this->move_back = !move_back;


}

// right = 0 => left;
// arm = 0 => leg;
void Transform::setPart(bool isLimb,bool right, bool arm) {
    this->isLimb = isLimb;
    this->right = right;
    this->arm = arm;
}
