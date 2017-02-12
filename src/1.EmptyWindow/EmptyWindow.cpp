///////////////////////////////////////////////
// 简介：
//		使用GLUT库创建一个空的窗口，以测试GLUT库
//		和GLEW库是否能够成功加载。
//
///////////////////////////////////////////////
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutMainLoop();

	return 0;
}
