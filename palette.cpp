#include "palette.hpp"
namespace cft
{
	Palette::Palette()
	{

	}
	Palette::Palette(GLuint shaderProgram)
	{
		shader = shaderProgram;
		
		// Square palette
		vertices[0] = glm::vec4((float)cft::xmin*(2.0/screen_width) - 1.0f,-((float)cft::ymin*(2.0/screen_width) - 1.0f), 0.9f, 1);
		vertices[1] = glm::vec4((float)cft::xmin*(2.0/screen_width) - 1.0f,-((float)cft::ymax*(2.0/screen_width) - 1.0f), 0.9f, 1);
		vertices[2] = glm::vec4((float)cft::xmax*(2.0/screen_width) - 1.0f,-((float)cft::ymax*(2.0/screen_width) - 1.0f), 0.9f, 1);
		vertices[3] = glm::vec4((float)cft::xmin*(2.0/screen_width) - 1.0f,-((float)cft::ymin*(2.0/screen_width) - 1.0f), 0.9f, 1);
		vertices[4] = glm::vec4((float)cft::xmax*(2.0/screen_width) - 1.0f,-((float)cft::ymax*(2.0/screen_width) - 1.0f), 0.9f, 1);
		vertices[5] = glm::vec4((float)cft::xmax*(2.0/screen_width) - 1.0f,-((float)cft::ymin*(2.0/screen_width) - 1.0f), 0.9f, 1);
		// Colors of the palette
		colors[0] = glm::vec4(1.0f,0.0f,0.0f,1.0f);
		colors[1] = glm::vec4(0.0f,1.0f,0.0f,1.0f);
		colors[2] = glm::vec4(0.0f,1.0f,0.0f,1.0f);
		colors[3] = glm::vec4(1.0f,0.0f,0.0f,1.0f);
		colors[4] = glm::vec4(0.0f,1.0f,0.0f,1.0f);
		colors[5] = glm::vec4(0.0f,0.0f,1.0f,1.0f);

		// Vertex buffers
		glGenVertexArrays (1, &vao);
  		glBindVertexArray (vao);
		glGenBuffers (1, &vbo);
  		glBindBuffer (GL_ARRAY_BUFFER, vbo);
  		glBufferData (GL_ARRAY_BUFFER, 12*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
  		glBufferSubData( GL_ARRAY_BUFFER, 0, 6*sizeof(glm::vec4), vertices);
  		glBufferSubData( GL_ARRAY_BUFFER, 6*sizeof(glm::vec4), 6*sizeof(glm::vec4), colors);

  		// Setting the shaders
  		GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shader, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(6*sizeof(glm::vec4)) );
		glBindVertexArray(0);

	}
	void Palette::Render()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
		projection_matrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(projection_matrix));
  		glDrawArrays(GL_TRIANGLES, 0, 6);
  		glBindVertexArray(0);
  		glBindBuffer(GL_ARRAY_BUFFER,0);
	}
	bool Palette::PickColor(double x, double y,  glm::vec4 *col)
	{
		if(x > xmin && x < xmax && y > ymin && y < ymax){
			col->x = (float)(xmax-x)/(xmax-xmin);
			col->z = (float)(x-xmin)/(xmax-xmin);
			col->y = (float)(y-ymin)/(ymax-ymin);
			col->w = 1.0f;
			return true;
		}
		else
			return false;
	}
}