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
  		glBufferData (GL_ARRAY_BUFFER, 2*max_vertices*sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	}
	void Model::LoadModel(std::string file)
	{
		total_vertices = 0;
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

		// Copy the data to the VBO
	  	glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(glm::vec4), &vertices[0] );
  		glBufferSubData( GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec4), colors.size()*sizeof(glm::vec4), &colors[0] );
	  	
	  	// Set the arguments for the shader
		GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shader, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size()*sizeof(glm::vec4)) );
		
	}
	void Model::SaveModel(std::string file)
	{
		std::fstream afile;
		afile.open(file, std::fstream::in | std::fstream::out);
		for(int i = 0; i < total_vertices; ++i)
		{
			afile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << " " << vertices[i].w << " " << colors[i].x << " " << colors[i].y << " " << colors[i].z << " " << colors[i].w << " \n";
		}
		afile.close();
	}
	void Model::Render()
	{
		GLfloat xrot=1.0,yrot=0.0,zrot=0.0;
		rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
	    rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
	    rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
		ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
  		modelview_matrix = ortho_matrix*rotation_matrix;
  		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  		glDrawArrays(GL_TRIANGLES, 0, total_vertices);
	}
}