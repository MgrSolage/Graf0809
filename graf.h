#ifndef GRAF_H
#define GRAF_H
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

typedef struct
{
	unsigned short int type;                 /* Magic identifier    'BM'    */
	unsigned int fileSize;                  /* File size in bytes          */
	unsigned short int reserved1, reserved2; /* Must be NULL */
	unsigned int offset;                     /* Offset to image data, bytes */
	unsigned int infoSize;                  /* Header size in bytes      */
} BMPHeadStruct;

typedef struct
{
	unsigned int width, height;      /* Width and height of image */
	unsigned short int planes;       /* Number of colour planes   */
	unsigned short int bits;         /* Bits per pixel            */
	unsigned int compression;        /* Compression type          */
	unsigned int imageSize;          /* Image size in bytes       */
	int xResolution, yResolution;    /* Pixels per meter          */
	unsigned int nColours;           /* Number of colours         */
	unsigned int importantColours;   /* Important colours         */
} BMPInfoStruct;

typedef union
{
	char			s1Byte[4];
	unsigned short  s2Byte[2];
	unsigned int	s4Byte;
} Graf4ByteUnion;

typedef union
{
	char		 s1byte[3];
	typedef struct
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
	} colour;
} RGB24bUnion;

class Graf
{
public:
	Graf();

	enum typeEnum {BMP16, BMPRGB24, BMPARGB32} type;
	RGB24bUnion* RGB24b;
	BMPHeadStruct head;
	BMPInfoStruct info;
	Graf BMPFileRead ( std::ifstream & infile);
	Graf BMPFileWrite ( std::ofstream & outfile);
	Graf FilePrint (bool ShowHead, bool ShowInfo);
	Graf Negative (void);
};


#endif // GRAF_H
