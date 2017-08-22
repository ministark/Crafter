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
		Model *model;
	public:
		Crafter();

		// Rendering
		void Init(GLFWwindow *window); // Initializes the Engine for rendering
		void Render();
		void Update();
		
		// Handlers are static as non static member functions couldn't be passed as arguments
		static void KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseHandler(GLFWwindow* window, int button, int action, int mods);
		static void CursorHandler(GLFWwindow* window, double x, double y);
		static void ResizeHandler(GLFWwindow* window, int width, int height);
		static void ErrorHandler(int error, const char* description);
		static bool key_up,key_down,key_left,key_right,key_M,key_I,key_L,key_K;
		static bool key_R,key_W,key_A,key_S,key_D,key_Z,key_X,key_PgUp,key_PgDown,key_shift;
		static bool button_left;
		static double posx,posy;

	};
}
#endif