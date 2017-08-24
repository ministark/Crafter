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
		
		GLuint shaderProgram;// Reference to the shader
		GLuint vbo, vao; 
		GLuint uModelViewMatrix;

		glm::vec4 vertices[3];
		glm::vec4 color[3];
		glm::vec4 *points;
		glm::vec4 *point_color;
		
		int state,index,total_lines,total_points;
		
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
		static bool key_1,key_2,key_3,key_4,key_5,key_6;
		static bool button_left;
		static double posx,posy,posz;
		static int col_r,col_g,col_b;

	};
}
#endif