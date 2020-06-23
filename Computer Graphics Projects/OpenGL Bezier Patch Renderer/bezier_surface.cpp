#include <stdlib.h>
#include <iostream>
#include <algorithm>

#ifdef __APPLE__
   #include <GLUT/glut.h>
#endif
#ifdef __linux__
   #include <GL/glut.h>
#endif


// Variables to keep amount of rotation in camera movement
float xrot = 0.0f;
float yrot = 0.0f;

// Pressing 'E' enables the editing mode
bool edit_mode = false;

// These variables indicate which bezier curve is being edited
int edit_index_i = -1;
int edit_index_j = -1;

// This variable indicate which point on the curve is being edited
int edit_point_i = 0;

// Resolution of each bezier curve (number of vertices that constitute the curve) 
int resolution = 50;

// Surface color in RGB format (0-1)
float surface_color[3] = {0.0, 1.0, 1.0};

// Initial control points of the surface
GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5, 1.0}, {-0.5, -1.5, 1.0}, 
    {0.5, -1.5, 1.0}, {1.5, -1.5, 1.0}}, 
   {{-1.5, -0.5, 0.0}, {-0.5, -0.5, 0.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, 0.0}}, 
   {{-1.5, 0.5, 0.0}, {-0.5, 0.5, 0.0}, 
    {0.5, 0.5, 0.0}, {1.5, 0.5, 0.0}}, 
   {{-1.5, 1.5, 1.0}, {-0.5, 1.5, 1.0}, 
    {0.5, 1.5, 1.0}, {1.5, 1.5, 1.0}}
};

GLfloat edit_j_ctrlpoints[4][3];


void display(void) {
   /* Displaying function that will be given as parameter to glutDisplayFunc */

   int i, j;

   // Enable lighting only in viewing mode.
   if (edit_mode){
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
   } else{
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
   }

   // Evaluator function to generate vertices of bezier patch defined by ctrlpoints 
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(resolution, 0.0, 1.0, resolution, 0.0, 1.0);

   // Clear the color and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Set the surface color
   glColor3fv(&surface_color[0]);

   // Transformations will be applied to the matrixes defined after here.
   glPushMatrix ();

   // Rotating the camera according to mouse input
   glRotatef(yrot, 0.0f, 1.0f, 0.0f);
   glRotatef(xrot, 1.0f, 0.0f, 0.0f);

   if (edit_mode){
      // EDITING MODE

      // Visualize the bezier curves that define the bezier patch
      glLineWidth(1.0);
      glEvalMesh2(GL_LINE, 0, resolution, 0, resolution);
      
      // Visualize the control points of the patch and curves
      glPointSize(5.0);
      glColor3f(1.0, 1.0, 0.0);
      for(i = 0; i < 4; i++){
         for(j = 0; j < 4; j++){
            glBegin(GL_POINTS);
               glVertex3fv(&ctrlpoints[i][j][0]);
            glEnd();
         }
      }

      // Highlight the curve and point that are selected to be modifed by the user
      // Selected curve is bold and white, selected point is bold and yellow.
      if(edit_index_i != -1 && edit_index_j == -1){
         // If the curve is selected with W-S buttons
         glColor3f(1.0, 1.0, 1.0);
         glLineWidth(5.0);

         // Draw the selected curve again with larger width
         glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[edit_index_i][0][0]);
         glEnable(GL_MAP1_VERTEX_3);
         glBegin(GL_LINE_STRIP);
            for (i = 0; i <= resolution; i++) 
               glEvalCoord1f((GLfloat) i/resolution);
         glEnd();

         // Draw the selected point
         glColor3f(0.0, 0.0, 1.0);
         glPointSize(15.0);
         glBegin(GL_POINTS);
            glVertex3fv(&ctrlpoints[edit_index_i][edit_point_i][0]);
         glEnd();
      } 
      else if (edit_index_i == -1 && edit_index_j != -1){
         // If the curve is selected with A-D buttons

         // Getting the control points of the selected curve is more complicated in this case.
         // I used another array to store the control points of selected curve (edit_j_ctrlpoints)
         glColor3f(1.0, 1.0, 1.0);
         glLineWidth(5.0);
         for (i = 0; i < 3; i++){
            for (j = 0; j < 4; j++){
               edit_j_ctrlpoints[j][i] = ctrlpoints[j][edit_index_j][i];
            }
         }
         // Highlight the selected curve
         glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &edit_j_ctrlpoints[0][0]);
         glEnable(GL_MAP1_VERTEX_3);
         glBegin(GL_LINE_STRIP);
            for (i = 0; i <= resolution; i++) 
               glEvalCoord1f((GLfloat) i/resolution);
         glEnd();

         // Highlight the selected point
         glColor3f(0.0, 0.0, 1.0);
         glPointSize(15.0);
         glBegin(GL_POINTS);
            glVertex3fv(&edit_j_ctrlpoints[edit_point_i][0]);
         glEnd();
      }
   } else {
      // VIEWING MODE
      glEvalMesh2(GL_FILL, 0, resolution, 0, resolution);
   }
   glPopMatrix ();
   glFlush();
}


void init(void){
   // Lighting initializations
   // For the implementation of lighting, I benefited from the following source (http://glprogramming.com/red/chapter05.html)
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

   // Default color used for cleaning the screen
   glClearColor (0.0, 0.0, 0.0, 0.0);
   // Smooth shading interpolates the color inside each polygon, while flat shading uses the same color values in whole polygon.
   glShadeModel (GL_SMOOTH);

   // Settings for lighting
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_AUTO_NORMAL);
}


