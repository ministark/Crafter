#ifndef _CRAFTER_HPP_
#define _CRAFTER_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace cft
{
	class Crafter
	{
	private:
		GLFWwindow *window;	// Holds the reference to the window
		GLuint shaderProgram;	// Reference to the shader 
		GLuint vbo, vao;	// Buffer objects

		float points[] = {
   		 	0.0f,  0.5f,  0.0f,
    		0.5f, -0.5f,  0.0f,
  		  	-0.5f, -0.5f,  0.0f
  			};
	
	public:
		void Init(GLFWwindow *window); // Initializes the Engine for rendering
		void Render();
	}

}