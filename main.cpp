#include <cstdlib>
#include <iostream>
#include <fstream>
#include "lutcube.h"
#include "graf.h"

using namespace std;

void menuView(){
		cout << endl
			 << "Menu:" << endl
			 << "	1. Wczytaj obraz" << endl
			 << "	2. Wykonaj negatyw" << endl
			 << "	3. Skoryguj przestrzen barw" << endl
			 << "	4. Rozjasnij wybrany obszar" << endl
			 << "	5. Zapisz obraz" << endl
			 << "	6. Zakoncz" << endl;
		}
bool menuYes(){
		char choice;
		while (true){
			cout << " <T/N>: ";
			cin.clear();
			cin.sync();
			cin >> choice;
//			cout << endl;
			if (choice == 'T' || choice =='Y' || choice =='t' || choice =='y' || choice =='1'){
				return 1;
				}
			else if (choice == 'N' || choice =='n' || choice =='Y'){
				return 0;
				}
			}
	}

enum menuChoice {none = 0, read = 1, negatyw = 2, lut = 3, brighten = 4, save = 5, Exit = 6};
menuChoice menuRead (grafStatus status){
		menuChoice retV;
		int choice;
		bool correct = NULL;
		while (cin.fail() || !correct){
			cin.clear();
			cin.sync();
			cout << "<1, 6>: ";
			cin >> choice;

			if (cin.good()) {
				switch (choice){
				case 1:{
				retV = read;
				if (status == 0 || status == 1){
					correct = 1;
					}
				else{
					if (status == 3) {
						cout << "Utracisz zmiany. Napewno chcesz wczytac?";
						if (menuYes()) {
							correct = 1;
							}
						}
					if (status == 2) {
						cout << "Tylko wczytales plik. Chcesz wczytac inny?";
						if (menuYes()) {
							correct = 1;
							}
						}
					}
				}
				break;

				case 2:{
				retV = negatyw;
				if (status == 0 || status == 2 || status == 3){
					correct = 1;
					}
				else{
					if (status == 1) {
						cout << "Nie wczytales pliku. Wczytaj plik <1>" << endl;
						}
					}
				}
				break;

				case 3:{
				retV = lut;
				if (status == 0 || status == 2 || status == 3){
					correct = 1;
					}
				else{
					if (status == 1) {
						cout << "Nie wczytales pliku. Wczytaj plik <1>" << endl;
						}
					}
				}
				break;

				case 4:{
				cout << "Opcja jeszcze niedostepna. Wybierz cos innego <1, 6>" << endl;

				}
				break;

				case 5:{
				retV = save;
				if (status == 3){
					correct = 1;
					}
				else{
					if (status == 0) {
						cout << "Nie dokonales zmian, chcesz zapisac jeszcze raz?";
						if (menuYes()) {
							correct = 1;
							}
						}
					if (status == 1) {
						cout << "Nie wczytales pliku. Wczytaj plik <1>" << endl;
						}
					if (status == 2) {
						cout << "Tylko wczytales plik, chcesz go zapisac?";
						if (menuYes()) {
							correct = 1;
							}
						}
					}
				}
				break;

				case 6:{
				retV = Exit;
				if (status == 0 || status == 1){
					correct = 1;
					}
				else{
					if (status == 2) {
						cout << "Tylko wczytales plik, na pewno chcesz zakonczyc?";
						if (menuYes()) {
							correct = 1;
							}
						}
					if (status == 3) {
						cout << "Porzucic zmiany?";
						if (menuYes()) {
							correct = 1;
							}
						}
					}
				}
				break;

				default:{
				cout << "Nieprawidlowa wartosc. Wprowadz <1, 6>" << endl;
				}
				break;
					}
				}

			}
		return retV;
	}

string menuPath(){
		cout << "Podaj sciezke wzgledna pliku: ";
		string path;
		cin >> path; //TODO poprawnosc sciezki
		return path;

	}

string test_filename = "C:/Users/Karol/Desktop/graftest/24bb.bmp";
string test_filename2 = "C:/Users/Karol/Desktop/graftest/chgd24bb.bmp";
string test_dir = "C:/Users/Karol/Desktop/graftest/";
string progVersion = "Grafikator. Program na zaliczenie; ver. 0.9";
int main()
	{
		cout << progVersion << endl;
		grafStatus status = empty;
		menuChoice menu;
		ifstream infile;
		ofstream outfile;
		graf_file_struct *image = new graf_file_struct;

		while(menu = Exit){
			menuView();
			menu = menuRead(status);

			switch (menu) {
			case read:
			{
			string path;
			cout << "Wczytac wartosc domyslna?";
			if (menuYes()){
				path = "24bb.bmp";
				}
			else{
				path = menuPath();
				}
			infile.open(&path[0], ios::binary);
			if(infile.good()){
				graf_bmp_read(&path[0], image);
				status = onlyLoad;
				}
			else {
				cout << "Nie udalo sie wczytac" << endl;
				}
			infile.close();
			}
			break;

			case negatyw:
			{
			graf_negative(image);
			status = altered;
			}
			break;

			case lut:
			{
			LUTCube theCube;
			enum { OK = 0, ErrorOpenInFile = 100, ErrorOpenOutFile };
			// Load a Cube
			string path;
			infile.clear();
			int returnC;
			do{
				cout << "Wczytac wartosc domyslna?";
				if (menuYes()){
					path = "test1D.cube";
					}
				else{
					path = menuPath();
					}
				infile.open(path);
				if ( !infile.good() ) {
					cout << "Could not open input file " << path << endl;
					returnC = ErrorOpenInFile;
					}
				}
			while (infile.fail());
			returnC = theCube.LoadCubeFile ( infile );
			infile.close();
			if ( returnC != OK ) {
				cout << "Could not parse the cube info in the input file. Return code = "
					 << returnC << endl;
				}
			if (returnC == OK){
				if (graf_lut_it(image, theCube) == 0){
					status = altered;
					}
				}
			}
			break;

			case save:
			{
			string path;
			cout << "Wczytac wartosc domyslna?";
			if (menuYes()){
				path = "chng24bb.bmp";
				}
			else{
				path = menuPath();
				}
			outfile.open(&path[0], ios::binary);
			graf_bmp_write(&path[0], image);
			outfile.close();
			cout << "Saved " << test_filename2 << " successfully" << endl;
			status = saved;
			}
			break;

			case Exit:{
			return 0;
			}
			break;

			default:{
			cout << "menu switch error" << endl;
			}
			break;
				}
			}

/*
		ifstream infile;
		ofstream outfile;
		graf_file_struct *image = new graf_file_struct;
		infile.open(&test_filename[0], ios::binary);


		unsigned char colour = 120;
		int R0;

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

*/
		return 0;
	}