void reshape(int w, int h) {
   // Reshape function for glutReshapeFunc. This function is activated every time window size 
   // is changed by the user. Main functionality is to adapt the coordinate system for varying window size.
   // My reference is the following link (http://glprogramming.com/red/chapter03.html)
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
               5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
   else
      glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 
               5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void mouseMotion(int x, int y){
   // Every time mouse is moved, this function activated and the coordinates of the cursor are stored.
   yrot = x;
   xrot = y;
   glutPostRedisplay();
}


void keyboardInput(unsigned char key, int x, int y) {
   // Keybord input function
   // Buttons can be used to enable/disable editing mode, change the location of control points or change the color of the surface. 

   if (key == 'c' || key == 'C'){
      std::cout<<"Enter color in RGB format"<<std::endl;
      std::cin >> surface_color[0] >> surface_color[1] >> surface_color[2];
      glutPostRedisplay();
      return;
   }

   if (key == 'r' || key == 'R'){
      std::cout<<"Enter resolution as integer (1 is minimum)"<<std::endl;
      int input_res;
      std::cin >> input_res;
      resolution = std::max(1, input_res);
      glutPostRedisplay();
      return;
   }

   if (key == 'e' || key == 'E'){
      if (edit_mode){
         edit_mode = false;
      } else {
         edit_mode = true;
      }
      glutPostRedisplay();
      return;  
   } 
   else {
      if (!edit_mode)
         return;
   } 
   
   if (key == 'w' || key == 'W'){ 
      edit_index_j = -1;
      edit_index_i = std::min(std::max(0, edit_index_i+1), 3);
      glutPostRedisplay();
      return;
   }

   if (key == 's' || key == 'S'){
      edit_index_j = -1;
      edit_index_i = std::min(std::max(0, edit_index_i-1), 3);
      glutPostRedisplay();
      return;
   }

   if (key == 'd' || key == 'C'){
      edit_index_i = -1;
      edit_index_j = std::min(std::max(0, edit_index_j+1), 3);
      glutPostRedisplay();
      return;
   } 

   if (key == 'a' || key == 'A'){
      edit_index_i = -1;
      edit_index_j = std::min(std::max(0, edit_index_j-1), 3);
      glutPostRedisplay();
      return;
   }

   if (edit_index_i != -1 || edit_index_j != -1){
      if (key == '1'){
         edit_point_i = 0;
         glutPostRedisplay();
         return;
      }
      if (key == '2'){
         edit_point_i = 1;
         glutPostRedisplay();
         return;
      }
      if (key == '3'){
         edit_point_i = 2;
         glutPostRedisplay();
         return;
      }
      if (key == '4'){
         edit_point_i = 3;
         glutPostRedisplay();
         return;
      }
   }
   return;
}


void modifyControlPoints(int key, int x, int y) {
   // Using arrow and F1-F2 keys, user can change the location of a control point
   // This function reads the keybord input and modify the coordinates of the selected control point
   // Each button hit changes the corresponding coordinate 0.25 units

   if (!edit_mode || (edit_index_i == -1 && edit_index_j == -1))
      return;
   
   if (edit_index_i != -1){
      if (key==GLUT_KEY_UP){
         ctrlpoints[edit_index_i][edit_point_i][1] += 0.25;
         glutPostRedisplay();
         return; 
      }
      if (key==GLUT_KEY_DOWN){
         ctrlpoints[edit_index_i][edit_point_i][1] -= 0.25;
         glutPostRedisplay();
         return; 
      }
      if (key==GLUT_KEY_RIGHT){
         ctrlpoints[edit_index_i][edit_point_i][0] += 0.25;
         glutPostRedisplay();
         return; 
      }
      if (key==GLUT_KEY_LEFT){
         ctrlpoints[edit_index_i][edit_point_i][0] -= 0.25;
         glutPostRedisplay();
         return;
      }
      if (key==GLUT_KEY_F1) {
         ctrlpoints[edit_index_i][edit_point_i][2] += 0.25;
         glutPostRedisplay();
         return;
      }
      if (key==GLUT_KEY_F2) {
         ctrlpoints[edit_index_i][edit_point_i][2] -= 0.25;
         glutPostRedisplay();
         return;
      }
   }

   if (edit_index_j != -1){
      if (key==GLUT_KEY_UP){
         ctrlpoints[edit_point_i][edit_index_j][1] += 0.25;
         glutPostRedisplay();
         return; 
      }
      if (key==GLUT_KEY_DOWN){
         ctrlpoints[edit_point_i][edit_index_j][1] -= 0.25;
         glutPostRedisplay();
         return; 
      }
      if (key==GLUT_KEY_RIGHT){
         ctrlpoints[edit_point_i][edit_index_j][0] += 0.25;
         glutPostRedisplay();
         return; 
      }
      if (key==GLUT_KEY_LEFT){
         ctrlpoints[edit_point_i][edit_index_j][0] -= 0.25;
         glutPostRedisplay();
         return;
      }
      if (key==GLUT_KEY_F1) {
         ctrlpoints[edit_point_i][edit_index_j][2] += 0.25;
         glutPostRedisplay();
         return;
      }
      if (key==GLUT_KEY_F2) {
         ctrlpoints[edit_point_i][edit_index_j][2] -= 0.25;
         glutPostRedisplay();
         return;
      }
   }
   return;
}


int main(int argc, char** argv)
{
   // Initializing GLUT window
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);

   // Call functions to start the rendering and other features
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMotionFunc(mouseMotion);
   glutKeyboardFunc(keyboardInput);
   glutSpecialFunc(modifyControlPoints);

   // Glut Main Loop
   glutMainLoop();
   return 0;
}
