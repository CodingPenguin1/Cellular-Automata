#include <GL/glut.h>
#include <chrono>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>


#define DISPLAY_SIZE 1000


uint8_t grid[DISPLAY_SIZE * DISPLAY_SIZE];
uint8_t current_bit = 1;

double start_time;
const int framerate_average_count = 5;
double *frame_times = new double[framerate_average_count];


double time() {
    return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now().time_since_epoch()).count();
}


void draw_string(float x, float y, std::string s) {
    glRasterPos2f(x, y);
    for (int i = 0; i < (int)s.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
}


void update_grid() {
    // for (int i = 0; i < 1; ++i) {
    //     uint8_t bit_0 = (grid[i] >> 1) & 1;
    //     uint8_t bit_1 = (grid[i] >> 2) & 1;
    // }
}


void display() {
    // Setup
    double frame_start_time = time();
    glClear(GL_COLOR_BUFFER_BIT);

    // Update grid
    update_grid();
    glRasterPos2f(-1, -1);
    glDrawPixels(DISPLAY_SIZE, DISPLAY_SIZE, GL_LUMINANCE, GL_UNSIGNED_BYTE, grid);

    // Calculate framerate
    for (int i = 1; i < framerate_average_count; ++i)
        frame_times[i] = frame_times[i + 1];
    frame_times[4] = time() - frame_start_time;
    double framerate = framerate_average_count / (frame_times[0] + frame_times[1] + frame_times[2] + frame_times[3] + frame_times[4]);

    // Render settings text
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    draw_string(-0.99, 0.97, std::to_string((int)framerate) + " FPS");
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}


void init(int argc, char **argv) {
    // Set up window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(DISPLAY_SIZE, DISPLAY_SIZE);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("[floating] Cellular Automata");
    glutDisplayFunc(display);
    glutIdleFunc(glutPostRedisplay);

    // Set up rendering
    glViewport(0, 0, DISPLAY_SIZE, DISPLAY_SIZE);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Set start time of program
    start_time = time();
    printf("Start time: %f\n", start_time);

    // Create empty grid
    for (int i = 0; i < DISPLAY_SIZE * DISPLAY_SIZE; ++i)
        grid[i] = 0;
    for (int i = 0; i < DISPLAY_SIZE; ++i) {
        grid[i + DISPLAY_SIZE * i] = 0b11111110;
        grid[DISPLAY_SIZE - 1 - i + DISPLAY_SIZE * i] = 0b11111110;
    }

    // Initialize framerate average arrayN
    for (int i = 0; i < framerate_average_count; ++i)
        frame_times[i] = 0.0;
}


int main(int argc, char **argv) {
    init(argc, argv);
    glutMainLoop();

    // Exit
    delete[] frame_times;
    return 0;
}
