#include <TFT_eSPI.h>
#include <DFRobotDFPlayerMini.h>

#define BUTTON_PIN 13

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);

DFRobotDFPlayerMini myDFPlayer;

const int screenWidth = 240;
const int screenHeight = 240;

const int dinoWidth = 20;
const int dinoHeight = 20;
const int dinoJumpHeight = 40; // Altura máxima del salto del dinosaurio
const int dinoSpeed = 3; // Velocidad de movimiento del dinosaurio

const int groundHeight = 10; // Altura del suelo

bool isJumping = false;
int dinoY = screenHeight - dinoHeight - groundHeight;

int obstacleX = screenWidth;
int obstacleWidth = 10;
int obstacleHeight = 20;
int obstacleSpeed = 2;

bool gameRunning = true;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  if(!myDFPlayer.begin(Serial)){
  //Serial.println("ERROR al inicializar el módulo DFPlayer Mini");
} else {
  //Serial.println("Módulo DFPlayer Mini inicializado correctamente");
}
  myDFPlayer.volume(20);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  img.setColorDepth(8);
  img.createSprite(240, 240);
  img.fillRect(0, screenHeight - groundHeight, screenWidth, groundHeight, TFT_WHITE);
  myDFPlayer.play(1);
}

void loop() {
  if (gameRunning) {
    // Detectar el salto del dinosaurio
    if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
      isJumping = true;
      //myDFPlayer.play(1);
    }

    img.fillRect(50, dinoY, dinoWidth, dinoHeight, TFT_BLACK);
    img.fillRect(obstacleX, screenHeight - obstacleHeight - groundHeight, obstacleWidth, obstacleHeight, TFT_BLACK);

    // Mover el dinosaurio
    moveDino();

    // Mover el obstáculo
    moveObstacle();

    // Dibujar el juego
    img.fillRect(50, dinoY, dinoWidth, dinoHeight, TFT_GREEN);
    img.fillRect(obstacleX, screenHeight - obstacleHeight - groundHeight, obstacleWidth, obstacleHeight, TFT_RED);

    // Verificar colisión
    checkCollision();

    img.pushSprite(0, 0);
  }
}

void moveDino() {
  if (isJumping) {
    // Salto del dinosaurio
    dinoY -= dinoSpeed;
    if (dinoY <= screenHeight - dinoHeight - groundHeight - dinoJumpHeight) {
      isJumping = false;
    }
  } else {
    // Caída del dinosaurio
    if (dinoY < screenHeight - dinoHeight - groundHeight) {
      dinoY += dinoSpeed;
    }
  }
}

void moveObstacle() {
  obstacleX -= obstacleSpeed;
  if (obstacleX + obstacleWidth <= 0) {
    obstacleX = screenWidth;
  }
}

void checkCollision() {
  if (obstacleX < 50 + dinoWidth && obstacleX + obstacleWidth > 50 &&
      screenHeight - obstacleHeight - groundHeight < dinoY + dinoHeight && 
      screenHeight - groundHeight > dinoY) {
    // Colisión detectada, detener el juego
    gameRunning = false;
    img.setTextColor(TFT_WHITE);
    img.setTextSize(2);
    img.drawString("Game Over!", screenWidth / 2, screenHeight / 2);
    img.pushSprite(0, 0);
    //myDFPlayer.play(2);
  }
}
