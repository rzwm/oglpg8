#pragma once
#include "GL/glew.h"
#include <string>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct ShaderInfo
{
	GLenum type;
	std::string filename;
	GLuint shader;
};

GLuint loadShaders(ShaderInfo* shaders);

#ifdef __cplusplus
}
#endif // __cplusplus
