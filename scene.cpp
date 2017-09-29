#include "scene.hpp"
namespace cft
{
	glm::mat4 Scene::WCSToVCS()
	{
		glm::vec3 n = glm::normalize(-LookAt+Eye);
		glm::vec3 u = glm::normalize(glm::cross(LookUp,n));
		glm::vec3 v =glm::cross(n,u);
		return glm::mat4({
			{u.x,v.x,n.x,0},
			{u.y,v.y,n.y,0},
			{u.z,v.z,n.z,0},
			{-Eye.x,-Eye.y,-Eye.z,1}
			});
	}

	glm::mat4 Scene::VCSToCCS()
	{
		return glm::mat4({
			{(2*N)/(R-L),0,0,0},
			{0,(2*N)/(T-B),0,0},
			{(R+L)/(R-L),(T+B)/(T-B),(-F-N)/(F-N),-1},
			{0,0,(-2*F*N)/(F-N),0}
		});
	}

	glm::mat4 Scene::CCSToNDCS()
	{
		return glm::mat4(1.0f);
	}

	glm::mat4 Scene::NDCSToDCS()
	{
		return glm::mat4({
			{1,0,0,0},
			{0,1,0,0},
			{0,0,0,0},
			{0,0,0,1}
		});
	}
	Scene::Scene()
	{

	}

	void Scene::Init(GLFWwindow *win)
	{
		// Saving reference to window
		window = win;

		// Set framebuffecol_r cleacol_r color
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   		glClearDepth(1.0);
    	glDepthFunc(GL_LESS);
    	glEnable(GL_DEPTH_TEST); 
    	glShadeModel(GL_SMOOTH);

    	// Setting the callbacks
  		glfwSetFramebufferSizeCallback(window, cft::Scene::ResizeHandler);
    	glfwSetKeyCallback(window, cft::Scene::KeyboardHandler);
    	glfwSetMouseButtonCallback(window, cft::Scene::MouseHandler);
    	glfwSetCursorPosCallback(window, cft::Scene::CursorHandler);
  		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		// Setting up the shaders
		std::vector<GLuint> shaderList;
 		shaderList.push_back(cft::LoadShaderGL(GL_VERTEX_SHADER, cft::vertex_shader));
  		shaderList.push_back(cft::LoadShaderGL(GL_FRAGMENT_SHADER, cft::fragment_shader));
  		shaderProgram = cft::CreateProgramGL(shaderList);

  		uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

  		total_points = 32;
  		line_points = 32;
  		point = new glm::vec4[total_points];
  		point_color = new glm::vec4[total_points];

  		LoadScene("myscene.scn");
	}


