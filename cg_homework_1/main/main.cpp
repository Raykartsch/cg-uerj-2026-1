// Run these commands in terminal to execute this code in Linux:
// 1. g++ -o code code.cpp -lglut -lGLU -lGL -lm
// 2. ./code


// Run these commands in terminal to execute this code in Windows:
// 1. g++ code.cpp -o code -lfreeglut -lglu32 -lopengl32
// 2. .\code



#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include "Animacao.hpp"
#include "Primitivas.cpp"
#include "Borboleta.cpp"
#include "Coelho.cpp"
#include "Cenario.cpp"
#include "Raposa.cpp"
#include "Animacao.cpp"

int main(int argc, char** argv) {
	srand(static_cast<unsigned int>(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (800, 800);
	glutInitWindowPosition (200, 200);
	glutCreateWindow("Trabalho 1 - Computacao Grafica");

	init();

	glutDisplayFunc(display);
	glutTimerFunc(msecs, anim, 0);
	glutKeyboardFunc(keyboard_callback);
	glutKeyboardUpFunc(keyboard_up_callback);
	glutSpecialFunc(arrowKeysDown);
	glutSpecialUpFunc(arrowKeysUp);

	glutMainLoop();
	return 0;
}
