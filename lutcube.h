// LUTcube.h
#ifndef LUTCUBE_H
#define LUTCUBE_H
#include <string>
#include <vector>
#include <fstream>

using namespace std;

enum LUTState { OK = 0, NotInitialized = 1,
				ReadError = 10, WriteError, PrematureEndOfFile, LineError,
				UnknownOrRepeatedKeyword = 20, TitleMissingQuote, DomainBoundsReversed,
				LUTSizeOutOfRange, CouldNotParseTableData };

typedef vector <float> tableRow;
typedef vector <tableRow> table1D;
typedef vector <table1D> table2D;
typedef vector <table2D> table3D;

class LUTCube {
public:
	LUTState status;
	string title;
	tableRow domainMin;
	tableRow domainMax;
	table1D LUT1D;
	table3D LUT3D;
	LUTCube ( void ) { status = NotInitialized; };
	LUTState LoadCubeFile ( ifstream & infile );
	LUTState SaveCubeFile ( ofstream & outfile );
};

tableRow ParseTableRow ( const string & lineOfText, LUTState & status );
string ReadLine (ifstream & infile, char lineSeparator, LUTState & status);

#endif // LUTCUBE_H
