/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

//======================================================================
// Trace viewer using OpenGL/glut. Syntax: traceview tracefile [enter]
// where "tracefile" is a binary file with a sequence of event records. 
// This version uses a fixed OpenGL ortho-2d mapping, and scales the
// data so that the displayed range is [0-1].  This was found to give
// an accurate display for a wide range of zoom factors and offsets.
//----------------------------------------------------------------------
// Bob Walkup, 2003
//======================================================================

#include <GL/glut.h>
#include <GL/glui.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#ifndef _WIN32
#include <strings.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#if defined _WIN32
#include <wtypes.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#define MAX_IDS 146
#include "mpitrace_ids.h"

//======================================================================
// the event structure used for all trace events
//======================================================================
struct intPair { int src; int dest; };

typedef union { long offset; struct intPair ranks; } unType;

struct eventstruct {
                     double tbeg;
                     double tend;
                     int taskid;
                     int eventid;
                     unType UN; 
                     int bytes;
                     int parent;
                     int grandparent;
                     int ioflag;
                   };

#define EVENT_SIZE 48

//======================================================================
// global variables   
//======================================================================
#define MOUSE_PICKS   0      // the mouse picks events 
#define MOUSE_CENTERS 1      // the mouse centers on the selected x
#define MOUSE_PANS    2      // the mouse pans right/left
#define MOUSE_ZOOMS   3      // the mouse zooms in/out
#define HELP_ID       4      // button to print help message
#define MENU_QUIT     5      // quit from the popup menu

#define ZOOM_IN_ID    10     // button to zoom in
#define ZOOM_OUT_ID   11     // button to zoom out
#define TIME_PLUS_ID  12     // button to shift time +
#define TIME_MINUS_ID 13     // button to shift time -
#define RESET_X_ID    14     // button to reset the x-axis
#define RESET_Y_ID    15     // button to reset the y-axis

#ifdef _WIN32
#define ADDRESS_OFFSET_64BIT 0x100000000    // AIX 64-bit mode address
#else
#define ADDRESS_OFFSET_64BIT 0x100000000LL  // AIX 64-bit mode address
#endif

int main_window;             // main display window
int legend_window;           // window for legends

int mouse_mode = MOUSE_PICKS;// default mode for the mouse

int xpixels = 1280;          // initial x size of the window
int ypixels = 480;           // initial y size of the window
int num_events;              // the number of events
int num_tasks;               // the number of parallel tasks
int min_rank;                // smallest rank in the set
int max_rank;                // largest  rank in the set

int legend_xsize;            // initial x size of legend box
int legend_ysize;            // initial y size of legend box

int moving = 0;              // check for a moving mouse
int ix_start;                // save mouse x coordinate
int iy_start;                // save mouse y coordinate
int text_color = 1;          // toggle text color for time values

double ymin;                 // ymin for the display window
double ymax;                 // ymax for the display window
double ymin_orig;            // original value for ymin
double ymax_orig;            // original value for ymax
double ycenter;              // the center y value
double yrange;               // the range of y values
double xmin;                 // xmin for the display window
double xmax;                 // xmax for the display window
double xmin_orig;            // original value for xmin
double xmax_orig;            // original value for xmax
double xcenter;              // the center x value
double xrange;               // the range of x values
double xscale;               // xscale = 1.0/(xpixels - 1)         

double recip_rand_max;       // for random numbers

int draw_zoom_box = 0;       // flag to draw zoom box
double zoom_x1;              // left  edge of zoom box
double zoom_x2;              // right edge of zoom box

struct eventstruct * event;  // the array of events
double * evbeg;              // array of scaled event begin times
double * evend;              // array of scaled event end   times

double * init_color = NULL;     // initial choice for color assignments
double * current_color = NULL;  // current choice for color assignments
int max_colors = 8;             // initial choice for number of color choices

int event_is_present[MAX_IDS]; // flag set to 1 if the event is present
int num_mpi_functions;         // total number of mpi functions present
char label[MAX_IDS][80];       // event labels
double color[MAX_IDS][3];      // event colors

GLUI_RadioGroup *radio;        // radio button group

//======================================================================
// function prototypes
//======================================================================
void draw(void);
void draw_legend(void);
void reshape(int width, int height);
void reshape_legend(int width, int height);
void key(unsigned char, int, int);
void special(int, int, int);
void mouse(int, int, int, int);
void motion(int, int);
void menu(int);
void button_callback(int);
void set_labels_and_colors(void);
void read_tracefile(char *);
void identify_event(int, int);
void rescale(struct eventstruct *, double *, double *, int, 
             double, double);
void bitmap_text(double, double, char *);
void reverse_byte_order(char *, char *);
void swap8(char *, char *);
void swap4(char *, char *);

double urand1(void);

void mpitrace_sortx(double * arr , int n, int * ind, int flag);
void random_colors(void);

//======================================================================
// the main routine
//======================================================================
int main(int argc, char * argv[])
{

  if (argc != 2)
  {
    printf("Syntax: traceview tracefile [enter]\n");
    exit(0);
  }

  recip_rand_max = 1.0 / ((double) RAND_MAX);

  read_tracefile(argv[1]);

  set_labels_and_colors();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  xscale = 1.0/((double) (xpixels - 1));

  // setup the events window
  glutInitWindowPosition(20, 40);
  glutInitWindowSize(xpixels,  ypixels);
  main_window = glutCreateWindow("Events");
  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  // setup the legends window
  legend_ysize = num_mpi_functions*20;
  legend_xsize = 150;
  glutInitWindowPosition(60+xpixels, 220);
  glutInitWindowSize(legend_xsize, legend_ysize);
  legend_window = glutCreateWindow("Legend");
  glutDisplayFunc(draw_legend);
  glutReshapeFunc(reshape_legend);

  //glui user interface code
  GLUI *glui = GLUI_Master.create_glui("Traceview", 0, 60+xpixels, 40);
  GLUI_Panel *mouse_panel = glui->add_panel( "Mouse Function" );
  radio = glui->add_radiogroup_to_panel(mouse_panel, &mouse_mode, -1, menu);
  glui->add_radiobutton_to_group(radio, "Identify");
  glui->add_radiobutton_to_group(radio, "Center");
  glui->add_radiobutton_to_group(radio, "Pan");
  glui->add_radiobutton_to_group(radio, "Zoom");
  glui->add_button("Help",     HELP_ID,       button_callback);
  glui->add_button("Quit", 0, (GLUI_Update_CB) exit);

  glui->add_column(false);
  glui->add_button("Zoom In",  ZOOM_IN_ID,    button_callback);
  glui->add_button("Zoom Out", ZOOM_OUT_ID,   button_callback);
  glui->add_button("Time +",   TIME_PLUS_ID,  button_callback);
  glui->add_button("Time -",   TIME_MINUS_ID, button_callback);
  glui->add_button("Reset x",  RESET_X_ID,    button_callback);
  glui->add_button("Reset y",  RESET_Y_ID,    button_callback);
  
  glui->set_main_gfx_window(main_window);
  
  glutMainLoop();
  return 0;
}

