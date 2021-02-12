// gcc mutatorboard.c -o mutatorboard -L/usr/X11R6/lib -lX11

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <string.h>
#include <math.h>

#define NIL (0)

#define X_CURSOR_STEP 10
#define Y_CURSOR_STEP 10

#define LOWER_X 0
#define LOWER_Y 400

#define MAX_X 512
#define MAX_Y 512
#define MAX_PAL 256

#define TOTAL_ORGANISMS 1024
#define INITIAL_MUTATION_RATE 7
#define ALPHABET 256

#define ITERATIONS 1
#define TILESIZE 16
#define ORGANISM_SIZE ITERATIONS*5 // size of each gene times number of genes to run

#define THRESHOLD_ULTRA_HIGH_SCORE 65000
#define THRESHOLD_SUPER_HIGH_SCORE 61400
#define THRESHOLD_HIGH_SCORE 58000
#define THRESHOLD_LOW_SCORE 57000
#define THRESHOLD_SUPER_LOW_SCORE 46400
#define THRESHOLD_ULTRA_LOW_SCORE 35000

#define TOTAL_TOTAL_ITERATIONS 65530

#define GRAY_MODE 1
//#define MAGIC_MODE 0

/// unique symbolboard variables

#define MAX_DEPTH 5 // number of sub-letter levels (messy past 9)
#define MAX_TYPE 256 // number of sub-letter levels
#define MAX_DESC 4 // number of sub-letter levels

#define MAX_OUTPUT 256
///

int global_array[MAX_DEPTH][MAX_TYPE][MAX_DESC];

void letterDraw(int x, int y, int type, int depth);
void draw_organisms(int x, int y, Display *dpy, Window w, GC gc, XColor *pixel_color);
void ready_palette(Display *dpy);
void mutate_palette(Display *dpy, int mutation_rate);
int score_it(void); 
 
XColor pixel_color[256];

int iteration_number=0;
int mutation_rate=INITIAL_MUTATION_RATE;
//unsigned int iteration_limit=TILESIZE/3; // this can be anything up to ORGANISM_SIZE
unsigned char public_tile_map[TILESIZE*TILESIZE];
unsigned char target_tile_map[TILESIZE*TILESIZE];
												  
			//*/									  

int helper;
unsigned char high_score_index[4];//={0, 0, 0, 0};
unsigned long high_score[4]={0, 0, 0, 0};
unsigned long current_score=0;
int die_roll=0;


main()
{
// BEGIN main() variables ***************************************

int depth_c=0, type_c=0, desc_c=0;
int i, subsub_i;

Display *dpy = XOpenDisplay(NIL);
assert(dpy);

Colormap screen_colormap = DefaultColormap(dpy, DefaultScreen(dpy));

// END OF main() VARIABLES ******************************************************

// INITIALIZATION PROCESS ******************(((((((((((((((((((((((((((((((((((((

srandom(221); // start RNG
ready_palette(dpy); // ready the palette

// prepare alphabets...

for(depth_c=0;depth_c<MAX_DEPTH;depth_c++)
{
  for(type_c=0;type_c<MAX_TYPE;type_c++)
     {
     for(desc_c=0;desc_c<MAX_DESC;desc_c++)
        {
        global_array[depth_c][type_c][desc_c]=random()%MAX_TYPE;
		
		}
		target_tile_map[type_c]=10;//type_c;
     }
}

// Create the window
      Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
				     MAX_X, MAX_Y, 0, pixel_color[0].pixel, pixel_color[0].pixel);
	
//	Window w2 = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
//				     50, 50, 0, pixel_color[0].pixel, pixel_color[0].pixel);

	
		        // We want to get MapNotify events
	  // including mouse events
      XSelectInput(dpy, w, StructureNotifyMask | ButtonPressMask | KeyPressMask);

      // "Map" the window (that is, make it appear on the screen)
      XMapWindow(dpy, w);
  //    XMapWindow(dpy, w2);
	  
	  // Create a "Graphics Context"
      GC gc = XCreateGC(dpy, w, 0, NIL);

XEvent e;
int x_button, y_button;
int the_win;


