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

		// Set framebuffer clear color
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
		

		//Initialize Variables
		state = MODELLING;
		index = 0;
		r = g = b = 0.5f;
		// Setting the model
		model = new cft::Model(shaderProgram);
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
			}
		}
		else if (key_I)
		{
			key_I = false;
			if (state != INSPECTION)
			{
				state = INSPECTION;
				std::cout << "Inspection Mode" << std::endl;
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
		else if (key_W)
		{
			if (state == INSPECTION)
				model->translate.y += delta_trans;
		}
		else if (key_A)
		{
			if (state == INSPECTION)
				model->translate.x -= delta_trans;
		}
		else if (key_S)
		{
			if (state == INSPECTION)
			model->translate.y -= delta_trans;
		}
		else if (key_D)
		{
			if (state == INSPECTION)
				model->translate.x += delta_trans;
		}
		else if (key_Z)
		{
			if (state == MODELLING)
				posz = posz == screen_depth ? screen_depth : posz + 1;
			else if (state == INSPECTION)
			{
				model->translate.z += delta_trans;
			}
		}
		else if (key_X)
		{
			if (state == MODELLING)
				posz = posz == 0 ? 0 : posz - 1;
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
				}
				else
					index--;
				std::cout <<"rp - " << posx << " , " << posy << std::endl;
			}
		}
		else if (button_left)
		{
			button_left = false;
			if (state == MODELLING)
			{
				int snapx = (int)posx - (int)posx%20;
				int snapy = (int)posy - (int)posy%20;
				int snapz = (int)posz - (int)posz%20;
				float x = (float)snapx*(2.0/screen_width) - 1,y = -((float)snapy*(2.0/screen_height) - 1), z = (float)snapz*(2.0/screen_depth) - 1,r,g,b;
				std::cout << snapx << " " << snapy << " " << snapz << std::endl;
				std::cout << x << " " << y << " " << z << std::endl;
				std::cout << r << " " << g << " " << b << std::endl;
				vertices[index] = glm::vec4(x,y,z,1.0);
				color[index] = glm::vec4(r,g,b,1.0);
				index++;
				if (index == 3)
				{
					index = 0;
					model->AddTriangle(vertices,color);
					std::cout << "add called" << std::endl;
				}
			}
		}
	}
	void Crafter::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  		glUseProgram(shaderProgram);
  		model->Render();
	}

	void Crafter::KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
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
	bool Crafter::button_left = false;
	double Crafter::posx = 0;
	double Crafter::posy = 0;
	double Crafter::posz = screen_depth/2;
}