//======================================================================
// button callback function
//======================================================================
void button_callback(int button)
{
  double dx;

  switch(button)
  {
    case ZOOM_IN_ID:  // zoom in
      xrange *= 0.9090909090909091;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case ZOOM_OUT_ID:  // zoom out
      xrange *= 1.1000000000000000;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case TIME_PLUS_ID:  // shift time forward
      dx =  0.005*xrange;
      xcenter += dx;
      xmin += dx;
      xmax += dx;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case TIME_MINUS_ID:  // shift time backward
      dx = -0.005*xrange;
      xcenter += dx;
      xmin += dx;
      xmax += dx;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case RESET_X_ID:    // reset the x axis
      xmin = xmin_orig;
      xmax = xmax_orig;
      xcenter = 0.5*(xmin_orig + xmax_orig);
      xrange = xmax_orig - xmin_orig;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case RESET_Y_ID:    // reset the y axis
      ymin = ymin_orig;
      ymax = ymax_orig;
      ycenter = 0.5*(ymin_orig + ymax_orig);
      yrange = ymax_orig - ymin_orig;
      break;

    case HELP_ID:  // print key assignments
      printf("\n");
      printf("For most operations there are shortcut keys listed below.  The mouse can\n");
      printf("be used in different modes selected by radio buttons in the traceview\n");
      printf("window.  In \"Identify\" mode, clicking on an MPI event with the left mouse\n");
      printf("button will print information about that event to the console window.  A\n");
      printf("zoom-to-region feature can be used by clicking with the right mouse button\n");
      printf("on the left boundary, then dragging the mouse to the right boundary in\n");
      printf("the \"Events\" window.  The mouse can also be used to center the display at\n");
      printf("a selected position, or to pan right/left by dragging with the left mouse\n");
      printf("down.  In \"Zoom\" mode, you can select a region to zoom into by clicking\n");
      printf("with the left mouse button on the left boundary, and dragging the mouse to\n");
      printf("the right boundary.  It is expected that the \"Identify\" mode will be\n");
      printf("used for most purposes.\n\n");
      printf("Key assignments for the events window:\n");
      printf(" \"x\" => x-axis zoom in  (time axis)\n");
      printf(" \"X\" => x-axis zoom out (time axis)\n");
      printf(" \"z\" => x-axis zoom in  (time axis)\n");
      printf(" \"Z\" => x-axis zoom out (time axis)\n");
      printf(" \"w\" => x-axis zoom in  (time axis) small increment\n");
      printf(" \"W\" => x-axis zoom out (time axis) small increment\n");
      printf(" \"s\" => print the time-scale range for the current window\n");
      printf(" \">\" => shift to larger  times (small step)\n");
      printf(" \"<\" => shift to smaller times (small step)\n");
      printf(" \"right arrow\" => shift to larger  times (big step)\n");
      printf(" \"left  arrow\" => shift to smaller times (big step)\n");
      printf(" \"y\" => y-axis zoom in  (MPI task axis)\n");
      printf(" \"Y\" => y-axis zoom out (MPI task axis)\n");
      printf(" \"u\" => shift to larger MPI task ids (small step)\n");
      printf(" \"U\" => shift to smaller MPI task ids (small step)\n");
      printf(" \"up   arrow\" => shift to larger MPI task ids (medium step)\n");
      printf(" \"down arrow\" => shift to smaller MPI task ids (medium step)\n");
      printf(" \"page up  \" => skip to larger MPI task ids (big step)\n");
      printf(" \"page down\" => skip to smaller MPI task ids (big step)\n");
      printf(" \"f\" => find an MPI routine, searching forward from the screen center\n");
      printf(" \"c\" => randomize the assignment of colors to MPI routines \n");
      printf(" \"r\" => reset     the assignment of colors to MPI routines \n");
      printf(" \"t\" => toggle the text color white/black \n");
      printf(" \"q\"   => quit\n");
      printf(" \"Esc\" => quit\n");
      printf("\n");
      break;

    default:
      break;
  }

  glutPostRedisplay();

  return;
}

//======================================================================
// routine to draw the trace events 
//======================================================================
void draw(void)
{
  int i, eventid;
  double x1, x2, y1, y2;
  double ytop, ymid, ybot;
  double red, green, blue;
  double xpix, deltax, xsize_in_pixels, intensity;
  double ypix, yscale, ysize_in_pixels;
  char xlstring[16];
  char xcstring[16];
  char xrstring[16];
  char ytstring[16];
  char ymstring[16];
  char ybstring[16];

  glViewport(0, 0, xpixels, ypixels);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1000.0, ymin, ymax);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  xpix = (double) xpixels;
  ypix = (double) ypixels;

  // for anti-aliasing in the y dimension
  yscale = (ymax - ymin)/ypix;
  ysize_in_pixels = 10.0/yscale;

  for (i=0; i<num_events; i++)
  {
     x1 = evbeg[i];
     x2 = evend[i];
     if ( (x1<1000.0) && (x2>0.0) )
     {
        eventid = event[i].eventid;
        red   = color[eventid][0];
        green = color[eventid][1];
        blue  = color[eventid][2];
        y1 = ((double) (event[i].taskid - min_rank)) * 10.0;
        y2 = y1 + 8.0;
        deltax = x2 - x1;
        xsize_in_pixels = 1.0e-3*deltax*xpix;
        // poor man's anti-aliasing - x dimension
        if (xsize_in_pixels < 1.5) 
        {
           x1 -= 0.5*(1.5 - deltax);
           x2 += 0.5*(1.5 - deltax);
           intensity = 0.40*(1.0 + xsize_in_pixels);
           red   *= intensity;
           green *= intensity;
           blue  *= intensity;
        }
        // poor man's anti-aliasing - y dimension 
        if (ysize_in_pixels < 1.5) 
        {
           y1 -= 0.5*yscale*(1.5 - ysize_in_pixels);
           y2 += 0.5*yscale*(1.5 - ysize_in_pixels);
           intensity = 0.40*(1.0 + ysize_in_pixels);
           red   *= intensity;
           green *= intensity;
           blue  *= intensity;
        }
        glColor3d(red, green, blue);
        glRectd(x1, y1, x2, y2);
     }
  }

  if (draw_zoom_box)
  {
     glColor3d(0.7, 0.7, 0.7);
     glBegin(GL_LINES);
       glVertex3d(zoom_x1, ymin, 0.0);
       glVertex3d(zoom_x1, ymax, 0.0);
       glVertex3d(zoom_x2, ymin, 0.0);
       glVertex3d(zoom_x2, ymax, 0.0);
     glEnd();
  }


  // draw text along x and y axes
  sprintf(xlstring,"%.6lf\n", xcenter - 0.4*xrange);
  sprintf(xcstring,"%.6lf\n", xcenter);
  sprintf(xrstring,"%.6lf\n", xcenter + 0.4*xrange);
  if (text_color == 1) glColor3d(0.85, 0.85, 0.85);
  else                 glColor3d(0.00, 0.00, 0.00);
  bitmap_text( 60.0, ymin + 0.01*yrange, xlstring);
  bitmap_text(460.0, ymin + 0.01*yrange, xcstring);
  bitmap_text(860.0, ymin + 0.01*yrange, xrstring);
  ytop = floor(0.1*(ymin + 0.9*yrange));
  ymid = floor(0.1*(ymin + 0.5*yrange));
  ybot = floor(0.1*(ymin + 0.1*yrange));
  sprintf(ytstring, "%d", min_rank + (int) ytop);
  sprintf(ymstring, "%d", min_rank + (int) ymid);
  sprintf(ybstring, "%d", min_rank + (int) ybot);
  bitmap_text( 5.0, 10.0*ytop, ytstring);
  bitmap_text( 5.0, 10.0*ymid, ymstring);
  bitmap_text( 5.0, 10.0*ybot, ybstring);

  glColor3d(0.7, 0.7, 0.7);
  glLineStipple(1,0x00FF);
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
    glVertex3d(100.0, ymin, 0.0);
    glVertex3d(100.0, ymax, 0.0);
    glVertex3d(500.0, ymin, 0.0);
    glVertex3d(500.0, ymax, 0.0);
    glVertex3d(900.0, ymin, 0.0);
    glVertex3d(900.0, ymax, 0.0);
  glEnd();
  glDisable(GL_LINE_STIPPLE);

  glutSwapBuffers();
  return;
}

