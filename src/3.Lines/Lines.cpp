///////////////////////////////////////////////
// 简介：
//		分别使用GL_LINES、GL_LINE_STRIP和GL_LINE_
//		LOOP绘制直线。
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
		glDrawArrays(GL_LINES, 0, 4);
		glDrawArrays(GL_LINE_STRIP, 4, 4);
		glDrawArrays(GL_LINE_LOOP, 8, 4);
	}
	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(20);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLfloat vertices[] =
	{
		// 使用GL_LINES绘制
		-0.7f,  0.6f,
		-0.3f,  0.6f,
		 0.3f,  0.8f,
		 0.7f,  1.0f,

		 // 使用GL_LINE_STRIP绘制
		 -0.7f,  0.1f,
		 -0.3f,  0.1f,
		  0.3f,  0.3f,
		  0.7f,  0.5f,

		 // 使用GL_LINE_LOOP绘制
		 -0.7f, -0.8f,
		 -0.3f, -0.8f,
		  0.3f, -0.6f,
		  0.7f, -0.4f,
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] = 
	{
		{ GL_VERTEX_SHADER, "Lines.vs" },
		{ GL_FRAGMENT_SHADER, "Lines.fs" },
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