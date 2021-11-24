#include "../libs/glm/glm.hpp"
#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glut.h"
#include "../libs/imgui/imgui_impl_opengl2.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::ivec3 ivec3;

// Pontos do triangulo
static vec2 pR = vec2(0.1f, 0.1f);
static vec2 pG = vec2(0.9f, 0.1f);
static vec2 pB = vec2(0.5f, 0.9f);

// Controle dos arrastes
static int choosingColor = 0;

static ivec3 draggingPoint = ivec3(0, 0, 0);

// Cor
static vec3 bg = vec3(0., 0., 0.);

// Ponto do clique da cor
static vec2 pClique = vec2(0., 0.);

// Pronto do clique projetado
static vec2 pProj = vec2(0., 0.);

static bool imgui_showDebug = false;
static bool imgui_mouse = false;

void imgui_init();
void imgui_display();
void drawUI();
/**
 * @brief Gets instersection between two lines defined by two points
 *
 * @param p1 First point of the first line
 * @param p2 Second point of the first line
 * @param p3 First point of the second line
 * @param p4 Second point of the second line
 * @return vec2* Array with coordinates from intersection point
 */
vec2* intersection(const vec2& p1, const vec2& p2, const vec2& p3,
                   const vec2& p4);

void imgui_display() {
    if (imgui_showDebug) {
        ImGui::Begin("Controls", &imgui_showDebug);
        ImGui::BeginDisabled(true);
        ImGui::Checkbox("Mouse", &imgui_mouse);
        ImGui::EndDisabled();
        {
            ImGui::BeginDisabled(true);
            ImGui::SliderFloat2("pClique", (float*)&pClique, 0, 1);
            ImGui::EndDisabled();
        }
        {
            ImGui::BeginDisabled(true);
            ImGui::SliderFloat2("pProj", (float*)&pProj, 0, 1);
            ImGui::EndDisabled();
        }
        {
            ImGui::BeginDisabled(true);
            ImGui::ColorEdit3("bg", (float*)&bg);
            ImGui::EndDisabled();
        }
        ImGui::End();
    }
    glutPostRedisplay();
}

void display(void) {
    // Define a cor do fundo
    glClearColor(bg.r, bg.g, bg.b, 0.0);

    /* Limpar todos os pixels  */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    /* Desenhar um polígono branco (retângulo) */
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(pR.x, pR.y, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(pG.x, pG.y, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(pB.x, pB.y, 0.0);
    glEnd();

    /* Desenha o ponto de clique. */
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(pClique.x, pClique.y, 0.0);
    glEnd();

    /* Desenha o ponto projetado. */
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glVertex3f(pProj.x, pProj.y, 0.0);
    glEnd();

    drawUI();

    /* Desenhar no frame buffer! */
    glutSwapBuffers(); // Funcao apropriada para janela double buffer
}

void init(void) {
    /* selecionar cor de fundo (preto) */
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* inicializar sistema de visualizacao */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void motion(int x, int y) {
    // Corrige a posicao do mouse para a posicao da janela de visualizacao
    y = TAMANHO_JANELA - y;
    GLfloat fX = (GLfloat)x / TAMANHO_JANELA;
    GLfloat fY = (GLfloat)y / TAMANHO_JANELA;

    if (choosingColor) {
        // Atualiza posicao do clique
        pClique.x = fX;
        pClique.y = fY;

        /**
                COLOQUE SEU CODIGO AQUI
        **/
        // GLfloat p3[2] = {pCliqueX, pCliqueY};
        vec2* inter = intersection(pR, pG, pB, pClique);
        pProj.x = inter->x;
        pProj.y = inter->y;
        free(inter);

    } else if (draggingPoint.r) {
        pR.x = (GLfloat)x / TAMANHO_JANELA;
        pR.y = (GLfloat)y / TAMANHO_JANELA;
    } else if (draggingPoint.g) {
        pG.x = (GLfloat)x / TAMANHO_JANELA;
        pG.y = (GLfloat)y / TAMANHO_JANELA;
    } else if (draggingPoint.b) {
        pB.x = (GLfloat)x / TAMANHO_JANELA;
        pB.y = (GLfloat)y / TAMANHO_JANELA;
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (imgui_mouse) {
        ImGui_ImplGLUT_MouseFunc(button, state, x, y);
        ImGui_ImplGLUT_MotionFunc(x, y);
        return;
    }
    // Corrige a posicao do mouse para a posicao da janela de visualizacao
    GLfloat fX = (GLfloat)x / TAMANHO_JANELA;
    GLfloat fY = (GLfloat)(TAMANHO_JANELA - y) / TAMANHO_JANELA;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        choosingColor = 1;
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if ((pR.x - fX) * (pR.x - fX) + (pR.y - fY) * (pR.y - fY) <
            (30.0 / TAMANHO_JANELA) * (30.0 / TAMANHO_JANELA)) {
            draggingPoint.r = 1;
        } else if ((pG.x - fX) * (pG.x - fX) + (pG.y - fY) * (pG.y - fY) <
                   (30.0 / TAMANHO_JANELA) * (30.0 / TAMANHO_JANELA)) {
            draggingPoint.g = 1;
        } else if ((pB.x - fX) * (pB.x - fX) + (pB.y - fY) * (pB.y - fY) <
                   (30.0 / TAMANHO_JANELA) * (30.0 / TAMANHO_JANELA)) {
            draggingPoint.b = 1;
        }

    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        choosingColor = 0;
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        draggingPoint.r = draggingPoint.g = draggingPoint.b = 0;
    }
    motion(x, y);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        exit(0);
        break;
    case 'd':
        imgui_showDebug = !imgui_showDebug;
        break;
    case 'm':
        imgui_mouse = !imgui_mouse;
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(TAMANHO_JANELA, TAMANHO_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cores");
    init();
    imgui_init();
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}

void imgui_init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();
}

void drawUI() {
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    imgui_display();

    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

vec2* intersection(const vec2& p1, const vec2& p2, const vec2& p3,
                   const vec2& p4) {
    GLfloat m1 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    GLfloat m2 = (p4[1] - p3[1]) / (p4[0] - p3[0]);
    GLfloat b1 = p1[1] - m1 * p1[0];
    GLfloat b2 = p3[1] - m2 * p3[0];

    GLfloat q0 = (b2 - b1) / (m1 - m2);
    GLfloat q1 = m1 * q0 + b1;
    vec2* q = new vec2(q0, q1);
    return q;
}