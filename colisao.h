#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>


void verificaColisao(GLfloat xBola, GLfloat yBola, GLfloat yJogador, int tamanhoJOGADOR_Y, int tamanhoBOLA_Y, int toquesTotal, bool xBolaDirecao, float incrementoVEL_X, float incrementoVEL_Y);
