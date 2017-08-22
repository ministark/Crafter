#ifndef _CRAFTER_HPP_
#define _CRAFTER_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "shader_util.hpp"
#include "config.hpp"
#include "model.hpp"

namespace cft
{
	class Crafter
	{
	private:
		GLFWwindow *window;	// Holds the reference to the window
		GLuint shaderProgram;	// Reference to the shader 
		int state;
		//cft::Model *model;

	public:
		Crafter();

		// Rendering
		void Init(GLFWwindow *window); // Initializes the Engine for rendering
		void Render();
		void Update();
		
		// Handlers are static as non static member functions couldn't be passed as arguments
		static void InputHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ResizeHandler(GLFWwindow* window, int width, int height);
		static void ErrorHandler(int error, const char* description);
		static bool key_up,key_down,key_left,key_right,key_M,key_I,key_L,key_K;

	};
}
#endif