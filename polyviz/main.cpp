#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glut.h"
#include "../libs/imgui/imgui_impl_opengl2.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_JANELA 500

GLfloat points[2][3] = {{10.0, 10.0, 0.0},
                        {TAMANHO_JANELA - 10.0, TAMANHO_JANELA - 10.0, 0.0}};

GLfloat currentInterpCoordinates[2] = {0, 0};

static bool imgui_showControls = true;
static bool shouldDrawPolyline = false;
static GLfloat t = 0;

double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_MAP1_VERTEX_3);

    // Muda para a matriz de projecao (aulas futuras)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-size, size, -size, size, -size, size);
    // glOrtho(0, 1, 1, 0, 0, 1);
    glOrtho(0, TAMANHO_JANELA, TAMANHO_JANELA, 0, 0, 1);
}

void imgui_display() {
    if (imgui_showControls) {
        ImGui::Begin("Controls", &imgui_showControls);
        ImGui::SliderFloat2("Point 1", points[0], 0, TAMANHO_JANELA);
        ImGui::SliderFloat2("Point 2", points[1], 0, TAMANHO_JANELA);
        ImGui::BeginDisabled();
        ImGui::SliderFloat2("Interp", currentInterpCoordinates, 0,
                            TAMANHO_JANELA);
        ImGui::EndDisabled();
        ImGui::SliderFloat("t", &t, 0, 1);
        ImGui::Checkbox("Draw polyline", &shouldDrawPolyline);
        if (ImGui::CollapsingHeader("Polyline Equation")) {
            ImGui::Text("\tI(t) = (1-t)P + tQ\n\tI(%.3f) = (%.3f)P + %.3fQ", t,
                        (1 - t), t);
            ImGui::Text("\tx(%.3f) = %.3f * %.3f + %.3f * %.3f", t, (1 - t),
                        points[0][0], t, points[1][0]);
            ImGui::Text("\ty(%.3f) = %.3f * %.3f + %.3f * %.3f", t, (1 - t),
                        points[0][1], t, points[1][1]);
        }
        ImGui::End();
    }
}

void drawPoints() {
    /* Desenha os pontos de controle. */
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 2; i++)
        glVertex3fv(&points[i][0]);
    glEnd();
    currentInterpCoordinates[0] = points[0][0] * (1 - t) + points[1][0] * t;
    currentInterpCoordinates[1] = points[0][1] * (1 - t) + points[1][1] * t;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glVertex3f(currentInterpCoordinates[0], currentInterpCoordinates[1], 0.0);
    glEnd();
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

void drawPolyline() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 2; i++)
        glVertex3fv(&points[i][0]);
    glEnd();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    drawPoints();
    if (shouldDrawPolyline) {
        drawPolyline();
    }
    drawUI();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '0':
        imgui_showControls = !imgui_showControls;
        break;
    }
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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(TAMANHO_JANELA, TAMANHO_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    imgui_init();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
