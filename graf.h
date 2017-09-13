#ifndef GRAF_H
#define GRAF_H
#include <iostream>
#include <fstream>
#include <cmath>
#include "lutcube.h"

using namespace std;


//typedef	unsigned char graf_byte;

typedef union graf_4byte_union
{
	char			s1byte[4];
	unsigned short  s2byte[2];
	unsigned int	s4byte;
} graf_4byte;

typedef struct
{
	unsigned short int type;                 /* Magic identifier    'BM'    */
	unsigned int file_size;                  /* File size in bytes          */
	unsigned short int reserved1, reserved2; /* Must be NULL */
	unsigned int offset;                     /* Offset to image data, bytes */
	unsigned int info_size;                  /* Header size in bytes      */
} graf_bmp_head_struct;

typedef struct
{
	unsigned int width, height;      /* Width and height of image */
	unsigned short int planes;       /* Number of colour planes   */
	unsigned short int bits;         /* Bits per pixel            */
	unsigned int compression;        /* Compression type          */
	unsigned int imagesize;          /* Image size in bytes       */
	int xresolution,yresolution;     /* Pixels per meter          */
	unsigned int ncolours;           /* Number of colours         */
	unsigned int importantcolours;   /* Important colours         */
} graf_bmp_info_struct;

// TODO struct palette dla indexed colour data

typedef struct
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} graf_RGB_24b_struct;

typedef union
{
	char			    s1byte[3];
	graf_RGB_24b_struct colour;
} graf_RGB_24b_union;

enum type_enum {bmp_16, bmp_rgb24, bmp_argb32};

typedef struct
{
	type_enum type;
	graf_RGB_24b_union* graf_RGB_24b;
	graf_bmp_head_struct head;
	graf_bmp_info_struct info;

}graf_file_struct;


void graf_bmp_write (string filename, graf_file_struct* graf_file);
void graf_bmp_read (string filename, graf_file_struct* graf_file);
void graf_print (graf_file_struct *graf_file, bool show_head, bool show_info);
void graf_negative (graf_file_struct *graf_file);
double delta (unsigned char colour, int N, int &wynik);


#endif // GRAF_H
