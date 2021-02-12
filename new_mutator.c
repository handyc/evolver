// gcc mutator2.c -o mutator2 -L/usr/X11R6/lib -lX11

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <string.h>
#include <math.h>

#define NIL (0)

#define DDA_STYLE 1

#define HIGH_MUT 600
//#define TEST_MUT 600
#define TEST_MUT 300
//300 is ideal
// try 0.015 to 0.02 == 15 to 20?

#define TILESIZE 64
#define TOTAL_ORGANISMS_SQRT 10
#define TOTAL_ORGANISMS (TOTAL_ORGANISMS_SQRT*TOTAL_ORGANISMS_SQRT)
#define INITIAL_MUTATION_RATE TEST_MUT


#define MAX_X TILESIZE*TOTAL_ORGANISMS_SQRT
#define MAX_Y TILESIZE*TOTAL_ORGANISMS_SQRT

//#define MAX_X 512
//#define MAX_Y TILESIZE*TOTAL_ORGANISMS_SQRT

#define MAX_PAL 256

#define ALPHABET 256

#define ITERATIONS 1000 // size of one gene

#define ORGANISM_SIZE ITERATIONS*5 // size of each gene times number of genes to run

#define THRESHOLD_ULTRA_HIGH_SCORE 1000000
#define THRESHOLD_SUPER_HIGH_SCORE 928000
#define THRESHOLD_HIGH_SCORE 800000
#define THRESHOLD_LOW_SCORE 500000
#define THRESHOLD_SUPER_LOW_SCORE 100000
#define THRESHOLD_ULTRA_LOW_SCORE 50000


//#define TOTAL_TOTAL_ITERATIONS 65530
//#define TOTAL_TOTAL_ITERATIONS 1048576
#define TOTAL_TOTAL_ITERATIONS 256*TILESIZE*TILESIZE

#define MAX_DEPTH 5
#define MAX_TYPE 256
#define MAX_DESC 4

//void letterDraw(int x, int y, int type, int depth);

int malloc2d(int *** grid, int nrows, int ncols);
void lineDDA(int x0, int y0, int x1, int y1, char color);
void draw_organisms(int x, int y, char *object_filename, Display *dpy, Window w, GC gc, XColor *pixel_color);
void mutate_organisms(int starting_index, int mom_index, int dad_index, Display *dpy, Window w, GC gc, XColor *pixel_color);
//void lineDDA(int x0, int y0, int x1, int y1, char color);
void sex_winners(int index_a, int index_b, int range_from);
void select_fittest(void);
//void prepare_tile()

void rotate_palette_left(Display *dpy);
void rotate_palette_right(Display *dpy);
void ready_palette(Display *dpy);
//void make_pal_test(Display *dpy, Colormap screen_colormap, XColor pixel_color[256][256]);

 XColor pixel_color[256];

char *scorereport="High score:";
char *mutationreport="Mutation rate:";
char *iterationreport="Iteration number:";
char scorestring[9]={'0','0','0','0','0','0','0','\0'};
char mutstring[7]={'0','0','0','0','0','\0'};
char iterationstring[7]={'0','0','0','0','0','\0'};




unsigned long iteration_number=0;
int sex_on=1;
long mutation_rate=INITIAL_MUTATION_RATE;
long other_mut_rate=7;
//unsigned int iteration_limit=TILESIZE/3; // this can be anything up to ORGANISM_SIZE

unsigned char target_tile_map[TILESIZE*TILESIZE]=
{
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,0,0,0,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,255,0,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,0,0,0,255,0,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,0,0,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,0,0,0,0,0,0,0,255,255,0,255,0,0,255,0,0,0,0,255,0,0,0,0,0,0,255,255,255,255,0,0,255,255,255,255,255,255,0,0,0,0,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,0,255,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,0,0,0,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,255,255,255,255,0,255,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,255,0,0,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,0,0,255,0,255,255,255,255,255,0,0,0,255,0,0,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,255,0,0,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,0,255,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,0,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,255,255,255,0,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};//*/
unsigned long helper;
unsigned long high_score_index[4];//={0, 0, 0, 0};
unsigned long high_score[4]={0, 0, 0, 0};
unsigned long current_score=0;

//

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

