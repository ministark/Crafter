#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "config.hpp"
#include "crafter.hpp"

using namespace std;


int main()
{
  // Setting up the window
  GLFWwindow *window;
  glfwSetErrorCallback(cft::error_callback);
  if (!glfwInit())  return -1;

  // Setting up OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Creating the window with the opengl and checks for an error
  window = glfwCreateWindow(cft::screen_width, cft::screen_height, cft, NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  // Setting the Context of the window
  glfwMakeContextCurrent(window);

  // Showing the hardware Details
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    std::cerr<<"GLEW Init Failed : %s"<<std::endl;
  }
  cout<<"Vendor: "<<glGetString (GL_VENDOR)<<endl;
  cout<<"Renderer: "<<glGetString (GL_RENDERER)<<endl;
  cout<<"Version: "<<glGetString (GL_VERSION)<<endl;
  cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;

  glfwSetKeyCallBack(window, cft::key_callback);
  glfwSetFramebufferSizeCallBack(window, cft::framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  cft::Crafter craft = new Crafter();
  craft.Init();
  
  while(glfwWindowShouldClose(window) == 0)
  {
    craft.Render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  gflwTerminate();
}