#pragma once
#include "GL/glew.h"
#include <string>

struct ShaderInfo
{
	GLenum type;
	std::string filename;
	GLuint shader;
};

GLuint loadShaders(ShaderInfo* shaders);
