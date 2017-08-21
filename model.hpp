#ifndef _MODEL_HPP_
#define _MODEL_HPP_
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "config.hpp"

namespace cft
{
	class Model // This class store the vertice and color of a model
	{	
	private:
		int total_vertices;
		std::vector<glm::vec4> vertices;	// Stores the list of the vertices
		std::vector<glm::vec4> colors;		// Stores the color of the corresponding vertices
		GLuint vbo, vao;	// The vertex buffer size initilize to max and sub buffer is used

		// Holding references to various matrices in the shader
		glm::mat4 ortho_matrix;
		glm::mat4 modelview_matrix;
		GLuint uModelViewMatrix;

		GLuint shader;	// Handle to the shaders
	public:
		Model();
		Model(GLuint shaderProgram);
		void LoadModel(std::string file);
		void SaveModel(std::string file);	// Setting the default argument to be the name of the file unless specified
		void Render();
	};
}
#endif
