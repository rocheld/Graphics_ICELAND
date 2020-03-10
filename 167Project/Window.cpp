#include "Window.h"
#include <math.h>
/*
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 
 * hi
 */
namespace
{
    int width, height;
    std::string windowTitle("GLFW Starter Project");

    // Boolean switches
    bool glRasterize = true;
    
    CubeMap* skybox;
    PointCloud* dragon;
    PointCloud* sphere;

    Object* dragonObj;
    Object* currentObj;// The object currently displaying.
    Object* sphereObj;

    float scale;
    float deg;
    std::vector<Object*> temp;
    
    // Camera
    glm::vec3 eye(0, 0, 20); // Camera position.
    glm::vec3 center(0, 0, 0); // The point we are looking at.
    glm::vec3 up(0, 1, 0); // The up direction of the camera.
    float fovy = 60;
    float near = 1;
    float far = 1000;
    glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
    glm::mat4 projection; // Projection matrix.
    

    int pointSize;
    glm::mat4 dragon_loc;

    GLuint program; // The shader program id.
    GLuint projectionLoc; // Location of projection in shader.
    GLuint viewLoc; // Location of view in shader.
    GLuint modelLoc; // Location of model in shader.
    GLuint colorLoc; // Location of color in shader.

    GLuint flagLoc;
    GLuint programQuad;
    // mouse pos
    glm::vec3 last_point;
    double curr_point[2];

    int MOVEMENT;
    const int ROTATION = 1;
    const int TRANSLATION = 2;
    const int ZOOM = 3;
    const int ARMYSPIN = 4;

    // variable for light
    GLuint lightColorLoc;
    GLuint lightPosLoc;
    GLuint ambientLoc;
    GLuint shinyLoc;
    GLuint diffuseLoc;
    GLuint specularLoc;
    GLuint lightPosLoc2;
    GLuint lightAmbientLoc;
    GLuint lightDiffuseLoc;
    GLuint lightSpecLoc;

    glm::vec3 lightPos = glm::vec3(0.0f,0.0f,15.0f);
    glm::vec3 default_lightPos = lightPos;
    glm::vec3 lightColor;
    glm::vec3 tempColor;
    glm::vec3 zero = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 ambient = glm::vec3(0.2f,0.2f,0.2f);
    glm::vec3 diffuse = glm::vec3(0.2f,0.2f,0.3f);
    glm::vec3 specular = glm::vec3(0.1f,0.1f,0.1f);
    glm::mat3 rot_matrix;

    float shininess = 1.0f;
    bool rotate_dir_light = false;
    bool lightMode = false;
    bool pointLight = false;
    bool dirLight = false;



    GLuint programSky;
    GLuint projectionSky; // Location of projection in shader.
    GLuint viewSky;
    GLuint modelSky;
    GLuint colorSky;
    GLuint programLine;
    GLuint projectionLine; // Location of projection in shader.
    GLuint viewLine;
    glm::vec3 camera_front = center;

};

bool Window::initializeProgram()
{
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    
    // skybox shader
    programSky = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    programLine = LoadShaders("shaders/line.vert", "shaders/line.frag");
    
    // Check the shader programs.
    if (!program)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    // Activate the shader program.
    glUseProgram(program);
    // Get the locations of uniform variables.
    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    modelLoc = glGetUniformLocation(program, "model");
    colorLoc = glGetUniformLocation(program, "color");
    flagLoc = glGetUniformLocation(program, "flag");
    
    //hw3
    glUseProgram(programSky);
    projectionSky = glGetUniformLocation(programSky, "projection");
    viewSky = glGetUniformLocation(programSky, "view");
    modelSky = glGetUniformLocation(programSky, "model");
    colorSky = glGetUniformLocation(programSky, "color");
    
    
    return true;
}