//======================================================================
// routine to draw legends
//======================================================================
void draw_legend(void)
{
  int i, present_count;
  double x1, x2, y1, y2, xinc, yinc;

  glViewport(0, 0, legend_xsize, legend_ysize);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, legend_xsize, 0.0, legend_ysize);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  yinc = legend_ysize/num_mpi_functions;
  xinc = legend_xsize/10;

  present_count = 0;
  for (i=0; i<MAX_IDS; i++)
  {
     if (event_is_present[i])
     {
       x1 =  0.0;
       x2 = xinc; 
       y1 = ((double) present_count) * yinc;
       y2 = y1 + 0.80*yinc;
       glColor3dv(color[i]);
       glRectd(x1, y1, x2, y2);
       glColor3d(1.0, 1.0, 1.0);
       bitmap_text(x2+2.0, y1+3.0, label[i]);
       present_count++;
     }
  }

  glutSwapBuffers();
  return;
}

//======================================================================
// routine to draw bitmapped text
//======================================================================
void bitmap_text(double x, double y, char * string)
{
   int i;

   glRasterPos2d(x, y);
   for(i=0; i<int(strlen(string)); i++) 
   {
     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
   }

   return;
}

//======================================================================
// routine to fix the viewport if the window is resized
//======================================================================
void reshape(int width, int height)
{
  xpixels = width;
  ypixels = height;
  xscale  = 1.0/((double) (xpixels - 1));
  return;
}

//======================================================================
// routine to fix the legend box if the window is resized
//======================================================================
void reshape_legend(int width, int height)
{
  legend_xsize = width;
  legend_ysize = height;
  return;
}

//======================================================================
// routine to assign keyboard keys certain functions (Esc or q to quit)
//======================================================================
void key(unsigned char k, int x, int y)
{
  int i, match, id, find_event, find_task, imin = 0;
  char routine_name[32];
  char task_string[8];
  char input[80];
  char * ptr = NULL;
  double dx, dy, tmin;

  switch (k)
  {
    case 'q':  // quit
      exit(0);
      break;

    case 27:   // Escape to quit
      exit(0);
      break;

    case 's':  // print the time scale for the current window
      printf("time-window size = %.6lf sec\n", xrange);
      break;

    case 'z':  // Zoom in - x axis
      xrange *= 0.9090909090909091;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case 'Z':  // Zoom out - x axis
      xrange *= 1.1000000000000000;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case 'w':  // Zoom in - x axis
      xrange *= 0.9970089730807577;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case 'W':  // Zoom out - x axis
      xrange *= 1.0030000000000000;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case 'x':  // Zoom in - x axis
      xrange *= 0.9090909090909091;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case 'X':  // Zoom out - x axis
      xrange *= 1.1000000000000000;
      xmin = xcenter - 0.5*xrange;
      xmax = xcenter + 0.5*xrange;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case 'y':  // make the y range smaller => magnify y
      yrange *= 0.87055056329612413914;
      ymin = ycenter - 0.5*yrange;
      ymax = ycenter + 0.5*yrange;
      break;

    case 'Y':  // make the y range larger => demagnify y
      yrange *= 1.1486983549970350068;
      ymin = ycenter - 0.5*yrange;
      ymax = ycenter + 0.5*yrange;
      break;

    case '<':
      dx = -0.002*xrange;
      xcenter += dx;
      xmin += dx;
      xmax += dx;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case '>':
      dx = +0.002*xrange;
      xcenter += dx;
      xmin += dx;
      xmax += dx;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case 't':  // toggle text color
      if (text_color == 0) text_color = 1;
      else                 text_color = 0;
      break;

    case 'c':   // color the MPI routines with random colors
      random_colors();
      glutSetWindow(legend_window);
      glutPostRedisplay();
      glutSetWindow(main_window);
      break;

    case 'r':   // reset colors of MPI routines to the originals
      set_labels_and_colors();
      glutSetWindow(legend_window);
      glutPostRedisplay();
      glutSetWindow(main_window);
      break;

    case 'u':  // pan up in small increments
      dy =  0.01*yrange;
      if (ymax <= ymax_orig + 10.0)
      {
         ymin += dy;
         ymax += dy;
         ycenter += dy;
      }
      break;

    case 'U':  // pan down in small increments
      dy = -0.01*yrange;
      if (ymin >= ymin_orig - 10.0)
      {
         ymin += dy;
         ymax += dy;
         ycenter += dy;
      }
      break;

    case 'f':  // find a routine 
      printf("enter: MPI_Routine taskid/*: ");
      ptr = fgets(input, sizeof(input), stdin); 
      if (ptr == NULL) break;
      match = sscanf(input, "%s %s", routine_name, task_string);
      if (match == 1) task_string[0] = '*';

      match = -1;
      for (id = 0; id < MAX_IDS; id++)
      {
#ifdef _WIN32
         if (0 == strncmp(label[id], routine_name, sizeof(label[id])))
#else
         if (0 == strncasecmp(label[id], routine_name, sizeof(label[id])))
#endif
         {
            match = 1;
            break;
         }
      }

      if (match != 1)
      {
         printf("no match found for routine %s\n", routine_name);
         return;
      }

      find_event = id;

      if (isdigit(task_string[0])) find_task = atoi(task_string);
      else                         find_task = -1;
      
      match = -1; tmin = 1.0e30;
      if (find_task == -1) /* match for any task */
      {
         for (i=0; i<num_events - 1; i++)
         {
             if (event[i].eventid == find_event)
             {
                 if (event[i].tbeg > xcenter)
                 {
                    if (event[i].tbeg < tmin)
                    {
                        imin = i;
                        tmin = event[i].tbeg;
                        match = 1;
                    }
                 }
             }
         }
         if (match == 1)
         {
             xcenter = event[imin].tbeg;
             xmin = xcenter - 0.5*xrange;
             xmax = xcenter - 0.5*xrange;
             printf("found %s in task %d at time %.6lf, duration = %.3lf msec\n",
                     label[find_event], event[imin].taskid, event[imin].tbeg,
                     1.0e3*(event[imin].tend - event[imin].tbeg));
         }
      }
      else /* match for a specific task */
      {
         for (i=0; i<num_events - 1; i++)
         {
            if (event[i].eventid == find_event && event[i].taskid == find_task)
            {
               if (event[i].tbeg > xcenter)
               {
                  match = 1;
                  xcenter = event[i].tbeg;
                  xmin = xcenter - 0.5*xrange;
                  xmax = xcenter - 0.5*xrange;
                  printf("found %s in task %d at time %.6lf, duration = %.3lf msec\n",
                          label[find_event], event[i].taskid, event[i].tbeg,
                          1.0e3*(event[i].tend - event[i].tbeg));
                  break;
               }
            }
         }
      }

      if (match == -1) printf("no match\n"); 
      else rescale(event, evbeg, evend, num_events, xmin, xrange);

      break;

    default:
      return;
  } 

//glutSetWindow(main_window);
  glutPostRedisplay();
  return;
}

