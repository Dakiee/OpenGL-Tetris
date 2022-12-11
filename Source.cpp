#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>
#include <cstring>
using namespace std;

bool paused = false;
const int R = 20;
const int C = 10;
const int width = 300;
const int height = 600;
int tim = 250;
int mat[R][C];
bool fresh = false;
bool over = false;
bool dark = true;
bool lines = false;
bool blocks = true;

void del(int index)
{
    for (int i = index; i < R - 1; i++)
    {
        for (int j = 0; j < C; j++)
        {
            mat[i][j] = mat[i + 1][j];
        }
    }
}

void lineUstgah()
{
    for (int i = 0; i < R; i++)
    {
        bool cleared = true;
        for (int j = 0; j < C; j++)
        {
            if (mat[i][j] == 0)
            {
                cleared = false;
                break;
            }
        }
        if (cleared)
        {
            del(i);
            glutPostRedisplay();
        }
    }
}

float *color(int x)
{
    static float c0[3] = {0.0, 0.0, 0.0};
    static float c1[3] = {1.0, 0.0, 0.0};
    static float c2[3] = {0.0, 1.0, 0.0};
    static float c3[3] = {0.0, 0.0, 1.0};
    static float c4[3] = {0.0, 1.0, 1.0};
    static float c5[3] = {1.0, 0.0, 1.0};
    static float c6[3] = {1.0, 1.0, 0.0};
    switch (x)
    {
    case 1:
        return c1;
        break;
    case 2:
        return c2;
        break;
    case 3:
        return c3;
        break;
    case 4:
        return c4;
        break;
    case 5:
        return c5;
        break;
    case 6:
        return c6;
        break;
    default:
        return c0;
    }
}

void clearMat()
{
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            mat[i][j] = 0;
}

class block
{
public:
    bool out[4][4], set, init = false;
    int s, p, c, x, y, sc;

    void project()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (out[i][j])
                    mat[x + i][y + j] = c;
    }

    bool shape()
    {
        bool temp[R + 2][C + 2];
        int tx = x + 1;
        int ty = y + 1;
        for (int i = 0; i < R + 2; i++)
            for (int j = 0; j < C + 2; j++)
                temp[i][j] = false;
        for (int i = 0; i < R + 2; i += (R + 1))
            for (int j = 0; j < C + 2; j++)
                temp[i][j] = true;
        for (int i = 0; i < R + 2; i++)
            for (int j = 0; j < C + 2; j += (C + 1))
                temp[i][j] = true;
        for (int i = 1; i <= R; i++)
            for (int j = 1; j <= C; j++)
                if (mat[i - 1][j - 1] != 0)
                    temp[i][j] = true;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (out[i][j])
                    if (temp[tx + i][ty + j])
                        return true;
        return false;
    }

    bool blockDown()
    {
        cleanBlock();
        x -= 1;
        if (!shape())
        {
            project();
        }
        else
        {
            x += 1;
            project();
            return false;
        }
        return true;
    }

    bool blockLeft()
    {
        cleanBlock();
        y -= 1;
        if (!shape())
        {
            project();
        }
        else
        {
            y += 1;
            project();
            return false;
        }
        return true;
    }

    bool blockRight()
    {
        cleanBlock();
        y += 1;
        if (!shape())
        {
            project();
        }
        else
        {
            y -= 1;
            project();
            return false;
        }
        return true;
    }

    bool blockUp()
    {
        int op = p;
        p = (p % 4) + 1;
        outClean();
        cleanBlock();
        drawOut();
        if (!shape())
        {
            project();
        }
        else
        {
            p = op;
            outClean();
            cleanBlock();
            drawOut();
            project();
            return false;
        }
        return true;
    }

    void outClean()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                out[i][j] = false;
    }

    void drawOut()
    {
        switch (s)
        {
        case 1:
            switch (p)
            {
            case 1:
            case 3:
                out[0][1] = true;
                out[1][0] = true;
                out[1][1] = true;
                out[2][0] = true;
                break;
            case 2:
            case 4:
                out[0][0] = true;
                out[0][1] = true;
                out[1][1] = true;
                out[1][2] = true;
                break;
            default:
                cout << "Aldaa" << endl;
            }
            break;
        case 2:
            switch (p)
            {
            case 1:
            case 3:
                out[0][0] = true;
                out[1][0] = true;
                out[1][1] = true;
                out[2][1] = true;
                break;
            case 2:
            case 4:
                out[0][1] = true;
                out[0][2] = true;
                out[1][0] = true;
                out[1][1] = true;
                break;
            default:
                cout << "Aldaa" << endl;
            }
            break;
        case 3:
        case 4:
            switch (p)
            {
            case 1:
            case 3:
                out[0][1] = true;
                out[1][1] = true;
                out[2][1] = true;
                out[3][1] = true;
                break;
            case 2:
            case 4:
                out[1][0] = true;
                out[1][1] = true;
                out[1][2] = true;
                out[1][3] = true;
                break;
            default:
                cout << "Aldaa" << endl;
            }
            break;
        case 5:
        case 6:
            switch (p)
            {
            case 1:
            case 2:
            case 3:
            case 4:
                out[0][0] = true;
                out[0][1] = true;
                out[1][0] = true;
                out[1][1] = true;
                break;
            default:
                cout << "Aldaa" << endl;
            }
            break;
        case 7:
            switch (p)
            {
            case 1:
                out[0][1] = true;
                out[0][2] = true;
                out[1][1] = true;
                out[2][1] = true;
                break;
            case 2:
                out[1][0] = true;
                out[1][1] = true;
                out[1][2] = true;
                out[2][2] = true;
                break;
            case 3:
                out[0][1] = true;
                out[1][1] = true;
                out[2][0] = true;
                out[2][1] = true;
                break;
            case 4:
                out[0][0] = true;
                out[1][0] = true;
                out[1][1] = true;
                out[1][2] = true;
                break;
            default:
                cout << "Aldaa" << endl;
            }
            break;
        case 8:
            switch (p)
            {
            case 1:
                out[0][0] = true;
                out[0][1] = true;
                out[1][1] = true;
                out[2][1] = true;
                break;
            case 2:
                out[0][2] = true;
                out[1][0] = true;
                out[1][1] = true;
                out[1][2] = true;
                break;
            case 3:
                out[0][1] = true;
                out[1][1] = true;
                out[2][1] = true;
                out[2][2] = true;
                break;
            case 4:
                out[1][0] = true;
                out[1][1] = true;
                out[1][2] = true;
                out[2][0] = true;
                break;
            default:
                cout << "Aldaa" << endl;
            }
            break;
        case 9:
        case 10:
            switch (p)
            {
            case 1:
                out[0][1] = true;
                out[1][1] = true;
                out[1][2] = true;
                out[2][1] = true;
                break;
            case 2:
                out[1][0] = true;
                out[1][1] = true;
                out[1][2] = true;
                out[2][1] = true;
                break;
            case 3:
                out[0][1] = true;
                out[1][0] = true;
                out[1][1] = true;
                out[2][1] = true;
                break;
            case 4:
                out[0][1] = true;
                out[1][0] = true;
                out[1][1] = true;
                out[1][2] = true;
                break;
            default:
                cout << "Aldaa" << endl;
            }
            break;
        default:
            cout << "Aldaa" << endl;
        }
    }

    bool blockInit()
    {
        lineUstgah();
        outClean();
        y = (C / 2) - 1;
        x = R - 4;
        set = false;
        init = true;
        s = (rand() % 10) + 1;
        p = (rand() % 4) + 1;
        c = (rand() % 6) + 1;
        drawOut();
        if (!shape())
        {
            project();
            return true;
        }
        else
        {
            clearMat();
            outClean();
            cleanBlock();
            fresh = true;
            over = true;
            paused = true;
            return false;
        }
    }

    void cleanBlock()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (out[i][j])
                {
                    mat[x + i][y + j] = 0;
                }
            }
        }
    }

    bool process()
    {
        if (true)
        {
            return blockDown();
        }
        else
        {
            cout << "test" << endl;
            return blockInit();
        }
    }
};

