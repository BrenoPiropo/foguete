
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
float asaRotation = 0.0;      // Ângulo de rotação inicial das asas
float asaRotationStep = 1.0;  // Valor a ser adicionado ao ângulo de rotação para movimento
float asaDisappearRate = 0.1; // Taxa de desaparecimento das asas
int timerIntervalAsa = 16;    // Intervalo do temporizador para atualização das asas

float cloudOffsetY = 0.0; // Deslocamento vertical para animação de nuvem
float platformY = 0.0;    // Coordenada Y inicial da plataforma
float rocketY = 0.0;      // Variável para controlar a posição vertical do foguete
int countdown = 5;        // Contador decrescente inicial (tempo para decolagem em segundos)
bool launchAuthorized = false; // Indica se a decolagem está autorizada
bool fireEffect = false; // Indica se o efeito de fogo deve ser exibido
bool smokeEffect = false; // Indica se o efeito de fumaça deve ser exibido
bool cloudEffect = true; // Indica se o efeito de nuvem deve ser exibido
char statusText[50];    // Variável para armazenar o texto a ser exibido

float backgroundColor[3] = {0.7f, 0.7f, 1.0f}; // Cor de fundo inicial (azul claro)
float targetColor[3] = {0.0f, 0.0f, 0.0f};      // Cor de fundo alvo (preto)
int colorChangeInterval = 3000; // Intervalo para mudança de cor (em segundos)
int colorChangeTimer = 0;     // Temporizador para mudança de cor
void updateAsas(int value) {
    if (asaRotation < 90.0) {
        asaRotation += asaRotationStep;
    }

    if (glutGet(GLUT_ELAPSED_TIME) > 6000) {
        // Gradualmente move as asas para fora da tela até que o ângulo de rotação seja maior que 180 graus
        asaRotation += asaDisappearRate;

        if (asaRotation > 180.0) {
            asaRotation = 180.0;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(timerIntervalAsa, updateAsas, 0);
}
void startAsasAnimation(int value) {
    // Inicia a animação das asas após 6 segundos
    glutTimerFunc(6000, updateAsas, 0);
}

// Função para calcular a cor de fundo com base no tempo decorrido
void calcularCorFundo() {
    if (launchAuthorized) {
        // Tempo decorrido desde a decolagem
        int tempoDecorrido = 5 + (15 - countdown);

        // Verificar se já passaram 10 segundos para alterar a cor
        if (tempoDecorrido >= 10) {
            // Calcular os componentes R, G e B interpolando entre a cor inicial e a cor alvo
            for (int i = 0; i < 3; ++i) {
                backgroundColor[i] -= 0.7 / colorChangeInterval; // Reduzir 0.7 a cada 10 segundos
            }

            // Limitar o valor mínimo para evitar valores negativos
            for (int i = 0; i < 3; ++i) {
                backgroundColor[i] = fmax(backgroundColor[i], targetColor[i]);
            }
        }
    }

    // Definir a cor de fundo
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
}

// Protótipo da função para desativar o efeito de fumaça
void DesativaFumaca(int value);

// Função para desenhar a plataforma
void plataforma() {
    glBegin(GL_QUADS);
    glColor3f(0.5, 0, 0.5); // Cor roxa para a plataforma
    glVertex2f(2.5, 0.0 - platformY);
    glVertex2f(3.9, 0.0 - platformY);
    glVertex2f(3.9, 0.2 - platformY);
    glVertex2f(2.5, 0.2 - platformY);
    glEnd();
}

// Função para desenhar a torre
void torre() {
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0); // Cor amarela para a torre
    glVertex2f(3.5, 0.2 - platformY);
    glVertex2f(3.9, 0.2 - platformY);
    glVertex2f(3.9, 2.8 - platformY);
    glVertex2f(3.5, 2.8 - platformY);
    glEnd();
}

// Função para desenhar as linhas de ligação
void linhasDeLigacao() {
    glColor3f(0, 0, 0); // Cor preta para as linhas de ligação
    glLineWidth(6.0);

    // Linha do topo da torre para o bico
    glBegin(GL_LINES);
    glVertex2f(3.5, 2.0 - platformY);
    glVertex2f(3.1, 2.0 - platformY);
    glEnd();

    // Linha da torre para o corpo do foguete (acima da asa direita)
    glBegin(GL_LINES);
    glVertex2f(3.5, 1.0 - platformY);
    glVertex2f(3.2, 1.0 - platformY);
    glEnd();

    // Linha do início da torre para a asa direita
    glBegin(GL_LINES);
    glVertex2f(3.5, 0.2 - platformY);
    glVertex2f(3.4, 0.2 - platformY);
    glEnd();
}

// Função para desenhar o bico do foguete
void bico() {
    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0); // Cor verde para o bico
    glVertex2f(3.0, 2.0 + rocketY);
    glVertex2f(3.2, 2.0 + rocketY);
    glVertex2f(3.1, 2.4 + rocketY);
    glEnd();
}

