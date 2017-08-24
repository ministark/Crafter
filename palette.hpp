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

namespace cft
{
	class Palette // This class store the vertice and color of a model
	{	
	private:
		glm::vec4 vertices[6];
		glm::vec4 colors[6];
		GLuint vbo, vao;	// The vertex buffer size initilize to max and sub buffer is used

		// Holding references to various matrices in the shader
		glm::mat4 projection_matrix;
		GLuint uModelViewMatrix;

		GLuint shader;	// Handle to the shaders
	public:
		Palette();
		Palette(GLuint shaderProgram);
		void Render();
		bool PickColor(double x, double y, glm::vec4 *col); //Takes mouse position and if mouse is in palette then returns a color
	};
}
#endif
