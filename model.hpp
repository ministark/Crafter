#ifdef _MODEL_HPP_
#define _MODEL_HPP_

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace cft
{
	class Model // This class store the vertice and color of a model
	{
		static total_models;	// This store the total model
		std::string model_name;
		int model_id;			// Unique id for each model
		std::vector<glm::vec4> vertices;	// Stores the list of the vertices
		int total_vertices;
		GLuint vbo, vao;	// The vertex buffer size initilize to max and sub buffer is used
	public:
		Model();
		void Init()
		void LoadModel(std::string file);
		void SaveModel(std::string file = model_name);	// Setting the default argument to be the name of the file unless specified
		void AddVertice();
	}
}

cft::Model::total_models = 0;