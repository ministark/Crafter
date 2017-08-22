#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#define MODELLING 0
#define INSPECTION 1

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


#include <string>
namespace cft
{
	// Screen Size
	const int screen_width = 640;
	const int screen_height = 480;
	const int screen_depth = 640;
	// Shaders 
	const std::string vertex_shader = "vertex_shader.glsl";
	const std::string fragment_shader = "fragment_shader.glsl";

	// Maximum vertices in the model
	const int max_vertices = 100;

	//Rotation and Translation
	const float delta_rot = 0.01;
}
#endif