//======================================================================
// routine to assign special keyboard keys to chenge viewing angles
//======================================================================
void special(int k, int ix, int iy)
{
  double dx, dy;

  switch (k)
  {
    case GLUT_KEY_LEFT:
      dx = -0.05*xrange;
      xcenter += dx;
      xmin += dx;
      xmax += dx;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case GLUT_KEY_RIGHT:
      dx =  0.05*xrange;
      xcenter += dx;
      xmin += dx;
      xmax += dx;
      rescale(event, evbeg, evend, num_events, xmin, xrange);
      break;

    case GLUT_KEY_UP:
      dy =  0.05*yrange;
      if (ymax <= ymax_orig + 10.0)
      {
         ymin += dy;
         ymax += dy;
         ycenter += dy;
      }
      break;

    case GLUT_KEY_DOWN:
      dy = -0.05*yrange;
      if (ymin >= ymin_orig - 10.0)
      {
         ymin += dy;
         ymax += dy;
         ycenter += dy;
      }
      break;

    case GLUT_KEY_PAGE_UP:
      dy =  0.5*(ymax - ymin);
      if (ymax <= ymax_orig + 0.7*dy)
      {
         ymin += dy;
         ymax += dy;
         ycenter += dy;
      }
      break;

    case GLUT_KEY_PAGE_DOWN:
      dy = 0.5*(ymax - ymin);
      if (ymin >= ymin_orig - 0.7*dy)
      {
         ymin -= dy;
         ymax -= dy;
         ycenter -= dy;
      }
      break;

    default:
      return;
  }

  glutPostRedisplay();
  return;
}

//======================================================================
// routine to respond to mouse clicks
//======================================================================
void mouse(int button, int state, int ix, int iy)
{
   double old_xmin, old_ymin;
   double yscale = 1.0/(ypixels - 1);

   if (mouse_mode == MOUSE_PICKS)
   {
     //--------------------------------------------------
     // click with left mouse button to identify events
     //--------------------------------------------------
     if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN))
     {
        identify_event(ix, iy);
        return;
     }
     else if ((button==GLUT_RIGHT_BUTTON) && (state==GLUT_DOWN))
     {
        moving = 1;
        draw_zoom_box = 1;
        ix_start = ix;
        iy_start = iy;
        return;
     }
     else if ((button==GLUT_RIGHT_BUTTON) && (state==GLUT_UP))
     {
        moving = 0;
        draw_zoom_box = 0;
        if (ix > ix_start)
        {
           old_xmin = xmin;
           xmin = old_xmin + ((double) ix_start)*xscale*xrange;
           xmax = old_xmin + ((double) ix)*xscale*xrange;
           xrange = xmax - xmin;
           xcenter = 0.5*(xmin + xmax);
           rescale(event, evbeg, evend, num_events, xmin, xrange);
        }
        glutPostRedisplay();
        return;
     }
   }
   else if (mouse_mode == MOUSE_CENTERS)
   {
     //--------------------------------------------------
     // click with left mouse button to re-center 
     //--------------------------------------------------
     if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN))
     {
        xcenter = xmin + xrange*((double) ix)*xscale;
        xmin = xcenter - 0.5*xrange;
        xmax = xcenter + 0.5*xrange;
        rescale(event, evbeg, evend, num_events, xmin, xrange);
        glutPostRedisplay();
        return;
     }
   }
   else if (mouse_mode == MOUSE_PANS)
   {
     //--------------------------------------------------
     // drag with the right mouse button to pan or zoom
     //--------------------------------------------------
     if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN))
     {
        moving = 1;
        ix_start = ix;
        iy_start = iy;
        return;
     }

     if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_UP))
     {
        moving = 0;
        return;
     }
   }
   else if (mouse_mode == MOUSE_ZOOMS)
   {
     if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_DOWN))
     {
        moving = 1;
        draw_zoom_box = 1;
        ix_start = ix;
        iy_start = iy;
        return;
     }
     else if ((button==GLUT_LEFT_BUTTON) && (state==GLUT_UP))
     {
        moving = 0; 
        draw_zoom_box = 0; 
        if (ix > ix_start)
        {
           old_xmin = xmin;
           xmin = old_xmin + ((double) ix_start)*xscale*xrange;
           xmax = old_xmin + ((double) ix)*xscale*xrange;
           xrange = xmax - xmin;
           yrange = ymax - ymin;
           xcenter = 0.5*(xmin + xmax);
           if (iy < iy_start) 
           {
               old_ymin = ymin;
               ymin = old_ymin + ((double) (ypixels - iy_start))*yscale*yrange;
               ymax = old_ymin + ((double) (ypixels - iy))*yscale*yrange;
               yrange = ymax - ymin;
               ycenter = 0.5*(ymin + ymax);
           }
           else
           {
               old_ymin = ymin;
               ymax = old_ymin + ((double) (ypixels - iy_start))*yscale*yrange;
               ymin = old_ymin + ((double) (ypixels - iy))*yscale*yrange;
               yrange = ymax - ymin;
               ycenter = 0.5*(ymin + ymax);
           }
           rescale(event, evbeg, evend, num_events, xmin, xrange);
           glutPostRedisplay();
        }
        return;
     }
   }

   return;
}

//======================================================================
// routine to respond to mouse motion
//======================================================================
void motion(int ix, int iy)
{
   double dx;

   switch (mouse_mode)
   {
      case MOUSE_PICKS:
        if (moving)
        {
           zoom_x1 = 1.0e3*((double) ix_start)*xscale;
           zoom_x2 = 1.0e3*((double) ix)*xscale;
           glutPostRedisplay();
        }
        break;

      case MOUSE_PANS:
        if (moving)
        {
           dx = ((double) (ix - ix_start))*xscale;
           xcenter = xcenter - dx*xrange;
           xmin = xcenter - 0.5*xrange;
           xmax = xcenter + 0.5*xrange;
           rescale(event, evbeg, evend, num_events, xmin, xrange);
           ix_start = ix;
           iy_start = iy;
           glutPostRedisplay();
        }
        break;

      case MOUSE_ZOOMS:
        if (moving)
        {
           zoom_x1 = 1.0e3*((double) ix_start)*xscale;
           zoom_x2 = 1.0e3*((double) ix)*xscale;
           glutPostRedisplay();
        }
        break;

      default:
        break;
   }

   return;
}