byte *VGA = (byte *)0xA0000;          /* this points to video memory. */
word *my_clock = (word *)0x046C;      /* this points to the 18.2hz system
                                         clock. */

typedef struct tagBITMAP              /* the structure for a bitmap. */
{
  word width;
  word height;
  byte *data;
} BITMAP;

/**************************************************************************
 *  fskip                                                                 *
 *     Skips bytes in a file.                                             *
 **************************************************************************/

void fskip(FILE *fp, int num_bytes)
{
   int i;
   for (i=0; i<num_bytes; i++)
      fgetc(fp);
}

/**************************************************************************
 *  set_mode                                                              *
 *     Sets the video mode.                                               *
 **************************************************************************/
/*
void set_mode(byte mode)
{
  union REGS regs;

  regs.h.ah = SET_MODE;
  regs.h.al = mode;
  int86(VIDEO_INT, &regs, &regs);
}
*/
/**************************************************************************
 *  load_bmp                                                              *
 *    Loads a bitmap file into memory.                                    *
 **************************************************************************/

void load_bmp(char *file,BITMAP *b)
{
  FILE *fp;
  long index;
  word num_colors;
  int x;

  /* open the file */
  if ((fp = fopen(file,"rb")) == NULL)
  {
    printf("Error opening file %s.\n",file);
  //  exit(1);
  }

  /* check to see if it is a valid bitmap file */
  if (fgetc(fp)!='B' || fgetc(fp)!='M')
  {
    fclose(fp);
    printf("%s is not a bitmap file.\n",file);
   // exit(1);
  }

  /* read in the width and height of the image, and the
     number of colors used; ignore the rest */
  fskip(fp,16);
  fread(&b->width, sizeof(word), 1, fp);
  fskip(fp,2);
  fread(&b->height,sizeof(word), 1, fp);
  fskip(fp,22);
  fread(&num_colors,sizeof(word), 1, fp);
  fskip(fp,6);

  /* assume we are working with an 8-bit file */
  if (num_colors==0) num_colors=256;


  /* try to allocate memory */
  if ((b->data = (byte *) malloc((word)(b->width*b->height))) == NULL)
  {
    fclose(fp);
    printf("Error allocating memory for file %s.\n",file);
   // exit(1);
  }

  /* Ignore the palette information for now.
     See palette.c for code to read the palette info. */
  fskip(fp,num_colors*4);

  /* read the bitmap */
  for(index=(b->height-1)*b->width;index>=0;index-=b->width)
    for(x=0;x<b->width;x++)
      b->data[(word)index+x]=(byte)fgetc(fp);

  fclose(fp);
}







//
BITMAP bmp;
long shitty=0;

//unsigned char public_tile_map[TILESIZE*TILESIZE];
//unsigned char global_array[MAX_DEPTH][MAX_TYPE][MAX_DESC];


unsigned char * public_tile_map;//[TILESIZE*TILESIZE];
//unsigned char ** child;//[TOTAL_ORGANISMS][ORGANISM_SIZE];

//unsigned char winner[4][TILESIZE*TILESIZE];

int ** child;
int ** winner;

//unsigned char child[TOTAL_ORGANISMS][ORGANISM_SIZE];
//unsigned char* global_array;//[MAX_DEPTH][MAX_TYPE][MAX_DESC];

//int* public_tile_map;//[TILESIZE*TILESIZE];
//int* global_array;//[MAX_DEPTH][MAX_TYPE][MAX_DESC];


