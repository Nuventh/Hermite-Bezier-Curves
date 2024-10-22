#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>

std::vector<float> points;  // Stores the user-placed points
bool isBezier = false;      // Flag to switch between Hermite and Bezier

// Helper function to compute factorial
int factorial(int n) {
    return (n == 0 || n == 1) ? 1 : n * factorial(n - 1);
}

// Helper function to compute binomial coefficient
int binomialCoefficient(int n, int k) {
    return factorial(n) / (factorial(k) * factorial(n - k));
}

void drawArrow(float x1, float y1, float x2, float y2) {
    float angle = atan2(y2 - y1, x2 - x1);
    float arrowLength = 7.5f;
    float arrowAngle = 0.4f;

    glVertex2f(x2, y2);
    glVertex2f(x2 - arrowLength * cos(angle - arrowAngle), y2 - arrowLength * sin(angle - arrowAngle));
    glVertex2f(x2, y2);
    glVertex2f(x2 - arrowLength * cos(angle + arrowAngle), y2 - arrowLength * sin(angle + arrowAngle));
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the user-placed points
    glColor3f(1.0, 1.0, 0.0);
    glPointSize(10.0);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < points.size(); i += 2) {
        glVertex2f(points[i], points[i + 1]);
    }
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    // Draw tangent vectors as lines only in Hermite mode
    if (!isBezier) {
        glColor3f(1.0, 1.0, 0.0);
        glLineWidth(2.0);
        glBegin(GL_LINES);

        for (size_t i = 0; i + 3 < points.size(); i += 2) {
            float x0 = points[i], y0 = points[i + 1];
            float tx0 = (i > 1) ? (points[i + 2] - points[i - 2]) * 0.5f : (points[i + 2] - points[i]) * 0.5f;
            float ty0 = (i > 1) ? (points[i + 3] - points[i - 1]) * 0.5f : (points[i + 3] - points[i + 1]) * 0.5f;

            // Draw the tangent vector at point (x0, y0)
            glVertex2f(x0, y0);
            glVertex2f(x0 + tx0, y0 + ty0);

            // Draw an arrow at the end of the tangent vector
            drawArrow(x0, y0, x0 + tx0, y0 + ty0);

            if (i + 2 < points.size()) {
                float x1 = points[i + 2], y1 = points[i + 3];
                float tx1 = (i + 4 < points.size()) ? (points[i + 4] - points[i]) * 0.5f : (points[i + 2] - points[i]) * 0.5f;
                float ty1 = (i + 4 < points.size()) ? (points[i + 5] - points[i + 1]) * 0.5f : (points[i + 3] - points[i + 1]) * 0.5f;
                // Draw the tangent vector at point (x1, y1)
                glVertex2f(x1, y1);
                glVertex2f(x1 + tx1, y1 + ty1);
                // Draw an arrow at the end of the tangent vector
                drawArrow(x1, y1, x1 + tx1, y1 + ty1);
            }
        }
        glEnd();
    }

    // Draw the control polygon for Bezier curves
    if (isBezier && points.size() >= 4) {
        glColor3f(0.8, 0.8, 0.8);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(3, 0xAAAA);

        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < points.size(); i += 2) {
            glVertex2f(points[i], points[i + 1]);
        }
        glEnd();

        glDisable(GL_LINE_STIPPLE);
    }

    // Draw the curves
    if (isBezier && points.size() >= 4) {  // Bezier curve
        glLineWidth(3.0);
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINE_STRIP);

        for (float t = 0.0; t <= 1.0; t += 0.01) {
            float x = 0.0, y = 0.0;
            size_t n = points.size() / 2 - 1;

            for (size_t i = 0; i <= n; ++i) {
                float b = binomialCoefficient(n, i) * std::pow(t, i) * std::pow(1 - t, n - i);
                x += b * points[2 * i];
                y += b * points[2 * i + 1];
            }
            glVertex2f(x, y);
        }
        glEnd();
    }
    else if (!isBezier && points.size() >= 8) {  // Hermite curve
        glLineWidth(3.0);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_STRIP);

        for (size_t i = 2; i + 1 < points.size(); i += 2) {
            float x0 = points[i - 2], y0 = points[i - 1];
            float x1 = points[i], y1 = points[i + 1];

            // Calculate tangents
            float tx0 = (i > 2) ? (points[i] - points[i - 4]) * 0.5f : (points[i] - points[i - 2]) * 0.5f;
            float ty0 = (i > 2) ? (points[i + 1] - points[i - 3]) * 0.5f : (points[i + 1] - points[i - 1]) * 0.5f;
            float tx1 = (i + 2 < points.size()) ? (points[i + 2] - points[i - 2]) * 0.5f : (points[i] - points[i - 2]) * 0.5f;
            float ty1 = (i + 2 < points.size()) ? (points[i + 3] - points[i - 1]) * 0.5f : (points[i + 1] - points[i - 1]) * 0.5f;

            for (float t = 0.0; t <= 1.0; t += 0.01) {
                float h00 = 2 * std::pow(t, 3) - 3 * std::pow(t, 2) + 1;
                float h10 = std::pow(t, 3) - 2 * std::pow(t, 2) + t;
                float h01 = -2 * std::pow(t, 3) + 3 * std::pow(t, 2);
                float h11 = std::pow(t, 3) - std::pow(t, 2);
                float x = h00 * x0 + h10 * tx0 + h01 * x1 + h11 * tx1;
                float y = h00 * y0 + h10 * ty0 + h01 * y1 + h11 * ty1;
                glVertex2f(x, y);
            }
        }
        glEnd();
    }
    glFlush();
    glutSwapBuffers();
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && points.size() < 24) {
        // Normalize the coordinates
        float normX = (float)x;
        float normY = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y);

        // Add the clicked point to the points vector
        points.push_back(normX);
        points.push_back(normY);
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    std::string message;
    if (key == 'h' || key == 'H') {
        isBezier = false;
        message = "Current view: Hermite curve";
    }
    else if (key == 'b' || key == 'B') {
        isBezier = true;
        message = "Current view: Bezier curve";
    }
    std::cout << "\r" << std::string(50, ' ') << "\r" << message << std::flush;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    std::cout << "Press b/B or h/H to switch between Bezier and Hermite curve" << std::endl;
    std::cout << "Current view: Hermite curve" << std::flush;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Hermite and Bezier Curves");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 800.0);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