//======================================================================
// routine to respond to menu selections
//======================================================================
void menu(int option)
{
   switch(option)
   {
      case MOUSE_PICKS:
        mouse_mode = MOUSE_PICKS;
        break;

      case MOUSE_PANS:
        mouse_mode = MOUSE_PANS;
        break;

      case MOUSE_ZOOMS:
        mouse_mode = MOUSE_ZOOMS;
        break;

      case MOUSE_CENTERS:
        mouse_mode = MOUSE_CENTERS;
        break;

      case MENU_QUIT:
        exit(0);

       default:
         break;
   }

   glutPostRedisplay();
   return;
}

//======================================================================
// routine to identify an event from coordinates
//======================================================================
void identify_event(int ix, int iy)
{
  int i, k, yband, eventid, src, dest, bytes;
  double yfrac, ypick, xpick, tbeg, tend;
  long loff;

  yfrac = ((double) (ypixels - iy))/((double) ypixels);
  ypick = ymin + yfrac*yrange;

  yband = (int) (0.10*ypick);

  xpick = xmin + xrange*((double) ix)*xscale;

  k = 0;
  while ((event[k].taskid != (yband + min_rank)) && (k < num_events)) k++;

  for (i=k; i<num_events; i++)
  {
    if ( (event[i].tbeg <= xpick) && (event[i].tend >= xpick) 
        && (event[i].taskid == yband + min_rank) )
    {
      if (event[i].ioflag == 0) 
      {
        eventid = event[i].eventid;
        tbeg  = event[i].tbeg;
        tend  = event[i].tend;
        src   = event[i].UN.ranks.src;
        dest  = event[i].UN.ranks.dest;
        bytes = event[i].bytes;
        printf("task id = %d, event = %s\n", event[i].taskid, label[eventid]);
        printf("  tbeg = %.6lf, tend = %.6lf, duration = %.3lf msec\n",
                  tbeg, tend, 1.0e3*(tend - tbeg)); 
        if (src   != -1)
        {
           if (eventid == WAIT_ID)
              printf("  source = %d (valid only for receive requests)\n", src);
           else
              printf("  source = %d\n", src);
        }
        if (dest  != -1) printf("  destination = %d\n", dest);
        if (bytes != -1) printf("  bytes = %d\n", bytes);
        printf("  parent address = %#10.8x\n", event[i].parent);
        printf("  grandparent address = %#10.8x\n", event[i].grandparent);
        break;
      }
      else
      {
        eventid = event[i].eventid;
        tbeg  = event[i].tbeg;
        tend  = event[i].tend;
        loff  = event[i].UN.offset;
        bytes = event[i].bytes;
        printf("task id = %d, event = %s\n", event[i].taskid, label[eventid]);
        printf("  tbeg = %.6lf, tend = %.6lf, duration = %.3lf msec\n",
                  tbeg, tend, 1.0e3*(tend - tbeg)); 
        if (bytes != -1)  printf("  bytes = %d\n", bytes);
        if (loff  != -1L) printf("  offset = %ld\n", loff);
        printf("  parent address = %#10.8x\n", event[i].parent);
        printf("  grandparent address = %#10.8x\n", event[i].grandparent);
        break;
      }
    }
  }

  return;
}

//======================================================================
// routine to read the input trace file
//======================================================================
void read_tracefile(char * filename)
{
  int i, k, rc, fd;
  int swap_bytes;
  int eventid;
  long bytes_read, bytes_left, num_bytes;
  char * ptr;
  char in[EVENT_SIZE];
  struct stat stat_buffer;

  rc = stat(filename, &stat_buffer);
  if (rc < 0)
  {
    printf("can't find the trace file %s\n", filename);
    exit(0);
  }

  num_bytes = stat_buffer.st_size;

#ifdef _WIN32
  fd = open(filename, O_RDONLY | O_BINARY);
#else
  fd = open(filename, O_RDONLY);
#endif
  if (fd < 0)
  {
    printf("can't open the trace file %s\n", filename);
    exit(0);
  }

  num_events = num_bytes / sizeof(struct eventstruct);
  
  event = (struct eventstruct *) malloc(num_bytes);
  if (event == NULL)
  {
    printf("malloc failed for the event array\n");
    exit(0);
  }

  evbeg = (double *) malloc(num_events*sizeof(double));
  if (evbeg == NULL)
  {
    printf("malloc failed for the evbeg array\n");
    exit(0);
  }

  evend = (double *) malloc(num_events*sizeof(double));
  if (evend == NULL)
  {
    printf("malloc failed for the evend array\n");
    exit(0);
  }

  printf("reading the trace file %s ...", filename);
#if defined _WIN32
  bytes_read = read(fd, event, num_bytes);
  if (bytes_read < 0)
  {
    printf("\nread failed for the trace file %s\n", filename);
    exit(0);
  }
#else
  bytes_left = num_bytes;
  ptr = (char *) event;
  while (bytes_left > 0) 
  {
     bytes_read = read(fd, ptr, bytes_left);
     if (bytes_read < 0) 
     {
        printf("\nread failed for the trace file %s\n", filename);
        exit(0);
     }
     else if (bytes_read == 0) break;
     bytes_left -= bytes_read;
     ptr += bytes_read;
  }
#endif

  close(fd);
  printf("\nfinished reading the trace file\n");
  printf("number of events = %d\n", num_events);

  /*-----------------------------------------------------*/
  /* check for endian mis-match and correct if necessary */
  /*-----------------------------------------------------*/
  swap_bytes = 0;
  for (i=0; i<num_events; i++)
  {
      if (0xFF000000 & event[i].eventid) swap_bytes++;
  }

  if (swap_bytes)
  {
      printf("swapping byte order for the event records ...\n");

      for (i=0; i<num_events; i++)
      {
          ptr = (char *) &event[i].tbeg;

          for (k=0; k<EVENT_SIZE; k++) in[k] = *ptr++;
          
          ptr = (char *) &event[i].tbeg;

          reverse_byte_order(in, ptr);
      }
  }

  for(i=0; i<MAX_IDS; i++) event_is_present[i] = 0;

  xmin_orig = event[0].tbeg;
  xmax_orig = event[0].tend;
  min_rank  = event[0].taskid;
  max_rank  = event[0].taskid;
  for (i=0; i<num_events; i++)
  {
    if (event[i].taskid > max_rank ) max_rank  = event[i].taskid;
    if (event[i].taskid < min_rank ) min_rank  = event[i].taskid;
    if (event[i].tbeg   < xmin_orig) xmin_orig = event[i].tbeg;
    if (event[i].tend   > xmax_orig) xmax_orig = event[i].tend;
    // ensure a minimum 0.1 microsec interval for every event
    if (event[i].tend <= event[i].tbeg) {  
      event[i].tend = event[i].tbeg + 1.0e-7;
      if (i < (num_events - 1)) event[i+1].tbeg += 1.0e-7;
    }
    eventid = event[i].eventid;
    event_is_present[eventid] = 1;
  }

  num_mpi_functions = 0;
  for(i=0; i<MAX_IDS; i++) num_mpi_functions +=  event_is_present[i];
  printf("number of mpi functions = %d\n", num_mpi_functions);

  num_tasks = max_rank - min_rank + 1;
  printf("number of tasks = %d\n", num_tasks);

  // pick ypixels based on the number of mpi tasks
  if (15*num_tasks > 750) ypixels = 750;
  else                    ypixels = 15*num_tasks; 

  legend_xsize = (double) 50;
  legend_ysize = (double) num_mpi_functions*20;

  xcenter = 0.5*(xmin_orig + xmax_orig);
  xrange  = (xmax_orig - xmin_orig);
  ymin_orig = 0.0;
  ymax_orig = ((double) num_tasks) * 10.0;
  xmin = xmin_orig;
  xmax = xmax_orig;
  ymin = ymin_orig;
  ymax = ymax_orig;
  ycenter = 0.5*(ymax + ymin);
  yrange = ymax - ymin;

  // scale the data to [0-1] for initial display
  rescale(event, evbeg, evend, num_events, xmin, xrange);

  printf("--------------------------------------------------------\n");
  printf("Events window: horizontal axis = elapsed time in seconds\n");
  printf("                 vertical axis = MPI task id\n");
  printf("--------------------------------------------------------\n");
  printf("tmin = %.6lf, tmax = %.6lf seconds\n", xmin, xmax);
  fflush(stdout);

  return;
}