int main()
{

malloc2d(&child, TOTAL_ORGANISMS, ORGANISM_SIZE);
malloc2d(&winner, 4, ORGANISM_SIZE);

/*
unsigned char ** a;
child = malloc(sizeof *a * TOTAL_ORGANISMS);
int i;
if (a)
{
  for (i = 0; i < TOTAL_ORGANISMS; i++)
  {
    a[i] = malloc(sizeof(unsigned char) * ORGANISM_SIZE);
  }
}
*/

//int* x;
//x = (int*) malloc(sizeof(int) * 10);

public_tile_map=(unsigned char *) malloc(sizeof(unsigned char) * (TILESIZE*TILESIZE));
//child=(unsigned char *) malloc(sizeof(unsigned char) * (TOTAL_ORGANISMS*ORGANISM_SIZE));

//if(public_tile_map=(unsigned char *) malloc(sizeof(unsigned char) * (TILESIZE*TILESIZE))==NULL)
//{

//}

//global_array=(unsigned char*) malloc(sizeof(unsigned char) * (MAX_DEPTH*MAX_TYPE*MAX_DESC));

//public_tile_map=(int*) malloc(sizeof(int) * (TILESIZE*TILESIZE));
//global_array=(int*) malloc(sizeof(int) * (MAX_DEPTH*MAX_TYPE*MAX_DESC));

/*
 if (child[TOTAL_ORGANISMS][ORGANISM_SIZE] = (byte *) malloc(TOTAL_ORGANISMS*ORGANISM_SIZE) == NULL)
  {
   // fclose(fp);
    printf("Error allocating memory for organisms.");
   // exit(1);
  }
*/

// BEGIN main() variables ***************************************

FILE *palette_file, *background_file;
//char *palette_filename[8]={"pal.c", "b", "c", "d", "e", "f", "g", "h"};
char *background_filename[8]={"background.bmp", "background.bmp", "background3.bmp", "background4.bmp", "background5.bmp", "background6.bmp", "background7.bmp", "background8.bmp"};

int counter=0, counter2=0, newpalcounter=0, option_to_quit=0;

int organism_number=0, position=0;
int get_char='0'; 
int x_coord=0, y_coord=0, palswitcher=0;
unsigned int colorcount=0;

int depth_c, type_c, desc_c;
int die_roll=0;

//load_bmp("sr.bmp",&bmp);

for(shitty=0;shitty<(TILESIZE*TILESIZE);shitty++)
{
//target_tile_map[shitty]=bmp.data[shitty];
//winner[3][TILESIZE*TILESIZE]=bmp.data[shitty];
//public_tile_map[shitty]=255;

/*
if(bmp.data[shitty]==0)
{
target_tile_map[shitty]=9;
}
else
{
target_tile_map[shitty]=0;
}
*/

}

Display *dpy = XOpenDisplay(NIL);
assert(dpy);

Colormap screen_colormap = DefaultColormap(dpy, DefaultScreen(dpy));

// END OF main() VARIABLES ******************************************************


// INITIALIZATION PROCESS ******************(((((((((((((((((((((((((((((((((((((

// Set all children to winner[0]

srandom(22134);


// initialize all organisms to 0
for(organism_number=0;organism_number<TOTAL_ORGANISMS;organism_number++)
{
for(position=0;position<ORGANISM_SIZE;position++)
{
//target_tile_map[position]=255;

//child[organism_number][position]=random()%256;  // At this point we have no idea whatsoever which
child[organism_number][position]=random()%256;  // At this point we have no idea whatsoever which


//child[organism_number][position]=target_tile_map[position];  // At this point we have no idea whatsoever which
												// specific genomes will satisy the goal.
												// Therefore, we will randomize all child organisms
												// in order to maximize our chances of finding
												// something closer to the goal
}
}

ready_palette(dpy); // ready the palette

// Create the window
      Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
				     MAX_X, MAX_Y, 0, pixel_color[255].pixel, pixel_color[255].pixel);
	
	  Window w2 = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 300, 150, 
				     300, 100, 0, pixel_color[255].pixel, pixel_color[255].pixel);

      // We want to get MapNotify events
	  // including mouse events
      XSelectInput(dpy, w, StructureNotifyMask | ButtonPressMask | KeyPressMask);

      // "Map" the window (that is, make it appear on the screen)
      XMapWindow(dpy, w);
      XMapWindow(dpy, w2);

      // Create a "Graphics Context"
      GC gc = XCreateGC(dpy, w, 0, NIL);

XEvent e;
int x_button, y_button;
int the_win;

