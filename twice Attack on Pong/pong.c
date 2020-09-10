//PARA RODAR O PROGRAMA g++ pong.c -lsfml-audio -lSOIL -lGLEW -lGL -lGLU -lglut -lm
//Necessário instalar a biblioteca de audio SFML
//Comando no linux para instalar audio SFML: sudo apt-get install libsfml-dev

#include <SFML/Audio.hpp>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

//Variaveis de musicas/sons utilizados no jogo
sf::Music background_music;
sf::Music background_musicORIGINAL;
sf::Music colisaoCimaBaixo;
sf::Music colisaoPlayer1;
sf::Music colisaoPlayer2;
sf::Music goal;
sf::Music twiceCOINcollect;
sf::Music aotCOINcollect;
sf::Music sasageyo;
sf::Music cheerUP;
sf::Music mouseCLICK;
sf::Music breakthrough;
sf::Music espelhoCOINcollect;
sf::Music mirroWorldSOM;
sf::Music reverseCOINcollect;
sf::Music fancyU;
sf::Music zawarudoCOINcollect;

//Variavel para seleção do item que será gerado, pelo teclado
int itemSELECIONADO = 0;

//Variavel para troca de música na tela de créditos ao clicar na foto de TWICE
int twiceCLIQUE = 1;

//Variaveis de estado das telas
int telaINICIO = 1;
int telaJOGO = 0;
int telaPAUSA = 0;
int telaSAIR = 0;
int telaCREDITOS = 0;
int telaRESET = 0;
int telaWIN = 0;

//Variavel para saber se houve colisão com item
int colisaoITEM = 0;

//Variaveis para tocar musica na tela de jogo e na tela de cŕeditos
int tocarMUSICA = 0;
int tocarMUSICAcreditos = 0;

//Variaveis para limitar efeito dos itens
int muralhaToques = 0;
int espelhoToques = 0;
int twiceToques = 0;
//Variaveis para definir qual item será gerado aleatoriamente
int idITEMaleatorio = 0;

//Variavel para definir em qual lado a muralha do item "AoT Coin" irá aparecer
bool xBolaDIRECAOaot = 0;

//Variaveis para movimentação do cenário
float xChao = 0;
float xChao2 = -1990;

float xFundo = 0;
float xFundo2 = -2000;

float xNuvem = 0;
float xNuvem2 = -2000;

float nuvemXjogo = 0;
float nuvemXjogo2 = -2000;

//Variaveis para definir tamanho do item no cenário
int XtamITEM = 105;
int YtamITEM = 130;

//Variaveis contadoras de frames em momentos especificos do jogo para fazer coisas especificas
int frame = 0; //Atualização na contagem de frame utilizada para geração de items aleatorios em momentos aleatorios do jogo
int frameAleatorio = 0; //Frame para definir em que momento será gerado um novo item
int frameZaWarudo = 0; //Frame para troca de cenário a partir do sprite "ZaWarudoSPRITE"
int atraso = 100; //Frame de atraso na ativação do item "Za Warudo Coin"
int frameTECLAS = 0; //Frame para troca de imagem dos sprites de teclas, uma animação simples mostrando as teclas de controle dos jogadores quando o jogo é iniciado

//Variaveis para posição de inicio de desenho dos itens
int x_aleatorioITEM = 0, y_aleatorioITEM = 0;

//Variaveis de posicao
float xBola = 0, yBola = 0;
float xJogador1 = 0, yJogador1 = 0;
float xJogador2 = 0, yJogador2 = 0;

//Variavel para saber se um item já foi gerado no cenário ou não, caso não (estado = 0), então gera um novo item
int itemGERADO = 0;

int pausado = 0;

//Define se o jogo começou ou não, - O jogo é começado a partir do movimento de um dos jogadores
bool startGAME = false;

//Define o tamanho da janela que o jogo será executado
#define tamanhoJANELA_X 1024
#define tamanhoJANELA_Y 720

//Define o tamanho do jogador e da bola
int tamanhoJOGADOR1_Y = 350;
int tamanhoJOGADOR2_Y = 350;
int tamanhoJOGADOR_Y = 350;
#define tamanhoJOGADOR_X 37
#define tamanhoBOLA_Y 13
#define tamanhoBOLA_X 15

//Define tamanho X e Y do mundo do jogo
#define tamanhoMUNDO_X 1000
#define tamanhoMUNDO_Y 1000

//Define o aumento de velocidade da Bola nos eixos X e Y ao colidor com uma das barras dos jogadores
#define aumentoVEL_COLISAO 0.00006

//Define a velocidade inicial da Bola nos eixos X e Y
#define velocidadeBOLAi_X 0.0002
#define velocidadeBOLAi_Y 0.0009
#define incrementoVEL_XINICIAL 0.0002
#define incrementoVEL_YINICIAL 0.0009

//Variavel para armazenar toques totais no jogo
int toquesTotal = 0;

//Variavel utilizada para definir a velocidade com que a barra se movimenta no eixo Y
int velocidadeJogador = 60;

//Variavel utilizada para definir o maximo/minimo do eixo Y alcançado pelos jogadores
float posicaoMAX_Ychao = 925;
float posicaoMAX_Yteto = 700 - tamanhoJOGADOR_Y/2 + 50;

//Aumento total na veloccidade da bola nos eixos // velocidaEixoX = velocidaEixoX+ aumentoVEL_COLISAO
//Definido inicialmente com os respectivos valores
float velocidaEixoX = velocidadeBOLAi_X;
float velocidaEixoY= velocidadeBOLAi_Y;

//Variaveis utilizadas para movimentar os jogadores/barras
int teclaW = 0;
int teclaS = 0;
int teclaO = 0;
int teclaL = 0;

//Variavel para definir posicao X da muralha gerada a partir do item "AoT Coin"
float posicaoMURALHAx = tamanhoMUNDO_X - 100;

//Variavel utilizada para resetar o jogo
int reset = 0;

//Variavel utilizada para pausar o jogo
bool systemPAUSE = false;

//Variavel utilizada para fazer com que o computador "jogue" contra si mesmo/Testar mais rapidamente e facilmente o jogo
bool automata = false;

//Variaveis de placar de pontuação de cada um dos dois jogadores
int placar1 = 0;
int placar2 = 0;

//Variavel utilizada para validar gol/pontuação
bool gol = true;

//Se xBolaDirecao = 0, bola vai para esquerda, se xBolaDirecao = 1, bola vai para direita
bool xBolaDirecao = false;

//Se yBolaDirecao = 0, bola vai para baixo, se yBolaDirecao = 1, bola vai para cima
bool yBolaDirecao = false;

//Zona de colisão do eixo X da bola com o jogador - Já que o eixo X dos jogadores são constantes
int zonaCOLISAO_X = tamanhoMUNDO_X - 2*tamanhoJOGADOR_X + 20;

//Variaveis de estado para cada uma das muralhas, para saber se foi gerada no cenário ou não
int muralhaESQUERDA = 0;
int muralhaDIREITA = 0;

//Variaveis de texturas utilizadas no jogo
GLuint idTexturaBackground01, idTexturaPlayer1, idTexturaPlayer2, idTexturaBola, idTexturaChao;
GLuint idTexturaStatusBar, idTexturaJogador1Text, idTexturaJogador2Text, idTexturaCoroa, idTexturaToques;
GLuint idTexturaTwiceCOIN, idTexturaTwiceBackground, idTexturaAotCOIN, idTexturaMuralhaESQ, idTexturaMuralhaDIR;
GLuint idTexturaAotBackground, idTexturaTelaINICIO, idTexturaTelaPausar, idTexturaTelaSair, idTexturaTelaCreditos;
GLuint idTexturaNUVEMS, idTexturaSpriteNUMBER, idTexturaMirroWorld, idTexturaReverseCOIN, idTexturaMirroCOIN, idTexturaPlayer1espelho;
GLuint idTexturaPlayer2espelho, idTexturaZaWarudoCOIN, idTexturaNUVEMStelaJogo, idTexturaTelaReset, idTexturaTeclaW, idTexturaTeclaS;
GLuint idTexturaTeclaO, idTexturaTeclaL, idTexturaTelaWINp1, idTexturaTelaWINp2, idTexturaZaWarudoBackground;

GLuint carregaTextura(const char* arquivo) {
  GLuint idTextura = SOIL_load_OGL_texture(
    arquivo,
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );

  if (idTextura == 0) {
    printf("Erro do SOIL: '%s'\n", SOIL_last_result());
  }
  return idTextura;
}

void somCLIQUE(){
  mouseCLICK.setVolume(80);
  mouseCLICK.play();
}

void colisaoCB(){
  colisaoCimaBaixo.setVolume(5.f);
  colisaoCimaBaixo.play();
}

void telaJOGARsom(){
  background_music.setLoop(true);
  background_music.setVolume(100);
  background_music.play();
  background_musicORIGINAL.setLoop(true);
  background_musicORIGINAL.setVolume(0);
  background_musicORIGINAL.play();
}

void telaCREDsom(){
  fancyU.stop();
  breakthrough.setLoop(true);
  breakthrough.setVolume(100);
  breakthrough.play();
}

