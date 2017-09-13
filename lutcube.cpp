// LUTcube.cpp
#include "lutcube.h"
#include <iostream>
#include <sstream>

string ReadLine ( ifstream & infile, char Delimiter, LUTState &status)
	{
		// Skip empty lines and comments
		const char CommentMarker = '#';
		string textLine("");
		while ( textLine.size() == 0 || textLine[0] == CommentMarker ) {
			if ( infile.eof() ) { status = PrematureEndOfFile; break; }
			getline ( infile, textLine, Delimiter );
			if ( infile.fail() ) { status = ReadError; break; }
			}
		return textLine;
	} // ReadLine
tableRow ParseTableRow ( const string & lineOfText, LUTState &status )
	{
		int N = 3;
		tableRow f ( N );
		istringstream line ( lineOfText );
		for (int i = 0; i < N; i++) {
			line >> f[i];
			if ( line.fail() ) { status = CouldNotParseTableData; break; };
			}
		return f;
	} // ParseTableRow
LUTState LUTCube:: LoadCubeFile ( ifstream & infile )
	{
		// Set defaults
		status = OK;
		title.clear();
		domainMin = tableRow ( 3, 0.0 );
		domainMax = tableRow ( 3, 1.0 );
		LUT1D.clear();
		LUT3D.clear();
		// Read file data line by line
		const char NewlineCharacter = '\n';
		char Delimiter = NewlineCharacter;
		// check if delimiter is old
		const char CarriageReturnCharacter = '\r';
		for (int i = 0; i < 255; i++) {
			char inc = infile.get();
			if ( inc == NewlineCharacter ) break;
			if ( inc == CarriageReturnCharacter ) {
				if ( infile.get() == NewlineCharacter ) break;
				Delimiter = CarriageReturnCharacter;
				clog << "INFO: This file uses non-compliant line separator \\r (0x0D)" << endl;
				break;
				}
			if ( i > 250 ) { status = LineError; break; }
			}
		infile.seekg ( 0 );
		infile.clear();
		// read keywords
		int N, CntTitle, CntSize, CntMin, CntMax;

		// each keyword to occur zero or one time
		N = CntTitle = CntSize = CntMin = CntMax = 0;
		while ( status == OK ) {
			long linePos = infile.tellg();
			string lineOfText = ReadLine ( infile, Delimiter, status );
			if ( ! status == OK ) break;
			// Parse keywords and parameters
			istringstream line ( lineOfText );
			string keyword;
			line >> keyword;
			if ( "+" < keyword && keyword < ":" ) {
				// lines of table data come after keywords
				// restore stream pos to re-read line of data
				infile.seekg ( linePos );
				break;
				}
			else if ( keyword == "TITLE" && CntTitle++ == 0 ) {
				const char QUOTE = '"';
				char startOfTitle;
				line >> startOfTitle;
				if ( startOfTitle != QUOTE ) { status = TitleMissingQuote; break; }
				getline ( line, title, QUOTE ); // read to "
				}
			else if ( keyword == "DOMAIN_MIN" && CntMin++ == 0 ) {
				line >> domainMin[0] >> domainMin[1] >> domainMin[2];
				}
			else if ( keyword == "DOMAIN_MAX" && CntMax++ == 0 ) {
				line >> domainMax[0] >> domainMax[1] >> domainMax[2];
				}
			else if ( keyword == "LUT_1D_SIZE" && CntSize++ == 0 ) {
				line >> N;
				if ( N < 2 || N > 65536 ) { status = LUTSizeOutOfRange; break; }
				LUT1D = table1D ( N, tableRow ( 3 ) );
				}
			else if ( keyword == "LUT_3D_SIZE" && CntSize++ == 0 ) {
				line >> N;
				if ( N < 2 || N > 256 ) { status = LUTSizeOutOfRange; break; }
				LUT3D = table3D ( N, table2D ( N, table1D ( N, tableRow ( 3 ) ) ) );
				}
			else { status = UnknownOrRepeatedKeyword; break; }
			if ( line.fail() ) { status = ReadError; break; }
			} // read keywords
		if ( status == OK && CntSize == 0 ) status = LUTSizeOutOfRange;
		if ( status == OK && ( domainMin[0] >= domainMax[0] || domainMin[1] >= domainMax[1]
							   || domainMin[2] >= domainMax[2] ) )
			status = DomainBoundsReversed;
		// read lines of table data
		if ( LUT1D.size() > 0 ) {
			N = LUT1D.size();
			for ( int i = 0; i < N && status == OK; i++ ) {
				LUT1D [i] = ParseTableRow ( ReadLine ( infile, Delimiter, status ), status );
				}
			} else {
			N = LUT3D.size();
			// NOTE that r loops fastest
			for ( int b = 0; b < N && status == OK; b++ ) {
				for ( int g = 0; g < N && status == OK; g++ ) {
					for ( int r = 0; r < N && status == OK; r++ ) {
						LUT3D[r][g][b] = ParseTableRow
								( ReadLine ( infile, Delimiter, status ), status );
						}
					}
				}
			} // read 3D LUT

		return status;

	} // LoadCubeFile
