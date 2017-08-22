#include "model.hpp"
namespace cft
{
	Model::Model()
	{

	}
	Model::Model(GLuint shaderProgram)
	{
		total_vertices = 0;
		shader = shaderProgram;
		uModelViewMatrix = glGetUniformLocation( shader, "uModelViewMatrix");
		glGenVertexArrays (1, &vao);
  		glBindVertexArray (vao);
		glGenBuffers (1, &vbo);
  		glBindBuffer (GL_ARRAY_BUFFER, vbo);
  		glBufferData (GL_ARRAY_BUFFER, sizeof(colors)+sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

	}
	void Model::LoadModel(std::string file)
	{
		//Code to load model from a file
		std::fstream afile;
		afile.open(file,std::fstream::in | std::fstream::out);
		float x, y, z, r, g, b;
		while(afile >> x >> y >> z >> r >> g >> b)
		{	
			vertices.push_back(glm::vec4(x, y, z, 1.0f));
			colors.push_back(glm::vec4(r,g,b,1.0f));
			++total_vertices;
		}
		afile.close();

		//Load the model into the vbo
  		glBindBuffer (GL_ARRAY_BUFFER, vbo);
  		glBufferData (GL_ARRAY_BUFFER, sizeof(colors)+sizeof(vertices), NULL, GL_DYNAMIC_DRAW);
	  	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices );
  		glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), &colors);
	  	
	  	// Set up vertex arrays
		GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shader, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)) );
		
	}
	void Model::SaveModel(std::string file)
	{

	}
	void Model::Render()
	{
		ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
  		modelview_matrix = ortho_matrix ;
  		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  		glDrawArrays(GL_TRIANGLES, 0, total_vertices);
	}
}