void matInit()
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            mat[i][j] = 0;
        }
    }
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void reshape(int w, int h)
{
    float x = (float)C, y = (float)R;
    glutReshapeWindow(width, height);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, x, 0.0, y);
    glMatrixMode(GL_MODELVIEW);
}

void mapMatrix()
{
    for (int y = 0; y < R; y++)
    {
        for (int x = 0; x < C; x++)
        {
            if (mat[y][x] != 0)
            {
                glBegin(GL_QUADS);
                float *c;
                c = color(mat[y][x]);
                glColor3f(c[0], c[1], c[2]);
                ;
                glVertex2f(x, y);
                glVertex2f(x + 1, y);
                glVertex2f(x + 1, y + 1);
                glVertex2f(x, y + 1);
                glEnd();
                if (blocks)
                {
                    if (!lines)
                    {
                        glColor3f(0.0, 0.0, 0.0);
                    }
                    else
                    {
                        glColor3f(1.0, 1.0, 1.0);
                    }
                }
                glLineWidth(2);
                glBegin(GL_LINES);
                glVertex2f(x, y);
                glVertex2f(x + 1, y);
                glVertex2f(x + 1, y);
                glVertex2f(x + 1, y + 1);
                glVertex2f(x + 1, y + 1);
                glVertex2f(x, y + 1);
                glVertex2f(x, y + 1);
                glVertex2f(x, y);
                glEnd();
            }
        }
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    if (!over)
        mapMatrix();
    glutSwapBuffers();
}

void printMat()
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

block b;
int t = 0;

void timer(int)
{
    if (!paused)
    {
        if (t == 0)
        {
            b.blockInit();
            t = 1;
        }
        if (!b.process())
        {
            if (b.x < R)
            {
                b.outClean();
                b.blockInit();
            }
        }
        glutPostRedisplay();
        glutTimerFunc(tim, timer, 0.0);
    }
}

void specialFunc(int Key, int x, int y)
{
    switch (Key)
    {
    case GLUT_KEY_UP:
        b.cleanBlock();
        b.blockUp();
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        b.cleanBlock();
        b.blockDown();
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        b.cleanBlock();
        b.blockLeft();
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        b.cleanBlock();
        b.blockRight();
        glutPostRedisplay();
        break;
    case GLUT_KEY_INSERT:
        glutPostRedisplay();
        break;
    default:
        cout << "error" << endl;
    }
}

void display()
{
    glutInitWindowSize(width, height);
    over = false;
    glutInitWindowPosition(200, 0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialFunc);
    glutTimerFunc(0, timer, 0);
    init();
    glutMainLoop();
}

int main(int argc, char **argv)
{
    matInit();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(200, 0);
    glutCreateWindow("Tetris");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
    return 0;
}