	void Scene::LoadScene(std::string file)
	{
		std::fstream afile;
		afile.open(file,std::fstream::in | std::fstream::out);
		std::string raw_model;		
		for (int i = 0; i < 3; ++i)
		{
			afile >> raw_model;
			models.push_back(new Model(shaderProgram));
			models[i]->LoadModel(raw_model);
			afile >> models[i]->scaling.x  >> models[i]->scaling.y >> models[i]->scaling.z;
			GLfloat rx, ry, rz;	afile >> rx >> ry >> rz;
			afile >> models[i]->translate.x  >> models[i]->translate.y >> models[i]->translate.z;
			glm::mat4 rotation_matrix1;
			rotation_matrix1 = glm::rotate(glm::mat4(1.00f), rx, glm::vec3(1.0f,0.0f,0.0f));
		    rotation_matrix1 = glm::rotate(rotation_matrix1, ry, glm::vec3(0.0f,1.0f,0.0f));
		    rotation_matrix1 = glm::rotate(rotation_matrix1, rz, glm::vec3(0.0f,0.0f,1.0f));
		    models[i]->rotation_matrix = rotation_matrix1;
		    models[i]->InitModellingMode();
		}

		afile >> Eye.x >> Eye.y >> Eye.z;
		afile >> LookAt.x >> LookAt.y >> LookAt.z;
		afile >> LookUp.x >> LookUp.y >> LookUp.z;
		afile >> L >> R >> B >> T;
		afile >> N >> F;
		afile.close();
		double fL = L*(F/N),fR = R*(F/N),fB = B*(F/N),fT = T*(F/N),eX,eY,eZ;
		point[0] = glm::vec4(L,T,-N,1.0f);
		point[1] = glm::vec4(R,T,-N,1.0f);
		point[2] = glm::vec4(L,T,-N,1.0f);
		point[3] = glm::vec4(L,B,-N,1.0f);
		point[4] = glm::vec4(R,B,-N,1.0f);
		point[5] = glm::vec4(L,B,-N,1.0f);
		point[6] = glm::vec4(R,B,-N,1.0f);
		point[7] = glm::vec4(R,T,-N,1.0f);
		point[8] = glm::vec4(fL,fT,-F,1.0f);
		point[9] = glm::vec4(fR,fT,-F,1.0f);
		point[10] = glm::vec4(fL,fT,-F,1.0f);
		point[11] = glm::vec4(fL,fB,-F,1.0f);
		point[12] = glm::vec4(fR,fB,-F,1.0f);
		point[13] = glm::vec4(fL,fB,-F,1.0f);
		point[14] = glm::vec4(fR,fB,-F,1.0f);
		point[15] = glm::vec4(fR,fT,-F,1.0f);
		point[16] = glm::vec4(L,T,-N,1.0f);
		point[17] = glm::vec4(fL,fT,-F,1.0f);
		point[18] = glm::vec4(R,T,-N,1.0f);
		point[19] = glm::vec4(fR,fT,-F,1.0f);
		point[20] = glm::vec4(L,B,-N,1.0f);
		point[21] = glm::vec4(fL,fB,-F,1.0f);
		point[22] = glm::vec4(R,B,-N,1.0f);
		point[23] = glm::vec4(fR,fB,-F,1.0f);
		point[24] = glm::vec4(L,T,-N,1.0f);
		point[25] = glm::vec4(Eye.x,Eye.y,Eye.z,1.0f);
		point[26] = glm::vec4(R,T,-N,1.0f);
		point[27] = glm::vec4(Eye.x,Eye.y,Eye.z,1.0f);
		point[28] = glm::vec4(R,B,-N,1.0f);
		point[29] = glm::vec4(Eye.x,Eye.y,Eye.z,1.0f);
		point[30] = glm::vec4(L,B,-N,1.0f);
		point[31] = glm::vec4(Eye.x,Eye.y,Eye.z,1.0f);
		// point[32] = glm::vec4(Eye.x,Eye.y,Eye.z,1.0f);
		glm::mat4 transform_matrix = glm::inverse(WCSToVCS());

		for (int i=0; i<24; i++)
		{
			point[i] = transform_matrix*point[i];
			point_color[i] = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		}

		for (int i=24; i<32; i++)
		{
			if(i%2==0)
				point[i] = transform_matrix*point[i];
			point_color[i] = glm::vec4(1.0f,1.0f,0.0f,1.0f);
		}

		glGenVertexArrays (1, &vao);
  		glBindVertexArray (vao);
		glGenBuffers (1, &vbo);
  		glBindBuffer (GL_ARRAY_BUFFER, vbo);
  		glBufferData (GL_ARRAY_BUFFER, 2*line_points*sizeof(glm::vec4), 0, GL_STATIC_DRAW);
  		glBufferSubData( GL_ARRAY_BUFFER, 0, line_points*sizeof(glm::vec4), point );
  		glBufferSubData( GL_ARRAY_BUFFER, line_points*sizeof(glm::vec4), line_points*sizeof(glm::vec4), point_color );
  		// glBufferSubData( GL_ARRAY_BUFFER, 2*line_points*sizeof(glm::vec4), sizeof(glm::vec4), &point[32]);
  		// glBufferSubData( GL_ARRAY_BUFFER, (2*line_points+1)*sizeof(glm::vec4), sizeof(glm::vec4), &point_color[32]);
  		GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(total_points*sizeof(glm::vec4)) );
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}	
	void Scene::Update()
	{

		if (key_1)
		{
			scene_matrix = glm::mat4(1.0f)*WCSToVCS();
			key_1 = false;

		}
		else if (key_2)
		{
			scene_matrix = glm::mat4(1.0f)*VCSToCCS()*WCSToVCS();
			key_2 = false;	
		}
		else if (key_3)
		{
			scene_matrix = glm::mat4(1.0f)*CCSToNDCS()*VCSToCCS()*WCSToVCS();
			key_3 = false;
		}
		else if (key_4)
		{	
			scene_matrix = glm::mat4(1.0f)*NDCSToDCS()*CCSToNDCS()*VCSToCCS()*WCSToVCS();
			key_4 = false;
		}
		else if (key_w)
		{
			translate.y -= delta_trans;
		}
		else if (key_a)
		{
			translate.x -= delta_trans;
		}
		else if (key_s)
		{
			translate.y += delta_trans;
		}
		else if (key_d)
		{
			translate.x += delta_trans;
		}
		else if (key_z)
		{
			translate.z += delta_trans;
		}
		else if (key_x)
		{
			translate.z -= delta_trans;
		}
		else if (key_up)
		{
			xrot = delta_rot;
		}
		else if (key_down)
		{
			xrot = -delta_rot;
		}
		else if (key_left)
		{
			yrot = delta_rot;
		}
		else if (key_right)
		{
			yrot = -delta_rot;
		}
		else if (key_pgup)
		{
			zrot = delta_rot;
		}
		else if (key_pgdwn)
		{
			zrot = -delta_rot;
		}
	}

	void Scene::Render()
	{
		glm::mat4 delta_rotation_matrix;
		delta_rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
	    delta_rotation_matrix = glm::rotate(delta_rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
	    delta_rotation_matrix = glm::rotate(delta_rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
	    rotation_matrix = delta_rotation_matrix*rotation_matrix;
	    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f),translate);
	    glm::mat4 model_scene_matrix = translation_matrix*rotation_matrix*scene_matrix;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  		glUseProgram(shaderProgram);
  		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);
		glPointSize(4.0);
		glm::mat4 ortho_matrix = glm::ortho(cft::left, cft::right, cft::top, cft::bottom, cft::near, cft::far);
		glm::mat4 view_matrix = ortho_matrix*model_scene_matrix;
		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glDrawArrays(GL_LINES, 0, line_points);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		for (int i = 0; i < 3; ++i)
		{
			models[i]->Render(model_scene_matrix);
		}
		xrot = 0.0f;
		yrot = 0.0f;
		zrot = 0.0f;
	}

	void Scene::KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			key_1 = true;
		else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			key_2 = true;
		else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
			key_3 = true;
		else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
			key_4 = true;
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			key_up = true;
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
			key_down = true;
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			key_left = true;
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			key_right = true;
		else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
			key_pgup = true;
		else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
			key_pgdwn = true;
		else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
			key_up = false;
		else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
			key_down = false;
		else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
			key_left = false;
		else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
			key_right = false;
		else if (key == GLFW_KEY_PAGE_UP && action == GLFW_RELEASE)
			key_pgup = false;
		else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_RELEASE)
			key_pgdwn = false;
		else if (key == GLFW_KEY_W && action == GLFW_PRESS)
			key_w = true;
		else if (key == GLFW_KEY_A && action == GLFW_PRESS)
			key_a = true;
		else if (key == GLFW_KEY_S && action == GLFW_PRESS)
			key_s = true; 
		else if (key == GLFW_KEY_D && action == GLFW_PRESS)
			key_d = true;
		else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
			key_w = false;
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
			key_a = false;
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
			key_s = false; 
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
			key_d = false;
		else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
			key_z = true;
		else if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
			key_z = false;
		else if (key == GLFW_KEY_X && action == GLFW_PRESS)
			key_x = true;
		else if (key == GLFW_KEY_X && action == GLFW_RELEASE)
			key_x = false;
	}


	void Scene::MouseHandler(GLFWwindow* window, int button, int action, int mods)
	{

	}
	void Scene::CursorHandler(GLFWwindow* window, double x, double y)
	{

	}

	void Scene::ResizeHandler(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Scene::ErrorHandler(int error, const char* description)
	{
		 std::cerr<<description<<std::endl;
	}

	bool Scene::key_1 = false;
	bool Scene::key_2 = false;
	bool Scene::key_3 = false;
	bool Scene::key_4 = false;
	bool Scene::key_up = false;
	bool Scene::key_down = false;
	bool Scene::key_left = false;
	bool Scene::key_right = false;
	bool Scene::key_pgup = false;
	bool Scene::key_pgdwn = false;
	bool Scene::key_w = false;
	bool Scene::key_a = false;
	bool Scene::key_s = false;
	bool Scene::key_d = false;
	bool Scene::key_z = false;
	bool Scene::key_x = false;

}