for(iteration_number=0;iteration_number<TOTAL_TOTAL_ITERATIONS;iteration_number++)
{

//high_score[0]=0;
//high_score[0]=47695;
helper=high_score[0]%10;
scorestring[7]=helper+48;
helper=high_score[0]/10;
helper=helper%10;
scorestring[6]=helper+48;
helper=high_score[0]/100;
helper=helper%10;
scorestring[5]=helper+48;
helper=high_score[0]/1000;
helper=helper%10;
scorestring[4]=helper+48;
helper=high_score[0]/10000;
helper=helper%10;
scorestring[3]=helper+48;
helper=high_score[0]/100000;
helper=helper%10;
scorestring[2]=helper+48;
helper=high_score[0]/1000000;
helper=helper%10;
scorestring[1]=helper+48;
helper=high_score[0]/10000000;
helper=helper%10;
scorestring[0]=helper+48;


helper=mutation_rate%10;
mutstring[4]=helper+48;
helper=mutation_rate/10;
helper=helper%10;
mutstring[3]=helper+48;
helper=mutation_rate/100;
helper=helper%10;
mutstring[2]=helper+48;
helper=mutation_rate/1000;
helper=helper%10;
mutstring[1]=helper+48;
helper=mutation_rate/10000;
helper=helper%10;
mutstring[0]=helper+48;

helper=iteration_number%10;
iterationstring[4]=helper+48;
helper=iteration_number/10;
helper=helper%10;
iterationstring[3]=helper+48;
helper=iteration_number/100;
helper=helper%10;
iterationstring[2]=helper+48;
helper=iteration_number/1000;
helper=helper%10;
iterationstring[1]=helper+48;
helper=iteration_number/10000;
helper=helper%10;
iterationstring[0]=helper+48;



XSetForeground(dpy, gc, pixel_color[255].pixel);
XFillRectangle(dpy, w2, gc, 0, 0, 200, 100);
XSetForeground(dpy, gc, pixel_color[9].pixel);

XDrawString (dpy, w2, gc, 5, 20, scorereport, strlen(scorereport));
XDrawString (dpy, w2, gc, 120, 20, scorestring, strlen(scorestring));

XDrawString (dpy, w2, gc, 5, 40, mutationreport, strlen(mutationreport));
XDrawString (dpy, w2, gc, 120, 40, mutstring, strlen(mutstring));

XDrawString (dpy, w2, gc, 5, 60, iterationreport, strlen(iterationreport));
XDrawString (dpy, w2, gc, 120, 60, iterationstring, strlen(iterationstring));

//XFlush(dpy);

draw_organisms(0, 0, background_filename[1], dpy, w, gc, pixel_color); // draw every single organism
																       // and score them all against target

//////

//////
ready_palette(dpy);

for(position=0;position<ORGANISM_SIZE;position++) // set winners from scores
			{
			winner[0][position]=child[high_score_index[0]][position];
			winner[1][position]=child[high_score_index[1]][position];
			winner[2][position]=child[high_score_index[2]][position];
			winner[3][position]=child[high_score_index[3]][position];
			}

//sex_winners(high_score_index[0], high_score_index[1], 1);
//sex_winners(high_score_index[1], high_score_index[2], 2);
//sex_winners(high_score_index[2], high_score_index[3], 3);
//sex_winners(high_score_index[3], high_score_index[0], 4);

	mutate_organisms(0, 0, 1, dpy, w, gc, pixel_color);
	mutate_organisms(1, 1, 2, dpy, w, gc, pixel_color);
	mutate_organisms(2, 2, 3, dpy, w, gc, pixel_color);
	mutate_organisms(3, 3, 0, dpy, w, gc, pixel_color);
	
		//	select_winner(0,0, background_filename[1], dpy, w, gc, pixel_color);
		  //  mutate_organisms(0, 0, background_filename[1], dpy, w, gc, pixel_color);
		    //draw_organisms(0, 0, background_filename[1], dpy, w, gc, pixel_color);	
}

free(child);
return 0;
}

int malloc2d(int *** grid, int nrows, int ncols){
    int i;
    *grid = malloc( sizeof(int *) * nrows);

    if (*grid == NULL){
        printf("ERROR: out of memory\n");
        return 1;
    }

    for (i=0;i<nrows;i++){
        (*grid)[i] = malloc( sizeof(int) * ncols);
        if ((*grid)[i] == NULL){
            printf("ERROR: out of memory\n");
            return 1;
        }
    }
    printf("Allocated!\n");
    return 0;
}