LUTState LUTCube:: SaveCubeFile ( ofstream & outfile )
	{
		if ( ! status == OK ) return status; // Write only good Cubes
		// Write keywords
		const char SPACE = ' ';
		const char QUOTE = '"';
		if ( title.size() > 0 ) outfile << "TITLE" << SPACE << QUOTE << title << QUOTE << endl;
		outfile << "# Created by LUTcube.cpp" << endl;
		outfile << "DOMAIN_MIN" << SPACE << domainMin[0] << SPACE << domainMin[1]
				<< SPACE << domainMin[2] << endl;
		outfile << "DOMAIN_MAX" << SPACE << domainMax[0] << SPACE << domainMax[1]
				<< SPACE << domainMax[2] << endl;
		// Write LUT data
		if ( LUT1D.size() > 0 ) {
			int N = LUT1D.size();
			outfile << "LUT_1D_SIZE" << SPACE << N << endl;
			for ( int i = 0; i < N && outfile.good(); i++ ) {
				outfile << LUT1D[i] [0] << SPACE << LUT1D[i] [1] << SPACE << LUT1D[i] [2] << endl;
				}
			} else {
			int N = LUT3D.size();
			outfile << "LUT_3D_SIZE" << SPACE << N << endl;
			// NOTE that r loops fastest
			for ( int b = 0; b < N && outfile.good(); b++ ) {
				for ( int g = 0; g < N && outfile.good(); g++ ) {
					for ( int r = 0; r < N && outfile.good(); r++ ) {
						outfile << LUT3D[r][g][b] [0] << SPACE << LUT3D[r][g][b] [1]
													  << SPACE << LUT3D[r][g][b] [2] << endl;
						}
					}
				}
			} // write 3D LUT
		outfile.flush();
		return ( outfile.good() ? OK : WriteError );
	} // SaveCubeFile

int test (int argc, char * const argv[])
	{
		LUTCube theCube;
		enum { OK = 0, ErrorOpenInFile = 100, ErrorOpenOutFile };
		if ( argc < 2 || 3 < argc ) {
			cout << "Usage: " << argv[0] << " cubeFileIn [cubeFileOut]" << endl;
			return OK;
			}
		// Load a Cube
		ifstream infile ( argv[1] );
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


		// Save a Cube
		if (argc > 2 )
			{
			ofstream outfile ( argv[2], fstream::trunc );
			if ( !outfile.good() ) {
				cout << "Could not open output file " << argv[1] << endl;
				return ErrorOpenOutFile;
				}
			int ret = theCube.SaveCubeFile ( outfile );
			outfile.close();
			if ( ret != OK ) {
				cout << "Could not write the cube to the output file. Return code = "
					 << ret << endl;
				return theCube.status;
				}
			}
		return OK;
	}
