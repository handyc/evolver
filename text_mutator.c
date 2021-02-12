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
#define TEST_MUT 300
//300 is ideal
// try 0.015 to 0.02 == 15 to 20?

#define TILESIZE 64
#define TOTAL_ORGANISMS_SQRT 16
#define TOTAL_ORGANISMS (TOTAL_ORGANISMS_SQRT*TOTAL_ORGANISMS_SQRT)
#define INITIAL_MUTATION_RATE TEST_MUT

#define MAX_X TILESIZE*TOTAL_ORGANISMS_SQRT
#define MAX_Y TILESIZE*TOTAL_ORGANISMS_SQRT

#define MAX_PAL 256
#define GENES 5 // size of one gene
#define ORGANISM_SIZE GENES*5 // size of each gene times number of genes to run

#define MAX_ITERATIONS 100000000 // maximum number of iterations to run program

int malloc2d(int *** grid, int nrows, int ncols);
//void letterDraw(int x, int y, int type, int depth);
void lineDDA(int x0, int y0, int x1, int y1, char color);
void draw_organisms(int x, int y);
void mutate_organisms(int starting_index, int mom_index, int dad_index);

char *scorereport="High score:";
char *mutationreport="Mutation rate:";
char *iterationreport="Iteration number:";
char scorestring[9]={'0','0','0','0','0','0','0','\0'};
char mutstring[7]={'0','0','0','0','0','\0'};
char iterationstring[7]={'0','0','0','0','0','\0'};

unsigned long iteration_number=0;
int sex_on=1;
unsigned long mutation_rate=INITIAL_MUTATION_RATE;
unsigned long other_mut_rate=7;
//unsigned int iteration_limit=TILESIZE/3; // this can be anything up to ORGANISM_SIZE

unsigned char target_tile_map[TILESIZE*TILESIZE];//*/
unsigned long helper;
unsigned long high_score_index[4];//={0, 0, 0, 0};
unsigned long high_score[4]={0, 0, 0, 0};
unsigned long current_score=0;

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

unsigned char * public_tile_map;//[TILESIZE*TILESIZE];
int ** child;
int ** winner;

int main()
{

malloc2d(&child, TOTAL_ORGANISMS, ORGANISM_SIZE);
malloc2d(&winner, 4, ORGANISM_SIZE);

public_tile_map=(unsigned char *) malloc(sizeof(unsigned char) * (TILESIZE*TILESIZE));

// BEGIN main() variables ***************************************

int counter=0, counter2=0, newpalcounter=0, option_to_quit=0;
int organism_number=0, position=0;
//int get_char='0'; 
int x_coord=0, y_coord=0, palswitcher=0;
unsigned int colorcount=0;

//int depth_c, type_c, desc_c;
int die_roll=0;

srandom(22134);

// initialize all organisms to 0
for(organism_number=0;organism_number<TOTAL_ORGANISMS;organism_number++)
{
for(position=0;position<ORGANISM_SIZE;position++)
{
child[organism_number][position]=random()%MAX_PAL;  // At this point we have no idea whatsoever which
}
}

for(iteration_number=0;iteration_number<MAX_ITERATIONS;iteration_number++)
{

printf("\nScore: %lu Mutation rate: %lu Iteration: %lu", high_score[0], mutation_rate, iteration_number);
draw_organisms(0, 0); // draw every single organism

for(position=0;position<ORGANISM_SIZE;position++) // set winners from scores
			{
			winner[0][position]=child[high_score_index[0]][position];
			}
	mutate_organisms(0, 0, 1);
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

void draw_organisms(int x, int y)
{
int x_coord=0, y_coord=0;
unsigned int newcounter_x, newcounter_y;
long mutation=0;
unsigned int position;
unsigned int gene_counter=0;
unsigned int child_number=0;
int in;

unsigned int sub_x, sub_y;

//scroll through each entire gene of every organism
//plot to tile_map as necessary

high_score[0]=0;
//high_score[1]=0;
//high_score[2]=0;
//high_score[3]=0;

/////

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
		for(in=0;in<GENES;in++)
	    {
	lineDDA((child[child_number][in])%TILESIZE, (child[child_number][in+1])%TILESIZE, (child[child_number][in+2])%TILESIZE,(child[child_number][in+3])%TILESIZE,0); 
	    }
		
		for(in=0;in<(TILESIZE*TILESIZE);in++)
		{
	    // Now compute scores for each organism
	    if(public_tile_map[in]==target_tile_map[in]) // did we hit the target byte?
	    {
		current_score+=255; // if so, give a score bonus
		}
	    //else current_score+=(255-abs(target_tile_map[in]-public_tile_map[in])); //child[child_number][in]));
	    // otherwise give some other bonus, maybe (probably better off without it)
		}
		
	     if(current_score>high_score[0]) {high_score_index[0]=child_number; high_score[0]=current_score;}
	
		   child_number++; // go to the next gene
		} // finished with one horizontal row
	 } // finished with one vertical column


}

void mutate_organisms(int starting_index, int mom_index, int dad_index)
{
long mutation=0;
unsigned int child_number;
unsigned int position;

for(child_number=(starting_index*TOTAL_ORGANISMS);child_number<(TOTAL_ORGANISMS*(starting_index+1));child_number++)
   {
   for(position=0;position<ORGANISM_SIZE;position++)
      {
      mutation=(random()%100000);//(TILESIZE*TILESIZE));
	  child[child_number][position]=winner[mom_index][position];//winner[winner_index][position];
		
		if(mutation<mutation_rate) // if we can mutate at this point
          { 
		  child[child_number][position]=random()%MAX_PAL;
		  }
	   }
   }
}

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