//
//  Geometry.hpp
//  HW3
//
//  Created by HyunChoi on 2020/02/26.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include "Node.hpp"
#include <string>
#include <vector>
class Geometry : public Node
{
    public:
    GLuint vao,vbo,vbo2,ebo;
    glm::mat4 model;
    std::vector<glm::vec3> points_input;
    std::vector<glm::vec3> normals_input;
    std::vector<glm::ivec3> indices;
    std::vector<unsigned int> vertices_indices;
    std::vector<unsigned int> normal_indices;
    
    std::vector<glm::vec3> vertices__;
    std::vector<glm::vec3> normals__;
    Geometry(std::string filename);
    ~Geometry();
    virtual void draw(glm::mat4 m);
    virtual void update();
};
#endif /* Geometry_hpp */
