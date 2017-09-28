#include "crafter.hpp"
namespace cft
{	
	Crafter::Crafter()
	{

	}

	void Crafter::Init(GLFWwindow *win)
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
  		glfwSetFramebufferSizeCallback(window, cft::Crafter::ResizeHandler);
    	glfwSetKeyCallback(window, cft::Crafter::KeyboardHandler);
    	glfwSetMouseButtonCallback(window, cft::Crafter::MouseHandler);
    	glfwSetCursorPosCallback(window, cft::Crafter::CursorHandler);
  		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		// Setting up the shaders
		std::vector<GLuint> shaderList;
 		shaderList.push_back(cft::LoadShaderGL(GL_VERTEX_SHADER, cft::vertex_shader));
  		shaderList.push_back(cft::LoadShaderGL(GL_FRAGMENT_SHADER, cft::fragment_shader));
  		shaderProgram = cft::CreateProgramGL(shaderList);
		
  		//Setup for lines in Modelling View
  		total_lines = screen_width/line_gap + screen_height/line_gap + 2;
  		total_points = 2*total_lines + 3;
  		points = new glm::vec4[total_points];
  		point_color = new glm::vec4[total_points];
  		int cpoint = 0;
  		for (int py = 0; py <= screen_height; py += line_gap)
  		{
  			float y = -((float)py*(2.0/screen_height) - 1.0f);
  			point_color[cpoint] = glm::vec4(0.5f,0.5f,0.5f,1.0f);points[cpoint++] = glm::vec4(1.0,y,1.0f,1.0f);
  			point_color[cpoint] = glm::vec4(0.5f,0.5f,0.5f,1.0f);points[cpoint++] = glm::vec4(-1.0,y,1.0f,1.0f);
  		}
  		for (int px = 0; px <= screen_width; px += line_gap)
  		{
  			float x = (float)px*(2.0/screen_width) - 1.0f;
  			point_color[cpoint] = glm::vec4(0.5f,0.5f,0.5f,1.0f);points[cpoint++] = glm::vec4(x,1.0f,1.0f,1.0f);
  			point_color[cpoint] = glm::vec4(0.5f,0.5f,0.5f,1.0f);points[cpoint++] = glm::vec4(x,-1.0f,1.0f,1.0f);
  		}
  		for (int i = 0; i < 3; i ++)
  		{
  			point_color[cpoint++] = glm::vec4(0.0f,1.0f,0.0f,1.0f);
  		}
  		uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
		glGenVertexArrays (1, &vao);
  		glBindVertexArray (vao);
		glGenBuffers (1, &vbo);
  		glBindBuffer (GL_ARRAY_BUFFER, vbo);
  		glBufferData (GL_ARRAY_BUFFER, 2*total_points*sizeof(glm::vec4), 0, GL_STATIC_DRAW);
  		glBufferSubData( GL_ARRAY_BUFFER, 0, total_points*sizeof(glm::vec4), points );
  		glBufferSubData( GL_ARRAY_BUFFER, total_points*sizeof(glm::vec4), total_points*sizeof(glm::vec4), point_color );
  		GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(total_points*sizeof(glm::vec4)) );
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		//Initialize Variables
		state = MODELLING;
		index = 0;
		
		// Setting the model
		model = new cft::Model(shaderProgram);
		palette = new cft::Palette(shaderProgram);
	}
	void Crafter::Update()
	{
		model->xrot = 0;
		model->yrot = 0;
		model->zrot = 0;
		if (key_M)
		{
			key_M = false;
			if (state != MODELLING)
			{
				state = MODELLING;
				std::cout << "Modelling Mode" << std::endl;
				model->InitModellingMode();
			}
		}
		else if (key_I)
		{
			key_I = false;
			if (state != INSPECTION)
			{
				state = INSPECTION;
				std::cout << "Inspection Mode" << std::endl;
				model->InitInspectionMode();
			}
		}
		else if (key_L)
		{
			key_L = false;
			std::string filename;
			std::cout << "File: ";
			std::cin >> filename;
			model->LoadModel(filename);
		}
		else if (key_K)
		{
			key_K = false;
			std::string filename;
			std::cout << "File: ";
			std::cin >> filename;
			model->SaveModel(filename);
			std::cout << "saved file" << std::endl;
		}
		else if (key_R)
		{
			key_R = false;
			if (state == INSPECTION)
			{
				model->RecenterModel();
			}
		}
		else if (key_W)
		{
			if (state == INSPECTION)
			{
				model->translate.y += delta_trans;
			}
		}
		else if (key_A)
		{
			if (state == INSPECTION)
			{
				model->translate.x -= delta_trans;
			}
		}
		else if (key_S)
		{
			if (state == INSPECTION)
			{
				model->translate.y -= delta_trans;
			}
		}
		else if (key_D)
		{
			if (state == INSPECTION)
			{
				model->translate.x += delta_trans;
			}
		}
		else if (key_Z)
		{
			if (state == MODELLING) 
			{
				key_Z = false;
				posz = posz == screen_depth ? screen_depth : posz + line_gap;
				std::cout << "Drawing plane Z = " << (float)posz*(2.0/screen_depth) - 1 << std::endl;
			}
			else if (state == INSPECTION)
			{
				model->translate.z += delta_trans;
			}
		}
		else if (key_X)
		{
			if (state == MODELLING)
			{
				key_X = false;
				posz = posz == 0 ? 0 : posz - line_gap;
				std::cout << "Drawing plane Z = " << (float)posz*(2.0/screen_depth) - 1 << std::endl;
			}
			else if (state == INSPECTION)
			{
				model->translate.z -= delta_trans;
				
			}
		}
		else if (key_up)
		{
			if (state == INSPECTION)
				model->xrot += delta_rot;
		}
		else if (key_down)
		{
			if (state == INSPECTION)
				model->xrot += -delta_rot;	
		}
		else if (key_left)
		{
			if (state == INSPECTION)
				model->yrot += delta_rot;	
		}
		else if (key_right)
		{
			if (state == INSPECTION)
				model->yrot += -delta_rot;
		}
		else if (key_PgUp)
		{
			if (state == INSPECTION)
				model->zrot += delta_rot;
		}
		else if (key_PgDown)
		{
			if (state == INSPECTION)
				model->zrot += -delta_rot;
		}
		else if (button_left && key_shift)
		{
			button_left = false;
			if (state == MODELLING)
			{
				if (index == 0)
				{
					index = 2;
					model->RemoveTriangle(vertices,color);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBindVertexArray(vao);
					glBufferSubData( GL_ARRAY_BUFFER, (total_points-3)*sizeof(glm::vec4), 3*sizeof(glm::vec4), &vertices[0]);
	  				glBufferSubData( GL_ARRAY_BUFFER, total_points*sizeof(glm::vec4), total_points*sizeof(glm::vec4), point_color );
	  				glBindVertexArray(0);
	  				glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
				else
					index--;
			}
		}
		else if (button_left)
		{
			button_left = false;
			if (state == MODELLING)
			{	
				glm::vec4 tcol;
				if (palette->PickColor(posx,posy,&tcol))
				{
					col_r = tcol.x*255;
					col_g = tcol.y*255;
					col_b = tcol.z*255;
					return;
				}
				int modx = (int)posx%line_gap;
				int mody = (int)posy%line_gap;
				int modz = (int)posz%line_gap;
				int snapx = (int)posx + (modx < line_gap - modx ? -modx : line_gap - modx);
				int snapy = (int)posy + (mody < line_gap - mody ? -mody : line_gap - mody);
				int snapz = (int)posz + (modz < line_gap - modz ? -modz : line_gap - modz);
				float x = (float)snapx*(2.0/screen_width) - 1,y = -((float)snapy*(2.0/screen_height) - 1), z = (float)snapz*(2.0/screen_depth) - 1;
				float r = (float)col_r/255.0,g = (float)col_g/255.0,b = (float)col_b/255.0;
				vertices[index] = glm::vec4(x,y,z,1.0);
				color[index] = glm::vec4(r,g,b,1.0);
				index++;
				if (index == 3)
				{
					index = 0;
					model->AddTriangle(vertices,color);
				}
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindVertexArray(vao);
				glBufferSubData( GL_ARRAY_BUFFER, (total_points-3)*sizeof(glm::vec4), 3*sizeof(glm::vec4), &vertices[0]);
  				glBufferSubData( GL_ARRAY_BUFFER, total_points*sizeof(glm::vec4), total_points*sizeof(glm::vec4), point_color );
  				glBindVertexArray(0);
  				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		else if (key_1)
		{
			if (state == MODELLING)
				col_r = col_r == 255 ? 255 : col_r + 1;
		}
		else if (key_2)
		{
			if (state == MODELLING)
				col_g = col_g == 255 ? 255 : col_g + 1;
		}
		else if (key_3)
		{
			if (state == MODELLING)
				col_b = col_b == 255 ? 255 : col_b + 1;
		}
		else if (key_4)
		{
			if (state == MODELLING)
				col_r = col_r == 0 ? 0 : col_r - 1;
		}
		else if (key_5)
		{
			if (state == MODELLING)
				col_g = col_g == 0 ? 0 : col_g - 1;
		}
		else if (key_6)
		{
			if (state == MODELLING)
				col_b = col_b == 0 ? 0 : col_b - 1;
		}

	}
	void Crafter::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  		glUseProgram(shaderProgram);
  		if (state == MODELLING)
  		{
  			glBindBuffer(GL_ARRAY_BUFFER, vbo);
  			glBindVertexArray(vao);
  			glPointSize(4.0);
  			glm::mat4 ortho_matrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  			glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(ortho_matrix));
  			glDrawArrays(GL_LINES, 0, total_points - 3);
  			glDrawArrays(GL_POINTS, total_points - 3, index);
  			glBindVertexArray(0);
  			glBindBuffer(GL_ARRAY_BUFFER, 0);
  			palette->Render();
  		}
  		model->Render(glm::mat4(1.0f));
	}

	void Crafter::KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key_L || key_K)
			return;
		else if (key == GLFW_KEY_M && action == GLFW_PRESS)
			key_M = true;
		else if (key == GLFW_KEY_I && action == GLFW_PRESS)
			key_I = true;
		else if (key == GLFW_KEY_L && action == GLFW_PRESS)
			key_L = true;
		else if (key == GLFW_KEY_K && action == GLFW_PRESS)
			key_K = true;
		else if (key == GLFW_KEY_R && action == GLFW_PRESS)
			key_R = true;
		else if (key == GLFW_KEY_W && action == GLFW_PRESS)
			key_W = true;
		else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
			key_W = false;
		else if (key == GLFW_KEY_A && action == GLFW_PRESS)
			key_A = true;
		else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
			key_A = false;
		else if (key == GLFW_KEY_S && action == GLFW_PRESS)
			key_S = true;
		else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
			key_S = false;
		else if (key == GLFW_KEY_D && action == GLFW_PRESS)
			key_D = true;
		else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
			key_D = false;
		else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
			key_Z = true;
		else if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
			key_Z = false;
		else if (key == GLFW_KEY_X && action == GLFW_PRESS)
			key_X = true;
		else if (key == GLFW_KEY_X && action == GLFW_RELEASE)
			key_X = false;
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			key_up = true;
		else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
			key_up = false;
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
			key_down = true;
		else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
			key_down = false;
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			key_left = true;
		else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
			key_left = false;
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			key_right = true;
		else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
			key_right = false;
		else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
			key_PgUp = true;
		else if (key == GLFW_KEY_PAGE_UP && action == GLFW_RELEASE)
			key_PgUp = false;
		else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
			key_PgDown= true;
		else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_RELEASE)
			key_PgDown = false;
		else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
			key_shift = true;
		else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
			key_shift = false;
		else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			key_1 = true;
		else if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
		{
			if (key_1)
				std::cout << "(R,G,B) = (" << col_r << "," << col_g << "," << col_b << ")" << std:: endl;
			key_1 = false;
		}
		else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			key_2 = true;
		else if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
		{
			if (key_2)
				std::cout << "(R,G,B) = (" << col_r << "," << col_g << "," << col_b << ")" << std:: endl;
			key_2 = false;
		}
		else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
			key_3 = true;
		else if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
		{
			if (key_3)
				std::cout << "(R,G,B) = (" << col_r << "," << col_g << "," << col_b << ")" << std:: endl;
			key_3 = false;
		}
		else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
			key_4 = true;
		else if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
		{
			if (key_4)
				std::cout << "(R,G,B) = (" << col_r << "," << col_g << "," << col_b << ")" << std:: endl;
			key_4 = false;
		}
		else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
			key_5 = true;
		else if (key == GLFW_KEY_5 && action == GLFW_RELEASE)
		{
			if (key_5)
				std::cout << "(R,G,B) = (" << col_r << "," << col_g << "," << col_b << ")" << std:: endl;
			key_5 = false;
		}
		else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
			key_6 = true;
		else if (key == GLFW_KEY_6 && action == GLFW_RELEASE)
		{
			if (key_6)
				std::cout << "(R,G,B) = (" << col_r << "," << col_g << "," << col_b << ")" << std:: endl;
			key_6 = false;
		}
	}
	
	void Crafter::MouseHandler(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
			button_left = true;
	}
	void Crafter::CursorHandler(GLFWwindow* window, double x, double y)
	{
		posx = x;
		posy = y;
	}

	void Crafter::ResizeHandler(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Crafter::ErrorHandler(int error, const char* description)
	{
		 std::cerr<<description<<std::endl;
	}

	bool Crafter::key_up = false;
	bool Crafter::key_down = false;
	bool Crafter::key_left = false;
	bool Crafter::key_right = false;
	bool Crafter::key_M = false;
	bool Crafter::key_I = false;
	bool Crafter::key_L = false;
	bool Crafter::key_K = false;
	bool Crafter::key_R = false;
	bool Crafter::key_W = false;
	bool Crafter::key_A = false;
	bool Crafter::key_S = false;
	bool Crafter::key_D = false;
	bool Crafter::key_Z = false;
	bool Crafter::key_X = false;
	bool Crafter::key_PgUp = false;
	bool Crafter::key_PgDown = false;
	bool Crafter::key_shift = false;
	bool Crafter::key_1 = false;
	bool Crafter::key_2 = false;
	bool Crafter::key_3 = false;
	bool Crafter::key_4 = false;
	bool Crafter::key_5 = false;
	bool Crafter::key_6 = false;
	bool Crafter::button_left = false;
	double Crafter::posx = 0;
	double Crafter::posy = 0;
	double Crafter::posz = screen_depth/2;
	int Crafter::col_r = 128;
	int Crafter::col_g = 128;
	int Crafter::col_b = 128;
}