while(1){// while(not quit)
XNextEvent(dpy, &e);
switch( e.type ) {
   
   
   
case KeyPress:
   //charcount = XLookupString(&event, buffer, bufsize, &keysym,
   //      &compose);
   /* Branch according to keysym, then use buffer
    * if the key is printable */
   break;
case MappingNotify:
   //XRefreshKeyboardMapping(&e);
   break;
default:

  // break;

for(depth_c=0;depth_c<MAX_DEPTH;depth_c++)
{
  for(type_c=0;type_c<MAX_TYPE;type_c++)
     {
     for(desc_c=0;desc_c<MAX_DESC;desc_c++)
        {
		//global_array[depth_c][type_c][desc_c]=global_array[depth_c][0][desc_c];
		//global_array[depth_c][type_c][desc_c]=(type_c*2)%MAX_TYPE;//global_array[depth_c][0][desc_c];
		die_roll=random()%100;
		//mutation_rate=random()%100;
		if(die_roll<mutation_rate)
		{
        //global_array[depth_c][type_c][desc_c]=global_array[depth_c][type_c][desc_c];

		global_array[depth_c][type_c][desc_c]=random()%MAX_TYPE;
        }
		}
	}
draw_organisms(0, 0, dpy, w, gc, pixel_color); // draw every single organism
											   // and score them all against target											   


break;


}

//end switch
}

} // end while
return 0;
}

void draw_organisms(int x, int y, Display *dpy, Window w, GC gc, XColor *pixel_color)
{
FILE *object_file;
int x_coord=0, y_coord=0;
int max_x=720, max_y=480; // read these from the object file
unsigned char get_char='2';
unsigned char get_char2='1';
unsigned char redirect_char;
unsigned int newcounter_x, newcounter_y;
unsigned char mutation=0;
unsigned int position;
//unsigned char tile_map[ORGANISM_SIZE];
unsigned int gene_counter=0;
unsigned int child_number=0;
int in;
int depth_c, type_c, desc_c;

unsigned int sub_x, sub_y;

unsigned int cursor_x, cursor_y;

int score_level=MAX_DEPTH-1;

char name_of_character[MAX_OUTPUT][32];// name of the current character="Leonard";


//scroll through each entire gene of every organism
//plot to tile_map as necessary

//high_score[0]=0;
//high_score[1]=0;
//high_score[2]=0;
//high_score[3]=0;

int current_score=0,high_score=0;

for(in=0;in<(TILESIZE*TILESIZE);in++) // clear the tile
	{
	public_tile_map[in]=0;
	}
				
cursor_x=0;
cursor_y=0;
		
for(in=0;in<MAX_OUTPUT;in++)
{
//mutate_palette(dpy, mutation_rate); // mutate the palette

// PROBLEM AREA
letterDraw(0, 0, in, MAX_DEPTH-1); // draw to a depth of MAX_DEPTH-1

// score it
current_score=score_it();
//score_level=%MAX_DEPTH

if(current_score>high_score)
{
high_score=current_score;
//change_up(in, MAX_DEPTH-1);
//change_up(in, score_level);
}

  for(y_coord=0;y_coord<TILESIZE;y_coord+=1)//(2*TILESIZE)) // Think of the screen as a matrix of 512 x 512
     {// step through each y,
	 
	 for(x_coord=0;x_coord<TILESIZE;x_coord+=1)//TILESIZE) // while stepping through each x,
	    {
	    current_score=0; // reset the current score	
	XSetForeground(dpy, gc, pixel_color[public_tile_map[x_coord+y_coord*TILESIZE]].pixel); // set the drawing color to that palette index,
   	XDrawPoint(dpy, w, gc, x_coord+cursor_x,y_coord+cursor_y);//(x_coord+newcounter_x), (y_coord)); // and plot the point at (x, y)
		
		} // finished with one horizontal row
	 } // finished with one vertical column

//	XSetForeground(dpy, gc, pixel_color[public_tile_map[x_coord+y_coord*TILESIZE]].pixel); // set the drawing color to that palette index,
	
	XSetForeground(dpy, gc, WhitePixel(dpy, DefaultScreen(dpy)));
	XDrawString(dpy, w, gc, LOWER_X, LOWER_Y, name_of_character[in], strlen(name_of_character[in]));		

XFlush(dpy);

//sleep(1);
 XSetForeground(dpy, gc, BlackPixel(dpy, DefaultScreen(dpy)));
XFillRectangle(dpy, w, gc, 0,350,500,500);

cursor_x+=TILESIZE+X_CURSOR_STEP;

//if(!MAGIC_MODE)
//{

if(cursor_x>500)
{
cursor_x=0;
cursor_y+=(TILESIZE+Y_CURSOR_STEP);
}

if(cursor_y>512)
{
cursor_y=0;
}

//}



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
	 if(GRAY_MODE)
	 {
	 pixel_color[colorcount].red = colorcount*pal_mult;//*pal_mult;//colorcount*pal_mult; // put this in red
     pixel_color[colorcount].green = colorcount*pal_mult;//colorcount*pal_mult; // put this in red
	 pixel_color[colorcount].blue =  colorcount*pal_mult;//colorcount*pal_mult; // put this in blue
	 }
	 else
	 {
	 // random palette 
	 
	pixel_color[colorcount].red = random()%256*pal_mult;//colorcount*pal_mult; // put this in red
	pixel_color[colorcount].green = random()%256*pal_mult;//colorcount*pal_mult; // put this in red
	pixel_color[colorcount].blue =  random()%256*pal_mult;//colorcount*pal_mult; // put this in blue
	 }
	 
	Status rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[colorcount]);
	 }
 
} // end of the function

