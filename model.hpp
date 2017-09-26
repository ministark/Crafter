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
/**
 * @file   model.hpp
 * @brief  Class declaration for the Model
 *
 * 
 */
namespace cft
{
	/**
  	*  	@brief Class that is abstraction of a 3D-Model. 
  	*
  	* 	This class stores the vertices of the model along with the color values. 
  	*	It also exposes variables to translate and rotate the object. The matrix
  	*	stores different Matrices like projection and transform. It has functions
  	*	to load and store the 3D-Model in a file. Other functions to add and remove
  	*	triangles from the model
  	*/  
	class Model 
	{	
	private:

		 /** @brief Store the total number of the vertices */
		int total_vertices;	
		 /** @brief Store the vertices of the model */
		std::vector<glm::vec4> vertices;
		 /** @brief Store the Color of vertices */	
		std::vector<glm::vec4> colors;		

		 /** @brief The vertex buffer which stores the vertices of the model */
		GLuint vbo;
		 /** @brief The array object which stores all the binding of vertex buffer */
		GLuint vao;

		 /** @brief Stores the view matrix i.e transformation to camera coordinate */
		glm::mat4 view_matrix;
		 /** @brief Stores the projection matrix to reduce a dimension */
		glm::mat4 projection_matrix;
		 /** @brief Stores the transformation of the model */
		glm::mat4 modelview_matrix;
		 /** @brief Stores the Rotation of the model */
		glm::mat4 rotation_matrix;
		 /** @brief Stores the translation of the object */
		glm::mat4 transform;
		 /** @brief A pointer to the location of the uModelViewMatrix in the shader */
		GLuint uModelViewMatrix;
		 /** @brief Handle to the complied shader */
		GLuint shader;

	public:
		//@{
		 /** @brief Values to rotate the object every frame */
		GLfloat xrot,yrot,zrot;
		 /** @brief Matrix for translation */
		glm::vec3 translate;
		/** @brief Centroid of the object */
		glm::vec3 centroid;
		//@}

		/** @brief Stores the Scene matrix stores the changes done to model */
		glm::mat4 scene_matrix;

		/** @brief Empty Constructor */
		Model();

		/** 
		*	@brief Overloaded constructor of the object  
		*
		*	This initilizes the vertex buffers, vector, and various Matrices
		*	\param shaderProgram a handle to shader to save a copy for the model
		*/
		Model(GLuint shaderProgram);

		/** 
		*	@brief Loads all the vertices of the model in a file 
		*
		*	Loads the vertices and their color from raw format in the file specified to the vectors and vertex buffers
		*	\param file The path where the model has to be Loaded form
		*/
		void LoadModel(std::string file);
		
		/** 
		*	@brief Stores all the vertices of the model in a file 
		*
		*	Stores the vertices and their color in raw format in the file specified from the vertices vector
		*	\param file The path where the model has to be stored
		*/
		void SaveModel(std::string file);


		/** 
		*	@brief Prepares the Matrices for orthogonal projection 
		*/
		void InitModellingMode();
		
		/** 
		*	@brief Prepares the Matrices for perspective projection and sets the camera
		*/
		void InitInspectionMode();
		
		/** 
		*	@brief Recenters the Model to its centroid
		*/
		void RecenterModel();
		
		/** 
		*	@brief Adds a triangle in the model
		*
		*	\param v The Array of vertices should be of size 3
		*	\param c The Array of color should be of size 3
		*/
		void AddTriangle(glm::vec4 *v, glm::vec4 *c);
		
		/** 
		*	@brief Adds a triangle in the model
		*
		*	\param v The Array of vertices that have been removed
		*	\param c The Array of color corresponding to the vertices that have been removed
		*/	
		void RemoveTriangle(glm::vec4 *v, glm::vec4 *c);

		/** 
		*	@brief Binds the vertex buffers and Draws the Model
		*
		*/	
		void Render();
	};
}
#endif
