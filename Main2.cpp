#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "config.hpp"
#include "scene.hpp"

/**
\mainpage Modelling Utility

This program is capable of creating raw models. Models are to be made using Modeling mode.
While modeing Color could be selected using color palatte and triangle can be drawn of the 
selected color. The inspection mode has features such as recentering about the centroid for
viewing what has been modeled. The program also saves model in raw format and could load
it for later use. Undo commands removes the last add vertices. Multiple use of undo retraces 
the history of how the Model was created.
*/

using namespace std;

/**
 * @file   Main.cpp
 * @brief  Main funciton which creates and passes the window to the Crafter
 *
 * 
 */

int main()
{
  // Setting up the window
  GLFWwindow *window;
  glfwSetErrorCallback(cft::Scene::ErrorHandler);

     
  if (!glfwInit())  return -1;

  // Setting up OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Creating the window with the opengl and checks for an error
  window = glfwCreateWindow(cft::screen_width, cft::screen_height, "Pipeline", NULL, NULL);
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

  

  cft::Scene *scene = new cft::Scene();
  scene->Init(window);

  
  while(glfwWindowShouldClose(window) == 0)
  {
    scene->Update();
    scene->Render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}