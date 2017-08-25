#ifndef _PALETTE_HPP_
#define _PALETTE_HPP_
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "config.hpp"
/**
 * @file   palette.hpp
 * @brief  Class declaration for the Color Palette
 *
 * 
 */
namespace cft
{
	/**
  	*  	@brief Class that displays a Rectangle Color-Palette to choose from 
  	*
  	*	This class renders a rectangle on screen. Clicking on that rectangle returns a
  	*	color value depending on the position of the Click
  	*/  
	class Palette 
	{	
		//@{
		/** @brief Vertices of the Rectangle and the corresponding color */
		glm::vec4 vertices[6];
		glm::vec4 colors[6];
		GLuint vbo, vao;
		//@}

		//@{
		/** @brief Matrices and reference to shader for passing them to the shaders */
		glm::mat4 projection_matrix;
		GLuint uModelViewMatrix;
		GLuint shader;
		//@}
	public:
		/** @brief Empty constructor should be made private to force to use overloaded constructor */
		Palette();
		/** 
		*	@brief Overloaded constructor of the object  
		*
		*	This initilizes the vertex buffers, vector, and Matrices
		*	\param shaderProgram a handle to shader to save a copy for the model
		*/
		Palette(GLuint shaderProgram);
		/** 
		*	@brief Binds the vertex buffers and Draws the Model
		*
		*/	
		void Render();
		/** 
		*	@brief Binds the vertex buffers and Draws the Model
		*
		*	\param x Mouse X when it was pressed
		*	\param y Mouse Y when it was pressed
		*	\param col It sets the color based on the position of the click
		*	\return Checks whether the mouse click was inside the rectangle and returns true
		*/	
		bool PickColor(double x, double y, glm::vec4 *col); //Takes mouse position and if mouse is in palette then returns a color
	};
}
#endif
