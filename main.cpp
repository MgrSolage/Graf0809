#include <cstdlib>
#include <iostream>
#include <fstream>
#include "lutcube.h"
#include "graf.h"

using namespace std;

string test_filename = "C:/Users/Karol/Desktop/graftest/24bb.bmp";
string test_filename2 = "C:/Users/Karol/Desktop/graftest/chgd24bb.bmp";
string test_dir = "C:/Users/Karol/Desktop/graftest/";

int main(int argc, char * const argv[])

	{
		ifstream infile;
		ofstream outfile;
		graf_file_struct *image = new graf_file_struct;
		infile.open(&test_filename[0], ios::binary);


		unsigned char colour = 120;
		int R0;
	//	double delta_r = delta (colour, 32, R0, 1.0, 255.0);
//		cout << R0 << " oraz reszta " << delta_r <<endl;

		// Read & print
		graf_bmp_read(&test_filename[0], image);
		graf_print(image, 0, 0);
		infile.close();
		cout << endl;
//		graf_negative(image);
		//=========================Load Cube
		LUTCube theCube;
		enum { OK = 0, ErrorOpenInFile = 100, ErrorOpenOutFile };
		// Load a Cube
		infile.open( argv[1] );
		if ( !infile.good() ) {
			cout << "Could not open input file " << argv[1] << endl;
			return ErrorOpenInFile;
			}
		int ret = theCube.LoadCubeFile ( infile );
		infile.close();
		if ( ret != OK ) {
			cout << "Could not parse the cube info in the input file. Return code = "
				 << ret << endl;
			return theCube.status;
			}


		graf_lut_it(image, theCube);
		graf_print(image, 1, 0);

		cout << "Opening " << test_filename2 << endl;
		outfile.open(&test_filename2[0], ios::binary);
		graf_bmp_write(&test_filename2[0], image);
		outfile.close();
		cout << "Saved " << test_filename2 << " successfully";


		cout << "Opening " <<  test_filename2 << endl;
		infile.open(&test_filename2[0], ios::binary);
		graf_bmp_read(&test_filename2[0], image);
		infile.close();

		graf_print(image, 0, 0);

		return 0;
	}
