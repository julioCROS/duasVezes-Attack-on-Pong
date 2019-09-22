#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#define aumentoVEL_COLISAO 0.00012


void verificaColisao(GLfloat xBola, GLfloat yBola, GLfloat yJogador, int tamanhoJOGADOR_Y, int tamanhoBOLA_Y, int toquesTotal, bool xBolaDirecao, float incrementoVEL_X, float incrementoVEL_Y)
{
  if((yBola <= (yJogador + tamanhoJOGADOR_Y/2 + tamanhoBOLA_Y / 2)) && (yBola >= (yJogador - tamanhoJOGADOR_Y / 2 - tamanhoBOLA_Y / 2)))
  {
    //Muda de direção ao colidir com a barra do jogador 1
    xBolaDirecao = !xBolaDirecao;
    toquesTotal++;
    //Aumenta velocidade ao bater na barra
    incrementoVEL_X = incrementoVEL_X + aumentoVEL_COLISAO;
    incrementoVEL_Y = incrementoVEL_Y + aumentoVEL_COLISAO;
  }
}
