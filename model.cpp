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
		translate = glm::vec3(0,0,0);centroid = glm::vec3(0,0,0);
		rotation_matrix = glm::mat4(1.0f);
		scene_matrix = glm::mat4(1.0f);
		projection_matrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		xrot=0.0,yrot=0.0,zrot=0.0;
		uModelViewMatrix = glGetUniformLocation( shader, "uModelViewMatrix");
		glGenVertexArrays (1, &vao);
  		glBindVertexArray (vao);
		glGenBuffers (1, &vbo);
  		glBindBuffer (GL_ARRAY_BUFFER, vbo);
  		glBufferData (GL_ARRAY_BUFFER, 2*max_vertices*sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
  		glBindBuffer (GL_ARRAY_BUFFER, 0);
  		glBindVertexArray(0);

	}
	void Model::LoadModel(std::string file)
	{
		vertices.clear();colors.clear();
		rotation_matrix = glm::mat4(1.0f);
		total_vertices = 0;
		//Code to load model from a file
		std::fstream afile;
		afile.open(file,std::fstream::in | std::fstream::out);
		float x, y, z, r, g, b;

		centroid = glm::vec3(0.0f,0.0f,0.0f);
		while(afile >> x >> y >> z >> r >> g >> b)
		{	
			vertices.push_back(glm::vec4(x, y, z, 1.0f));
			colors.push_back(glm::vec4(r,g,b,1.0f));
			++total_vertices;
			centroid.x += x;
			centroid.y += y;
			centroid.z += z;
		}
		afile.close();
		centroid /= total_vertices;

		// Copy the data to the VBO
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBindVertexArray(vao);
  		glBufferData (GL_ARRAY_BUFFER, 2*max_vertices*sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	  	glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(glm::vec4), &vertices[0] );
  		glBufferSubData( GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec4), colors.size()*sizeof(glm::vec4), &colors[0] );
	  	
	  	// Set the arguments for the shader
		GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shader, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size()*sizeof(glm::vec4)) );
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		
	}
	void Model::SaveModel(std::string file)
	{
		std::ofstream afile;
		afile.open(file, std::ofstream::out);
		for(int i = 0; i < total_vertices; ++i)
		{
			afile << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z  << " " << colors[i].x << " " << colors[i].y << " " << colors[i].z << std::endl;
		}
		afile.close();
	}
	void Model::InitModellingMode()
	{
		rotation_matrix = glm::mat4(1.0f);
		translate = glm::vec3(0.0f,0.0f,0.0f);
		projection_matrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	}
	void Model::InitInspectionMode()
	{
		view_matrix = glm::lookAt(glm::vec3(camera_x,cft::camera_y,cft::camera_z),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));	
		projection_matrix = glm::perspective(glm::radians(45.0f), (float)screen_width/screen_height, 0.01f, 50.0f)*view_matrix;
	}
	void Model::RecenterModel()
	{
		translate = -centroid;
	}
	void Model::AddTriangle(glm::vec4 *v, glm::vec4 *c)
	{
		centroid *= total_vertices;
		for (int i=0; i<3; i++)
		{
			vertices.push_back(v[i]);
			colors.push_back(c[i]);
			centroid.x += v[i].x;
			centroid.y += v[i].y;
			centroid.z += v[i].z;
		}
		total_vertices += 3;
		centroid /= total_vertices;
		// Copying the Data
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(glm::vec4), &vertices[0] );
  		glBufferSubData( GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec4), colors.size()*sizeof(glm::vec4), &colors[0] );

  		//
  		GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shader, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size()*sizeof(glm::vec4)) );
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	void Model::RemoveTriangle(glm::vec4 *v, glm::vec4 *c)
	{
		if (total_vertices < 3)
			return;
		centroid *= total_vertices;
		for (int i=2; i>=0; i--)
		{
			v[i] = vertices.back();vertices.pop_back();
			c[i] = colors.back();colors.pop_back();
			centroid.x -= v[i].x;
			centroid.y -= v[i].y;
			centroid.z -= v[i].z;
		}
		total_vertices -= 3;
		centroid /= total_vertices;
	}
	void Model::Render()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
		glm::mat4 rotation_matrix1;
		glm::mat4 transform_rotation = glm::translate(glm::mat4(1.0f),-centroid);
		rotation_matrix1 = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
	    rotation_matrix1 = glm::rotate(rotation_matrix1, yrot, glm::vec3(0.0f,1.0f,0.0f));
	    rotation_matrix1 = glm::rotate(rotation_matrix1, zrot, glm::vec3(0.0f,0.0f,1.0f));
	    rotation_matrix = rotation_matrix1*rotation_matrix;
		transform = glm::translate(glm::mat4(1.0f),translate);
  		modelview_matrix = projection_matrix*transform*rotation_matrix*transform_rotation*scene_matrix;
  		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  		glDrawArrays(GL_TRIANGLES, 0, total_vertices);
  		glBindVertexArray(0);
  		glBindBuffer(GL_ARRAY_BUFFER,0);
	}
}