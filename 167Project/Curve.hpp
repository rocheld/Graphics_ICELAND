//
//  Curve.hpp
//  HW3
//
//  Created by HyunChoi on 2020/02/28.
//  Copyright © 2020 HyunChoi. All rights reserved.
//

#ifndef Curve_hpp
#define Curve_hpp

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
#include "Geometry.hpp"
#include "Transform.hpp"
#include <string>
#include <vector>
#include "shader.h"

class Curve
{
    public:
    Curve();
    ~Curve();
    Geometry* cc;
    std::vector<glm::vec3> control_points();
    std::vector<glm::vec3> getPoints();
    void draw();
    void update();
    
};
#endif /* Curve_hpp */
