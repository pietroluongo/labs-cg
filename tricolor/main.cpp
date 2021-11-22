#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glut.h"
#include "../libs/imgui/imgui_impl_opengl2.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

// Pontos do triangulo
static float pRx = 0.1;
static float pRy = 0.1;
static float pGx = 0.9;
static float pGy = 0.1;
static float pBx = 0.5;
static float pBy = 0.9;

// Controle dos arrastes
static int draggingPointR = 0, draggingPointG = 0, draggingPointB = 0,
           choosingColor = 0;

// Cor
static float gR = 0., gG = 0., gB = 0.;

// Ponto do clique da cor
static float pCliqueX = 0, pCliqueY = 0;
// Pronto do clique projetado
static float pProjX = 0, pProjY = 0;

static bool imgui_showDebug = false;
static bool imgui_mouse = false;

void imgui_init();
void imgui_display();
void drawUI();

void imgui_display() {
    if (imgui_showDebug) {
        ImGui::Begin("Controls", &imgui_showDebug);
        ImGui::BeginDisabled(true);
        ImGui::Checkbox("Mouse", &imgui_mouse);
        ImGui::EndDisabled();
        {
            ImGui::BeginDisabled(true);
            float pClique[2] = {pCliqueX, pCliqueY};
            ImGui::SliderFloat2("pClique", pClique, 0, 1);
            ImGui::EndDisabled();
        }
        {
            ImGui::BeginDisabled(true);
            float pProj[2] = {pProjX, pProjY};
            ImGui::SliderFloat2("pProj", pProj, 0, 1);
            ImGui::EndDisabled();
        }
        ImGui::End();
    }
    glutPostRedisplay();
}

void display(void) {
    // Define a cor do fundo
    glClearColor(gR, gG, gB, 0.0);

    /* Limpar todos os pixels  */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    /* Desenhar um polígono branco (retângulo) */
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(pRx, pRy, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(pGx, pGy, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(pBx, pBy, 0.0);
    glEnd();

    /* Desenha o ponto de clique. */
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(pCliqueX, pCliqueY, 0.0);
    glEnd();

    /* Desenha o ponto projetado. */
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glVertex3f(pProjX, pProjY, 0.0);
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
        pCliqueX = fX;
        pCliqueY = fY;

        /**
                COLOQUE SEU CODIGO AQUI
        **/

    } else if (draggingPointR) {
        pRx = (GLfloat)x / TAMANHO_JANELA;
        pRy = (GLfloat)y / TAMANHO_JANELA;
    } else if (draggingPointG) {
        pGx = (GLfloat)x / TAMANHO_JANELA;
        pGy = (GLfloat)y / TAMANHO_JANELA;
    } else if (draggingPointB) {
        pBx = (GLfloat)x / TAMANHO_JANELA;
        pBy = (GLfloat)y / TAMANHO_JANELA;
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
        if ((pRx - fX) * (pRx - fX) + (pRy - fY) * (pRy - fY) <
            (30.0 / TAMANHO_JANELA) * (30.0 / TAMANHO_JANELA)) {
            draggingPointR = 1;
        } else if ((pGx - fX) * (pGx - fX) + (pGy - fY) * (pGy - fY) <
                   (30.0 / TAMANHO_JANELA) * (30.0 / TAMANHO_JANELA)) {
            draggingPointG = 1;
        } else if ((pBx - fX) * (pBx - fX) + (pBy - fY) * (pBy - fY) <
                   (30.0 / TAMANHO_JANELA) * (30.0 / TAMANHO_JANELA)) {
            draggingPointB = 1;
        }

    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        choosingColor = 0;
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        draggingPointR = draggingPointG = draggingPointB = 0;
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