//======================================================================
// routine to set labels and colors for each event
//======================================================================
void set_labels_and_colors(void)
{
  int i, j, r, g, b, nmax;
  double inc[4];

  // set a color palette based on the number of functions
  // minimum 12 colors in the palette
  nmax = 2;
  while ((nmax*nmax*nmax - nmax + 6) < num_mpi_functions) nmax++;

  // maximum 66 colors in the palette
  if (nmax > 4) nmax = 4;

  max_colors = nmax*nmax*nmax - nmax + 6;

  if (init_color != NULL) free(init_color);

  init_color = (double *) malloc(3*max_colors*sizeof(double));

  if (nmax == 2) 
  {
     inc[0] = 0.75;
     inc[1] = 1.00;
  }
  else if (nmax == 3) 
  {
     inc[0] = 0.55;
     inc[1] = 0.80;
     inc[2] = 1.00;
  }
  else if (nmax == 4) 
  {
     inc[0] = 0.45;
     inc[1] = 0.64;
     inc[2] = 0.88;
     inc[3] = 1.00;
  }

  i = 0;
  init_color[i]   = 1.0;
  init_color[i+1] = 0.0;
  init_color[i+2] = 0.0;
  i+=3;
  init_color[i]   = 0.0;
  init_color[i+1] = 1.0;
  init_color[i+2] = 0.0;
  i+=3;
  init_color[i]   = 0.0;
  init_color[i+1] = 0.0;
  init_color[i+2] = 1.0;
  i+=3;

  init_color[i]   = 1.0;
  init_color[i+1] = 1.0;
  init_color[i+2] = 0.0;
  i+=3;
  init_color[i]   = 0.0;
  init_color[i+1] = 1.0;
  init_color[i+2] = 1.0;
  i+=3;
  init_color[i]   = 1.0;
  init_color[i+1] = 0.0;
  init_color[i+2] = 1.0;
  i+=3;

  for (b=0; b<nmax; b++) {
    for (g=0; g<nmax; g++) {
      for (r=0; r<nmax; r++) {
        if (r==g && r==b) continue;
        else {
          init_color[i]   = inc[r];
          init_color[i+1] = inc[g];
          init_color[i+2] = inc[b];
          i += 3;
        }
      }
    }
  }

  if (current_color != NULL) free(current_color);

  current_color = (double *) malloc(3*max_colors*sizeof(double));

  for (i=0; i<3*max_colors; i++) current_color[i] = init_color[i];

   strcpy(label[COMM_RANK_ID],                "MPI_Comm_rank");
   strcpy(label[COMM_SIZE_ID],                "MPI_Comm_size");
   strcpy(label[SEND_ID],                     "MPI_Send");
   strcpy(label[SSEND_ID],                    "MPI_Ssend");
   strcpy(label[RSEND_ID],                    "MPI_Rsend");
   strcpy(label[BSEND_ID],                    "MPI_Bsend");
   strcpy(label[ISEND_ID],                    "MPI_Isend");
   strcpy(label[ISSEND_ID],                   "MPI_Issend");
   strcpy(label[IRSEND_ID],                   "MPI_Irsend");
   strcpy(label[IBSEND_ID],                   "MPI_Ibsend");
   strcpy(label[SEND_INIT_ID],                "MPI_Send_init");
   strcpy(label[SSEND_INIT_ID],               "MPI_Ssend_init");
   strcpy(label[RSEND_INIT_ID],               "MPI_Rsend_init");
   strcpy(label[BSEND_INIT_ID],               "MPI_Bsend_init");
   strcpy(label[RECV_INIT_ID],                "MPI_Recv_init");
   strcpy(label[RECV_ID],                     "MPI_Recv");
   strcpy(label[IRECV_ID],                    "MPI_Irecv");
   strcpy(label[SENDRECV_ID],                 "MPI_Sendrecv");
   strcpy(label[SENDRECV_REPLACE_ID],         "MPI_Sendrecv_replace");
   strcpy(label[BUFFER_ATTACH_ID],            "MPI_Buffer_attach");
   strcpy(label[BUFFER_DETACH_ID],            "MPI_Buffer_detach");
   strcpy(label[PROBE_ID],                    "MPI_Probe");
   strcpy(label[IPROBE_ID],                   "MPI_Iprobe");
   strcpy(label[TEST_ID],                     "MPI_Test");
   strcpy(label[TESTANY_ID],                  "MPI_Testany");
   strcpy(label[TESTALL_ID],                  "MPI_Testall");
   strcpy(label[TESTSOME_ID],                 "MPI_Testsome");
   strcpy(label[WAIT_ID],                     "MPI_Wait");
   strcpy(label[WAITANY_ID],                  "MPI_Waitany");
   strcpy(label[WAITALL_ID],                  "MPI_Waitall");
   strcpy(label[WAITSOME_ID],                 "MPI_Waitsome");
   strcpy(label[START_ID],                    "MPI_Start");
   strcpy(label[STARTALL_ID],                 "MPI_Startall");
   strcpy(label[BCAST_ID],                    "MPI_Bcast");
   strcpy(label[IBCAST_ID],                   "MPI_Ibcast");
   strcpy(label[BARRIER_ID],                  "MPI_Barrier");
   strcpy(label[IBARRIER_ID],                 "MPI_Ibarrier");
   strcpy(label[REDUCE_ID],                   "MPI_Reduce");
   strcpy(label[IREDUCE_ID],                  "MPI_Ireduce");
   strcpy(label[ALLREDUCE_ID],                "MPI_Allreduce");
   strcpy(label[IALLREDUCE_ID],               "MPI_Iallreduce");
   strcpy(label[REDUCE_SCATTER_ID],           "MPI_Reduce_scatter");
   strcpy(label[IREDUCE_SCATTER_ID],          "MPI_Ireduce_scatter");
   strcpy(label[REDUCE_SCATTER_BLOCK_ID],     "MPI_Reduce_scatter_block");
   strcpy(label[IREDUCE_SCATTER_BLOCK_ID],    "MPI_Ireduce_scatter_block");
   strcpy(label[GATHER_ID],                   "MPI_Gather");
   strcpy(label[IGATHER_ID],                  "MPI_Igather");
   strcpy(label[GATHERV_ID],                  "MPI_Gatherv");
   strcpy(label[IGATHERV_ID],                 "MPI_Igatherv");
   strcpy(label[SCAN_ID],                     "MPI_Scan");
   strcpy(label[ISCAN_ID],                    "MPI_Iscan");
   strcpy(label[EXSCAN_ID],                   "MPI_Exscan");
   strcpy(label[IEXSCAN_ID],                  "MPI_Iexscan");
   strcpy(label[ALLGATHER_ID],                "MPI_Allgather");
   strcpy(label[NEIGHBOR_ALLGATHER_ID],       "MPI_Neighbor_allgather");
   strcpy(label[IALLGATHER_ID],               "MPI_Iallgather");
   strcpy(label[INEIGHBOR_ALLGATHER_ID],      "MPI_Ineighbor_allgather");
   strcpy(label[ALLGATHERV_ID],               "MPI_Allgatherv");
   strcpy(label[NEIGHBOR_ALLGATHERV_ID],      "MPI_Neighbor_allgatherv");
   strcpy(label[IALLGATHERV_ID],              "MPI_Iallgatherv");
   strcpy(label[INEIGHBOR_ALLGATHERV_ID],     "MPI_Ineighbor_allgatherv");
   strcpy(label[SCATTER_ID],                  "MPI_Scatter");
   strcpy(label[ISCATTER_ID],                 "MPI_Iscatter");
   strcpy(label[SCATTERV_ID],                 "MPI_Scatterv");
   strcpy(label[ISCATTERV_ID],                "MPI_Iscatterv");
   strcpy(label[ALLTOALL_ID],                 "MPI_Alltoall");
   strcpy(label[NEIGHBOR_ALLTOALL_ID],        "MPI_Neighbor_alltoall");
   strcpy(label[IALLTOALL_ID],                "MPI_Ialltoall");
   strcpy(label[INEIGHBOR_ALLTOALL_ID],       "MPI_Ineighbor_alltoall");
   strcpy(label[ALLTOALLV_ID],                "MPI_Alltoallv");
   strcpy(label[NEIGHBOR_ALLTOALLV_ID],       "MPI_Neighbor_alltoallv");
   strcpy(label[IALLTOALLV_ID],               "MPI_Ialltoallv");
   strcpy(label[INEIGHBOR_ALLTOALLV_ID],      "MPI_Ineighbor_alltoallv");
   strcpy(label[ALLTOALLW_ID],                "MPI_Alltoallw");
   strcpy(label[NEIGHBOR_ALLTOALLW_ID],       "MPI_Neighbor_alltoallw");
   strcpy(label[IALLTOALLW_ID],               "MPI_Ialltoallw");
   strcpy(label[INEIGHBOR_ALLTOALLW_ID],      "MPI_Ineighbor_alltoallw");
   strcpy(label[ACCUMULATE_ID],               "MPI_Accumulate");
   strcpy(label[FETCH_AND_OP_ID],             "MPI_Fetch_and_op");
   strcpy(label[GET_ID],                      "MPI_Get");
   strcpy(label[GET_ACCUMULATE_ID],           "MPI_Get_accumulate");
   strcpy(label[PUT_ID],                      "MPI_Put");
   strcpy(label[RACCUMULATE_ID],              "MPI_Raccumulate");
   strcpy(label[RGET_ID],                     "MPI_Rget");
   strcpy(label[RGET_ACCUMULATE_ID],          "MPI_Rget_accumulate");
   strcpy(label[RPUT_ID],                     "MPI_Rput");
   strcpy(label[WIN_ALLOCATE_ID],             "MPI_Win_allocate");
   strcpy(label[WIN_ALLOCATE_SHARED_ID],      "MPI_Win_allocate_shared");
   strcpy(label[WIN_ATTACH_ID],               "MPI_Win_attach");
   strcpy(label[WIN_COMPLETE_ID],             "MPI_Win_complete");
   strcpy(label[WIN_CREATE_ID],               "MPI_Win_create");
   strcpy(label[WIN_CREATE_DYNAMIC_ID],       "MPI_Win_create_dynamic");
   strcpy(label[WIN_FENCE_ID],                "MPI_Win_fence");
   strcpy(label[WIN_FLUSH_ID],                "MPI_Win_flush");
   strcpy(label[WIN_FLUSH_ALL_ID],            "MPI_Win_flush_all");
   strcpy(label[WIN_FLUSH_LOCAL_ID],          "MPI_Win_flush_local");
   strcpy(label[WIN_FLUSH_LOCAL_ALL_ID],      "MPI_Win_flush_local_all");
   strcpy(label[WIN_FREE_ID],                 "MPI_Win_free");
   strcpy(label[WIN_LOCK_ID],                 "MPI_Win_lock");
   strcpy(label[WIN_LOCK_ALL_ID],             "MPI_Win_lock_all");
   strcpy(label[WIN_LOCK_ID],                 "MPI_Win_lock");
   strcpy(label[WIN_POST_ID],                 "MPI_Win_post");
   strcpy(label[WIN_START_ID],                "MPI_Win_start");
   strcpy(label[WIN_SYNC_ID],                 "MPI_Win_sync");
   strcpy(label[WIN_TEST_ID],                 "MPI_Win_test");
   strcpy(label[WIN_UNLOCK_ID],               "MPI_Win_unlock");
   strcpy(label[WIN_UNLOCK_ALL_ID],           "MPI_Win_unlock_all");
   strcpy(label[WIN_WAIT_ID],                 "MPI_Win_wait");

   strcpy(label[FILE_CLOSE_ID],               "MPI_File_close");
   strcpy(label[FILE_DELETE_ID],              "MPI_File_delete");
   strcpy(label[FILE_IREAD_ID],               "MPI_File_iread");
   strcpy(label[FILE_IREAD_AT_ID],            "MPI_File_iread_at");
   strcpy(label[FILE_IREAD_SHARED_ID],        "MPI_File_iread_shared");
   strcpy(label[FILE_IWRITE_ID],              "MPI_File_iwrite");
   strcpy(label[FILE_IWRITE_AT_ID],           "MPI_File_iwrite_at");
   strcpy(label[FILE_IWRITE_SHARED_ID],       "MPI_File_iwrite_shared");
   strcpy(label[FILE_OPEN_ID],                "MPI_File_open");
   strcpy(label[FILE_PREALLOCATE_ID],         "MPI_File_preallocate");
   strcpy(label[FILE_READ_ID],                "MPI_File_read");
   strcpy(label[FILE_READ_ALL_ID],            "MPI_File_read_all");
   strcpy(label[FILE_READ_ALL_BEGIN_ID],      "MPI_File_read_all_begin");
   strcpy(label[FILE_READ_ALL_END_ID],        "MPI_File_read_all_end");
   strcpy(label[FILE_READ_AT_ID],             "MPI_File_read_at");
   strcpy(label[FILE_READ_AT_ALL_ID],         "MPI_File_read_at_all");
   strcpy(label[FILE_READ_AT_ALL_BEGIN_ID],   "MPI_File_read_at_all_begin");
   strcpy(label[FILE_READ_AT_ALL_END_ID],     "MPI_File_read_at_all_end");
   strcpy(label[FILE_READ_ORDERED_ID],        "MPI_File_read_ordered");
   strcpy(label[FILE_READ_ORDERED_BEGIN_ID],  "MPI_File_read_ordered_begin");
   strcpy(label[FILE_READ_ORDERED_END_ID],    "MPI_File_read_ordered_end");
   strcpy(label[FILE_READ_SHARED_ID],         "MPI_File_read_shared");
   strcpy(label[FILE_SEEK_ID],                "MPI_File_seek");
   strcpy(label[FILE_SEEK_SHARED_ID],         "MPI_File_seek_shared");
   strcpy(label[FILE_SET_VIEW_ID],            "MPI_File_set_view");
   strcpy(label[FILE_SYNC_ID],                "MPI_File_sync");
   strcpy(label[FILE_WRITE_ID],               "MPI_File_write");
   strcpy(label[FILE_WRITE_ALL_ID],           "MPI_File_write_all");
   strcpy(label[FILE_WRITE_ALL_BEGIN_ID],     "MPI_File_write_all_begin");
   strcpy(label[FILE_WRITE_ALL_END_ID],       "MPI_File_write_all_end");
   strcpy(label[FILE_WRITE_AT_ID],            "MPI_File_write_at");
   strcpy(label[FILE_WRITE_AT_ALL_ID],        "MPI_File_write_at_all");
   strcpy(label[FILE_WRITE_AT_ALL_BEGIN_ID],  "MPI_File_write_at_all_begin");
   strcpy(label[FILE_WRITE_AT_ALL_END_ID],    "MPI_File_write_at_all_end");
   strcpy(label[FILE_WRITE_ORDERED_ID],       "MPI_File_write_ordered");
   strcpy(label[FILE_WRITE_ORDERED_BEGIN_ID], "MPI_File_write_ordered_begin");
   strcpy(label[FILE_WRITE_ORDERED_END_ID],   "MPI_File_write_ordered_end");
   strcpy(label[FILE_WRITE_SHARED_ID],        "MPI_File_write_shared");

   srand(13579);

   // set color for each eventid that is present, using the current palette
   j = 0;
   for(i=0; i<MAX_IDS; i++) 
   {
      if (event_is_present[i]) {
        color[i][0] = current_color[j];
        color[i][1] = current_color[j+1];
        color[i][2] = current_color[j+2];
        j += 3;
        if (j >= (3*max_colors)) j -= (3*max_colors);
      }
   }

   return;
}
//======================================================================
// routine to rescale coordinates to [0-1]
//======================================================================
void rescale(struct eventstruct * ev, double * t1, double * t2, 
             int npts, double min, double scale)
{
  int i;
  double inv_scale;

  inv_scale = 1000.0/scale;
  for (i=0; i<npts; i++)
  {
     t1[i] = (ev[i].tbeg - min)*inv_scale;
     t2[i] = (ev[i].tend - min)*inv_scale;
  }
}

