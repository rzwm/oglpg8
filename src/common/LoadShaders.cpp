#include "LoadShaders.h"
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

const GLchar* readShader(const char* filename)
{
	FILE* file = nullptr;
	fopen_s(&file, filename, "rb");
	if (nullptr == file)
	{
#ifndef NDEBUG
		std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif // !NDEBUG
		return nullptr;
	}

	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];

	fread(source, 1, len, file);
	fclose(file);
	file = nullptr;

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}

GLuint loadShaders(ShaderInfo* shaders)
{
	if (nullptr == shaders)
	{
		return 0;
	}

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;
	while (entry->type != GL_NONE)
	{
		GLuint shader = glCreateShader(entry->type);
		entry->shader = shader;

		const GLchar* source = readShader(entry->filename.c_str());
		if (nullptr == source)
		{
			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}
			glDeleteProgram(program);
			return 0;
		}

		glShaderSource(shader, 1, &source, NULL);
		delete[] source;

		glCompileShader(shader);

		GLint compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE)
		{
#ifndef NDEBUG
			GLsizei len = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(shader, len, &len, log);
			log[len] = 0;
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
#endif // NDEBUG

			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}
			glDeleteProgram(program);
			return 0;
		}

		glAttachShader(program, shader);

		++entry;
	}

	glLinkProgram(program);

	GLint linked = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE)
	{
#ifndef NDEBUG
		GLsizei len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		log[len] = 0;
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
#endif // NDEBUG

		for (entry = shaders; entry->type != GL_NONE; ++entry)
		{
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

#ifdef __cplusplus
}
#endif // __cplusplus
