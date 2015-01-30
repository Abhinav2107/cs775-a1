#include "gl_interface.hpp"

extern double t;
extern double p;
extern bool play;
extern int frame;
extern int camera;

void initGL(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_SMOOTH)
}

void error_callback(int error, const char * description) {
    std::cerr << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        t = t + 10;
        if(t > 360)
            t = t - 360;
    } 
    else if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        t = t - 10;
        if(t < 0)
            t = t + 360;
    }
    else if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        p = p + 10;
        if(p > 360)
            p = p - 360;
    }
    else if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        p = p - 10;
        if(p < 0)
            p = p + 360;
    }
    else if(key == GLFW_KEY_SPACE && (action == GLFW_PRESS)) {
        play = !play;
    }
    else if(key == GLFW_KEY_1 && (action == GLFW_PRESS)) {
        camera = 1;
    }
    else if(key == GLFW_KEY_2 && (action == GLFW_PRESS)) {
        camera = 2;
    }
    else if(key == GLFW_KEY_N && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        frame--;
    }
    else if(key == GLFW_KEY_M && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        frame++;
    }
    else if(key == GLFW_KEY_R && (action == GLFW_PRESS)) {
        frame = -1;
        play = false;
    }

}
        