/*===========================================================*/
/* routine to reverse the byte order in each event record    */
/*===========================================================*/
void reverse_byte_order(char * in, char * out)
{
   swap8(&in[0],  &out[0]);     /* 8-bytes for tbeg        */
   swap8(&in[8],  &out[8]);     /* 8-bytes for tend        */
   swap4(&in[16], &out[16]);    /* 4-bytes for taskid      */
   swap4(&in[20], &out[20]);    /* 4-bytes for eventid     */
   swap4(&in[24], &out[24]);    /* 4-bytes for src         */
   swap4(&in[28], &out[28]);    /* 4-bytes for dest        */
   swap4(&in[32], &out[32]);    /* 4-bytes for bytes       */
   swap4(&in[36], &out[36]);    /* 4-bytes for parent      */
   swap4(&in[40], &out[40]);    /* 4-bytes for grandparent */
   swap4(&in[44], &out[44]);    /* 4-bytes for abi         */
}

void swap8(char * in, char * out)
{
   out[7] = in[0];
   out[6] = in[1];
   out[5] = in[2];
   out[4] = in[3];
   out[3] = in[4];
   out[2] = in[5];
   out[1] = in[6];
   out[0] = in[7];
}

void swap4(char * in, char * out)
{
   out[3] = in[0];
   out[2] = in[1];
   out[1] = in[2];
   out[0] = in[3];
}

