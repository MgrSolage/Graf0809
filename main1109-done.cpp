#include <iostream>
#include <fstream>

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


//===================================/* Functions */====================================

void graf_bmp_write (string filename, graf_file_struct* graf_file)
	{
		ofstream file;
		file.open(filename, ios::binary);
		if (file.is_open()!=1)
			{
			cerr << "Failed in creating" << endl;
			exit(1);
			}

		graf_4byte *temp_u = new graf_4byte;

		file.write("BM", 2);

		temp_u->s4byte = graf_file->head.file_size;
		file.write(temp_u->s1byte, 4);

		temp_u->s2byte[0] = graf_file->head.reserved1;
		temp_u->s2byte[1] = graf_file->head.reserved2;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->head.offset;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->head.info_size;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->info.width;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->info.height;
		file.write(temp_u->s1byte, 4);

		temp_u->s2byte[0] = graf_file->info.planes;
		file.write(temp_u->s1byte, 2);

		temp_u->s2byte[0] = graf_file->info.bits;
		file.write(temp_u->s1byte, 2);

		temp_u->s4byte = graf_file->info.compression;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->info.imagesize;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->info.xresolution;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->info.yresolution;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->info.ncolours;
		file.write(temp_u->s1byte, 4);

		temp_u->s4byte = graf_file->info.importantcolours;
		file.write(temp_u->s1byte, 4);

		int graf_row_pbits = graf_file->info.bits*graf_file->info.width;
		unsigned short graf_row_bytes = ((graf_file->info.bits*graf_file->info.width+31)/32)*4;
		unsigned short graf_padding;
		if ((8*graf_row_bytes-graf_row_pbits)%32){
			graf_padding = (8*graf_row_bytes-graf_row_pbits)/8;
			}
		else{
			graf_padding = 0;
			}

		if (graf_file->type == bmp_rgb24){
			for (int i = graf_file->info.height-1; i >= 0; i--){
				file.write(graf_file->graf_RGB_24b[i*graf_file->info.width].s1byte, graf_row_bytes-graf_padding);

				temp_u->s4byte = 0;
				file.write(temp_u->s1byte, graf_padding);
				}


			delete temp_u;
			file.close();
			}
	}

void graf_bmp_read (string filename, graf_file_struct* graf_file)
	{
		ifstream file;
		file.open(filename, ios::binary);
		if (file.is_open()!=1)
			{
			cerr << "Failed in opening" << endl;
			exit(1);
			}

		graf_4byte *temp_u = new graf_4byte;

		file.read(temp_u->s1byte, 2);
		graf_file->head.type = temp_u->s2byte[0];
		if (graf_file->head.type != 0x4d42){
			cout << "Incorrect header(hex): " << hex << graf_file->head.type << endl;
			bool warn = file.fail();
			if (warn){
				cout << "File failed somehow" << endl;
				}
			else{
				cout << "Reading not failed" << endl;
				}
			exit(2);
			}

		file.read(temp_u->s1byte, 4);
		graf_file->head.file_size = temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->head.reserved1=temp_u->s2byte[0];
		graf_file->head.reserved2=temp_u->s2byte[1];
		if (graf_file->head.reserved1!=0||graf_file->head.reserved2!=0){
			cerr << "Reserved bytes invalid";
			exit(2);
			}

		file.read(temp_u->s1byte, 4);
		graf_file->head.offset = temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->head.info_size = temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->info.width = temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->info.height = temp_u->s4byte;

		file.read(temp_u->s1byte, 2);
		graf_file->info.planes = temp_u->s2byte[0];

		file.read(temp_u->s1byte, 2);
		graf_file->info.bits = temp_u->s2byte[0];

		file.read(temp_u->s1byte, 4);
		graf_file->info.compression = temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->info.imagesize = temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->info.xresolution = temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->info.yresolution= temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->info.ncolours= temp_u->s4byte;

		file.read(temp_u->s1byte, 4);
		graf_file->info.importantcolours= temp_u->s4byte;

		if (graf_file->info.compression!=0){
			cerr << "Unsupported BMP compression";
			exit(3);
			}
		int graf_row_pbits = graf_file->info.bits*graf_file->info.width;
		unsigned short graf_row_bytes = ((graf_file->info.bits*graf_file->info.width+31)/32)*4;
		unsigned short graf_padding;
		if ((8*graf_row_bytes-graf_row_pbits)%32){
			graf_padding = (8*graf_row_bytes-graf_row_pbits)/8;
			}
		else{
			graf_padding = 0;
			}

		if (file.tellg() != graf_file->head.offset){
			cerr << "Head longer than expected" <<endl;
			file.seekg(graf_file->head.offset);
			}


		if (graf_file->info.bits == 24){
			graf_file->type = bmp_rgb24;
			graf_file->graf_RGB_24b = new graf_RGB_24b_union[graf_file->info.height*graf_file->info.width];

			for (int i = graf_file->info.height-1; i >= 0 && file.eof()!=1; i--){
				file.read(graf_file->graf_RGB_24b[i*graf_file->info.width].s1byte, graf_row_bytes-graf_padding);

				temp_u->s4byte = 0;
				file.read(temp_u->s1byte, graf_padding);
//				cout << file.tellg() << endl;
				if (temp_u->s4byte != 0){
					cerr << "Padding error";
					}
				}

			delete temp_u;
			file.close();
			}
	}


