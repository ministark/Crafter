#ifndef _CRAFTER_HPP_
#define _CRAFTER_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "shader_util.hpp"
#include "config.hpp"
#include "model.hpp"
#include "palette.hpp"

/**
 * @file   crafter.hpp
 * @brief  Class declaration for the program managing the state and models
 *
 * 
 */
namespace cft
{
	/**
  	*  	@brief Class that is basically the main program. 
  	*
  	* 	This class has various input handler and hand various functions changing state
  	*	variables accordingly. State variable defines the behaviour of the rendering and 
  	*	modification of the model or loading and saving of the model. It has two modes 
  	*	Modelling and Inspection modes. Modelling mode lets you create model by generating
  	*	vertices. Inspection lets you view your model in perspective projection 
  	*/ 
	class Crafter
	{
		/** @brief Store the handle for GLFW Window */
		GLFWwindow *window;
		
		/** @brief Store the handle for shader program */
		GLuint shaderProgram;

		//@{
		 /** @brief The vertex buffer and array buffers which stores the vertices of the model and other references */
		GLuint vbo, vao; 
		//@}

		/** @brief Pointer pointing to modeling matrix in the shader */
		GLuint uModelViewMatrix;

		//@{
		/** @brief Variable for displaying points marked on the screen during modeling mode */
		glm::vec4 vertices[3];
		glm::vec4 color[3];
		glm::vec4 *points;
		glm::vec4 *point_color;
		//}@
		
		//@{
		/** @brief Varible for displaying the grid during the Modelling mode */
		int state,index,total_lines,total_points;
		//@}

		/** @brief The current model in focus */
		Model *model;

		/** @brief Color palette to choose color from */
		Palette *palette;
	public:
		/** @brief Empty constructor should be made private to force to use overloaded constructor */
		Crafter();

		/** @brief Initializes all the buffers and variables and set the handlers */
		void Init(GLFWwindow *window);

		/** @brief Updates all the variable and state and sets the matrices accordingly */
		void Update();
		/** @brief Renders the grid, pallete and model in modelling mode and only model in inspection mode */
		void Render();
		
		// @{
		/** @brief Handlers are static as non static member functions couldn't be passed as arguments so are the variable storing the state*/
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
		//@}
	};
}
#endif