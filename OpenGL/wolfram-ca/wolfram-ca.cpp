#include <GL/glut.h>
#include <chrono>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>


#define WIDTH 1000
#define HEIGHT 1000


uint32_t buffer_a[WIDTH * HEIGHT];
uint32_t buffer_b[WIDTH * HEIGHT];
uint32_t *grid[2] = {buffer_a, buffer_b};
uint8_t current_buffer = 0;

double start_time;
const int framerate_average_count = 5;
double *frame_times = new double[framerate_average_count];
uint8_t rule = 101;
const uint32_t OFF_COLOR = 0x000000ff;
const uint32_t ON_COLOR = 0xffffffff;
int row = HEIGHT - 1;


double time() {
    return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now().time_since_epoch()).count();
}


void draw_string(float x, float y, std::string s) {
    glRasterPos2f(x, y);
    for (int i = 0; i < (int)s.size(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
}


void update_grid() {
    // Based on work by The Coding Train on YouTube
    // https://www.youtube.com/watch?v=W1zKu3fDQR8
    uint8_t next_buffer = current_buffer ? 0 : 1;

    // If current row is bottom row, shift all rows up 1
    if (row == HEIGHT - 1)
        for (int i = WIDTH; i < WIDTH * HEIGHT; ++i)
            grid[current_buffer][i - WIDTH] = grid[current_buffer][i];

    // Copy current buffer to next buffer
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
        grid[next_buffer][i] = grid[current_buffer][i];

    for (int col = 0; col < HEIGHT; ++col) {
        // Get state of current cell
        int i = row * WIDTH + col;
        uint8_t current_cell_state = 0b00000000;
        if (grid[current_buffer][i] > OFF_COLOR)
            current_cell_state = 0b00000010;

        // Get state of cell left of current cell
        int left_cell_index = i - 1;
        if (col == 0)
            left_cell_index = row * WIDTH + (HEIGHT - 1);
        uint8_t left_cell_state = 0b00000000;
        if (grid[current_buffer][left_cell_index] > OFF_COLOR)
            left_cell_state = 0b00000100;

        // Get state of cell right of current cell
        int right_cell_index = i + 1;
        if (col == HEIGHT - 1)
            right_cell_index = row * WIDTH;
        uint32_t right_cell_state = 0b00000000;
        if (grid[current_buffer][right_cell_index] > OFF_COLOR)
            right_cell_state = 0b00000001;

        // Combine into a single value of neighborhood configuration
        uint8_t configuration = left_cell_state | current_cell_state | right_cell_state;

        // Get next generation state for current cell column (value to put in next row, same column as current cell)
        uint8_t next_state = (rule & (0x01 << configuration)) >> configuration;
        if (next_state)
            grid[next_buffer][i] = ON_COLOR;
        else
            grid[next_buffer][i] = OFF_COLOR;


        // grid[next_buffer][WIDTH * row + row] = 0xff000000;
    }

    if (row < HEIGHT - 1)
        ++row;
    current_buffer = next_buffer;
}


void display() {
    // Setup
    double frame_start_time = time();
    glClear(GL_COLOR_BUFFER_BIT);

    // Update grid
    update_grid();
    glRasterPos2f(-1, -1);
    glDrawPixels(WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, grid[current_buffer]);

    // Calculate framerate
    for (int i = 1; i < framerate_average_count; ++i)
        frame_times[i] = frame_times[i + 1];
    frame_times[4] = time() - frame_start_time;
    double framerate = framerate_average_count / (frame_times[0] + frame_times[1] + frame_times[2] + frame_times[3] + frame_times[4]);

    // Render settings text
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.0, 0.0, 1.0);
    draw_string(-0.99, 0.97, std::to_string((int)framerate) + " FPS");
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
    // sleep(1);
}


void init(int argc, char **argv) {
    // Set up window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("[floating] Cellular Automata");
    glutDisplayFunc(display);
    glutIdleFunc(glutPostRedisplay);

    // Set up rendering
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Set start time of program
    start_time = time();
    printf("Start time: %f\n", start_time);

    // Create empty grid
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        grid[0][i] = OFF_COLOR;
        grid[1][i] = OFF_COLOR;
    }

    // Put 1 pixel in the top row, center col
    grid[current_buffer][(WIDTH * HEIGHT) - (WIDTH / 2)] = ON_COLOR;

    // Initialize framerate average array
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
