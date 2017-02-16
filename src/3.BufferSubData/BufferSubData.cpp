///////////////////////////////////////////////
// 简介：
//		试验glBufferSubData()和glClearBufferSubData()
//		两个函数。
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
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(8 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	const GLfloat vertices[] =
	{
		 0.9f, -0.9f,
		 0.9f,  0.9f,
		-0.9f,  0.9f,
		-0.9f, -0.9f
	};

	const GLfloat colors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
	// 启用以下两句使绘制的图像变为灰色
	//GLfloat red = 0.5f;
	//glClearBufferSubData(GL_ARRAY_BUFFER, GL_R32F, sizeof(vertices), sizeof(colors), GL_RED, GL_FLOAT, (const void*)&red);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "BufferSubData.vs" },
		{ GL_FRAGMENT_SHADER, "BufferSubData.fs" },
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