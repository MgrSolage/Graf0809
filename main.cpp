#include <cstdlib>
#include <iostream>
#include <fstream>
#include "lutcube.h"
#include "graf.h"

using namespace std;

string testFilename = "C:/Users/Karol/Desktop/graftest/24bb.bmp";
string testFilename2;
string testDir = "C:/Users/Karol/Desktop/graftest/";
//		testFilename = testDir + testFilename;

int main()
	{
		Graf image;
		ifstream infile;
		ofstream outfile;

		infile.open(&testFilename[0], ios::binary);
		image.BMPFileRead(infile);
		infile.close();
		image.FilePrint(0, 0);
		cout << endl;

		testFilename2 = testDir + "chgd24bb.bmp";
		cout << "Opening " << testFilename2 << endl;

		outfile.open(&testFilename2[0], ios::binary);
		image.BMPFileWrite(outfile);
		infile.close();
		cout << endl;

		infile.open(&testFilename2[0], ios::binary);
		image.BMPFileRead(infile);
		infile.close();
		image.FilePrint(0, 0);

		return 0;
	}