void draw_organisms(int x, int y, char *object_filename, Display *dpy, Window w, GC gc, XColor *pixel_color)
{
FILE *object_file;
int x_coord=0, y_coord=0;
int max_x=720, max_y=480; // read these from the object file
unsigned char get_char='2';
unsigned char get_char2='1';
unsigned char redirect_char;
unsigned int newcounter_x, newcounter_y;
long mutation=0;
unsigned int position;
//unsigned char tile_map[ORGANISM_SIZE];
unsigned int gene_counter=0;
unsigned int child_number=0;
int in;

unsigned int sub_x, sub_y;

//scroll through each entire gene of every organism
//plot to tile_map as necessary

high_score[0]=0;
high_score[1]=0;
high_score[2]=0;
high_score[3]=0;

/////

//mutate_palette();


//////

  for(y_coord=0;y_coord<MAX_Y;y_coord+=TILESIZE) // Think of the screen as a matrix of 512 x 512
     {// step through each y,
	 for(x_coord=0;x_coord<MAX_X;x_coord+=TILESIZE) // while stepping through each x,
	    {
	    current_score=0; // reset the current score
		
		for(in=0;in<(TILESIZE*TILESIZE);in++)
		{
		 public_tile_map[in]=255;
	
		//public_tile_map[in]=bmp.data[in];
		
		
		
		
		}

	    //for(in=0;in<ORGANISM_SIZE;in++)
		for(in=0;in<ITERATIONS;in++)
	    {
	    // here's the tricky part...
	    // That's right, normally the next portion of code would be mapping from the genome "child[x][i]"
	    // to public_tile_map using some kind of Bresenham algorithm or similar
	    // here we just set one to the other:
	    // *********************************************************************
	    //public_tile_map[in]=child[child_number][in];
	    // *********************************************************************



//child[child_number][in+4]);
    // draw the gene
	
	//lineDDA(child[child_number][(in*5)+0]%TILESIZE,child[child_number][(in*5)+1]%TILESIZE,child[child_number][(in*5)+2]%TILESIZE,child[child_number][(in*5)+3]%TILESIZE,child[child_number][(in*5)+4]%TILESIZE);
    
	//letterDraw(int x, int y, int type, int depth)
	
	// uncomment below for old style	
	
	if(DDA_STYLE==1)
	{
	lineDDA((child[child_number][in])%TILESIZE, (child[child_number][in+1])%TILESIZE, (child[child_number][in+2])%TILESIZE,(child[child_number][in+3])%TILESIZE,0); 
	// suspicious
	}
	else
	{
	//letterDraw(0, 0, child[child_number][(in*5)+0]%(TOTAL_ORGANISMS/4), 4);
	}
	//commmented out above
	
	//lineDDA(TILESIZE-child[child_number][(in*5)+0]%TILESIZE,child[child_number][(in*5)+1]%TILESIZE,TILESIZE-child[child_number][(in*5)+2]%TILESIZE,child[child_number][(in*5)+3]%TILESIZE,child[child_number][(in*5)+4]%TILESIZE);
	//lineDDA(child[child_number][(in*5)+0]%TILESIZE,TILESIZE-child[child_number][(in*5)+1]%TILESIZE,child[child_number][(in*5)+2]%TILESIZE,TILESIZE-child[child_number][(in*5)+3]%TILESIZE,child[child_number][(in*5)+4]%TILESIZE);
    
	//lineDDA(5,2,child[child_number][in+1]%TILESIZE,child[child_number][in+2]%TILESIZE,9);//child[child_number][in]%TILESIZE, child[child_number][in+1]%TILESIZE, child[child_number][in+2]%TILESIZE,child[child_number][in+3]%TILESIZE,9); //child[child_number][in+4]);
    
	// draw the gene
	
	    // *************
	
	    }
		
		for(in=0;in<(TILESIZE*TILESIZE);in++)
		{
		
	    // Now compute scores for each organism
	    if(public_tile_map[in]==target_tile_map[in]) // did we hit the target byte?
	    {
	    //current_score+=1;
		//if(public_tile_map[in+1]==target_tile_map[in+1]) // did we hit the target byte?
	    //{
		//current_score+=8;
		//if(public_tile_map[in+2]==target_tile_map[in+2]) // did we hit the target byte?
	   // {
		//current_score+=14;
		//if(public_tile_map[in+3]==target_tile_map[in+3]) // did we hit the target byte?
	   // {
		//current_score+=108;
		//if(public_tile_map[in+4]==target_tile_map[in+4]) // did we hit the target byte?
	   // {
		current_score+=255; // if so, give a score bonus
	//	}
	//	}
	//	}
	//	}
		}
	    //else current_score+=(255-abs(target_tile_map[in]-public_tile_map[in])); //child[child_number][in]));
	    // otherwise give some other bonus, maybe (probably better off without it)
		}
		
		
	     if(current_score>high_score[0]) {high_score_index[0]=child_number; high_score[0]=current_score;}
	else if(current_score>high_score[1]) {high_score_index[1]=child_number; high_score[1]=current_score;}
	else if(current_score>high_score[2]) {high_score_index[2]=child_number; high_score[2]=current_score;}
	else if(current_score>high_score[3]) {high_score_index[3]=child_number; high_score[3]=current_score;}
	// is this one of the top four scores?  if so, store it and its index
	
		//get_char=public_tile_map[newcounter_x+newcounter_y*TILESIZE];
			
		//if(high_score>THRESHOLD_HIGH_SCORE) // should we bother drawing?
		//{																	
		for(newcounter_y=0;newcounter_y<TILESIZE;newcounter_y++)
		   {
		   for(newcounter_x=0;newcounter_x<TILESIZE;newcounter_x++)
			  {
			  //get_char=bmp.data[newcounter_x+newcounter_y*TILESIZE];
			  
			  get_char=public_tile_map[newcounter_x+newcounter_y*TILESIZE];
			  
			  
			  XSetForeground(dpy, gc, pixel_color[get_char].pixel); // set the drawing color to that palette index,
			  XDrawPoint(dpy, w, gc, (x_coord+newcounter_x), (y_coord+newcounter_y)); // and plot the point at (x, y)
			  }
		   }
		//} // endif
		   
		   child_number++; // go to the next gene
		} // finished with one horizontal row
	 } // finished with one vertical column

///// Now, some fuzzy logic corrections controls
	
//mutation_rate=60000;
//sex_on=1;

/*

	if(high_score[0]>THRESHOLD_ULTRA_HIGH_SCORE) // are we close to a match?
	{
	//if(mutation_rate>2)                          // if so, and mutation rate is not at lowest,
	
	mutation_rate=100;                             // crank it down a bit
	//sex_on=0;
	//mutation_rate=60000;
	}
	else if(high_score[0]>THRESHOLD_SUPER_HIGH_SCORE)  // if we're almost close to a match
	{
	 //sex_on=1;
	if(mutation_rate<500)
	mutation_rate=HIGH_MUT;
	else if(mutation_rate>0)
	 mutation_rate-=1;
	 else
	  {
	  //mutation_rate=HIGH_MUT;
	  mutation_rate=700;
	  
	  //sex_on=1;
	  }
	
	}
	else if(high_score[0]>THRESHOLD_HIGH_SCORE)  // if we're almost close to a match
	{
     //sex_on=1;
	 if(mutation_rate>0)
	 //mutation_rate++;
	 mutation_rate=700;
	 else
	  {
	  mutation_rate=HIGH_MUT;
	  //mutation_rate=700;
	  
	  //sex_on=1;
	  }
	
	}
	else if(high_score[0]>THRESHOLD_LOW_SCORE)   // if we're nowhere close,
	{
	 //sex_on=1;
	 if(mutation_rate>0)
	 {
	 mutation_rate-=1;
	 }
	 else
	  {
	  //mutation_rate=HIGH_MUT;
	  mutation_rate=1000;
	  //mutation_rate=700;
	  
	  //sex_on=1;
	  }
	
	}
    else if(high_score[0]>THRESHOLD_SUPER_LOW_SCORE)   // if we're nowhere close,
	{
	 //sex_on=1;
	 if(mutation_rate>0)
	 {
	 mutation_rate-=1;
	 //mutation_rate+=1;
	 }
	 else
	  {
	  mutation_rate=HIGH_MUT;
	  //mutation_rate=700;
	  //mutation_rate=1000;
	  
	  //sex_on=1;
	  }
	
	
	}
	else if(high_score[0]>THRESHOLD_ULTRA_LOW_SCORE)   // if we're nowhere close,
	 {
	 //sex_on=1;
	 if(mutation_rate>0)
	 {
	 mutation_rate-=1;
	//mutation_rate+=1;
	}
	else
	{
	mutation_rate=700;
	//mutation_rate=HIGH_MUT;
	}
	
	}
	else
	 {
	 if(mutation_rate>100)
	 {
	 mutation_rate+=1;
	 //mutation_rate--;
	 }
	 else
	  {
	  //mutation_rate=HIGH_MUT;
	  mutation_rate=700;
	  
	  //sex_on=1;
	  }
	
	 }
*/
/////
		
	//	fclose(object_file);		 
//XFlush(dpy);
}

