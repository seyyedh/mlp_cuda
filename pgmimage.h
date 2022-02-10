#ifndef _PGMIMAGE_H_

#define _PGMIMAGE_H_

typedef struct {
  char *name;
  int rows, cols;
  int *data;
} IMAGE;

typedef struct {
  int n;
  IMAGE **list;
} IMAGELIST;

/*** User accessible macros ***/

#define ROWS(img)  ((img)->rows)
#define COLS(img)  ((img)->cols)
#define NAME(img)   ((img)->name)

/*** User accessible functions ***/

IMAGE *img_open();
IMAGE *img_creat();
IMAGE *img_alloc();
IMAGELIST *imgl_alloc();

void img_setpixel();
int img_getpixel();
int img_write();
void img_free();

char *img_basename(char *filename);
void imgl_munge_name(char *buf);
void imgl_load_images_from_textfile();

void imgl_add();
void imgl_free();

#include "backprop.h"

void load_target(
    IMAGE *img,
    BPNN *net);

void load_input_with_image(
    IMAGE *img,
    BPNN *net);

#endif
