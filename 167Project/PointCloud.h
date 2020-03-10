#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

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

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> normal;
    std::vector<glm::ivec3> triangle;
    
	GLuint vao, vao2, vbo, vbo2, ebo; // Added vbo2
	GLfloat pointSize;
    
    float c_x,c_y,c_z; // center
    float max_x, max_y, max_z;
    float min_x, min_y, min_z;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();
    
    bool bunny;
    int object_name;
    float rotation_count;  // how many times the object rotated.
    
    float scale_factor;
	void draw();
	void update();
    void move(int op);
    
    void updateScaleFactor(float factor);
	void updatePointSize(GLfloat size);
	void spin(float deg);
    void reset();
    void ScaleAndCenter();
    std::vector<glm::vec3> getPoints();
    std::vector<glm::vec3> getNormals();
    
    unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif

