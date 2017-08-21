#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "config.hpp"
#include "crafter.hpp"
// #include "gl_framework.hpp"

using namespace std;


int main()
{
  // Setting up the window
  GLFWwindow *window;
  glfwSetErrorCallback(cft::Crafter::ErrorHandler);

     
  if (!glfwInit())  return -1;

  // Setting up OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Creating the window with the opengl and checks for an error
  window = glfwCreateWindow(cft::screen_width, cft::screen_height, "Crafter", NULL, NULL);
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

  

  cft::Crafter *craft = new cft::Crafter();
  craft->Init(window);

  
  while(glfwWindowShouldClose(window) == 0)
  {
    craft->Update();
    craft->Render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}