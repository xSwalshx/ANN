#include <iostream>
#include <cmath>

#include <Windows.h>
#include <Commdlg.h>

#include <fstream>
#include <algorithm>

#include "GA.h"

///GLOBAL VARIABLES
const int maxX = 20;        //The max the array can go in the X axis
const int maxY = 20;        //The max the array can go in the Y axis
int mazeArray[maxY][maxX];  //mazeArray[y][x] or mazeArray[rows][columns]

//PUBLIC VAR TO ENABLE TESTING FEATURES
bool testing = false;

//FUNCTION DECLARATIONS
std::string openFile(HWND hwnd);

//MAIN FUNCTION
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
    std::cout << "  INVALID SELECTION : NOT SHOWING TESTING STATSISTICS! \n" << std::endl;
    std::cout << "//---------------------------------------------------//" << std::endl;
    testing = false;
    break;
  }

  //CHOOSE A MAZE FILE TO OPEN
  std::cout << "*******************************************************" << std::endl;
  std::cout << "* NOTE: THIS PROGRAM CAN ONLY OPEN ASCI ENCODED FILES *" << std::endl;
  std::cout << "*******************************************************" << std::endl;
  std::cout << "Choose a file to open" << std::endl;
  std::string mazeFileLoc = openFile(0); //LOAD THE FILE PASSED IN AS A STRING FROM THE USER
  std::cout << "Chosen File Location : " << mazeFileLoc.c_str() << "\n" << std::endl; //PRINT THE CHOSEN FILE LOCATION TO CONSOLE
  std::cout << "//---------------------------------------------------//\n" << std::endl;

  //------------------------------------------------------------------------------------------------------------------------//
  
  //OPEN THE FILE AND STORE THE CONTENT IN A SRING AND GET X AND Y VALUES
  std::string line;
  std::string strX;
  std::string strY;
  int width; //INPUT VALUE OF X FROM MAZE
  int height; //INPUT VALUE OF Y FROM MAZE

  std::ifstream maze(mazeFileLoc);

  if (maze.is_open())
  {
    while (std::getline(maze, line))
    {
      //PRINT THE CONTENT OF THE MAZE FILE
      if (testing == true) { std::cout << "Contents of the file : " << line << std::endl; }

      //GET THE X VALUE
      strX = line.substr(0, line.find(' '));
      width = std::stoi(strX, 0);

      //PRINT TESTING STATISTICS
      if (testing == true) 
      { 
        std::cout << "String Value of X : " << strX << std::endl;
        std::cout << "Int value of X : " << width << std::endl; 
      }

      //GET THE Y VALUE
      int count = strX.length();
      strY = line.substr(count+=1, line.find(' '));
      height = std::stoi(strY, 0);

      //PRINT TESTING STATISTICS
      if (testing == true) 
      { 
        std::cout << "String Value of Y : " << strY << std::endl;
        std::cout << "Int value of Y : " << height << "\n" << std::endl; 
      }
      break;
    }
    maze.close();
  }
  else
  {
    std::cout << "Unable to open file at memeory location : " << mazeFileLoc << std::endl;
  }

  //------------------------------------------------------------------------------------------------------------------------//
  
  //CREATE A 2D ARRAY BASED OFF THE FILE MAZE
  int count = 3; //START AT THE FIRST PART OF THE ACTUAL MAZE

  if (testing == true) { std::cout << "//---------------------------------------------------//" << std::endl; }

  //NESTED FOR LOOP TO GO THROUGHT THE MAZE AND PUT IT IN THE 2D ARRAY
  for (int i = 0; i < height; i++) //FOR LOOP TO GO THROUGH Y AXIS
  {
    for (int j = 0; j < width; j++) //FOR LOOP TO GO THROUGH THE X AXIS
    {
      std::string pos = line.substr(count, 1);

      //IF THERE IS A SPACE THEN SKIP IT AND ADD TO COUNT
      /* https://en.cppreference.com/w/cpp/algorithm/remove */
      std::remove_if(pos.begin(), pos.end(), isspace);
      if (pos == " ")
      {
        count++; //INCREMENT COUNT SO WE DON'T STAY ON SPACE AND MOVE TO NEXT PART OF STRING
        j--; //SO WE DON'T MISS A MEMBER IN THE ARRAY GO BACK BY 1 ON j
        continue;
      }

      int value = std::stoi(pos);
      mazeArray[i][j] = value;

      count++;
    }
  }

  //------------------------------------------------------------------------------------------------------------------------//
  ///USE GA TO FIND THE ROUTE THROUGH THE MAZE
  //START TIMER
  /* https://stackoverflow.com/questions/1735307/c-keeping-track-of-how-many-seconds-has-passed-since-start-of-program */

  //DRAW THE MAZE ON THE SCREEN
  for (int i = 0; i < height; i++) //ROWS
  {
    std::cout << "\n";

    for (int j = 0; j < width; j++) //COLUMNS
    {
      if (testing == true)
      {
        std::cout << "MAZE ARRAY[" << i << "][" << j << "] = " << mazeArray[i][j] << "\t";
      }
      else
      {
        std::cout << "\t" << mazeArray[i][j];
      }
    }
  }

  ///CLASS GA
  GA p; //Create the player
  p.start = time(0); //Start the timer

  //CALL FUNCTIONS
  p.initPlayer(mazeArray, width, height, testing);
  p.genPopulation();

  while (!p.solutionFound)
  {
    p.calcFitness();
    p.selection();
    //check for solution
    p.crossover();
    p.generationAdd();
  }
  
  //------------------------------------------------------------------------------------------------------------------------//

  std::cout << "\n" << std::endl;
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