void mutate_organisms(int starting_index, int mom_index, int dad_index, Display *dpy, Window w, GC gc, XColor *pixel_color)
{
FILE *object_file;
int x_coord=0, y_coord=0;
int max_x=720, max_y=480; // read these from the object file
unsigned char get_char='2';
unsigned char get_char2='1';
unsigned char redirect_char;
unsigned int newcounter_x, newcounter_y;
unsigned char pal_redirect[256];
unsigned char tile_map[TILESIZE][TILESIZE];
long mutation=0;
unsigned int child_number;
unsigned int position;
int mom_or_dad=0;

//for(child_number=starting_index;child_number<(starting_index*TILESIZE);child_number++)
for(child_number=(starting_index*TOTAL_ORGANISMS/4);child_number<(TOTAL_ORGANISMS/4*(starting_index+1));child_number++)//TOTAL_ORGANISMS;child_number++)
   {
   for(position=0;position<ORGANISM_SIZE;position++)
      {
      mutation=(random()%100000);//(TILESIZE*TILESIZE));
        
		// flip a coin...
		mom_or_dad=random()%2;
		if(mom_or_dad==0 && sex_on)
		child[child_number][position]=winner[mom_index][position];//winner[winner_index][position];
		else 
		child[child_number][position]=winner[dad_index][position];//winner[winner_index][position];
		
		//child[child_number][position]=winner[3][position];
		
		if(mutation<mutation_rate) // if we can mutate at this point
          {
		  //if(mutation>(mutation_rate/2)) {child[child_number][position]=random()%ALPHABET;}
		  //else
	      //child[child_number][position]=255-child[child_number][position];
		 
		   child[child_number][position]=random()%ALPHABET;
		  //child[child_number][position]=target_tile_map[position];
		   }
	   }
   }

}

