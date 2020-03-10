#include "PointCloud.h"
#include <iostream>

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
	: pointSize(pointSize)
{
    std::string name;
    FILE* fp;
    float x,y,z;
    float r,g,b;
    float a,b2,c;
    float t1,t2,t3;
    
    int c1,c2;

    rotation_count = 0.0f;
    scale_factor = 1.0f;
    
    max_x = 0;
    max_y = 0;
    max_z = 0;
    min_x = 100;
    min_y = 100;
    min_z = 100;
    
    if(objFilename == "dragon.obj") {
        fp = fopen("dragon.obj","rb");
        object_name = 2;
        bunny = 0;
    }
    else if(objFilename == "sphere.obj") {
        fp = fopen("sphere.obj","rb");
        object_name = 3;
    }

    if (fp==NULL) {std::cerr << "error loading file" <<std::endl; exit(-1);}
    
    
    do {
        c1 = fgetc(fp);
        c2 = fgetc(fp);
        
        // Read vertex
        if((c1=='v') && (c2==' ')) {
            if(object_name == 0) {
                fscanf(fp, "%f %f %f %f %f %f\n",&x, &y, &z, &r, &g, &b);
            }
            else
                fscanf(fp, "%f %f %f\n",&x, &y, &z);
            
            if(min_x > x)
                min_x = x;
            
            if(max_x < x)
                max_x = x;
            
            if(min_y > y)
                min_y = y;
            
            if(max_y < y)
                max_y = y;
            
            if(min_z > z)
                min_z = z;
            
            if(max_z < z)
                max_z = z;
            
            points.push_back(glm::vec3(x,y,z));
        }
        // Read normal
        else if( (c1=='v') && (c2 =='n') ){
            fscanf(fp, "%f %f %f\n", &a, &b2, &c);
        
            // Mapping normals 0 to 1;
            glm::vec3 input = glm::vec3(a,b2,c);
            glm::vec3 norm = 0.5f*normalize(input) + 0.5f;
            normal.push_back(norm);
        }
        
        // Ignore comment
        else if(c1 == '#' && object_name != 0)  {
            char c;
            do {
                c = fgetc(fp);
            }while( c != '\n');
        }
        
        // Read face
        else if( (c1=='f') && (c2 == ' ')) {
            fscanf(fp, "%f//%f %f//%f %f//%f\n", &t1,&t1,&t2,&t2,&t3,&t3);
            glm::ivec3 tri = glm::ivec3(t1-1,t2-1,t3-1);
            triangle.push_back(tri);
        }
    } while(c1 != EOF||c2 != EOF);
    
    
    // debug
    std::cout <<  objFilename << " triangles " << triangle.size()<< std::endl;
    fclose(fp);
    
    // Set the model matrix to an identity matrix.
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);
    
    
    // Points
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
    
    // Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
    
    // Triangle
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)*triangle.size(), triangle.data(), GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Normal
    // GENERATE A NEW VERTEX BUFFER
    glGenBuffers(1, &vbo2);
    // Bind it as a GL_ARRAY_BUFFER type
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    // Pass in the data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normal.size(),normal.data(),GL_STATIC_DRAW);
    //Enable the next attribute array
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),0);
	
    // Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
    

}
PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
    
    glDeleteBuffers(1, &vbo2);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	// Draw points 
	glDrawArrays(GL_POINTS, 0, points.size());
    // Draw triangles
    glDrawElements(GL_TRIANGLES, 3*triangle.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void PointCloud::update()
{

    //spin(0.1f);
    
}
void PointCloud::ScaleAndCenter() {
    
    // Center of object model
    c_x = (max_x + min_x) / 2;
    c_y = (max_y + min_y) / 2;
    c_z = (max_z + min_z) / 2;
    
    // Find a scale factor for the object to be fit in a 2x2x2 cube
    float scale_x = 2.f / (max_x - min_x);
    float scale_y = 2.f / (max_y - min_y);
    float scale_z = 2.f / (max_z - min_z);
   
    float factor = std::min(scale_z,std::min(scale_x,scale_y));
    
    //std::cout << size_x << " " << size_y << " " << size_z << std::endl;
    glm::vec3 center = glm::vec3(-c_x,-c_y,-c_z);
    
    model = glm::scale(glm::vec3(factor));
    model = glm::translate(model,center);

    
}
void PointCloud::move(int op) {
    
    
    if(object_name == 0) // bunny
        model = glm::rotate(model, glm::radians((float) rotation_count), glm::vec3(0.0f,0.0f,-1.0f));
        
    else
        model = glm::rotate(model, glm::radians((float) rotation_count), glm::vec3(0.0f,-1.0f,0.0f));
    
    switch(op) {
                
            case 1: // right
                model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
                break;
            case 2: // left
                model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
                break;
            case 3: // up
                model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            case 4: // down
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
                break;
            case 5: // In
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
                break;
            case 6: // out
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
                break;
            default:
                std::cerr << rotation_count <<std::endl;
                break;
    }
    
    if(object_name == 0) // ID = 0 => bunny
        model = glm::rotate(model, glm::radians((float) rotation_count), glm::vec3(0.0f,0.0f,1.0f));
        
    else
        model = glm::rotate(model, glm::radians((float) rotation_count), glm::vec3(0.0f,1.0f,0.0f));
    
	/*
	 * TODO: Section 3: Modify this function to spin the dragon and bunny about
	 * different axes. Look at the spin function for an idea
	 */
    
}

void PointCloud::updatePointSize(GLfloat size)
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
    // MOVE TO SCALE
    //glm::scale( model, glm::vec3(x,y,z);
    //model = glm::scale(model, glm::vec3(size, size,size));
    if(pointSize + size >= 1.0f)
       pointSize = size+pointSize;
}

void PointCloud::spin(float deg)
{
    
    // rotate( model, glm::radians(deg), glm::vec3( x, y, z);
    rotation_count += deg;
    if(rotation_count > 360)
        rotation_count-=360;
    
    // rotate z-axis
    if(object_name == 0){ // bunny
        model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
    }
	// Update the model matrix by multiplying a rotation matrix
    // slide: https://cseweb.ucsd.edu/classes/wi20/cse167-a/lec3.pdf
    else
        // rotate y-axis
        model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    
}

// if factor is 0, reset float scale_factor to 1
void PointCloud::updateScaleFactor(float factor) {
    scale_factor *= factor;
}

void PointCloud::reset() {
    rotation_count = 0;
    scale_factor = 1.0f;
}

std::vector<glm::vec3> PointCloud::getPoints() {
    return points;
}

std::vector<glm::vec3> PointCloud::getNormals() {
    return normal;
}
