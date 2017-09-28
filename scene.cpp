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
		return glm::mat4(1.0f);
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

  		LoadScene("myscene.scn");
	}

	
	void Scene::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  		glUseProgram(shaderProgram);
		for (int i = 0; i < 3; ++i)
		{
			models[i]->Render();
		}
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
			rotation_matrix1 = glm::rotate(glm::mat4(1.0f), rx, glm::vec3(1.0f,0.0f,0.0f));
		    rotation_matrix1 = glm::rotate(rotation_matrix1, ry, glm::vec3(0.0f,1.0f,0.0f));
		    rotation_matrix1 = glm::rotate(rotation_matrix1, rz, glm::vec3(0.0f,0.0f,1.0f));
		    models[i]->rotation_matrix = rotation_matrix1;
		    models[i]->InitInspectionMode();
		}

		afile >> Eye.x >> Eye.y >> Eye.z;
		afile >> LookAt.x >> LookAt.y >> LookAt.z;
		afile >> LookUp.x >> LookUp.y >> LookUp.z;
		afile >> L >> R >> B >> T;
		afile >> N >> F;
		afile.close();
	}	
	void Scene::UpdateScene(glm::mat4 scene_matrix)
	{
		int len = models.size();
		for (int i=0; i<len; i++)
			models[i]->scene_matrix = scene_matrix;
	}
	void Scene::Update()
	{

		if (key_1)
		{
			glm::mat4 scene_matrix = glm::mat4(1.0f);
			scene_matrix *= WCSToVCS();
			UpdateScene(scene_matrix);
			key_1 = false;

		}
		else if (key_2)
		{
			glm::mat4 scene_matrix = glm::mat4(1.0f);
			scene_matrix *= WCSToVCS();
			scene_matrix *= VCSToCCS();		
			UpdateScene(scene_matrix);
			key_2 = false;	
		}
		else if (key_3)
		{
			glm::mat4 scene_matrix = glm::mat4(1.0f);
			scene_matrix *= WCSToVCS();
			scene_matrix *= VCSToCCS();
			scene_matrix *= CCSToNDCS();
			UpdateScene(scene_matrix);
			key_3 = false;
		}
		else if (key_4)
		{	
			glm::mat4 scene_matrix = glm::mat4(1.0f);
			scene_matrix *= WCSToVCS();
			scene_matrix *= VCSToCCS();
			scene_matrix *= CCSToNDCS();
			scene_matrix *= NDCSToDCS();
			UpdateScene(scene_matrix);
			key_4 = false;
		}
		
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
	bool Scene::key_5 = false;
	bool Scene::key_6 = false;
}