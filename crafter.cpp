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
		state = MODELLING;
		// Set framebuffer clear color
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   		glClearDepth(1.0);
    	glDepthFunc(GL_LESS);
    	glEnable(GL_DEPTH_TEST); 
    	glShadeModel(GL_SMOOTH);

    	// Setting the callbacks
  		glfwSetFramebufferSizeCallback(window, cft::Crafter::ResizeHandler);
    	glfwSetKeyCallback(window, cft::Crafter::InputHandler);
  		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		// Setting up the shaders
		std::vector<GLuint> shaderList;
 		shaderList.push_back(cft::LoadShaderGL(GL_VERTEX_SHADER, cft::vertex_shader));
  		shaderList.push_back(cft::LoadShaderGL(GL_FRAGMENT_SHADER, cft::fragment_shader));
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
	void Crafter::Update()
	{
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
			string filename;
			std::cout << "File: " << std::endl;
			std::cin >> filename;
			
		}
	}
	void Crafter::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  		glUseProgram(shaderProgram);
  		glBindVertexArray (vao);
  		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Crafter::InputHandler(GLFWwindow* window, int key, int scancode, int action, int mods) 
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
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			key_up = true;
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
			key_down = true;
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			key_left = true;
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			key_right = true;
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

}