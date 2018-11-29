#include <iostream>
#include <cmath>

#include <Windows.h>
#include <Commdlg.h>

#include <fstream>
#include <string>
#include <vector>

//FUNCTION DECLARATIONS
std::string openFile(HWND hwnd);

int main()
{

  std::cout << "Choose a file to open" << std::endl;

	//LOAD THE FILE PASSED IN AS A STRING FROM THE USER
	std::string mazeFileLoc = openFile(0);
	
  std::cout << "Chosen File Location : " << mazeFileLoc.c_str() << "\n";
  
	//OPEN THE FILE IN BINARY
  std::fstream mazeFile(mazeFileLoc, std::ios::in | std::ios::binary);

  if (mazeFile.is_open())
  {
	  //STORE THE FIRST CHARACTER AS AN CHAR VALUE
    char test = mazeFile.get();
    std::cout << "First Character is : " << test << std::endl;
    
    //PUT THE CHAR VALUE IN A STRING
    std::string strTest;
    strTest.insert(strTest.begin(), test);
    std::cout << "String First Character is : " << strTest << std::endl;

    //USE STOI TO GET THE INT VALUE OF STRING
    int testIntVal = std::stoi(strTest);
    std::cout << "Int Value of first character is : " << testIntVal << std::endl;

    mazeFile.close();
  }

	//IF STATEMENTS THAT CHECK WHICH FILE ENCODING TYPE HAS BEEN USED FOR THE TEXT FILE 
	//CHECK FOR UTF-16
	//ELSE
	//OPEN AS UTF-8 & ANSI

	/*
	//OPEN AND READ THE MAZE FILE
	if (mazeFile.is_open())
	{
	  while (getline(mazeFile, str_Maze))
	  {
		std::cout << str_Maze << '\n' << std::endl;

		//FINE THE NUMBER OF COLUMNS AND ROWS
		std::string str_Columns = str_Maze.substr(0, 1);
		const int columns = std::stoi(str_Columns, nullptr, 0);

		std::string str_Rows = str_Maze.substr(1, 2);
		const int rows = std::stoi(str_Rows, nullptr, 0);

		//IMPUT THE VALUES INTO A 2D ARRAY
		std::cout << "Number of Columns : " << columns << std::endl;
		std::cout << "Number of Rows : " << rows << std::endl;

		std::vector < std::vector <int> > mazeVector;
		mazeVector.resize(rows, std::vector<int>(columns, 0));



		//int myArray[columns][rows];

	  }
	  mazeFile.close();
	}
	else
	{
	  std::cout << "Unable to open file";
	}
	*/

	system("Pause");
	return 0;
}

//CHOOSE A FILE TO OPEN AND RETURN LOCATION AS A STRING
std::string openFile(HWND hwnd)
{
  /* https://stackoverflow.com/questions/38774248/how-to-save-ofn-lpstrfile-to-string-properly */
	std::string mazeFileLocation(MAX_PATH, '\0');

  OPENFILENAME ofn = { sizeof(OPENFILENAME) };
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = "Text\0*.TXT\0"; //FILTER WHAT FILES ARE WANTED
  ofn.lpstrFile = &mazeFileLocation[0];
  ofn.nMaxFile = MAX_PATH;

  /*
  OPENFILENAME ofn;

  // another memory buffer to contain the file name
  char szFile[100];

	// OPEN A FILE NAME : /* https://www.daniweb.com/programming/software-development/code/217307/a-simple-getopenfilename-example
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Text\0*.TXT\0"; //FILTER WHAT FILES ARE WANTED
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	*/

  ofn.Flags = OFN_FILEMUSTEXIST; //CHECK IF THE FILE EXISTS

	if (GetOpenFileName(&ofn))
	{
		/* https://stackoverflow.com/questions/38774248/how-to-save-ofn-lpstrfile-to-string-properly */
		mazeFileLocation.resize(strlen(mazeFileLocation.c_str())); // SET THE STRING AS THE OFN
	}

	return mazeFileLocation;
}