double urand1(void)
{
   double r;
   r = ((double) rand()) * recip_rand_max;
   return r;
}


/*===========================================================================*/
/* incremental Shell sort with increment array: inc[k] = 1 + 3*2^k + 4^(k+1) */
/*===========================================================================*/
void mpitrace_sortx(double * arr , int n, int * ind, int flag)
{
   int h, i, j, k, inc[20];
   int numinc, pwr2, pwr4;
   double val;

   if (n <= 1) {
      ind[0] = 0;
      return;
   }

   pwr2 = 1;
   pwr4 = 4;

   numinc = 0;
   h = 1;
   inc[numinc] = h;
   while (numinc < 20) {
      h = 1 + 3*pwr2 + pwr4;
      if (h > n) break;
      numinc++;
      inc[numinc] = h;
      pwr2 *= 2;
      pwr4 *= 4;
   }

   for (i=0; i<n; i++) ind[i] = i;

   if (flag > 0) { // sort in increasing order 
      for (; numinc >= 0; numinc--) {
         h = inc[numinc];
         for (i = h; i < n; i++) {
            val = arr[i];
            k   = ind[i];

            j = i;
   
            while ( (j >= h) && (arr[j-h] > val) ) {
               arr[j] = arr[j-h];
               ind[j] = ind[j-h];
               j = j - h;
            }

            arr[j] = val;
            ind[j] = k;
         }
      }
   }
   else { // sort in decreasing order 
      for (; numinc >= 0; numinc--) {
         h = inc[numinc];
         for (i = h; i < n; i++) {
            val = arr[i];
            k   = ind[i];

            j = i;
   
            while ( (j >= h) && (arr[j-h] < val) ) {
               arr[j] = arr[j-h];
               ind[j] = ind[j-h];
               j = j - h;
            }

            arr[j] = val;
            ind[j] = k;
         }
      }
   }
}

void random_colors(void)
{
   int * ind;
   double * xrand;
   int i, j;

   ind = (int *) malloc(max_colors*sizeof(int));
   xrand = (double *) malloc(max_colors*sizeof(double));

   for (i=0; i<max_colors; i++) xrand[i] = urand1();

   mpitrace_sortx(xrand, max_colors, ind, 1);

   // randomize the available colors in the palette
   for (i=0; i<max_colors; i++) {
     j = ind[i];
     current_color[3*i]   = init_color[3*j];
     current_color[3*i+1] = init_color[3*j+1];
     current_color[3*i+2] = init_color[3*j+2];
   }

   // assign new colors to each MPI event that is present
   j = 0;
   for(i=0; i<MAX_IDS; i++) {
     if (event_is_present[i]) {
       color[i][0] = current_color[j];
       color[i][1] = current_color[j+1];
       color[i][2] = current_color[j+2];
       j += 3;
     }
     else {
       color[i][0] = 0.0;
       color[i][1] = 0.0;
       color[i][2] = 0.0;
     }

   }

   free(ind);
   free(xrand);
}
