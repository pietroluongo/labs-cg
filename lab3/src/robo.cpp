#include "../include/robo.h"
#include "../include/utils.h"
#include <math.h>

/**
 * @brief Desenha um retângulo do robô
 * @param height Altura do retângulo
 * @param width Largura do retângulo
 * @param R Cor vermelha do retângulo
 * @param G Cor verde do retângulo
 * @param B Cor azul do retângulo
 */
void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G,
                       GLfloat B) {
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
    // clang-format off
        glVertex2f(-width/2, 0);
        glVertex2f(width/2, 0);
        glVertex2f(width/2, height);
        glVertex2f(-width/2, height);
    // clang-format on
    glEnd();
}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B) {}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R,
                       GLfloat G, GLfloat B) {}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2,
                        GLfloat theta3) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(90, 0, 0, 1);
    DesenhaRect(paddleHeight, paddleWidth, 0, 0, 1);
    glPopMatrix();
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1,
                       GLfloat theta2, GLfloat theta3) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    Robo::DesenhaRect(baseHeight, baseWidth, 1, 0, 0);
    Robo::DesenhaBraco(0, baseHeight, theta1, theta2, theta3);
    glPopMatrix();
}

void Robo::RodaBraco1(GLfloat inc) {}

void Robo::RodaBraco2(GLfloat inc) {}

void Robo::RodaBraco3(GLfloat inc) {}

void Robo::MoveEmX(GLfloat dx) { this->gX += dx; }

// Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat& xOut,
                 GLfloat& yOut) {}

Tiro* Robo::Atira() {}
