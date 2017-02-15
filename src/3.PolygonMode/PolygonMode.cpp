///////////////////////////////////////////////
// 简介：
//		试验函数glPolygonMode()和glCullFace()的
//		作用。
//
///////////////////////////////////////////////
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include "LoadShaders.h"

GLuint VAO;
GLuint VBO;
GLuint g_program;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (g_program != 0)
	{
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const void*)0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 9);

		glDisableVertexAttribArray(0);
	}

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(10);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	//glCullFace(GL_FRONT);
	//glCullFace(GL_FRONT_AND_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLfloat vertices[] =
	{
		-1.0f, -0.9f,
		-0.6f, -0.9f,
		-0.8f,  1.0f,

		-0.2f, -0.9f,
		 0.2f, -0.9f,
		 0.0f,  1.0f,

		 0.6f, -0.9f,
		 1.0f, -0.9f,
		 0.8f,  1.0f,
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "PolygonMode.vs" },
		{ GL_FRAGMENT_SHADER, "PolygonMode.fs" },
		{ GL_NONE, "" }
	};
	g_program = loadShaders(shaders);
	if (g_program != 0)
	{
		glUseProgram(g_program);
	}
}

void uninit()
{
	if (g_program != 0)
	{
		glDeleteProgram(g_program);
		g_program = 0;
	}

	glDeleteBuffers(1, &VBO);
	VBO = 0;
	glDeleteVertexArrays(1, &VAO);
	VAO = 0;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
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

	init();

	glutMainLoop();

	uninit();

	return 0;
}