// Função para desenhar o corpo do foguete
void corpo() {
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0); // Cor preta para o corpo
    glVertex2f(3.0, 0.2 + rocketY);
    glVertex2f(3.2, 0.2 + rocketY);
    glVertex2f(3.2, 2.0 + rocketY);
    glVertex2f(3.0, 2.0 + rocketY);
    glEnd();
}

// Função para desenhar a asa esquerda do foguete
void asaEsquerda() {
    if (asaRotation <= 120.0) {
        glPushMatrix();
        glTranslatef(3.0, 0.2 + rocketY, 0.0);
        glRotatef(asaRotation, 0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLES);
        glColor3f(0, 0, 0);
        glVertex2f(-0.2, 0.0);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 0.8);
        glEnd();
        glPopMatrix();
    }
}

// Função para desenhar a asa direita do foguete

void asaDireita() {
    if (asaRotation <= 120.0) {
        glPushMatrix();
        glTranslatef(3.2, 0.2 + rocketY, 0.0);
        glRotatef(-asaRotation, 0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLES);
        glColor3f(0, 0, 0);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.2, 0.0);
        glVertex2f(0.0, 0.8);
        glEnd();
        glPopMatrix();
    }
}

// Função para desenhar a fumaça ao lado do foguete
void criarFumacaLado() {
    if (smokeEffect) {
        glColor4f(0.5, 0.5, 0.5, 0.7); // Cor cinza e transparência para a fumaça

        int numParticles = 40000; // Número de partículas de fumaça

        // Lado esquerdo
        for (int i = 0; i < numParticles; ++i) {
            float offsetX = ((float)rand() / RAND_MAX) * 0.2; // Posição X aleatória na área da fumaça
            float offsetY = ((float)rand() / RAND_MAX) * 0.4 - 0.2; // Posição Y aleatória na área da fumaça

            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j <= 360; j += 10) {
                float radian = j * M_PI / 180.0;
                float x = 2.9 - offsetX + 0.1 * cos(radian);
                float y = 0.1 + rocketY + offsetY + 0.05 * sin(radian);
                glVertex2f(x, y);
            }
            glEnd();
        }

        // Lado direito
        for (int i = 0; i < numParticles; ++i) {
            float offsetX = ((float)rand() / RAND_MAX) * 0.2; // Posição X aleatória na área da fumaça
            float offsetY = ((float)rand() / RAND_MAX) * 0.4 - 0.2; // Posição Y aleatória na área da fumaça

            glBegin(GL_TRIANGLE_FAN);
            for (int j = 0; j <= 360; j += 10) {
                float radian = j * M_PI / 180.0;
                float x = 3.2 + offsetX + 0.1 * cos(radian);
                float y = 0.1 + rocketY + offsetY + 0.05 * sin(radian);
                glVertex2f(x, y);
            }
            glEnd();
        }
    }
}

