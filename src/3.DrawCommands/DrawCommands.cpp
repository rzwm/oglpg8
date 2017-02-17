#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include "LoadShaders.h"
#include "vmath.h"
using namespace vmath;

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint g_program;
GLfloat g_aspect;
GLint g_projection_matrix_loc;
GLint g_model_matrix_loc;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (g_program != 0)
	{
		mat4 projection_matrix(frustum(-1.0f, 1.0f, -g_aspect, g_aspect, 1.0f, 500.0f));
		glUniformMatrix4fv(g_projection_matrix_loc, 1, GL_FALSE, projection_matrix);
		
		mat4 model_matrix;

		glBindVertexArray(VAO);

		// DrawArrays
		model_matrix = vmath::translate(-3.0f, 0.0f, -5.0f);
		glUniformMatrix4fv(g_model_matrix_loc, 1, GL_FALSE, model_matrix);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// DrawElements
		model_matrix = vmath::translate(-1.0f, 0.0f, -5.0f);
		glUniformMatrix4fv(g_model_matrix_loc, 1, GL_FALSE, model_matrix);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (const void*)0);

		// DrawElementesBaseVertex
		model_matrix = vmath::translate(1.0f, 0.0f, -5.0f);
		glUniformMatrix4fv(g_model_matrix_loc, 1, GL_FALSE, model_matrix);
		glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (const void*)0, 1);

		// DrawArraysInstanced
		model_matrix = vmath::translate(3.0f, 0.0f, -5.0f);
		glUniformMatrix4fv(g_model_matrix_loc, 1, GL_FALSE, model_matrix);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);
	}


	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	g_aspect = static_cast<GLfloat>(w) / static_cast<GLfloat>(h);
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	const GLfloat vertex_positions[] =
	{
		-1.0f, -1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 1.0f
	};

	const GLfloat vertex_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f
	};

	const GLushort vertex_indices[] =
	{
		0, 1, 2
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors),
				 NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "DrawCommands.vs" },
		{ GL_FRAGMENT_SHADER, "DrawCommands.fs" },
		{ GL_NONE, "" }
	};
	g_program = loadShaders(shaders);

	if (g_program != 0)
	{
		glUseProgram(g_program);
		g_projection_matrix_loc = glGetUniformLocation(g_program, "projection_matrix");
		g_model_matrix_loc = glGetUniformLocation(g_program, "model_matrix");

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(vertex_positions));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

	}
}

void uninit()
{
	glUseProgram(0);
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
	glutReshapeFunc(reshape);
	
	init();

	glutMainLoop();

	uninit();

	return 0;
}