void graf_print (graf_file_struct *graf_file, bool show_head, bool show_info){
		if (graf_file->type == bmp_rgb24){
			if (show_head){
				cout << "Magic(hex)       = " << hex << graf_file->head.type << endl;
				cout << "File size(dec)   = " << dec << graf_file->head.file_size << endl;
				cout << "Reserved(dec)    = " << dec << graf_file->head.reserved1 << " & " << graf_file->head.reserved2<< endl;
				cout << "Offset(dec)      = " << dec << graf_file->head.offset << endl;
				cout << "Header size(dec) = " << dec << graf_file->head.info_size << endl;
				}
			if (show_info){
				cout << "Width(dec)             = " << dec << graf_file->info.width << endl;
				cout << "Height(dec)            = " << dec << graf_file->info.height << endl;
				cout << "Planes(dec)            = " << dec << graf_file->info.planes << endl;
				cout << "Bits per pixel(dec)    = " << dec << graf_file->info.bits << endl;
				cout << "Compression(dec)       = " << dec << graf_file->info.compression << endl;
				cout << "Image size(dec)        = " << dec << graf_file->info.imagesize << endl;
				cout << "X pix per meter(dec)   = " << dec << graf_file->info.xresolution<< endl;
				cout << "Y pix per emeter(dec)  = " << dec << graf_file->info.yresolution << endl;
				cout << "Number of colours(dec) = " << dec << graf_file->info.ncolours<< endl;
				cout << "Colors important(dec)  = " << dec << graf_file->info.importantcolours << endl;
				}

			for (int i = graf_file->info.height-1; i >= 0; i--){
				for (int j = 0; j < graf_file->info.width; j++){
					cout << (int)graf_file->graf_RGB_24b[i*graf_file->info.width+j].colour.red << ' ' \
						<< (int)graf_file->graf_RGB_24b[i*graf_file->info.width+j].colour.green << ' '\
						<< (int)graf_file->graf_RGB_24b[i*graf_file->info.width+j].colour.blue << "   ";
					}
				}
			}

	}

void graf_negative (graf_file_struct *graf_file){
	if (graf_file->type==bmp_rgb24){
		for (int i = 0; i < graf_file->info.width*graf_file->info.height; i++){
			graf_file->graf_RGB_24b[i].s1byte[0] = ~graf_file->graf_RGB_24b[i].s1byte[0];
			graf_file->graf_RGB_24b[i].s1byte[1] = ~graf_file->graf_RGB_24b[i].s1byte[1];
			graf_file->graf_RGB_24b[i].s1byte[2] = ~graf_file->graf_RGB_24b[i].s1byte[2];
			}
		}
	}

string test_filename = "24bb.bmp";
string test_dir = "C:/Users/Karol/Desktop/graftest/";

int main()
	{
		graf_file_struct *image = new graf_file_struct;
		//		cin >> test_filename;
		test_filename = test_dir + test_filename;
		//		cout << test_filename << endl;

		graf_bmp_read(&test_filename[0], image);
		graf_print(image, 1, 1);
		cout << endl;
//		graf_negative(image);

		test_filename = test_dir + "chgd24bb.bmp";
				cout << test_filename << endl;
		graf_bmp_write(&test_filename[0], image);

		test_filename = test_dir + "24bb.bmp";
				cout << test_filename << endl;
		graf_bmp_read(&test_filename[0], image);

		graf_print(image, 1, 1);


		delete image;
		return 0;
	}
