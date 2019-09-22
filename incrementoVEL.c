#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

void incrementoVEL(bool xBolaDirecao, bool yBolaDirecao, GLfloat xBola, GLfloat yBola, float incrementoVEL_X, float incrementoVEL_Y){
    if ( xBolaDirecao == 1 && yBolaDirecao == 0 )
        {
            xBola += incrementoVEL_X;
            yBola -= incrementoVEL_Y;
        }
        else if ( xBolaDirecao == 1 && yBolaDirecao == 1 )
        {
            xBola += incrementoVEL_X;
            yBola += incrementoVEL_Y;
        }
        else if ( xBolaDirecao == 0 && yBolaDirecao == 0 )
        {
            xBola -= incrementoVEL_X;
            yBola -= incrementoVEL_Y;
        }
        else if ( xBolaDirecao == 0 && yBolaDirecao == 1 )
        {
            xBola -= incrementoVEL_X;
            yBola += incrementoVEL_Y;
        }
}