void desenharPlacar(){
  switch(placar1 / 10){
    case 0:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);

    glTexCoord2f( 0.1, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);

    glTexCoord2f( 0.1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);

    glTexCoord2f(0, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85),  800,  0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 1:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.1 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.2, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 2:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.2 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.3, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 3:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.3 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.4, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 4:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.4 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.5, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 5:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.5 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.6, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 6:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.6 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.7, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 7:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.7 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.8, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 8:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 9:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.9 , 0);
    glVertex3f( (-tamanhoMUNDO_X + 85), 700,  0);
    glTexCoord2f(1, 0);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50, 700,  0);
    glTexCoord2f(1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) + 50,  800,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (-tamanhoMUNDO_X + 85) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

  }
  switch(placar1 % 10){
    case 0:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0, 0);
    glVertex3f( -tamanhoMUNDO_X + 140, 700,  0);

    glTexCoord2f(0.1, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);

    glTexCoord2f(0.1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);

    glTexCoord2f(0, 1);
    glVertex3f( -tamanhoMUNDO_X + 140,  800,  0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 1:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.1, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.2, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 2:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.2, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.3, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 800,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140), 800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 3:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.3, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.4, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 4:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.4, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.5, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 5:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.5, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.6, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 6:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.6, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.7, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 7:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.7, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.8, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 8:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 9:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.9, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140), 700,  0);
    glTexCoord2f(1, 0);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50, 700,  0);
    glTexCoord2f(1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140) + 50,  800,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (-tamanhoMUNDO_X + 140),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;
  }

  switch(placar2 / 10){
    case 0:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.1, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);

    glTexCoord2f( 0, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);

    glTexCoord2f( 0, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);

    glTexCoord2f(0.1, 1);
    glVertex3f( (tamanhoMUNDO_X - 140),  800,  0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 1:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.2, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.1, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 2:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.3, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.2, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 3:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.4, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.3, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 4:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.5, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.4, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 5:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.6, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.5, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 6:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.7, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.6, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 7:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.7, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 8:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.9, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.8, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 9:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(1, 0);
    glVertex3f( (tamanhoMUNDO_X - 140), 700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) - 50,  800,  0);
    glTexCoord2f(1, 1);
    glVertex3f( (tamanhoMUNDO_X - 140) ,  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;
  }
  switch(placar2 % 10){
    case 0:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.1, 0);
    glVertex3f( tamanhoMUNDO_X - 85, 700,  0);

    glTexCoord2f( 0, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);

    glTexCoord2f( 0, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);

    glTexCoord2f(0.1, 1);
    glVertex3f( tamanhoMUNDO_X - 85,  800,  0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 1:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.2, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.1, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 2:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.3, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.2, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 800,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (tamanhoMUNDO_X - 85), 800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 3:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.4, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.3, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 4:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.5, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.4, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 5:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.6, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.5, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 6:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.7, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.6, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 7:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.7, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 8:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.9, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.8, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 9:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(1, 0);
    glVertex3f( (tamanhoMUNDO_X - 85), 700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( (tamanhoMUNDO_X - 85) - 50,  800,  0);
    glTexCoord2f(1, 1);
    glVertex3f( (tamanhoMUNDO_X - 85),  800,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;
  }

}

void toquesTOTAISplacar(){
  switch (toquesTotal / 100){
    case 0:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0, 0);
    glVertex3f( -130, 700,  0);

    glTexCoord2f( 0.1, 0);
    glVertex3f( -70, 700,  0);

    glTexCoord2f( 0.1, 1);
    glVertex3f( -70,  820,  0);

    glTexCoord2f(0, 1);
    glVertex3f( -130,  820,  0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 1:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.1, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.2, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f( -130 ,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 2:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.2, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.3, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f(-70, 820,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( -130, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 3:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.3, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.4, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( -130,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 4:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.4, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.5, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( -130,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 5:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.5, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.6, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( -130,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 6:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.6, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.7, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( -130,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 7:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.7, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.8, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( -130,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 8:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( -130,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 9:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.9, 0);
    glVertex3f( -130, 700,  0);
    glTexCoord2f(1, 0);
    glVertex3f(-70, 700,  0);
    glTexCoord2f(1, 1);
    glVertex3f(-70,  820,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( -130,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;
  }


  switch(toquesTotal / 10){
    case 0:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0, 0);
    glVertex3f( -60, 700,  0);

    glTexCoord2f( 0.1, 0);
    glVertex3f( 0, 700,  0);

    glTexCoord2f( 0.1, 1);
    glVertex3f( 0,  820,  0);

    glTexCoord2f(0, 1);
    glVertex3f( -60,  820,  0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 1:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.1, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.2, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f( -60 ,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 2:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.2, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.3, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( 0, 820,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( -60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 3:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.3, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.4, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( -60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 4:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.4, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.5, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( -60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 5:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.5, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.6, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( -60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 6:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.6, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.7, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( -60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 7:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.7, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.8, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( -60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 8:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( -60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 9:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.9, 0);
    glVertex3f( -60, 700,  0);
    glTexCoord2f(1, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(1, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( -60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;
  }

  switch(toquesTotal % 10){
    case 0:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0, 0);
    glVertex3f(60,700,  0);
    glTexCoord2f(0.1, 0);
    glVertex3f(0, 700,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f(0,  820,  0);
    glTexCoord2f(0, 1);
    glVertex3f(60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 1:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.2, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.1, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.1, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( 60,  820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 2:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.3, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.2, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.2, 1);
    glVertex3f( 0, 820,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( 60,820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 3:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.4, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.3, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.3, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( 60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 4:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.5, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.4, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.4, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( 60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 5:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.6, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.5, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.5, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( 60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 6:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.7, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.6, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.6, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( 60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 7:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.7, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.7, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( 60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 8:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(0.8, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(0.8, 1);
    glVertex3f( 60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;

    case 9:
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaSpriteNUMBER);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada vértice
    glTexCoord2f(1, 0);
    glVertex3f( 60,700,  0);
    glTexCoord2f(0.9, 0);
    glVertex3f( 0, 700,  0);
    glTexCoord2f(0.9, 1);
    glVertex3f( 0,  820,  0);
    glTexCoord2f(1, 1);
    glVertex3f( 60, 820,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    break;
  }
}

void teclaWpress(int estado){
  if(estado == 1){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaW);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(-tamanhoMUNDO_X + 250, 100, 0);
    glTexCoord2f(0.47, 0);
    glVertex3f(-tamanhoMUNDO_X + 375, 100 ,0);
    glTexCoord2f(0.47, 1);
    glVertex3f(-tamanhoMUNDO_X + 375, 250, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-tamanhoMUNDO_X + 250, 250, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }

  if(estado == 2){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaW);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.48, 0);
    glVertex3f(-tamanhoMUNDO_X + 250, 100, 0);
    glTexCoord2f(1, 0);
    glVertex3f(-tamanhoMUNDO_X + 375, 100 ,0);
    glTexCoord2f(1, 1);
    glVertex3f(-tamanhoMUNDO_X + 375, 250, 0);
    glTexCoord2f(0.48, 1);
    glVertex3f(-tamanhoMUNDO_X + 250, 250, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }
}

void teclaSpress(int estado){
  if(estado == 1){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaS);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(-tamanhoMUNDO_X + 250, -100, 0);
    glTexCoord2f(0.47, 0);
    glVertex3f(-tamanhoMUNDO_X + 375, -100 ,0);
    glTexCoord2f(0.47, 1);
    glVertex3f(-tamanhoMUNDO_X + 375, 50, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-tamanhoMUNDO_X + 250, 50, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }

  if(estado == 2){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaS);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.48, 0);
    glVertex3f(-tamanhoMUNDO_X + 250, -100, 0);
    glTexCoord2f(1, 0);
    glVertex3f(-tamanhoMUNDO_X + 375, -100 ,0);
    glTexCoord2f(1, 1);
    glVertex3f(-tamanhoMUNDO_X + 375, 50, 0);
    glTexCoord2f(0.48, 1);
    glVertex3f(-tamanhoMUNDO_X + 250, 50, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }
}

void teclaOpress(int estado){
  if(estado == 1){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaO);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(tamanhoMUNDO_X - 375, 100, 0);
    glTexCoord2f(0.47, 0);
    glVertex3f(tamanhoMUNDO_X  - 250, 100 ,0);
    glTexCoord2f(0.47, 1);
    glVertex3f(tamanhoMUNDO_X - 250, 250, 0);
    glTexCoord2f(0, 1);
    glVertex3f(tamanhoMUNDO_X - 375, 250, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }

  if(estado == 2){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaO);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.48, 0);
    glVertex3f(tamanhoMUNDO_X - 375, 100, 0);
    glTexCoord2f(1, 0);
    glVertex3f(tamanhoMUNDO_X - 250, 100 ,0);
    glTexCoord2f(1, 1);
    glVertex3f(tamanhoMUNDO_X - 250, 250, 0);
    glTexCoord2f(0.48, 1);
    glVertex3f(tamanhoMUNDO_X - 375, 250, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }
}

void teclaLpress(int estado){
  if(estado == 1){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaL);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(tamanhoMUNDO_X - 375 , -100, 0);
    glTexCoord2f(0.47, 0);
    glVertex3f(tamanhoMUNDO_X - 250, -100 ,0);
    glTexCoord2f(0.47, 1);
    glVertex3f(tamanhoMUNDO_X - 250, 50, 0);
    glTexCoord2f(0, 1);
    glVertex3f(tamanhoMUNDO_X - 375, 50, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }

  if(estado == 2){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaTeclaL);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.48, 0);
    glVertex3f(tamanhoMUNDO_X - 375 , -100, 0);
    glTexCoord2f(1, 0);
    glVertex3f(tamanhoMUNDO_X - 250, -100 ,0);
    glTexCoord2f(1, 1);
    glVertex3f(tamanhoMUNDO_X - 250, 50, 0);
    glTexCoord2f(0.48, 1);
    glVertex3f(tamanhoMUNDO_X - 375, 50, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }
}

//Função para aleatorizar a habilidade, a posição e o tempo que será spawnado um item
void numsAleatoriosITEM(){
  srand(time(NULL));
  if(colisaoITEM == 0){
    if(itemGERADO == 0){
      if(itemSELECIONADO == 0)
      idITEMaleatorio = 1 + rand()%5;
      //1- twiceCOIN
      //2- Attack on Pong
      //3- Mirro World
      //4- Reverse Coin
      //5- Za Warudo Coin
      frameAleatorio = 50 + rand()%300;
      itemGERADO++;
    }

    if(frame == frameAleatorio){
      x_aleatorioITEM = -500 + rand()%1000;
      y_aleatorioITEM = -500 + rand()%1000;
      itemGERADO = 0;
      frame = 0;
    }
  }

  if(colisaoITEM == 1){
    //Valores apenas para tirar o item do mapa visivel
    x_aleatorioITEM = 3000;
    y_aleatorioITEM = 3000;
  }

  if(frame > 950)
    frame = 0;

}

void desenhaITEM(GLuint idTexturaITEM){
  if((x_aleatorioITEM != 0) and (y_aleatorioITEM != 0)){
    glColor3f(1.0f, 1.0f, 1.0f);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaITEM);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(x_aleatorioITEM, y_aleatorioITEM, 0);
    glTexCoord2f(1, 0);
    glVertex3f(x_aleatorioITEM + XtamITEM ,y_aleatorioITEM ,0);
    glTexCoord2f(1, 1);
    glVertex3f(x_aleatorioITEM + XtamITEM, y_aleatorioITEM + YtamITEM, 0);
    glTexCoord2f(0, 1);
    glVertex3f(x_aleatorioITEM, y_aleatorioITEM + YtamITEM, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  }
}

//Funcao utilizada para resetar o jogo
void resetar(){
  xBola = 0;
  yBola = 0;
  yJogador1 = 0;
  yJogador2 = 0;
  placar1 = 0;
  placar2 = 0;
  velocidaEixoX= velocidadeBOLAi_X;
  velocidaEixoY= velocidadeBOLAi_Y;
  reset = 0;
  automata = false;
  startGAME = false;
  if((telaJOGO == 1) && (telaINICIO == 0))
    telaJOGARsom();
  toquesTotal = 0;
  twiceToques = 0;
  muralhaToques = 0;
  espelhoToques = 0;
  itemGERADO = 0;
  frame = 0;
  colisaoITEM = 0;
  mirroWorldSOM.stop();
  espelhoCOINcollect.stop();
  sasageyo.stop();
  aotCOINcollect.stop();
  espelhoCOINcollect.stop();
  twiceCOINcollect.stop();
}

void desenhaPlayer1(GLuint texturaJOGADOR1){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);
  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, texturaJOGADOR1);

  glPushMatrix();
  glTranslatef(xJogador1, yJogador1, 0);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0, 0);
  glVertex3f(-tamanhoMUNDO_X + tamanhoJOGADOR_X/2, -tamanhoJOGADOR1_Y/2, 0);
  glTexCoord2f(1, 0);
  glVertex3f(-tamanhoMUNDO_X + tamanhoJOGADOR_X + tamanhoJOGADOR_X/2, -tamanhoJOGADOR1_Y/2, 0);
  glTexCoord2f(1, 1);
  glVertex3f(-tamanhoMUNDO_X + tamanhoJOGADOR_X + tamanhoJOGADOR_X/2, tamanhoJOGADOR1_Y/2, 0);
  glTexCoord2f(0, 1);
  glVertex3f(-tamanhoMUNDO_X + tamanhoJOGADOR_X/2, tamanhoJOGADOR1_Y/2, 0);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

//Funcao utilizada para definir a posicao do jogador 1 / Barra esquerda
void player1(){

  if(teclaW == 1){
    if(yJogador1 < posicaoMAX_Yteto){
      yJogador1 = yJogador1 + velocidadeJogador;
    }
    else{
      yJogador1 = posicaoMAX_Yteto - 50;
      teclaW = 0;
    }
  }

  if(teclaS == 1){
    if(yJogador1 > -posicaoMAX_Ychao + 144){
      yJogador1 = yJogador1 - velocidadeJogador;
    }
    else{
      yJogador1 = -posicaoMAX_Ychao + 144;
      teclaS = 0;
    }
  }

  if((colisaoITEM == 1) && (idITEMaleatorio == 3)){
    desenhaPlayer1(idTexturaPlayer1espelho);
  }
  else{
    desenhaPlayer1(idTexturaPlayer1);
  }

}

void desenhaPlayer2(GLuint texturaJOGADOR2){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, texturaJOGADOR2);

  glPushMatrix();
  glTranslatef(xJogador2, yJogador2, 0);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0, 0);
  glVertex3f(tamanhoMUNDO_X - tamanhoJOGADOR_X/2, -tamanhoJOGADOR2_Y/2, 0);
  glTexCoord2f(1, 0);
  glVertex3f(tamanhoMUNDO_X - tamanhoJOGADOR_X - tamanhoJOGADOR_X/2, -tamanhoJOGADOR2_Y/2, 0);
  glTexCoord2f(1, 1);
  glVertex3f(tamanhoMUNDO_X - tamanhoJOGADOR_X - tamanhoJOGADOR_X/2, tamanhoJOGADOR2_Y/2, 0);
  glTexCoord2f(0, 1);
  glVertex3f(tamanhoMUNDO_X - tamanhoJOGADOR_X/2, tamanhoJOGADOR2_Y/2, 0);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

//Funcao utilizada para definir a posicao do jogador 2 / Barra esquerda
void player2(){

  if(teclaO == 1){
    if(yJogador2 < posicaoMAX_Yteto){
      teclaO = 1;
      yJogador2 = yJogador2 + velocidadeJogador;
    }
    else{
      yJogador2 = posicaoMAX_Yteto;
      teclaO = 0;
    }
  }

  if(teclaL == 1){
    if(yJogador2 > -posicaoMAX_Ychao + 144){
      teclaL = 1;
      yJogador2 = yJogador2 - velocidadeJogador;
    }
    else{
      yJogador2 = -posicaoMAX_Ychao + 144;
      teclaL = 0;
    }
  }

  if((colisaoITEM == 1) && (idITEMaleatorio == 3)){
    desenhaPlayer2(idTexturaPlayer2espelho);
  }
  else{
    desenhaPlayer2(idTexturaPlayer2);
  }

}

//Funcao utilizada para desenhar a bola
void ball(){
  glColor3f(1.0f, 1.0f, 1.0f);
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaBola);

  glPushMatrix();
  glTranslatef(xBola, yBola, 0);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0, 0);
  glVertex3f(-2*tamanhoBOLA_X, -3*tamanhoBOLA_Y, 0);
  glTexCoord2f(1, 0);
  glVertex3f(2*tamanhoBOLA_X, -3*tamanhoBOLA_Y, 0);
  glTexCoord2f(1, 1);
  glVertex3f(2*tamanhoBOLA_X, 3*tamanhoBOLA_Y, 0);
  glTexCoord2f(0, 1);
  glVertex3f(-2*tamanhoBOLA_X, 3*tamanhoBOLA_Y, 0);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

void background(float xF){
  glColor3f (1, 1, 1);

  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaBackground01);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f(-tamanhoMUNDO_X + xF, -600,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X + xF, -600,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X + xF,  tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f(-tamanhoMUNDO_X + xF,  tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void chaoBGcolor(){
  glColor3f(0.08235f, 0.08627f, 0.1647f);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f(-tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X,  -600,  0);

  glTexCoord2f(0, 1);
  glVertex3f(-tamanhoMUNDO_X,  -600,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void nuvemMOV(float x){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaNUVEMStelaJogo);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f(-tamanhoMUNDO_X + x, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X + x, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X + x,  tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f(-tamanhoMUNDO_X + x,  tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void chao(float x){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaChao);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0.01, 0);
  glVertex3f(-tamanhoMUNDO_X + x, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1 - 0.01, 0);
  glVertex3f( tamanhoMUNDO_X + x, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1 - 0.01, 1);
  glVertex3f( tamanhoMUNDO_X + x,  -600,  0);

  glTexCoord2f(0.01, 1);
  glVertex3f(-tamanhoMUNDO_X + x,  -600,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void statusBar(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaStatusBar);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f(-tamanhoMUNDO_X, -tamanhoMUNDO_Y + 320,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y + 320,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X,  tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f(-tamanhoMUNDO_X,  tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  //pontuacao(placar1,true, 10);
  //pontuacao(placar2,false, 10);

}

void linhaSeparacao(){
  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(1.3f);
  glBegin(GL_LINES);
  glVertex2f(0, tamanhoMUNDO_Y);
  glVertex2f(0, -tamanhoMUNDO_X);
  glEnd();
}

void jogador1text(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaJogador1Text);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  //Faz com que a escrita de "JOGADOR 1" fique maior que "JOGADOR 2", caso jogador 1 esteja ganhando
  if(placar1 > placar2){
    glTexCoord2f(0, 0);
    glVertex3f(-tamanhoMUNDO_X + 50, 820,  0);

    glTexCoord2f(1, 0);
    glVertex3f( (-tamanhoMUNDO_X + 50) + 310, 820,  0);

    glTexCoord2f(1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 50) + 310,  910,  0);

    glTexCoord2f(0, 1);
    glVertex3f( -tamanhoMUNDO_X + 50,  910,  0);
  }

  //Faz com que a escrita de "JOGADOR 1" fique do mesmo tamanho de "JOGADOR 2", caso esteja empatado ou perdendo
  else if((placar1 < placar2) or (placar1 == placar2)){
    glTexCoord2f(0, 0);
    glVertex3f(-tamanhoMUNDO_X + 50, 820,  0);

    glTexCoord2f(1, 0);
    glVertex3f( (-tamanhoMUNDO_X + 50) + 220, 820,  0);

    glTexCoord2f(1, 1);
    glVertex3f( (-tamanhoMUNDO_X + 50) + 220,  890,  0);

    glTexCoord2f(0, 1);
    glVertex3f( -tamanhoMUNDO_X + 50,  890,  0);
  }

  glEnd();
  glDisable(GL_TEXTURE_2D);

  //Desenha Coroa no jogador que está ganhando
  if(placar1 > placar2){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaCoroa);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f( ((-tamanhoMUNDO_X + 50) + 310) + 30, 820,  0);

    glTexCoord2f(1, 0);
    glVertex3f( ((-tamanhoMUNDO_X + 50) + 310) + 115, 820,  0);

    glTexCoord2f(1, 1);
    glVertex3f( ((-tamanhoMUNDO_X + 50) + 310) + 115,  915,  0);

    glTexCoord2f(0, 1);
    glVertex3f( ((-tamanhoMUNDO_X + 50) + 310) + 30,  915,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }

}

void jogador2text(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaJogador2Text);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  if(placar2 > placar1){
    glTexCoord2f(0, 0);
    glVertex3f((tamanhoMUNDO_X - 50) - 310, 820,  0);

    glTexCoord2f(1, 0);
    glVertex3f( tamanhoMUNDO_X -50 , 820,  0);

    glTexCoord2f(1, 1);
    glVertex3f( tamanhoMUNDO_X -50,  910,  0);

    glTexCoord2f(0, 1);
    glVertex3f((tamanhoMUNDO_X - 50) - 310,  910,  0);
  }

  else if((placar2 < placar1) or (placar2 == placar1)){
    glTexCoord2f(0, 0);
    glVertex3f((tamanhoMUNDO_X - 50) - 220, 820,  0);

    glTexCoord2f(1, 0);
    glVertex3f( tamanhoMUNDO_X -50 , 820,  0);

    glTexCoord2f(1, 1);
    glVertex3f( tamanhoMUNDO_X -50,  890,  0);

    glTexCoord2f(0, 1);
    glVertex3f((tamanhoMUNDO_X - 50) - 220,  890,  0);
  }

  glEnd();
  glDisable(GL_TEXTURE_2D);

  if(placar2 > placar1){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaCoroa);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f( ((tamanhoMUNDO_X - 50) - 310) - 115, 820,  0);

    glTexCoord2f(1, 0);
    glVertex3f( ((tamanhoMUNDO_X - 50) - 310) - 30 , 820,  0);

    glTexCoord2f(1, 1);
    glVertex3f( ((tamanhoMUNDO_X - 50) - 310) - 30,  915,  0);

    glTexCoord2f(0, 1);
    glVertex3f(((tamanhoMUNDO_X - 50) - 310) - 115,  915,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }

}

void toquesTT(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaToques);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0, 0);
  glVertex3f(-250, 850,  0);

  glTexCoord2f(1, 0);
  glVertex3f( 190, 850,  0);

  glTexCoord2f(1, 1);
  glVertex3f( 190,  920,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -250,  920,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void vitoria(GLuint idTexturaWIN){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaWIN);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f(-tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X,  tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f(-tamanhoMUNDO_X,  tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void itemFundo(GLuint idTexturaFundo){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f(-tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X,  tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f(-tamanhoMUNDO_X,  tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void desenhaMuralhaESQ(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaMuralhaESQ);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f(-posicaoMURALHAx, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( -posicaoMURALHAx + 500, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( -posicaoMURALHAx + 500 , tamanhoMUNDO_Y - 200,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -posicaoMURALHAx, tamanhoMUNDO_Y - 200,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void desenhaMuralhaDIR(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaMuralhaDIR);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f( posicaoMURALHAx - 500, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( posicaoMURALHAx, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( posicaoMURALHAx,  tamanhoMUNDO_Y - 200 ,  0);

  glTexCoord2f(0, 1);
  glVertex3f( posicaoMURALHAx - 500,  tamanhoMUNDO_Y - 200,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void backgroundINICIO(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaTelaINICIO);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f( -tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void desenhaNUVEM(float xNUV){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaNUVEMS);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f( -tamanhoMUNDO_X + xNUV, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X + xNUV, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X + xNUV, tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -tamanhoMUNDO_X + xNUV, tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void backgroundSAIR(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaTelaSair);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f( -tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void backgroundPAUSA(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaTelaPausar);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f( -tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void backgroundTELAS(GLuint textura){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, textura);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f( -tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void backgroundRESET(){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaTelaReset);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(0, 0);
  glVertex3f( -tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(1, 1);
  glVertex3f( tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);

  glTexCoord2f(0, 1);
  glVertex3f( -tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void backgroundZaWarudo(float x){
  // Habilita o uso de texturas
  glEnable(GL_TEXTURE_2D);

  // Começa a usar a textura que criamos
  glBindTexture(GL_TEXTURE_2D, idTexturaZaWarudoBackground);
  glBegin(GL_TRIANGLE_FAN);
  // Associamos um canto da textura para cada vértice
  glTexCoord2f(x, 0);
  glVertex3f( -tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(x + 0.090909090, 0);
  glVertex3f( tamanhoMUNDO_X, -tamanhoMUNDO_Y,  0);

  glTexCoord2f(x + 0.090909090, 1);
  glVertex3f( tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);

  glTexCoord2f(x, 1);
  glVertex3f( -tamanhoMUNDO_X, tamanhoMUNDO_Y,  0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

//Funcao utilizada para desenhar todo o jogo, chamando as respectivas funcoes de desenho (ex : player1(), player2(), bola())
void desenha(){
  if(telaINICIO == 1){
    backgroundINICIO();
    desenhaNUVEM(xNuvem2);
    desenhaNUVEM(xNuvem);
    background_music.setVolume(0);
    breakthrough.setVolume(0);
    fancyU.setVolume(0);
    cheerUP.setVolume(90);
  }

  if(telaCREDITOS == 1){
    cheerUP.setVolume(0);
    backgroundTELAS(idTexturaTelaCreditos);
    if(tocarMUSICAcreditos == 0){
      telaCREDsom();
      tocarMUSICAcreditos++;
    }
  }

  if((telaJOGO == 1) && (telaINICIO == 0) && (telaCREDITOS == 0)){
    cheerUP.setVolume(0);
    if(colisaoITEM == 0){
      if(tocarMUSICA == 0){
        telaJOGARsom();
        tocarMUSICA++;
      }
      background_music.setVolume(100);
    }

    if(colisaoITEM == 1)
    background_music.setVolume(0);

    if((systemPAUSE == true) && (telaRESET == 0) && (telaWIN == 0)){
      telaPAUSA = 1;
      if(colisaoITEM == 1){
        if(idITEMaleatorio == 1){
          background_musicORIGINAL.setVolume(0);
        }
        if(idITEMaleatorio == 2){
          sasageyo.setVolume(0);
        }
        if(idITEMaleatorio == 3){
          mirroWorldSOM.setVolume(0);
        }

        pausado = 0;
      }
    }

    //Se o jogo não está pausado entao desenha/ atualiza
    if(systemPAUSE == false){
      telaPAUSA = 0;
      if(pausado == 0){
        if(colisaoITEM == 1){
          //Se houve colisão com algum item, então uma música respectiva ao item começa a ser tocada
          if(idITEMaleatorio == 1){
            background_music.setVolume(0);
            background_musicORIGINAL.setVolume(100);
          }

          if(idITEMaleatorio == 2)
          sasageyo.setVolume(100);

          if(idITEMaleatorio == 3)
          mirroWorldSOM.setVolume(100);

        }
        pausado++;
      }

      //Se o modo "automata" ligar, então a posição Y das barras dos jogadores será tal que as barras sempre irão acompanhar a posição Y da bola, e com isso colidir com ela
      //#IA
      if(automata == true){
        yJogador1 = yBola;
        yJogador2 = yBola;
      }

      // Limpa a tela para poder desenhar
      glClear(GL_COLOR_BUFFER_BIT);
      chaoBGcolor();
      glColor3f(1.0f, 1.0f, 1.0f);
      //Movimentação das nuvems ao fundo
      nuvemMOV(nuvemXjogo);
      nuvemMOV(nuvemXjogo2);
      //Movimentação da cidade ao fundo
      background(xFundo2);
      background(xFundo);
      //Movimentação do chão
      chao(xChao);
      chao(xChao2);
      glColor3f(1.0f, 1.0f, 1.0f);
      //Se o item aleatorizado for o item 1, então ele é desenhado na tela
      if(idITEMaleatorio == 1){
        desenhaITEM(idTexturaTwiceCOIN);
        //Se houver colisão com o item, então o cenário é trocado para um novo referente ao item
        if(colisaoITEM == 1){
          itemFundo(idTexturaTwiceBackground);
        }
      }

      //Se o item aleatorizado for o item 2, então ele é desenhado na tela
      if(idITEMaleatorio == 2){
        desenhaITEM(idTexturaAotCOIN);
        //Se houver colisão com o item, então o cenário é trocado para um novo referente ao item
        if(colisaoITEM == 1){
          itemFundo(idTexturaAotBackground);
          //A analise de quem deu o ultimo toque é feitar a partir da analise da direção da bola
          if(xBolaDIRECAOaot == 1){
            muralhaDIREITA = 0;
            desenhaMuralhaESQ();
            muralhaESQUERDA = 1;
          }
          if(xBolaDIRECAOaot == 0){
            muralhaESQUERDA= 0;
            desenhaMuralhaDIR();
            muralhaDIREITA = 1;
          }
        }
      }

      //Se o item aleatorizado for o item 3, então ele é desenhado na tela
      if(idITEMaleatorio == 3){
        desenhaITEM(idTexturaMirroCOIN);
        //Se houver colisão com o item, então o cenário é trocado para um novo referente ao item
        if(colisaoITEM == 1)
          itemFundo(idTexturaMirroWorld);
      }

      //Se o item aleatorizado for o item 4, então ele é desenhado na tela
      if(idITEMaleatorio == 4)
        desenhaITEM(idTexturaReverseCOIN);

      if(idITEMaleatorio == 5){
        desenhaITEM(idTexturaZaWarudoCOIN);
        if(colisaoITEM == 1){
          //Animação de background ao colidir com o item 5 - "Za Warudo Coin"
          if(frameZaWarudo > atraso+40)
          backgroundZaWarudo(10*0.090909090);
          if((frameZaWarudo > atraso+36) && (frameZaWarudo <= atraso+40))
          backgroundZaWarudo(9*0.090909090);
          if((frameZaWarudo > atraso+32) && (frameZaWarudo <= atraso+36))
          backgroundZaWarudo(8*0.090909090);
          if((frameZaWarudo > atraso+28) && (frameZaWarudo <= atraso+32))
          backgroundZaWarudo(7*0.090909090);
          if((frameZaWarudo > atraso+24) && (frameZaWarudo <= atraso+28))
          backgroundZaWarudo(6*0.090909090);
          if((frameZaWarudo > atraso+20) && (frameZaWarudo <= atraso+24))
          backgroundZaWarudo(5*0.090909090);
          if((frameZaWarudo > atraso+16) && (frameZaWarudo <= atraso+20))
          backgroundZaWarudo(4*0.090909090);
          if((frameZaWarudo > atraso+12) && (frameZaWarudo <= atraso+16))
          backgroundZaWarudo(3*0.090909090);
          if((frameZaWarudo > atraso+8) && (frameZaWarudo <= atraso+12))
          backgroundZaWarudo(2*0.090909090);
          if((frameZaWarudo > atraso+4) && (frameZaWarudo <= atraso+8))
          backgroundZaWarudo(0.090909090);
          if((frameZaWarudo > atraso + 0) && (frameZaWarudo <= atraso + 4))
          backgroundZaWarudo(0);
        }
      }

      //Desenhar a barra supeior em que fica o placar
      statusBar();
      //Desenhar o jogador 1
      player1();
      //Desenhar o jogador 2
      player2();
      //Desenhar a bola
      ball();
      //Desenhar a imagem "Jogador 1" no placar
      jogador1text();
      //Desenhar a imagem "Jogador 2" no placar
      jogador2text();
      //Desenhar a imagem "Toques totais" no placar
      toquesTT();
      //Desenha os numeros dos Toques totais
      toquesTOTAISplacar();
      //Desenha o placar
      desenharPlacar();

      //Desenha as teclas mostrando os comandos de movimentação dos jogadores na primeira vez que a partida é iniciada
      if((startGAME == false) && (placar1 == 0) && (placar2 == 0)){
        if(frameTECLAS < 30){
          teclaWpress(1);
          teclaSpress(2);
          teclaOpress(2);
          teclaLpress(1);
        }

        if((frameTECLAS > 30) && (frameTECLAS < 60)){
          teclaWpress(2);
          teclaSpress(1);
          teclaOpress(1);
          teclaLpress(2);
        }

        if(frameTECLAS > 60){
          frameTECLAS = 0;
        }
      }

      if(telaRESET == 1){
        backgroundRESET();
        systemPAUSE = !systemPAUSE;
      }

      //Se a opção/botão de reset for pressionada, então o jogo é resetado para seus estados iniciais chamando a funcao resetar
      if(reset == 1)
      resetar();
      //Função para aleatorizar o item, o lugar e o tempo em que ele será spawnado
      numsAleatoriosITEM();
    }
  }

  if(telaWIN == 1){
    if(placar1 > placar2){
      systemPAUSE = true;
      vitoria(idTexturaTelaWINp1);
    }
    else if(placar2 > placar1){
      systemPAUSE = true;
      vitoria(idTexturaTelaWINp2);
    }
  }

  if(telaPAUSA == 1)
    backgroundPAUSA();

  if(telaSAIR == 1)
    backgroundSAIR();

  glutSwapBuffers();
}


//Inicia movimento da bola
void chuteInicial(){
  srand(time(NULL));
  int numeroRandom = 0;
  numeroRandom == rand() % 1000;
  //É sorteador um numero aleatorio que irá determinar para qual das 4 direções abaixo a bola irá sair
  if( (numeroRandom) < 250){
    xBola += incrementoVEL_XINICIAL;
    yBola -= incrementoVEL_YINICIAL;
    xBolaDirecao = 1;
    yBolaDirecao = 0;
  }
  else if( (numeroRandom) >= 250 && (numeroRandom) < 500){
    xBola += incrementoVEL_XINICIAL;
    yBola += incrementoVEL_YINICIAL;
    xBolaDirecao = 1;
    yBolaDirecao = 1;
  }
  else if( (numeroRandom) >= 500 && (numeroRandom) < 750){
    xBola -= incrementoVEL_XINICIAL;
    yBola -= incrementoVEL_YINICIAL;
    xBolaDirecao = 0;
    yBolaDirecao = 0;
  }
  else if( (numeroRandom) >= 750 && (numeroRandom) < 1000){
    xBola -= incrementoVEL_XINICIAL;
    yBola += incrementoVEL_YINICIAL;
    xBolaDirecao = 0;
    yBolaDirecao = 1;
  }
}

//Continua o movimento da bola e caso ocorra colisao nas barras/teto/chão
void outrosChutes(){
  if ( xBolaDirecao == 1 && yBolaDirecao == 0 ){
    xBola += velocidaEixoX;
    yBola -= velocidaEixoY;
  }
  else if ( xBolaDirecao == 1 && yBolaDirecao == 1 ){
    xBola += velocidaEixoX;
    yBola += velocidaEixoY;
  }
  else if ( xBolaDirecao == 0 && yBolaDirecao == 0 ){
    xBola -= velocidaEixoX;
    yBola -= velocidaEixoY;
  }
  else if ( xBolaDirecao == 0 && yBolaDirecao == 1 ){
    xBola -= velocidaEixoX;
    yBola += velocidaEixoY;
  }

  //Colisão com jogador 1
  //Se entrou na zona de colisao do eixo X analisa se o Y da bola é igual ao Y do jogador
  //Se sim, então há a colisão invertendo o sentido da direção X da bola
  if(xBola <= -zonaCOLISAO_X){
    if((yBola <= (yJogador1 + tamanhoJOGADOR1_Y/2 + tamanhoBOLA_Y / 2)) && (yBola >= (yJogador1 - tamanhoJOGADOR1_Y / 2 - tamanhoBOLA_Y / 2))){
      //Se o jogador 2 pegou o item 3 - Mirro Coin
      //Então a bola não irá ser rebatida pelo jogador 1, e sim ser teletransportada do jogador 1 ao jogador 2 na colisão
      if((idITEMaleatorio == 3) && (colisaoITEM == 1)){
        if(xBolaDirecao == 0){
          xBola = -tamanhoMUNDO_X + tamanhoJOGADOR_X/2 + 1900;
          yBola = yJogador2;
          espelhoCOINcollect.setVolume(100);
          espelhoCOINcollect.play();
          espelhoToques++;
        }
        if(xBolaDirecao == 1){
          //Muda de direção ao colidir com a barra do jogador 1
          xBolaDirecao = !xBolaDirecao;
          colisaoPlayer1.setVolume(28.f);
          colisaoPlayer1.play();
          toquesTotal++;
        }
      }

      else{
        //Muda de direção ao colidir com a barra do jogador 1
        xBolaDirecao = !xBolaDirecao;
        colisaoPlayer1.setVolume(28.f);
        colisaoPlayer1.play();
        toquesTotal++;
        //Aumenta velocidade ao colidir na barra do jogador 1
        velocidaEixoX= velocidaEixoX+ aumentoVEL_COLISAO;
        velocidaEixoY= velocidaEixoY+ aumentoVEL_COLISAO;
        //Contador para limitar o efeito do item 1 - TWICE coin
        if(idITEMaleatorio == 1){
          if(colisaoITEM == 1)
          twiceToques++;
        }
      }

    }
  }


  //Colisão com jogador 2
  //Se entrou na zona de colisao do eixo X analisa se o Y da bola é igual ao Y do jogador
  //Se sim, então há a colisão invertendo o sentido da direção X da bola
  if(xBola >= zonaCOLISAO_X){
    if((yBola <= (yJogador2 + tamanhoJOGADOR1_Y/2 + tamanhoBOLA_Y / 2)) && (yBola >= (yJogador2 - tamanhoJOGADOR1_Y / 2 - tamanhoBOLA_Y / 2))){
      //Se o jogador 1 pegou o item 3 - Mirro Coin
      //Então a bola não irá ser rebatida pelo jogador 2, e sim ser teletransportada do jogador 2 ao jogador 1 na colisão
      if((idITEMaleatorio == 3) && (colisaoITEM == 1)){
        if(xBolaDirecao == 1){
          xBola = -tamanhoMUNDO_X + tamanhoJOGADOR_X + tamanhoJOGADOR_X/2 + 30;
          yBola = yJogador1;
          espelhoCOINcollect.setVolume(100);
          espelhoCOINcollect.play();
          espelhoToques++;
        }
      }

      else{
        //Muda de direção ao colidir com a barra do jogador 2
        xBolaDirecao = !xBolaDirecao;
        colisaoPlayer2.setVolume(28.f);
        colisaoPlayer2.play();
        toquesTotal++;
        //Aumenta velocidade ao colidir na barra do jogador 2
        velocidaEixoX= velocidaEixoX+ aumentoVEL_COLISAO;
        velocidaEixoY= velocidaEixoY+ aumentoVEL_COLISAO;
        //Contador para limitar o efeito do item 1 - TWICE coin
        if(idITEMaleatorio == 1){
          if(colisaoITEM == 1)
          twiceToques++;
        }
      }
    }
  }

  //Colisão com qualquer item do cenário
  if((x_aleatorioITEM != 0) && (y_aleatorioITEM != 0)){
    if(colisaoITEM == 0){
      //Se a posição X da bola está entre a Area X do item
      if((xBola <= x_aleatorioITEM + XtamITEM + 10) && (xBola >= x_aleatorioITEM - 10 )) {
        //Se a posição Y da bola está entre a Area Y do item
        if((yBola <= (y_aleatorioITEM + YtamITEM + 10)) && (yBola >= y_aleatorioITEM - 10)){
          //Colisão com item 1 - "TWICE Coin"
          if(idITEMaleatorio == 1){
            colisaoITEM = 1;
            twiceCOINcollect.setVolume(60);
            twiceCOINcollect.play();
            background_music.setVolume(0);
            background_musicORIGINAL.setVolume(100);
          }

          //Colisão com item 2- "AoT Coin"
          if(idITEMaleatorio == 2){
            xBolaDIRECAOaot = xBolaDirecao;
            colisaoITEM = 1;
            background_music.setVolume(0);
            aotCOINcollect.setVolume(80);
            aotCOINcollect.play();
            sasageyo.setVolume(70);
            sasageyo.play();
          }

          //Colisão com item 3- "Mirro Coin"
          if(idITEMaleatorio == 3){
            background_music.setVolume(0);
            espelhoCOINcollect.setVolume(100);
            espelhoCOINcollect.play();
            mirroWorldSOM.setLoop(true);
            mirroWorldSOM.setVolume(85);
            mirroWorldSOM.play();
            colisaoITEM = 1;
          }

          //Colisão com item 4- "Reveser Coin"
          if(idITEMaleatorio == 4){
            reverseCOINcollect.play();
            if(xBolaDirecao == 0){
              xBolaDirecao = 1;
              yBolaDirecao = !yBolaDirecao;
              colisaoITEM = 0;
              itemGERADO = 0;
              background_music.setVolume(100);
              x_aleatorioITEM = 3000;
              y_aleatorioITEM = 3000;
            }
            else{
              xBolaDirecao = 0;
              yBolaDirecao = !yBolaDirecao;
              colisaoITEM = 0;
              itemGERADO = 0;
              background_music.setVolume(100);
              x_aleatorioITEM = 3000;
              y_aleatorioITEM = 3000;
            }
            colisaoITEM = 1;
            colisaoITEM = 0;
          }

          //Colisão com item 5- "Za Warudo Coin"
          if(idITEMaleatorio == 5){
            colisaoITEM = 1;
            zawarudoCOINcollect.setVolume(100);
            zawarudoCOINcollect.play();
            background_music.setVolume(0);
          }

        }
      }
    }
  }

  //Lógica utilizada para deixar a bola e os jogadores em "camera lenta" quando o efeito do item 5 - "Za Warudo Coin" é ativado
  if(frameZaWarudo >= 125){
    velocidaEixoX= 0.00006;
    velocidaEixoY= 0.0001;
    velocidadeJogador = 20;
  }

  //Colisão com a muralha
  if(idITEMaleatorio == 2){
    if(colisaoITEM == 1){

      if(muralhaESQUERDA == 1){
        //Colisao com muralha esquerda
        if(xBola <= -posicaoMURALHAx + 300 ){
          xBolaDirecao = 1;
          colisaoCimaBaixo.setVolume(58);
          colisaoCimaBaixo.play();
          toquesTotal++;
          muralhaToques++;
        }
      }

      if(muralhaDIREITA == 1){
        //Colisao com muralha direita
        if(xBola >= posicaoMURALHAx - 300){
          xBolaDirecao = 0;
          colisaoCimaBaixo.setVolume(58);
          colisaoCimaBaixo.play();
          toquesTotal++;
          muralhaToques++;
        }
      }

    }
  }

  //Lógica limitante do efeito do item 1
  if(twiceToques == 6){
    colisaoITEM = 0;
    itemGERADO = 0;
    twiceToques = 0;
    background_musicORIGINAL.setVolume(0);
    background_music.setVolume(100);
  }

  //Lógica limitante do efeito do item 2
  if(muralhaToques == 4){
    colisaoITEM = 0;
    itemGERADO = 0;
    muralhaToques = 0;
    sasageyo.stop();
    background_music.setVolume(100);
  }

  //Lógica limitante do efeito do item 3
  if(espelhoToques == 4){
    colisaoITEM = 0;
    itemGERADO = 0;
    espelhoToques = 0;
    mirroWorldSOM.stop();
    background_music.setVolume(100);
  }

  //Lógica limitante do efeito do item 5
  if(frameZaWarudo > 444){
    colisaoITEM = 0;
    itemGERADO = 0;
    velocidaEixoX= velocidadeBOLAi_X + 0.0002;
    velocidaEixoY= velocidadeBOLAi_Y + 0.0005;
    zawarudoCOINcollect.stop();
    frameZaWarudo = 0;
    velocidadeJogador = 60;
  }
}

//Função para analisar o placar, para saber quem está ganhando
void analiseDePLACAR(){
  if(placar1 > placar2){
    if((placar1 >= 10) && (placar2 >= 10)){
      if(placar1 - placar2 >= 2){
        telaWIN = 1;
      }
    }
    else if((placar1 >= 12) && (placar2 < 10)){
      telaWIN = 1;
    }
  }

  else if(placar2 > placar1){
    if((placar1 >= 10) && (placar2 >= 10)){
      if(placar2 - placar1 >= 2){
        telaWIN = 1;
      }
    }
    else if((placar2 >= 12) && (placar1 < 10)){
      telaWIN = 1;
    }
  }
}

//Função utilizada para atualizar a posição da bola caso tenho sido gol, ou caso tenha acontecido colisão com o teto ou o chão
void atualizaPosicaoBola(){
  if(systemPAUSE == false){
    //Gol jogador 2
    if(xBola < -tamanhoMUNDO_X){
      //Se algum item estiver com o efeito ativo, então suas musicas são interrompidas
      background_musicORIGINAL.setVolume(0);  //Musica do item 1
      sasageyo.stop();                  //Musica do item 2
      mirroWorldSOM.stop();             //Musica do item 3
      background_music.setVolume(100);
      //Aumento do placar para o jogador 2
      //Se o efeito do item 'Twice Coin' estiver ativo, então o gol é contado duas vezes (TWICE)
      if((idITEMaleatorio == 1) && (colisaoITEM == 1)){
        placar2 = placar2 + 2;
      }
      else{
        placar2++;
      }
      //Som/Efeito de gol
      goal.setVolume(60.f);
      goal.play();
      //Reseta a posição da bola
      xBola = 0;
      yBola = 0;
      gol = true;
      startGAME = false;
      //Reseta a velocidade da bola
      velocidaEixoX= velocidadeBOLAi_X;
      velocidaEixoY= velocidadeBOLAi_Y;
      colisaoITEM = 0;
      if(startGAME == true)
        chuteInicial();
      analiseDePLACAR();
    }

    //Gol jogador 1
    if(xBola > tamanhoMUNDO_X){
      //Se algum item estiver com o efeito ativo, então suas musicas são interrompidas
      background_musicORIGINAL.setVolume(0);  //Musica do item 1
      sasageyo.stop();                  //Musica do item 2
      mirroWorldSOM.stop();             //Musica do item 3
      background_music.setVolume(100);
      //Aumento do placar para o jogador 1
      //Se o efeito do item 'Twice Coin' estiver ativo, então o gol é contado duas vezes (TWICE)
      if((idITEMaleatorio == 1) && (colisaoITEM == 1)){
        placar1 = placar1 + 2;
      }
      else{
        placar1++;
      }
      //Som/Efeito de gol
      goal.setVolume(60.f);
      goal.play();
      //Reseta a posição da bola
      xBola = 0;
      yBola = 0;
      gol = true;
      startGAME = false;
      //Reseta a velocidade da bola
      velocidaEixoX= velocidadeBOLAi_X;
      velocidaEixoY= velocidadeBOLAi_Y;
      colisaoITEM = 0;
      if(startGAME == true)
        chuteInicial();
      analiseDePLACAR();
    }

    //Se a bola colidir com o teto, então a bola muda sua direção para baixo
    //yBolaDirecao = 0 -> Para baixo
    //yBolaDirecao = 1 -> Para cima
    else if (yBola > posicaoMAX_Yteto + 100){
      //Função para utilizar o som de colisão nas paredes de cima e baixo
      colisaoCB();
      yBolaDirecao = 0;
      if(startGAME == true)
        outrosChutes();
    }

    //Se a bola colidir com o chão, então a bola muda sua direção para cima
    else if (yBola < -tamanhoMUNDO_Y + 125){
      //Função para utilizar o som de colisão nas paredes de cima e baixo
      colisaoCB();
      yBolaDirecao = 1;

      if(startGAME == true)
        outrosChutes();
    }

    else{
      if(startGAME == true)
        outrosChutes();
    }
  }
}

//Função utilizada para redimensionar a tela de forma que o jogo fique no centro da tela
void redimensiona(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-tamanhoMUNDO_X, tamanhoMUNDO_X, -tamanhoMUNDO_Y, tamanhoMUNDO_Y, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

//Converte as posicoes para qualquer tamanho de tela
int coodX(int x) {
  return (glutGet(GLUT_WINDOW_WIDTH) * x) / 1024;
}

int coodY(int y) {
  return (glutGet(GLUT_WINDOW_HEIGHT) * y) / 720;
}

//Função utilizada para cliques do mouse nos menus e telas do jogo
void cliqueMOUSE(int button, int state,int x, int y){
  if(telaINICIO == 1){
    //Se está na tela de inicio, então o jogo é reiniciado
    resetar();
    //Para não tocar musica da tela de jogo / a musica ser sempre reiniciada ao voltar para a tela de inicio
    tocarMUSICA = 0;
    //Para não tocar musica da tela de créditos / a musica ser sempre reiniciada ao voltar para a tela de inicio
    tocarMUSICAcreditos = 0;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      //Botão 'INICIAR'
      if( ((x > coodX(15)) && (x < coodX(210))) && ((y < coodY(360)) && (y > coodY(310))) ){
        systemPAUSE = false;
        telaINICIO = 0;
        telaJOGO = 1;
        somCLIQUE();
      }
      //Botão 'SAIR'
      if( ((x > coodX(15)) && (x < coodX(210))) && ((y < coodY(450)) && (y > coodY(395))) ){
        telaSAIR = 1;
        somCLIQUE();
      }

      //Botão 'CRÉDITOS'
      if( ((x > coodX(290)) && (x < coodX(435))) && ((y < coodY(475)) && (y > coodY(300))) ){
        telaINICIO = 0;
        telaJOGO = 0;
        telaCREDITOS = 1;
        somCLIQUE();
      }
    }
  }

  if(telaSAIR == 1){
    telaPAUSA = 0;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      //Botão 'SIM'
      if(((x > coodX(375)) && (x < coodX(645))) && ((y < coodY(385)) && (y > coodY(315)))){
        somCLIQUE();
        exit(0);
      }

      //Botão 'NAO'
      if(((x > coodX(375)) && (x < coodX(645))) && ((y < coodY(470)) && (y > coodY(400)))){
        telaSAIR = 0;
        somCLIQUE();
      }
    }
  }

  if(telaRESET == 1){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      //Botão 'SIM'
      if(((x > coodX(375)) && (x < coodX(645))) && ((y < coodY(385)) && (y > coodY(315)))){
        somCLIQUE();
        resetar();
        telaRESET = 0;
        systemPAUSE = !systemPAUSE;
      }

      //Botão 'NAO'
      if(((x > coodX(375)) && (x < coodX(645))) && ((y < coodY(470)) && (y > coodY(400)))){
        telaRESET = 0;
        systemPAUSE = !systemPAUSE;
        somCLIQUE();
      }
    }
  }

  //Botões da tela de pausa
  if((telaPAUSA == 1) && (telaINICIO == 0) && (systemPAUSE == true) && (telaSAIR == 0) && (telaWIN == 0)){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      //Botão 'DESPAUSAR'
      if(((x > coodX(425)) && (x < coodX(585))) && ((y < coodY(370)) && (y > coodY(330)))){
        somCLIQUE();
        telaPAUSA = 0;
        systemPAUSE = !systemPAUSE;
      }

      //Botão 'INICIO'
      if(((x > coodX(425)) && (x < coodX(585))) && ((y < coodY(420)) && (y > coodY(380)))){
        somCLIQUE();
        telaPAUSA = 0;
        telaJOGO = 0;
        telaINICIO = 1;
      }

      //Botão 'SAIR'
      if(((x > coodX(425)) && (x < coodX(585))) && ((y < coodY(470)) && (y > coodY(430)))){
        somCLIQUE();
        telaPAUSA = 0;
        telaSAIR = 1;
      }
    }
  }

  //Botões da tela de vitória
  if(telaWIN == 1){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      //Botão 'REINICIAR'
      if(((x > coodX(375)) && (x < coodX(645))) && ((y < coodY(385)) && (y > coodY(315)))){
        somCLIQUE();
        resetar();
        telaRESET = 0;
        systemPAUSE = !systemPAUSE;
        telaWIN = 0;
      }

      //Botão 'INICIO'
      if(((x > coodX(375)) && (x < coodX(645))) && ((y < coodY(470)) && (y > coodY(400)))){
        somCLIQUE();
        systemPAUSE = !systemPAUSE;
        telaWIN = 0;
        telaJOGO = 0;
        telaINICIO = 1;
      }
    }
  }

  //Botões da tela de créditos
  if((telaCREDITOS == 1) && (telaINICIO == 0) && (telaJOGO == 0)){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
      //Botão 'VOLTAR'
      if(((x > coodX(0)) && (x < coodX(185))) && ((y < coodY(130)) && (y > coodY(80)))){
        somCLIQUE();
        telaJOGO = 0;
        telaCREDITOS= 0;
        telaINICIO = 1;
      }
      //Botão 'TWICE'
      if(((x > coodX(280)) && (x < coodX(725))) && ((y < coodY(710)) && (y > coodY(480)))){
        somCLIQUE();
        twiceCLIQUE++;

        if(twiceCLIQUE%2 != 0 ){
          fancyU.stop();
          breakthrough.setLoop(true);
          breakthrough.setVolume(100);
          breakthrough.play();
        }

        if(twiceCLIQUE%2 == 0){
          breakthrough.stop();
          fancyU.setVolume(100);
          fancyU.setLoop(true);
          fancyU.play();
        }

      }
    }
  }
}

//Função utilizada para movimentação dos jogadores e controle do estado de jogo a partir de comandos do teclado
void teclado(unsigned char key, int x, int y){
  switch (key){
    //Ao pressionar 'ESC' um menu para confirmação de saída é aberto
    case 27:
    telaSAIR = 1;
    break;

    //Se está na tela de jogo, então os comandos de movimentação podem ser utilizados
    if((telaJOGO == 1) && (telaINICIO == 0) && (telaCREDITOS == 0)){
      //Ao pressionar 'W' o jogador 1 se move para cima
      case 'w':
      teclaW = 1;
      startGAME = true;
      break;

      //Ao pressionar 'S' o jogador 1 se move para baixo
      case 's':
      teclaS = 1;
      startGAME = true;
      break;

      //Ao pressionar 'O' o jogador 2 se move para cima
      case 'o':
      teclaO = 1;
      startGAME = true;
      break;

      //Ao pressionar 'L' o jogador 2 se move para baixo
      case 'l':
      teclaL = 1;
      startGAME = true;
      break;

      //Ao pressionar 'P' o jogo é pausado até se pressionar 'P' novamente ou selecionar 'DESPAUSA' no menu de pausa
      case 'p':
      systemPAUSE = !systemPAUSE;
      break;

      //Ao pressionar 'R' o jogo é resetado para seus estados iniciais
      case 'r':
      telaRESET = 1;
      break;

      //Ao pressionar 'M' o computador joga contra si próprio (?IA super aprimorada?)
      case 'M':
      automata = !automata;
      break;

      case '1':
      if(colisaoITEM != 1){
        itemSELECIONADO = 1;
        idITEMaleatorio = 1;
      }
      break;

      case '2':
      if(colisaoITEM != 1){
        itemSELECIONADO = 1;
        idITEMaleatorio = 2;
      }
      break;

      case '3':
      if(colisaoITEM != 1){
        itemSELECIONADO = 1;
        idITEMaleatorio = 3;
      }
      break;

      case '4':
      if(colisaoITEM != 1){
        itemSELECIONADO = 1;
        idITEMaleatorio = 4;
      }
      break;

      case '5':
      if(colisaoITEM != 1){
        itemSELECIONADO = 1;
        idITEMaleatorio = 5;
      }
      break;

      case '0':
      if(colisaoITEM != 1){
        itemSELECIONADO = 0;
        idITEMaleatorio = 0;
      }
      break;

    }
  }
}

//Função utilizada em conjunto com a função 'teclado()' para que ambos jogadores consigam se movimentar ao mesmo tempo
void keyUP(unsigned char key, int x, int y){
  switch(key){
    case 'w':
    teclaW = 0;
    break;

    case 's':
    teclaS = 0;
    break;

    case 'o':
    teclaO = 0;
    break;

    case 'l':
    teclaL = 0;
    break;
  }
}

//Função utilizada para atualizar posição da bola
void atualiza(){
  //Se o jogo começou, isto é, se algum dos jogadores já pressionou alguma tecla de movimentação e o jogo se encontra na tela de jogo
  //Então a posição da bola é atualizada
  if(startGAME == true)    {
    if(telaJOGO == 1)
    atualizaPosicaoBola();
  }
}

void inicializarPrograma(){
  glClearColor(0.6627, 0.003921568627451, 0.2470, 1); //Cor vermelha fundo cidade (RGB (169,1,63)) -> (169/255, 1/255, 63/255)
  glEnable(GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //Atribuição das imagens ás variaveis
  idTexturaBackground01 = carregaTextura("img/backgrounds/backgroundTelaJogo.png");
  idTexturaPlayer1 = carregaTextura("img/hud/player1.png");
  idTexturaPlayer2 = carregaTextura("img/hud/player2.png");
  idTexturaPlayer1espelho = carregaTextura("img/hud/playerEspelho.png");
  idTexturaPlayer2espelho = carregaTextura("img/hud/playerEspelho.png");
  idTexturaBola = carregaTextura("img/hud/bola.png");
  idTexturaChao = carregaTextura("img/backgrounds/backgroundChao.png");
  idTexturaStatusBar = carregaTextura("img/hud/barraStatus.png");
  idTexturaJogador1Text = carregaTextura("img/hud/jogador1texto.png");
  idTexturaJogador2Text = carregaTextura("img/hud/jogador2texto.png");
  idTexturaCoroa = carregaTextura("img/hud/crown8bits.png");
  idTexturaToques = carregaTextura("img/hud/toquesTOTAL.png");
  idTexturaTwiceCOIN = carregaTextura("img/power-UP/twiceCOIN.png");
  idTexturaTwiceBackground = carregaTextura("img/power-UP/backgroundTWICE.png");
  idTexturaAotCOIN = carregaTextura("img/hud/asasDaLiberdade.png");
  idTexturaAotBackground = carregaTextura("img/power-UP/backgroundCOLOSSAL.png");
  idTexturaMuralhaDIR = carregaTextura("img/power-UP/muralhaDIR.png");
  idTexturaMuralhaESQ = carregaTextura("img/power-UP/muralhaESQ.png");
  idTexturaTelaINICIO =carregaTextura("img/telas/telaINICIO.png");
  idTexturaNUVEMS = carregaTextura("img/backgrounds/nuvem.png");
  idTexturaTelaSair = carregaTextura("img/telas/telaSAIR.png");
  idTexturaTelaPausar = carregaTextura("img/telas/telaPAUSAR.png");
  idTexturaTelaReset = carregaTextura("img/telas/telaConfirmarREINICIAR.png");
  idTexturaSpriteNUMBER = carregaTextura("img/hud/spriteNUMBER.png");
  idTexturaMirroWorld = carregaTextura("img/power-UP/mirroWORLDfundo.png");
  idTexturaTelaCreditos = carregaTextura("img/telas/telaCREDITOS.png");
  idTexturaReverseCOIN = carregaTextura("img/power-UP/reverseCOIN.png");
  idTexturaMirroCOIN = carregaTextura("img/power-UP/mirroCOIN.png");
  idTexturaZaWarudoCOIN = carregaTextura("img/power-UP/zawarudoCOIN.png");
  idTexturaNUVEMStelaJogo = carregaTextura("img/backgrounds/nuvemTELAJOGO.png");
  idTexturaTeclaW = carregaTextura("img/hud/teclaWpress.png");
  idTexturaTeclaS = carregaTextura("img/hud/teclaSpress.png");
  idTexturaTeclaO = carregaTextura("img/hud/teclaOpress.png");
  idTexturaTeclaL = carregaTextura("img/hud/teclaLpress.png");
  idTexturaTelaWINp1 = carregaTextura("img/telas/p1WIN.png");
  idTexturaTelaWINp2 = carregaTextura("img/telas/p2Win.png");
  idTexturaZaWarudoBackground = carregaTextura("img/power-UP/ZaWarudoSPRITE.png");
  chuteInicial();
}

void atualizaCena(int num){
  glutPostRedisplay();
  glutTimerFunc(17, atualizaCena, 0); // Por que 17 ?

  //Lógica utilizada para movimentação das nuvems ao fundo na tela de inicio
  xNuvem += 3;
  if(xNuvem > 2000)
  xNuvem = -2000;

  xNuvem2 += 3;
  if(xNuvem2 > 2000)
  xNuvem2 = -2000;

  //Lógica utilizada para movimentação do chão e da cidade ao fundo na tela de jogo
  if(telaJOGO == 1){

    xChao += 6.0;
    if(xChao > 1990)
      xChao = -1990;

    xChao2 += 6.0;
    if(xChao2 > 1990)
      xChao2 = -1990;

    xFundo += 1.0;
    if(xFundo > 2000)
      xFundo = -2000;

    xFundo2 += 1.0;
    if(xFundo2 > 2000)
      xFundo2 = -2000;

    nuvemXjogo += 0.3;
    if(nuvemXjogo > 2000)
      nuvemXjogo = -2000;

    nuvemXjogo2 += 0.3;
    if(nuvemXjogo2 > 2000)
      nuvemXjogo2 = -2000;

    frame++;
    frameTECLAS++;
    if(colisaoITEM == 1){
      if(idITEMaleatorio == 5){
        if(systemPAUSE != true)
          frameZaWarudo++;
      }
    }

  }
}



int main(int argc, char** argv){
  //Carregamento das musicas/sons utilizados no jogo, caso haja erro para abrir o arquivo, uma mensagem é mostrada
  if(!background_music.openFromFile("sounds/knock8Bits.ogg"))
  printf("\n ERRO AO CARREGAR Knock8Bits.ogg");

  if(!background_musicORIGINAL.openFromFile("sounds/knockORIGINAL.ogg"))
  printf("\n ERRO AO CARREGAR knockORIGINAL.ogg");

  if(!colisaoCimaBaixo.openFromFile("sounds/colisaoCimaBaixo.ogg"))
  printf("\n ERRO AO CARREGAR colisaoCimaBaixo.ogg");

  if(!colisaoPlayer1.openFromFile("sounds/colisaoPlayer1.ogg"))
  printf("\n ERRO AO CARREGAR colisaoPlayer1.ogg");

  if(!colisaoPlayer2.openFromFile("sounds/colisaoPlayer2.ogg"))
  printf("\n ERRO AO CARREGAR colisaoPlayer2.ogg");

  if(!goal.openFromFile("sounds/gol.ogg"))
  printf("\n ERRO AO CARREGAR gol.ogg");

  if(!twiceCOINcollect.openFromFile("sounds/twicecoinCOLLECT.ogg"))
  printf("\n ERRO AO CARREGAR twiceCOINcollect.ogg");

  if(!sasageyo.openFromFile("sounds/sasageyo.ogg"))
  printf("\n ERRO AO CARREGAR sasageyo.ogg");

  if(!aotCOINcollect.openFromFile("sounds/Jeager.ogg"))
  printf("\n ERRO AO CARREGAR aotCOINcollect.ogg");

  if(!cheerUP.openFromFile("sounds/cheerUP.ogg"))
  printf("\n ERRO AO CARREGAR cheerUP.ogg");

  if(!mouseCLICK.openFromFile("sounds/mouseCLICK.ogg"))
  printf("\n ERRO AO CARREGAR mouseCLICK.ogg");

  if(!breakthrough.openFromFile("sounds/breakthrough.ogg"))
  printf("\n ERRO AO CARREGAR breakthrough.ogg");

  if(!espelhoCOINcollect.openFromFile("sounds/mirroWorldCollect.ogg"))
  printf("\n ERRO AO CARREGAR mirroWorldCollect.ogg");

  if(!mirroWorldSOM.openFromFile("sounds/mirroWorldSOMbg.ogg"))
  printf("\n ERRO AO CARREGAR mirroWorldSOMbg.ogg");

  if(!reverseCOINcollect.openFromFile("sounds/reverseCOINsound.ogg"))
  printf("\n ERRO AO CARREGAR reverseCOINsound.ogg");

  if(!fancyU.openFromFile("sounds/fancyU.ogg"))
  printf("\n ERRO AO CARREGAR fancyU.ogg");

  if(!zawarudoCOINcollect.openFromFile("sounds/zawarudoCOINcollect.ogg"))
  printf("\n ERRO AO CARREGAR zawarudoCOINcollect.ogg");


  //Seta a música da tela de inicio para ficar em Loop caso termine e a inicia
  cheerUP.setLoop(true);
  cheerUP.play();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(tamanhoJANELA_X, tamanhoJANELA_Y);
  glutInitWindowPosition(0, 0);

  //트와이스 아딱 온 퐁;
  glutCreateWindow("duasVEZES attack on PONG");
  //MouseFunc utilizada para clique do mouse nos menus/telas
  glutMouseFunc(cliqueMOUSE);
  glutReshapeFunc(redimensiona);
  glutKeyboardFunc(teclado);
  glutKeyboardUpFunc(keyUP);
  glutDisplayFunc(desenha);
  glutIdleFunc(atualiza);
  glutTimerFunc(17, atualizaCena, 0);
  inicializarPrograma();
  glutMainLoop();
  return 0;
}