void ready_palette(Display *dpy)
{
FILE *palette_file;
unsigned int newpalcounter=0;
unsigned int colorcount=0;
unsigned int pal_value=0;
int pal_mult=256; // should be pretty high, change later
unsigned char digit_0=0;
unsigned char digit_1=0;
unsigned char digit_2=0;
unsigned char digit_offset=0;

Colormap screen_colormap = DefaultColormap(dpy, DefaultScreen(dpy));

for(colorcount=0;colorcount<256;colorcount++) // otherwise, continue making the palette:
	 { // loop through the entire 256-color palette
     
	 // grayscale palette
     
	 pixel_color[colorcount].red = colorcount*pal_mult;//*pal_mult;//colorcount*pal_mult; // put this in red
     pixel_color[colorcount].green = colorcount*pal_mult;//colorcount*pal_mult; // put this in red
	 pixel_color[colorcount].blue =  colorcount*pal_mult;//colorcount*pal_mult; // put this in blue
	 
	 // random palette 
	 
	//pixel_color[colorcount].red = random()%256*pal_mult;//colorcount*pal_mult; // put this in red
	//pixel_color[colorcount].green = random()%256*pal_mult;//colorcount*pal_mult; // put this in red
	//pixel_color[colorcount].blue =  random()%256*pal_mult;//colorcount*pal_mult; // put this in blue
	 
	 
	Status rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[colorcount]);
	 }
	 
	 
pixel_color[255].red = 65535;//*pal_mult;//colorcount*pal_mult; // put this in red
     pixel_color[255].green = 65535;//colorcount*pal_mult; // put this in red
	 pixel_color[255].blue =  65535;//colorcount*pal_mult; // put this in blue
	 
Status rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[255]);

	 
pixel_color[0].red = 0;//*pal_mult;//colorcount*pal_mult; // put this in red
     pixel_color[0].green = 0;//colorcount*pal_mult; // put this in red
	 pixel_color[0].blue =  0;//colorcount*pal_mult; // put this in blue
	
	rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[0]);
 
	 
	/* 
	 // tracer for testing
	 pixel_color[colorcount].red = 0; // put this in red
     pixel_color[colorcount].green = 30000; // put this in red
	 pixel_color[colorcount].blue =  30000; // put this in blue
	 
	Status rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[colorcount]);

	 */
	 
	 
	 
} // end of the function

