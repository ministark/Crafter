#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "shader_util.hpp"
#include "config.hpp"
#include "model.hpp"

/**
 * @file   Scene.hpp
 * @brief  Class declaration for the scene which manages the model and different stages for the pipeline
 *
 * 
 */
namespace cft
{
	/**
  	*  	@brief Class that is basically the main program. 
  	*
  	* 	This class has various input handler and various functions changing state variables accordingly
  	*	for changing pipeline stages. State variable defines the behaviour of the rendering of the models. 
  	*	This class also has a vertices for the frustum and eye loaded from the scene file
  	*/ 
	class Scene
	{
		/** @brief Store the handle for GLFW Window */
		GLFWwindow *window;
		
		/** @brief Store the handle for shader program */
		GLuint shaderProgram;

		//@{
		 /** @brief The vertex buffer and array buffers which stores the vertices of the frustum and eye */
		GLuint vbo, vao; 
		//@}

		/** @brief Pointer pointing to modeling matrix in the shader */
		GLuint uModelViewMatrix;

		//@{
		/** @brief Variable for the frustum, Eye, Lookat, and up vector */
		glm::vec4 Eye;
		glm::vec4 LookAt;
		glm::vec4 LookUp;
		double L, R, B, T, N, F;
		//}@

		/** @brief All the models*/
		std::vector<Model*> models;


	public:
		/** @brief Empty constructor should be made private to force to use overloaded constructor */
		Scene();

		/** @brief Initializes all the buffers and variables and set the handlers */
		void Init(GLFWwindow *window);

		/** @brief Updates all the variable and state and sets the matrices accordingly */
		void Update();
		/** @brief Renders the grid, pallete and model in modelling mode and only model in inspection mode */
		void Render();
		
		/** @brief Loads the scene file and updates all the variable*/
		void LoadScene(std::string file);

		// @{
		/** @brief Handlers are static as non static member functions couldn't be passed as arguments so are the variable storing the state*/
		static void KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseHandler(GLFWwindow* window, int button, int action, int mods);
		static void CursorHandler(GLFWwindow* window, double x, double y);
		static void ResizeHandler(GLFWwindow* window, int width, int height);
		static void ErrorHandler(int error, const char* description);
		static bool key_1,key_2,key_3,key_4,key_5,key_6;
		//@}
	};
}
#endif