// Função para desenhar o efeito de fogo
void fogo() {
    if (fireEffect) {
        glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.5, 0.0); // Cor laranja para o fogo
        glVertex2f(3.0, 0.2 + rocketY);
        glVertex2f(3.2, 0.2 + rocketY);
        glVertex2f(3.1, -0.2 + rocketY);
        glEnd();
    }
}
void nuvens() {
    if (cloudEffect) {
        glColor4f(1.0, 1.0, 1.0, 0.8); // Cor branca e transparência para a nuvem

        int numClouds = 7; // Número total de nuvens (5 originais + 2 adicionais)

        // Nuvens originais
        for (int i = 0; i < 5; ++i) {
            int numParticles = 100; // Número de partículas em cada nuvem

            float cloudX = 2.0 + i; // Posição X constante da nuvem
            float cloudY = 3.8; // Ajuste a altura inicial das nuvens para a parte superior da janela

            for (int j = 0; j < numParticles; ++j) {
                float particleX = cloudX + ((float)rand() / RAND_MAX) * 0.5; // Posição X da partícula
                float particleY = cloudY - cloudOffsetY + ((float)rand() / RAND_MAX) * 0.2; // Posição Y da partícula, ajustada pelo deslocamento vertical

                glBegin(GL_TRIANGLE_FAN);
                for (int k = 0; k <= 360; k += 10) {
                    float radian = k * M_PI / 180.0;
                    float x = particleX + 0.1 * cos(radian);
                    float y = particleY + 0.05 * sin(radian);
                    glVertex2f(x, y);
                }
                glEnd();
            }
        }

        // Nuven adicionais
        int numParticles = 100; // Número de partículas em cada nuvem

        // Primeira nuvem adicional
        float cloudX1 = 1.5; // Posição X constante da primeira nuvem adicional
        float cloudY1 = 3.5; // Ajuste a altura inicial da primeira nuvem adicional para a parte superior da janela

        for (int j = 0; j < numParticles; ++j) {
            float particleX1 = cloudX1 + ((float)rand() / RAND_MAX) * 0.5; // Posição X da partícula
            float particleY1 = cloudY1 - cloudOffsetY + ((float)rand() / RAND_MAX) * 0.2; // Posição Y da partícula, ajustada pelo deslocamento vertical

            glBegin(GL_TRIANGLE_FAN);
            for (int k = 0; k <= 360; k += 10) {
                float radian = k * M_PI / 180.0;
                float x = particleX1 + 0.1 * cos(radian);
                float y = particleY1 + 0.05 * sin(radian);
                glVertex2f(x, y);
            }
            glEnd();
        }

        // Segunda nuvem adicional
        float cloudX2 = -2.0; // Posição X constante da segunda nuvem adicional
        float cloudY2 = -3.7; // Ajuste a altura inicial da segunda nuvem adicional para a parte superior da janela

        for (int j = 0; j < numParticles; ++j) {
            float particleX2 = cloudX2 + ((float)rand() / RAND_MAX) * 0.5; // Posição X da partícula
            float particleY2 = cloudY2 - cloudOffsetY + ((float)rand() / RAND_MAX) * 0.2; // Posição Y da partícula, ajustada pelo deslocamento vertical

            glBegin(GL_TRIANGLE_FAN);
            for (int k = 0; k <= 360; k += 10) {
                float radian = k * M_PI / 180.0;
                float x = particleX2 + 0.1 * cos(radian);
                float y = particleY2 + 0.05 * sin(radian);
                glVertex2f(x, y);
            }
            glEnd();
        }
    }
}
// Função para atualizar o deslocamento vertical das nuvens (animação de descida)
void moverNuvens(int value) {
    cloudOffsetY += 0.002; // Ajuste a velocidade conforme necessário para a animação de descida
    glutTimerFunc(16, moverNuvens, 0);
    glutPostRedisplay();
}

void DesenhaCena(void) {
    // Calcular a cor de fundo com base no tempo decorrido
    calcularCorFundo();

    // Limpar o buffer de cores
    glClear(GL_COLOR_BUFFER_BIT);

    // Restante do código permanece inalterado...
    plataforma();
    torre();
    linhasDeLigacao();
    criarFumacaLado();
    bico();
    corpo();
    asaEsquerda();
    asaDireita();
    fogo();
    nuvens();

    // Renderizar texto de status
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(1.5, 3.5);
    for (int i = 0; statusText[i] != '\0'; ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, statusText[i]);
    }

    // Trocar os buffers
    glutSwapBuffers();
}

// Função de temporização para animação
void Atualiza(int value) {
    if (launchAuthorized) {
        platformY += 0.005; // Ajuste a velocidade conforme necessário para o movimento da plataforma

        // Ativar o efeito de fogo e fumaça após a autorização de decolagem
        if (!fireEffect && platformY) {
            fireEffect = true;
            smokeEffect = true;
            glutTimerFunc(3000, DesativaFumaca, 0); // Desativar a fumaça após 5 segundos
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, Atualiza, 0);
}

// Função para desativar o efeito de fumaça
void DesativaFumaca(int value) {
    smokeEffect = false;
    glutPostRedisplay();
}

// Função de temporização para o cronômetro
void Temporizador(int value) {
    if (countdown > 0) {
        snprintf(statusText, sizeof(statusText), "Tempo para decolagem: %d segundos", countdown);
        countdown--;
        glutTimerFunc(1000, Temporizador, 0);
    } else {
        snprintf(statusText, sizeof(statusText), "Decolagem autorizada");
        launchAuthorized = true;
        glutTimerFunc(16, Atualiza, 0);
        glutTimerFunc(16, moverNuvens, 0); // Iniciar animação de descida das nuvens
    }

    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Foguete em OpenGL");

    glOrtho(0.0, 5.0, 0.0, 4.0, -1.0, 1.0);

    glutDisplayFunc(DesenhaCena);

    // Inicie o temporizador
    glutTimerFunc(0, Temporizador, 0);

    glutTimerFunc(40000, startAsasAnimation, 0);

    glutMainLoop();

    return 0;
}
