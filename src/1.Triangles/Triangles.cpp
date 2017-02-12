///////////////////////////////////////////////
// 简介：
//		在窗口中绘制两个三角形。
//
///////////////////////////////////////////////
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include <LoadShaders.h>

GLuint VAO;
GLuint VBO;
GLuint g_program;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (g_program != 0)
	{
		glUseProgram(g_program);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLfloat vertices[] =
	{
		-0.90f, -0.90f,
		 0.85f, -0.90f,
		-0.90f,  0.85f,

		 0.90f, -0.85f,
		 0.90f,  0.90f,
		-0.85f,  0.90f
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "triangles.vs" },
		{ GL_FRAGMENT_SHADER, "triangles.fs" },
		{ GL_NONE, "" }
	};
	g_program = loadShaders(shaders);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const void*) 0);
	glEnableVertexAttribArray(0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}

	glutDisplayFunc(display);

	init();

	glutMainLoop();

	return 0;
}
