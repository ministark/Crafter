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
		{
			state = MODELLING;
			std::cout << "Modelling Mode" << std::endl;
		}
		else if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			state = INSPECTION;
			std::cout << "Inspection Mode" << std::endl;
		}
		else if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			std::string filename;
			std::cout << "Enter File Name: " << std::endl;
			std::cin >> filename;
		}
	}
	 
	void Crafter::ResizeHandler(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Crafter::ErrorHandler(int error, const char* description)
	{
		 std::cerr<<description<<std::endl;
	}

	int Crafter::state = MODELLING;
}