bool Window::initializeObjects()
{

    skybox = new CubeMap();
    dragon = new PointCloud("dragon.obj", 1.0f);
    sphere = new PointCloud("sphere.obj",1.0f);
    
    sphereObj = sphere;
    dragonObj = dragon;
    
    dragon_loc = dragonObj->getModel();

    pointSize = 1;


    dragon->ScaleAndCenter();
    
    glm::mat4 model = sphereObj->getModel();
    model = glm::scale(model,glm::vec3(0.4f,0.4f,0.4f));
    sphereObj->setModel(model);
    
    // Define rotation matrix
    float deg = -0.15f;
    float cc= glm::cos(glm::radians(deg));
    float ss = glm::sin(glm::radians(deg));
    rot_matrix[0] = glm::vec3(cc,0,-ss);
    rot_matrix[1] = glm::vec3(0,1,0);
    rot_matrix[2] = glm::vec3(ss,0,cc);
    
    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.
    delete dragon;
    delete sphere;
    
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);
    
    

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
    width = w;   // ADD THIS LINE
    height = h;  // ADD THIS LINE
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif

    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    projection = glm::perspective(glm::radians(fovy),
        (float)width / (float)height, near, far);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.
    dragonObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{
   
        currentObj = skybox;
        glUseProgram(programSky);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 c = skybox->getColor();
        glm::mat4 m = skybox->getModel();
        glUniformMatrix4fv(projectionSky, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewSky, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelSky, 1, GL_FALSE, glm::value_ptr(m));
        glUniform3fv(colorSky, 1, glm::value_ptr(c));
        skybox->draw();
        
        // Switch back to using OpenGL's rasterizer
        glUseProgram(program);
        
        glm::mat4 model = dragonObj->getModel();
        glm::vec3 color = dragonObj->getColor();

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

        dragonObj->draw();
        
        
        // Gets events, including input such as keyboard and mouse or window resizing.
        glfwPollEvents();
        // Swap buffers.
        glfwSwapBuffers(window);
        
    
}
void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    glm::vec3 direction;
    float velocity;
    float rot_angle, scale_factor;
    glm::vec3 curr;
    glm::mat4 m;
    
    curr = trackBallMapping(glm::vec3(xpos,ypos,0.0f));
    direction = curr - last_point;
    glm::vec3 cross_product = glm::cross(last_point,curr);
    velocity = glm::length(direction);
    
    switch (MOVEMENT) {
        case ROTATION:
            
            if( velocity < 0.001) {
                    std::cerr << "slow" << std::endl;
            }
            else {
                    
                    // camera moving
                    glm::mat4 matrix1 = glm::mat4(1.0f);
                    glm::mat4 matrix2 = glm::mat4(1.0f);
                    glm::mat4 matrix3 = glm::mat4(1.0f);
                    glm::vec4 xyzw = glm::vec4(camera_front,1.0f);
                    
                    matrix1 = glm::translate(matrix1,-eye);
                    matrix2 = glm::rotate(matrix2, velocity, cross_product);
                    matrix3 = glm::translate(matrix3,eye);
       
                    xyzw = matrix3*matrix2*matrix1*xyzw;
    
                    camera_front.x = xyzw.x/xyzw.w;
                    camera_front.y = xyzw.y/xyzw.w;
                    view = glm::lookAt(eye, camera_front, up);
                
                    m = currentObj->getModel();
                    m = glm::rotate(m, velocity*2.0f,cross_product);
                    currentObj->setModel(m);
                }
            break;
            
        //Moving object
        /*
        case TRANSLATION:
            m = currentObj->getModel();
            m = glm::translate(m, glm::vec3((xpos-curr_point[0])*0.05f,-(ypos-curr_point[1])*0.05f,0.0f));
            currentObj->setModel(m);
            break;
        */
        default:
            break;
    }
    

    curr_point[0] = xpos;
    curr_point[1] = ypos;
    last_point = curr;
    
}
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    glm::mat4 model;
    model = currentObj->getModel();
    model = glm::translate(model, glm::vec3(0.0f,0.0f,yoffset*0.1f));
    currentObj->setModel(model);
}

glm::vec3 Window::trackBallMapping(glm::vec3 loc) {
    
    glm::vec3 v;
    float d;
    float x = loc[0];
    float y = loc[1];
    
    v.x = (2.0*x -width) / width;
    v.y = (height-2.0*y) / height;
    v.z = 0.0f;
    
    d = glm::length(v);
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrtf(1.001 - d*d);
    v = glm::normalize(v);
    
    return v;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    float x;
    float y;
    x = curr_point[0];
    y = curr_point[1];
    
    last_point = trackBallMapping(glm::vec3(x,y,0.0f));
    glm::vec3 point;
    
    if(action == GLFW_PRESS) {
        if(mods == GLFW_MOD_SHIFT && button == GLFW_MOUSE_BUTTON_LEFT) {
            MOVEMENT = ZOOM;
            MOVEMENT = ARMYSPIN;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT) {
            MOVEMENT = ROTATION;
            std::cout << "LEFT clicked at " <<  curr_point[0] << " " << curr_point[1] << std::endl;
        }
        
        else if( button == GLFW_MOUSE_BUTTON_RIGHT) {
            MOVEMENT = TRANSLATION;
        }
        
    }
    
    else {
        MOVEMENT = 0;
    }
}

void Window::light_mode(int mode) {
    
}
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        // Uppercase key presses (shift held down + key press)
        if (mods == GLFW_MOD_SHIFT) {
            switch (key) {
                default:
                    break;
            }
        }
    }
}

