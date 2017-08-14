#include "crafter.hpp"
#include "shader_util.hpp"
#include "config.hpp"
namespace cft
{
	crafter::Init(GLFWwindow *window)
	{	
		//Set framebuffer clear color
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   		glClearDepth(1.0);
    	glDepthFunc(GL_LESS);
    	glEnable(GL_DEPTH_TEST); 
    	glShadeModel(GL_SMOOTH);

		// Setting up the shaders
		std::vector<GLuint> shaderList;
 		shaderList.push_back(cft::LoadShaderGL(GL_VERTEX_SHADER, ctf::vertex_shader));
  		shaderList.push_back(cft::LoadShaderGL(GL_FRAGMENT_SHADER, ctf::fragment_shader));
  		shaderProgram = cft::CreateProgramGL(shaderList);
		
		// Setting up the vertex buffers
		glGenBuffers (1, &vbo);
  		glBindBuffer (GL_ARRAY_BUFFER, vbo);
  		glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);
		glGenVertexArrays (1, &vao);
  		glBindVertexArray (vao);
  		glEnableVertexAttribArray (0);
  		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	crafter::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  		glUseProgram(shaderProgram);
  		glBindVertexArray (vao);
  		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}