#include <cstdlib>
#include <iostream>
#include <fstream>
#include "lutcube.h"
#include "graf.h"

using namespace std;

string test_filename = "24bb.bmp";
string test_dir = "C:/Users/Karol/Desktop/graftest/";

int main()
	{
		ifstream infile;
		ofstream outfile;

		graf_file_struct *image = new graf_file_struct;
		test_filename = test_dir + test_filename;

		infile.open(&test_filename[0], ios::binary);

		graf_bmp_read(&test_filename[0], image);
		graf_print(image, 1, 1);
		infile.close();
		cout << endl;
		graf_negative(image);

		test_filename = test_dir + "chgd24bb.bmp";
		cout << "Opening " << test_filename << endl;
		outfile.open(&test_filename[0], ios::binary);
		graf_bmp_write(&test_filename[0], image);
		outfile.close();
		cout << "Saved " << test_filename << " successfully";


		cout << "Opening " <<  test_filename << endl;
		infile.open(&test_filename[0], ios::binary);
		graf_bmp_read(&test_filename[0], image);
		infile.close();

		graf_print(image, 1, 1);

		return 0;
	}
