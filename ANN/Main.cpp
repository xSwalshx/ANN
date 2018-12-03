#include <iostream>
#include <cmath>

#include <Windows.h>
#include <Commdlg.h>

#include <fstream>
#include <string>
#include <vector>

#include <streambuf>
#include <algorithm>
#include <iterator>
#include <cerrno>

//FUNCTION DECLARATIONS
std::string openFile(HWND hwnd);

//PUBLIC VAR TO ENABLE TESTING FEATURES
bool testing = false;

int main()
{
  //ARE YOU WANTING TO TEST THE CODE
  std::cout << "//---------------------------------------------------//" << std::endl;
  std::cout << "      ARE YOU WANTING TO SHOW TESTING STATISTICS?      " << std::endl;
  std::cout << "//---------------------------------------------------//" << std::endl;
  std::cout << "            ENTER 'Y' for yes OR 'N' for no            " << std::endl;
  char decision;
  std::cout << "                     DECISION : ";
  std::cin >> decision;
  switch (decision)
  {
  case 'Y':
    std::cout << "       YOUR ARE NOW VIEWING TESTING STATISTICS         " << std::endl;
    std::cout << "//---------------------------------------------------//\n" << std::endl;
    testing = true;
    break;
  case 'y':
    std::cout << "       YOUR ARE NOW VIEWING TESTING STATISTICS         " << std::endl;
    std::cout << "//---------------------------------------------------//\n" << std::endl;
    testing = true;
    break;
  case 'N':
    std::cout << "         YOUR WILL NOT SEE TESTING STATISTICS          " << std::endl;
    std::cout << "//---------------------------------------------------//\n" << std::endl;
    testing = false;
    break;
  case 'n':
    std::cout << "         YOUR WILL NOT SEE TESTING STATISTICS          " << std::endl;
    std::cout << "//---------------------------------------------------//\n" << std::endl;
    testing = false;
    break;
  default:
    std::cout << "INVALID SELECTION : PLEASE TRY AGAIN!" << std::endl;
    break;
  }

  //CHOOSE A MAZE FILE TO OPEN
  std::cout << "Choose a file to open" << std::endl;
  std::string mazeFileLoc = openFile(0); //LOAD THE FILE PASSED IN AS A STRING FROM THE USER
  std::cout << "Chosen File Location : " << mazeFileLoc.c_str() << "\n"; //PRINT THE CHOSEN FILE LOCATION TO CONSOLE
  
  //------------------------------------------------------------------------------------------------------------------------//
  //OPEN THE FILE IN BINARY
  std::ifstream mazeFile(mazeFileLoc, std::ios::in | std::ios::binary);
  
  //READ THE FILE AND STORE IT IN A STRING
  /* http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html */
  std::string contents;
  if (mazeFile)
  {
    mazeFile.seekg(0, std::ios::end);
    contents.reserve(mazeFile.tellg());
    mazeFile.seekg(0, std::ios::beg);
    std::copy((std::istreambuf_iterator<char>(mazeFile)), std::istreambuf_iterator<char>(), std::back_inserter(contents));
    mazeFile.close();
  }
  else 
  { 
    std::cout << "Unable to open file at memeory location : " << mazeFileLoc << std::endl;
  }

  if (testing == true) { std::cout << "Contents of the file : " << contents << "\n" << std::endl; }
  //------------------------------------------------------------------------------------------------------------------------//
  //OPEN THE FILE AND STORE THE CONTENT IN A SRING AND GET X AND Y VALUES
  std::string line;
  std::string strX;
  std::string strY;
  const int maxX = 101; //THE MAX SIZE THE MAZE CAN GO TO IS 100 x 100
  const int maxY = 101;
  int x; //INPUT VALUE OF X FROM MAZE
  int y; //INPUT VALUE OF Y FROM MAZE

  std::ifstream maze(mazeFileLoc);

  if (maze.is_open())
  {
    while (std::getline(maze, line))
    {
      //GET THE Y VALUE
      strY = line.substr(0, 1);
      if (testing == true) { std::cout << "String Value of Y : " << strY << std::endl; }
      y = std::stoi(strY, 0);
      if (testing == true) { std::cout << "Int value of Y : " << y << std::endl; }
      
      //GET THE X VALUE
      strX = line.substr(1, 2);
      if (testing == true) { std::cout << "String Value of X : " << strX << std::endl; }
      x = std::stoi(strX, 0);
      if (testing == true) { std::cout << "Int value of X : " << x << std::endl; }
    }
    maze.close();
  }
  else 
  {
    std::cout << "Unable to open file at memeory location : " << mazeFileLoc << std::endl;
  }
  //------------------------------------------------------------------------------------------------------------------------//
  //CREATE A 2D ARRAY BASED OFF THE FILE MAZE
  int mazeArray[maxX][maxY];

  int count = 3; //START AT THE FIRST PART OF THE ACTUAL MAZE

  //NESTED FOR LOOP TO GO THROUGHT THE MAZE AND PUT IT IN THE 2D ARRAY
  for (int i = 0; i < y; i++) //FOR LOOP TO GO THROUGH Y AXIS
  {
    std::cout << "//--------------------------------------//" << std::endl;
    for (int j = 0; j < x; j++) //FOR LOOP TO GO THROUGH THE X AXIS
    {
      std::string pos = line.substr(count,1);
      std::remove_if(pos.begin(), pos.end(), isspace);

      //IF THERE IS A SPACE THEN SKIP IT AND ADD TO COUNT
      if (pos == " ") 
      {
        count++;
        j--; //SO WE DON'T MISS A MEMBER IN THE ARRAY GO BACK BY 1 ON j
        continue;
      }

      int value = std::stoi(pos);
      mazeArray[i][j] = value;

      if (testing == true) 
      { 
      std::cout << "THE VALUE OF COUNT : " << count << std::endl; 
      std::cout << "THE VALUE OF POS : " << pos << std::endl;
      std::cout << "THE VALUE OF VALUE : " << value << std::endl; 
      std::cout << "THE VALUE OF MAZE ARRAY[" << i << "][" << j << "] = " << value << "\n" << std::endl;
      }

      count++;
    }
  }

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

  ofn.Flags = OFN_FILEMUSTEXIST; //CHECK IF THE FILE EXISTS

	if (GetOpenFileName(&ofn))
	{
		/* https://stackoverflow.com/questions/38774248/how-to-save-ofn-lpstrfile-to-string-properly */
		mazeFileLocation.resize(strlen(mazeFileLocation.c_str())); // SET THE STRING AS THE OFN
	}

	return mazeFileLocation;
}