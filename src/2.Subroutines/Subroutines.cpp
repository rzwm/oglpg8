///////////////////////////////////////////////
// 简介：
//		根据 2.6 着色器子程序 一节，练习使用着色器
//		子程序。在顶点着色器中实现两个子程序-红色
//		和蓝色，在软件端选择要使用的子程序以改变三
//		角形的颜色。
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

	if (0 != g_program)
	{
		glUseProgram(g_program);
		// 书中的“注意”有说明：调用glUseProgram()时会重新设置所有子程序uniform的值，
		// 所以这里每次都要重新设置。
		{
			GLint location = glGetSubroutineUniformLocation(g_program, GL_VERTEX_SHADER, "selected_func_color");
			if (location != -1)
			{
				// 把“Blue”改为“Red”试试！
				GLuint index = glGetSubroutineIndex(g_program, GL_VERTEX_SHADER, "Blue");
				if (index != GL_INVALID_INDEX)
				{
					glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &index);
				}
			}
		}
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
		-0.5f, -1.0f,
		 0.5f, -1.0f,
		 0.0f,  1.0f
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "Subroutines.vs" },
		{ GL_FRAGMENT_SHADER, "Subroutines.fs" },
		{ GL_NONE, "" }
	};
	g_program = loadShaders(shaders);
	if (0 != g_program)
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