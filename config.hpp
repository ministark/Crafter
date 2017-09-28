#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#define MODELLING 0
#define INSPECTION 1

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#include <string>
/**
 * @file  config.hpp
 * @brief  This stores all constants required to run the Crafter for simplicity of modifying
 *
 * 
 */

namespace cft
{
	// Screen Size
	const int screen_width = 1000;
	const int screen_height = 1000;
	const int screen_depth = 1000;
	// Shaders 
	const std::string vertex_shader = "vertex_shader.glsl";
	const std::string fragment_shader = "fragment_shader.glsl";

	// Maximum vertices in the model
	const int max_vertices = 500;

	// Grid Space in the model
	const int line_gap = 20;

	//Rotation and Translation
	const float delta_rot = 0.01;
	const float delta_trans = 0.01;

	//Camera position
	const float camera_x = 0.0f;
	const float camera_y = 0.0f;
	const float camera_z = 3.0f;

	//Palette position
	const float xmin = screen_width - 200.0f;
	const float ymin = 0.0f;
	const float xmax = screen_width;
	const float ymax = 200.0f;


}
#endif