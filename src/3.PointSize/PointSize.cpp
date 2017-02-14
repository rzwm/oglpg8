///////////////////////////////////////////////
// 简介：
//		根据 3.1.1 点 节的内容，试验使用glPointSize()
//		和gl_PointSize两种方法设置点的大小。
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
		glUseProgram(g_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);
	}

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 启用此句，会导致使用顶点着色器的gl_PointSize来设置点的大小
	//glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(10);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLfloat vertices[] =
	{
		0.0f, 0.0f
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "PointSize.vs" },
		{ GL_FRAGMENT_SHADER, "PointSize.fs" },
		{ GL_NONE, "" }
	};

	g_program = loadShaders(shaders);
	if (g_program != 0)
	{
		glUseProgram(g_program);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const void*)0);
		glEnableVertexAttribArray(0);
	}
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