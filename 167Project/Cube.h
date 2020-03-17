#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <time.h>

#include "Object.h"

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
    
public:
    float birthTime;
    float velocity;
    
	Cube(float size, float time, int flip);
	~Cube();

	void draw();
	void update();

	void spin(float deg);

};

#endif

