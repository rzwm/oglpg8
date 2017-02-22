///////////////////////////////////////////////
// 简介：
//		试验glPrimitiveRestart()函数，使用此函数
//		绘制两个三角形条带组成一个正方体。
//
///////////////////////////////////////////////
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include "LoadShaders.h"
#include "vmath.h"
#include <Windows.h>

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint g_program;
GLfloat g_aspect;
GLint g_render_model_matrix_loc;
GLint g_render_projection_matrix_loc;

void display()
{
	float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);
	static const float q = 0.0f;
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (g_program != 0)
	{
		vmath::mat4 model_matrix(vmath::translate(0.0f, 0.0f, -5.0f) * vmath::rotate(t * 360.f, Y) * vmath::rotate(t * 720.f, Z));
		vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -1 / g_aspect, 1 / g_aspect, 1.0f, 500.f));

		glUniformMatrix4fv(g_render_model_matrix_loc, 1, GL_FALSE, model_matrix);
		glUniformMatrix4fv(g_render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

		glBindVertexArray(VAO);

#define USE_PRIMITIVE_RESTART 1
#if USE_PRIMITIVE_RESTART
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, (const void*)0);
#else
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const void*)0);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const void*)(9 * sizeof(GLushort)));
#endif
	}

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	g_aspect = float(width) / float(height);
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const GLfloat cube_positions[] =
	{
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	const GLfloat cube_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors), cube_colors);

	const GLushort cube_indicies[] =
	{
		0, 1, 2, 3, 6, 7, 4, 5,
		0xFFFF,
		2, 6, 0, 4, 1, 5, 3, 7
	};
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indicies), cube_indicies, GL_STATIC_DRAW);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "PrimitiveRestart.vs" },
		{ GL_FRAGMENT_SHADER, "PrimitiveRestart.fs" },
		{ GL_NONE, "" }
	};
	g_program = loadShaders(shaders);
	if (g_program != 0)
	{
		glUseProgram(g_program);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(cube_positions));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		g_render_model_matrix_loc = glGetUniformLocation(g_program, "model_matrix");
		g_render_projection_matrix_loc = glGetUniformLocation(g_program, "projection_matrix");
	}
}

void uninit()
{
	if (g_program != 0)
	{
		glDeleteProgram(g_program);
		g_program = 0;
	}

	glDeleteBuffers(1, &EBO);
	EBO = 0;
	glDeleteBuffers(1, &VBO);
	VBO = 0;
	glDeleteVertexArrays(1, &VAO);
	VAO = 0;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutCreateWindow(argv[0]);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);

	init();

	glutMainLoop();

	uninit();

	return 0;
}