void mutate_palette(Display *dpy, int mutation_rate)
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

int die_roll;

Colormap screen_colormap = DefaultColormap(dpy, DefaultScreen(dpy));

for(colorcount=0;colorcount<256;colorcount++) // otherwise, continue making the palette:
	 { // loop through the entire 256-color palette
     
	 die_roll=random()%100;
	 if(die_roll<mutation_rate)
	 {
	 // grayscale palette
	 if(GRAY_MODE)
	 {
	 pixel_color[colorcount].red = colorcount*pal_mult;//*pal_mult;//colorcount*pal_mult; // put this in red
     pixel_color[colorcount].green = colorcount*pal_mult;//colorcount*pal_mult; // put this in red
	 pixel_color[colorcount].blue =  colorcount*pal_mult;//colorcount*pal_mult; // put this in blue
	 }
	 else
	 {
	 // random palette 
	 
	pixel_color[colorcount].red = random()%256*pal_mult;//colorcount*pal_mult; // put this in red
	pixel_color[colorcount].green = random()%256*pal_mult;//colorcount*pal_mult; // put this in red
	pixel_color[colorcount].blue =  random()%256*pal_mult;//colorcount*pal_mult; // put this in blue
	 }
	 
	Status rc = XAllocColor(dpy,
						screen_colormap,
                        &pixel_color[colorcount]);
	 }
	 
	 }
 
} // end of the function

void letterDraw(int x, int y, int type, int depth)
    {
   	//long delay;
	double depth_offset=0;
	
	 if(depth==0)
	 {
	 public_tile_map[x+TILESIZE*y]=global_array[0][type][0];
	 return;
	 }
	 else
		{
		depth_offset=pow(2.0, (double)depth);
	
	       letterDraw(x, y, global_array[depth][type][0], depth-1);
		   letterDraw(x+((int)depth_offset), y, global_array[depth][type][1], depth-1);
		   letterDraw(x, y+((int)depth_offset), global_array[depth][type][2], depth-1);
		   letterDraw(x+((int)depth_offset), y+((int)depth_offset), global_array[depth][type][3], depth-1);
	     }
	
	}

int score_it(void)
{
int counter;
int score=0;

for(counter=0;counter<(TILESIZE*TILESIZE);counter++)
{
if(public_tile_map[counter]==target_tile_map[counter])
{
score++;
}

}

return score;
}

int change_up(int type, int depth)
{
int depth_c=0;
//MAX_DEPTH-1, 
int type_c, desc_c;
//global_array[MAX_DEPTH][MAX_TYPE][MAX_DESC];
int tempo1=type_c;
int tempo2=type_c;
int tempo3=type_c;
int tempo4=type_c;

//for(depth_c=MAX_DEPTH-1;depth_c>=depth;depth_c--)
//{
for(type_c=0;type_c<MAX_TYPE;type_c++)
{
global_array[depth_c][type_c][0]=global_array[depth_c][tempo1][0];
global_array[depth_c][type_c][1]=global_array[depth_c][tempo2][1];
global_array[depth_c][type_c][2]=global_array[depth_c][tempo3][2];
global_array[depth_c][type_c][3]=global_array[depth_c][tempo4][3];
}

//tempo1=global_array[depth_c][tempo1][0];
//tempo2=global_array[depth_c][tempo2][1];
//tempo3=global_array[depth_c][tempo3][2];
//tempo4=global_array[depth_c][tempo4][3];
//}


}
