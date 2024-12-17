#include <GL/freeglut.h>

// Angle of rotation
float angle = 0.0f;

// Function to render the scene
void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

    glLoadIdentity(); // Reset transformations
    glTranslatef(0.0f, 0.0f, -5.0f); // Move the object back

    // Rotate the object
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotate around the Y-axis

    // Draw a triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // Red vertex
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);  // Green vertex
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);   // Blue vertex
    glEnd();

    glutSwapBuffers(); // Swap buffers (for double buffering)
}

// Function to handle the idle state
void Update() {
    angle += 0.1f; // Increment the angle
    if (angle > 360.0f) angle -= 360.0f;

    glutPostRedisplay(); // Request a redraw
}

// Function to initialize OpenGL
void InitializeGL() {
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set a dark background color
}

// Entry point
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Double-buffered, RGB, with depth testing
    glutInitWindowSize(800, 600); // Window dimensions
    glutCreateWindow("FreeGLUT Test"); // Create the window with a title

    InitializeGL(); // Initialize OpenGL settings

    // Register callback functions
    glutDisplayFunc(RenderScene); // Render function
    glutIdleFunc(Update);         // Idle function for animation

    glutMainLoop(); // Enter the FreeGLUT main loop
    return 0;
}