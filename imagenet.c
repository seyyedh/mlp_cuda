#include <stdio.h>
#include "pgmimage.h"
#include "backprop.h"
#include <string.h>

extern void exit();

#define TARGET_HIGH 0.8
#define TARGET_LOW 0.2


/*** This is the target output encoding for a network with one output unit.
     It scans the image name, and if it's an image of me (js) then
     it sets the target unit to HIGH; otherwise it sets it to LOW.
     Remember, units are indexed starting at 1, so target unit 1
     is the one to change....  ***/

void load_target(
    IMAGE *img,
    BPNN *net)
{
  int scale;
  char userid[40], head[40], expression[40], eyes[40], photo[40];

  userid[0] = head[0] = expression[0] = eyes[0] = photo[0] = '\0';

  /*** scan in the image features ***/
  sscanf(NAME(img), "%[^_]_%[^_]_%[^_]_%[^_]_%d.%[^_]",
    userid, head, expression, eyes, &scale, photo);

	//target(4,3,2,1)	  
    if (!strcmp(userid, "at33"))		//0000
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "boland"))		//0001
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "bpm"))		//0010
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "ch4f"))	//0011
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "cheyer"))	//0100
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "choon"))		//0101
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "danieln"))	//0110
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "glickman"))		//0111
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_LOW;}
    
    else if (!strcmp(userid, "kawamura"))	//1000
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_HIGH;}
    
    else if (!strcmp(userid, "megak"))	//1001
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_HIGH;}
    
    else if (!strcmp(userid, "mitchell"))		//1010
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_HIGH;}
    
    else if (!strcmp(userid, "night"))		//1011
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_LOW ;net->target[4] = TARGET_HIGH;}
    
    else if (!strcmp(userid, "phoebe"))	//1100
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_HIGH;}
    
    else if (!strcmp(userid, "steffi"))	//1101
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_LOW ;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_HIGH;}
    
    else if (!strcmp(userid, "tammo"))	//1110
    { net->target[1] = TARGET_LOW ;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_HIGH;}
    
    else if (!strcmp(userid, "an2i"))	//1111
    { net->target[1] = TARGET_HIGH;net->target[2] = TARGET_HIGH;net->target[3] = TARGET_HIGH;net->target[4] = TARGET_HIGH;}
	else
	{
		printf("user id error");
		//getch();
		exit (0);
	}

	//target(6,5)
	if (!strcmp(head, "left"))			//11
	{ net->target[5] = TARGET_HIGH;net->target[6] = TARGET_HIGH;}

	else if (!strcmp(head, "right"))		//01
	{ net->target[5] = TARGET_HIGH;net->target[6] = TARGET_LOW;}

	else if (!strcmp(head, "up"))			//10
	{ net->target[5] = TARGET_LOW ;net->target[6] = TARGET_HIGH;}

	else if (!strcmp(head, "straight"))	//00
	{ net->target[5] = TARGET_LOW ;net->target[6] = TARGET_LOW;}
	else
	{
		printf("head direction error");
		//getch();
		exit (0);
	}

	//target(7)
	if (!strcmp(eyes, "open"))
		net->target[7] = TARGET_HIGH;
	else if(!strcmp(eyes, "sunglasses"))
		net->target[7] = TARGET_LOW;
	else
	{
		printf("eyes description error");
		//getch();
		exit (0);
	}
}

/***********************************************************************/
/********* You shouldn't need to change any of the code below.   *******/
/***********************************************************************/
/*
void load_input_with_image(
    IMAGE *img,
    BPNN *net)
{
  double *units;
  int nr, nc, imgsize, i, j, k;

  nr = ROWS(img);
  nc = COLS(img);
  imgsize = nr * nc;;
  if (imgsize != net->input_n) {
    printf("LOAD_INPUT_WITH_IMAGE: This image has %d pixels,\n", imgsize);
    printf("   but your net has %d input units.  I give up.\n", net->input_n);
    exit (-1);
  }

  units = net->input_units;
  k = 1;
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      units[k] = ((double) img_getpixel(img, i, j)) / 255.0;
      k++;
    }
  }
}
*/

void load_input_with_image(IMAGE *img, BPNN *net)
/*IMAGE *img;
BPNN *net;*/
{
  double *units;
  int nr, nc, imgsize, i, j, k;
  double Temp;

  nr = ROWS(img);
  nc = COLS(img);
  imgsize = nr * nc;;
  if (imgsize != net->input_n)
  {
    printf("LOAD_INPUT_WITH_IMAGE: This image has %d pixels,\n", imgsize);
    printf("   but your net has %d input units.  I give up.\n", net->input_n);
    exit (-1);
  }

  units = net->input_units;
  k = 1;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
	{
		Temp=(double) img_getpixel(img, i, j);

		units[k] = ((double) img_getpixel(img, i, j)) / 256.0;//255.0///////////
		//units[k] = ((double) img_getpixel(img, i, j)) / 160.0;//255.0///////////

		// printf("units pre:%f\t",units[k]);
		// getch();

		if(Temp>255)
			printf("img:%d\t,Temp:%f\t,units:%.2f\n",img_getpixel(img, i, j),Temp,units[k]);

      k++;
    }
  }
}