void rotate_palette_left(Display *dpy)
{
FILE *palette_file;
unsigned int newpalcounter=0;
unsigned int colorcount=0;
unsigned int pal_value=0;

unsigned char digit_0=0;
unsigned char digit_1=0;
unsigned char digit_2=0;
unsigned char digit_offset=0;

XColor temp_color;
Colormap screen_colormap = DefaultColormap(dpy, DefaultScreen(dpy));

temp_color.red = pixel_color[0].red; // put this in red
temp_color.green = pixel_color[0].green; // put this in green
temp_color.blue = pixel_color[0].blue; // put this in blue

Status rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[0]);

for(colorcount=0;colorcount<255;colorcount++) // otherwise, continue making the palette:
	 { // loop through the entire 256-color palette
     pixel_color[colorcount].red =   pixel_color[colorcount+1].red; //  pixel_color[colorcount+1].red; // put this in red
     pixel_color[colorcount].green = pixel_color[colorcount+1].green; // put this in green
     pixel_color[colorcount].blue =  pixel_color[colorcount+1].blue; // put this in blue
	
	rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[colorcount]);
	}

pixel_color[255].red = temp_color.red; // put this in red
pixel_color[255].green = temp_color.green; // put this in green
pixel_color[255].blue = temp_color.blue; // put this in blue
	 

	rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[255]);

} // end of the function

void rotate_palette_right(Display *dpy)
{
FILE *palette_file;
unsigned int newpalcounter=0;
unsigned int colorcount=0;
unsigned int pal_value=0;
unsigned char digit_0=0;
unsigned char digit_1=0;
unsigned char digit_2=0;
unsigned char digit_offset=0;

XColor temp_color;
Colormap screen_colormap = DefaultColormap(dpy, DefaultScreen(dpy));

temp_color.red = pixel_color[255].red; // put this in red
temp_color.green = pixel_color[255].green; // put this in green
temp_color.blue = pixel_color[255].blue; // put this in blue

	Status rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[255]);

for(colorcount=255;colorcount>0;colorcount--) // otherwise, continue making the palette:
	 { // loop through the entire 256-color palette
     pixel_color[colorcount].red =   pixel_color[colorcount-1].red; //  pixel_color[colorcount+1].red; // put this in red
     pixel_color[colorcount].green = pixel_color[colorcount-1].green; // put this in green
     pixel_color[colorcount].blue =  pixel_color[colorcount-1].blue; // put this in blue
	 
	rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[colorcount]);
	 }

pixel_color[0].red = temp_color.red; // put this in red
pixel_color[0].green = temp_color.green; // put this in green
pixel_color[0].blue = temp_color.blue; // put this in blue
	 
	 
	rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[0]);
	 
} // end of the function

//DDA

void lineDDA(int x0, int y0, int x1, int y1, char color)
    {
     
        int dy = y1 - y0;
        int dx = x1 - x0;
        float t = 0.5, m;                      // offset for rounding
        int tt;

 public_tile_map[x0+y0*TILESIZE]=color;
        if (abs(dx) > abs(dy)) {         // slope < 1
            m = (float) dy / (float) dx;      // compute slope
            t += y0; 
            if (dx<0) {dx = -1;} else dx = 1;  
            m *= dx;
            while (x0 != x1) {
                x0 += dx;                           // step to next x value
                t += m;                             // add slope to y value
				tt = t;
 public_tile_map[x0+tt*TILESIZE]=color;
            }  
			
        } else {                                    // slope >= 1
            m = (float) dx / (float) dy;      // compute slope
            t += x0;
            if (dy<0) {dy = -1;} else dy = 1;
            m *= dy;
            while (y0 != y1) {
                y0 += dy;                           // step to next y value
                t += m;
				tt=t;                             // add slope to x value
public_tile_map[tt+y0*TILESIZE]=color;
}
        }
    }
	
	

void sex_winners(int index_a, int index_b, int range_from)
{
//child[organism_number][position]=winner[position];//target_tile_map[position];
//
}

