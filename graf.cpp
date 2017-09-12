#include "graf.h"
#include <iostream>
#include <fstream>

Graf::Graf()
	{

	}

Graf BMPFileWrite (std::ofstream & outfile)
	{
		if (outfile.is_open()!=1)
			{
			std::cerr << "Failed in creating" << std::endl;
			exit(1);
			}

		Graf4ByteUnion *temp_u = new Graf4ByteUnion;

		outfile.write("BM", 2);

		temp_u->s4Byte = head.fileSize;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s2Byte[0] = Graf.head.reserved1;
		temp_u->s2Byte[1] = head.reserved2;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::head.offset;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::head.infoSize;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::info.width;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::info.height;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s2Byte[0] = Graf::info.planes;
		outfile.write(temp_u->s1Byte, 2);

		temp_u->s2Byte[0] = Graf::info.bits;
		outfile.write(temp_u->s1Byte, 2);

		temp_u->s4Byte = Graf::info.compression;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::info.imageSize;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::info.xResolution;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::info.yResolution;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::info.nColours;
		outfile.write(temp_u->s1Byte, 4);

		temp_u->s4Byte = Graf::info.importantColours;
		outfile.write(temp_u->s1Byte, 4);

		int RowPBits = Graf::info.bits*info.width;
		unsigned short RowBytes = ((Graf::info.bits*Graf::info.width+31)/32)*4;
		unsigned short Padding;
		if ((8*RowBytes-RowPBits)%32){
			Padding = (8*RowBytes-RowPBits)/8;
			}
		else{
			Padding = 0;
			}

		if (Graf::type == BMPRGB24){
			for (int i = info.height-1; i >= 0; i--){
				outfile.write(RGB24b[i*info.width].s1byte, RowBytes-Padding);
				temp_u->s4Byte = 0;
				outfile.write(temp_u->s1Byte, Padding);
				}


			delete temp_u;
			outfile.close();
			}
	}

Graf BMPFileRead (std::ifstream & infile)
	{
		if (infile.is_open()!=1)
			{
			std::cerr << "Failed in opening" << std::endl;
			exit(1);
			}

		Graf4ByteUnion *temp_u = new Graf4ByteUnion;

		infile.read(temp_u->s1Byte, 2);
		Graf::head.type = temp_u->s2Byte[0];
		if (Graf::head.type != 0x4d42){
			std::cout << "Incorrect header(hex): " << std::hex << Graf::head.type << std::endl;
			bool warn = infile.fail();
			if (warn){
				std::cout << "File failed somehow" << std::endl;
				}
			else{
				std::cout << "Reading not failed" << std::endl;
				}
			exit(2);
			}


		infile.read(temp_u->s1Byte, 4);
		Graf::head.fileSize = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::head.reserved1=temp_u->s2Byte[0];
		Graf::head.reserved2=temp_u->s2Byte[1];
		if (Graf::head.reserved1!=0||Graf::head.reserved2!=0){
			std::cerr << "Reserved bytes invalid";
			exit(2);
			}

		infile.read(temp_u->s1Byte, 4);
		Graf::head.offset = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::head.infoSize = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::info.width = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::info.height = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 2);
		Graf::info.planes = temp_u->s2Byte[0];

		infile.read(temp_u->s1Byte, 2);
		Graf::info.bits = temp_u->s2Byte[0];

		infile.read(temp_u->s1Byte, 4);
		Graf::info.compression = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::info.imageSize = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::info.xResolution = temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::info.yResolution= temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::info.nColours= temp_u->s4Byte;

		infile.read(temp_u->s1Byte, 4);
		Graf::info.importantColours= temp_u->s4Byte;

		if (Graf::info.compression!=0){
			std::cerr << "Unsupported BMP compression";
			exit(3);
			}
		int RowPBits = Graf::info.bits*Graf::info.width;
		unsigned short RowBytes = ((Graf::info.bits*Graf::info.width+31)/32)*4;
		unsigned short Padding;
		if ((8*RowBytes-RowPBits)%32){
			Padding = (8*RowBytes-RowPBits)/8;
			}
		else{
			Padding = 0;
			}

		if (infile.tellg() != Graf::head.offset){
			std::cerr << "Head longer than expected" <<std::endl;
			infile.seekg(Graf::head.offset);
			}


		if (Graf::info.bits == 24){
			type = BMPRGB24;
			RGB24b = new RGB24bUnion[Graf::info.height*Graf::info.width];

			for (int i = Graf::info.height-1; i >= 0 && infile.eof()!=1; i--){
				infile.read(RGB24b[i*info.width].s1byte, RowBytes-Padding);

				temp_u->s4Byte = 0;
				infile.read(temp_u->s1Byte, Padding);
				if (temp_u->s4Byte != 0){
					std::cerr << "Padding error";
					}
				}

			delete temp_u;
			infile.close();
			}
	}

Graf Graf::FilePrint (bool ShowHead, bool ShowInfo){
		if (type == BMPRGB24){
			if (ShowHead){
				std::cout << "Magic(hex)       = " << std::hex << Graf::head.type << std::endl;
				std::cout << "File size(dec)   = " << std::dec << Graf::head.fileSize << std::endl;
				std::cout << "Reserved(dec)    = " << Graf::head.reserved1 << " & " << Graf::head.reserved2<< std::endl;
				std::cout << "Offset(dec)      = " << Graf::head.offset << std::endl;
				std::cout << "Header size(dec) = " << Graf::head.infoSize << std::endl;
				}
			if (ShowInfo){
				std::cout << "Width(dec)             = " << Graf::info.width << std::endl;
				std::cout << "Height(dec)            = " << std::dec << Graf::info.height << std::endl;
				std::cout << "Planes(dec)            = " << Graf::info.planes << std::endl;
				std::cout << "Bits per pixel(dec)    = " << Graf::info.bits << std::endl;
				std::cout << "Compression(dec)       = " << Graf::info.compression << std::endl;
				std::cout << "Image size(dec)        = " << Graf::info.imageSize << std::endl;
				std::cout << "X pix per meter(dec)   = " << Graf::info.xResolution<< std::endl;
				std::cout << "Y pix per emeter(dec)  = " << Graf::info.yResolution << std::endl;
				std::cout << "Number of colours(dec) = " << Graf::info.nColours<< std::endl;
				std::cout << "Colors important(dec)  = " << Graf::info.importantColours << std::endl;
				}

			for (int i = Graf::info.height-1; i >= 0; i--){
				for (int j = 0; j < Graf::info.width; j++){
					std::cout << (int)RGB24b[i*Graf::info.width+j].colour.red << ' ' \
						<< (int)RGB24b[i*Graf::info.width+j].colour.green << ' '\
						<< (int)RGB24b[i*Graf::info.width+j].colour.blue << "   ";
					}
				}
			}

	}

Graf Graf::Negative (void){
	if (type==BMPRGB24){
		for (int i = 0; i < Graf::info.width*Graf::info.height; i++){
			RGB24b[i].s1byte[0] = ~RGB24b[i].s1byte[0];
			RGB24b[i].s1byte[1] = ~RGB24b[i].s1byte[1];
			RGB24b[i].s1byte[2] = ~RGB24b[i].s1byte[2];
			}
		}
	}
