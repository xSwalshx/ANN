#include <iostream>
#include <cmath>

#include <Windows.h>
#include <Commdlg.h>
#include <time.h>

#include <fstream>
#include <string>
#include <vector>
#include <random>

#include "GA.h"

//GLOBAL VARIABLES
const int maxX = 101; //THE MAX SIZE THE MAZE CAN GO TO IS 100 x 100
const int maxY = 101;
int mazeArray[maxY][maxX]; // mazeArray[y][x]
int posX; //POSITION OF THE PLAYER
int posY;
time_t start;

//PUBLIC VAR TO ENABLE TESTING FEATURES
bool testing = false;

//FUNCTION DECLARATIONS
std::string openFile(HWND hwnd);
void moveFunction(int mazeArray[maxY][maxX], int &posX, int &posY, int x, int y, std::string move);
void solutionFound(std::vector <std::string> chromosomeList, int cN, int count, int generation, double mutationChance);

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
  std::cout << "Choose a file to open" << std::endl;
  std::string mazeFileLoc = openFile(0); //LOAD THE FILE PASSED IN AS A STRING FROM THE USER
  std::cout << "Chosen File Location : " << mazeFileLoc.c_str() << "\n" << std::endl; //PRINT THE CHOSEN FILE LOCATION TO CONSOLE
  std::cout << "//---------------------------------------------------//\n" << std::endl;
  
  //------------------------------------------------------------------------------------------------------------------------//
  //OPEN THE FILE AND STORE THE CONTENT IN A SRING AND GET X AND Y VALUES
  std::string line;
  std::string strX;
  std::string strY;
  int x; //INPUT VALUE OF X FROM MAZE
  int y; //INPUT VALUE OF Y FROM MAZE

  std::ifstream maze(mazeFileLoc);

  if (maze.is_open())
  {
    while (std::getline(maze, line))
    {
      //PRINT THE CONTENT OF THE MAZE FILE
      if (testing == true) { std::cout << "Contents of the file : " << line << std::endl; }

      //GET THE X VALUE
      strX = line.substr(0, 1);
      if (testing == true) { std::cout << "String Value of X : " << strX << std::endl; }
      x = std::stoi(strX, 0);
      if (testing == true) { std::cout << "Int value of X : " << x << std::endl; }
      
      //GET THE Y VALUE
      strY = line.substr(1, 2);
      if (testing == true) { std::cout << "String Value of Y : " << strY << std::endl; }
      y = std::stoi(strY, 0);
      if (testing == true) { std::cout << "Int value of Y : " << y << "\n" << std::endl; }
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
  for (int i = 0; i < y; i++) //FOR LOOP TO GO THROUGH Y AXIS
  {
    for (int j = 0; j < x; j++) //FOR LOOP TO GO THROUGH THE X AXIS
    {
      std::string pos = line.substr(count,1);

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
  start = time(0);

  //DRAW THE MAZE ON THE SCREEN
  for (int i = 0; i < y; i++) //ROWS
  {
    std::cout << "\n";

    for (int j = 0; j < x; j++) //COLUMNS
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

  //CREATE A VECTOR OF STRINGS
  int N = 4; //NUMBER OF CHROMOSOMES/GENOMES IN THE POPULATION (INDIVIDUAL)
  int cN = 16; //CURRENT LENGTH OF EACH INDIVIDUAL (START AT 16 BITS AND GROW TO 400 BITS)

  //CREATE CHROMOSOMES BASED OF RANDOM
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  //CREATE A VECTOR OF CHROMOSOMES TO THE SIZE OF N
  std::vector <std::string> chromosomeList;
  for (int i = 0; i < N; i++)
  {
    //CREATE A STRING CALLED MEMEBER
    std::string member;

    //CREATE A MEMBER AT THE SIZE OF N
    for (int j = 0; j < cN; j++)
    {
      //generate r RAND between 0 and 1
      double r = ((double)rand() / (RAND_MAX));

      if (r <= 0.5)
      {
        //chromosome position j = 0
        //if (testing == true) { std::cout << "RANDOM VALUE = " << r << "\t" << "Vector [" << i << "] : String Position [" << j << "] = " << 0 << std::endl; }
        std::string i = "0";
        member.insert(j, i);
      }
      else
      {
        //chromosome position j = 1
        //if (testing == true) { std::cout << "RANDOM VALUE = " << r << "\t" << "Vector [" << i << "] : String Position [" << j << "] = " << 1 << std::endl; }
        std::string i = "1";
        member.insert(j, i);
      }
    }
    //PUT THE FILLED STRING AT THE BACK OF THE VECTOR
    chromosomeList.emplace_back(member);  
  }

  //FOR LOOP TO GO THROUGH ALL THE OF MEMBERS IN THE VECTOR
  if (testing == true)
  {
    std::cout << "\n\n" << "//---------------------------------------------------//" << std::endl;
    std::cout << "   VECTOR LIST:" << std::endl;
    for (int i = 0; i < chromosomeList.size(); i++)
    {
      std::cout << "\tVector [" << i << "] = " << chromosomeList[i] << std::endl;
    }
  }

  //------------------------------------------------------------------------------------------------------------------------//

  //CREATE THE PLAYER
  int startX;
  int startY;
  int finishX;
  int finishY;
  bool spFound = false;
  bool fpFound = false;

  //FIND THE START AND FINISH POINT
  for (int i = 0; i < y; i++)
  {
    for (int j = 0; j < x; j++)
    {
      //FIND THE START X AND Y
      if (mazeArray[i][j] == 2)
      {
        if (testing == true) { std::cout << "\nStarting point found at X:" << j << " Y:" << i << std::endl; }
        startX = j;
        startY = i;
        spFound = true;
      }
      
      //FIND THE FINISH X AND Y
      if (mazeArray[i][j] == 3)
      {
        if (testing == true) { std::cout << "Finish point found at X:" << j << " Y:" << i << std::endl; }
        finishX = j;
        finishY = i;
        fpFound = true;
      }

      //IF START AND FINISH POINTS HAVE BEEN FOUND THEN BREAK FOR LOOP
      if (spFound && fpFound == true) { break; }

    }

    //IF START AND FINISH POINTS HAVE BEEN FOUND THEN BREAK FOR LOOP
    if (spFound && fpFound == true) { break; }

  }

  //SET THE PLAYERS POS X AND Y TO START POINT
  posX = startX;
  posY = startY;

  //GO THROUGH EACH CHROMOSOME IN THE CHROMOSOME LIST
  std::cout << "\n";
  for (int count = 0; count < chromosomeList.size(); count++)
  {
    std::string strChromosome;
    strChromosome = chromosomeList[count]; //TEMP STORE CHROMOSOME AS A STRING
    if (testing == true) { std::cout << "Chromosome in use = " << strChromosome << std::endl; }

    int movesCount = 0;

    for (int i = 0; i < cN; i+=2)
    {
      //TEMP STORE THE MOVE AS A STRING
      movesCount++;
      std::string strMove;
      strMove = strChromosome.substr(i, 2); //TEMP STORE THE MOVE INFORMATION (00/01/10/11)
      if (testing == true) { std::cout << "Move #" << movesCount << " = " << strMove << std::endl; }

      //CALL THE MOVE FUNCTION (for each move within the chromosome)
      moveFunction(mazeArray, posX, posY, x, y, strMove);

      //CALCULATE THE FITNESS OF THE CHROMOSOME (store this as a double in a 1d array)


    }

    //RESET THE POSITON X AND Y TO STARTING POINTS ONCE THE MOVES FOR CHROMOSOME HAS BEEN COMPLETE
    posX = startX;
    posY = startY;

    if (testing == true) { std::cout << "//---------------------------------------------------//" << std::endl; }

  }

  std::cout << "\n" << std::endl;
  system("Pause");
	return 0;

}

//MOVE FUNCTION
void moveFunction(int mazeArray[maxY][maxX], int &posX, int &posY, int x, int y, std::string move)
{
  //UP
  if (move == "00")
  {
    //IF THE PLAYER IS NOT ON THE EDGE OF THE MAZE
    if (posY > 0)
    {
      //MOVE THE POSITION OF THE PLAYER
      posY -= 1;

      //FIND THE MAZE CHUNK
      int mazeChunk;
      mazeChunk = mazeArray[posY][posX];

      //CHECK CHUNK TO SEE IF ITS A PATH OR WALL (0 | 1)
      if (mazeChunk == 0 || mazeChunk == 2) //IF IT IS A PATH
      {
        if (testing == true) { std::cout << "\tPLAYER MOVED UP " << std::endl; }
        return;
      }
      else if (mazeChunk == 1)
      {
        //CHANGE POS BACK TO NORMAL
        posY += 1;
        if (testing == true) { std::cout << "\tUNABLE TO COMPLETE MOVE UP AS THERE IS A WALL" << std::endl; }
        return;
      }
      else if (mazeChunk == 3)
      {
        //solutionFound(chromosomeList, cN, count, generation, mutationChance); //implement into moveFunc once the generations and that have been done.
      }
    }
    else if (posY == 0)
    {
      std::cout << "Unable to move up as it is out of bounds" << std::endl;
      return;
    }
  }
  
  //DOWN
  else if (move == "10")
  {
    //IF THE PLAYER IS NOT ON THE EDGE OF THE MAZE
    if (posY < y)
    {
      //MOVE THE POSITION OF THE PLAYER
      posY += 1;

      //FIND THE MAZE CHUNK
      int mazeChunk;
      mazeChunk = mazeArray[posY][posX];

      //CHECK CHUNK TO SEE IF ITS A PATH OR WALL (0 | 1)
      if (mazeChunk == 0 || mazeChunk == 2) //IF IT IS A PATH
      {
        if (testing == true) { std::cout << "\tPLAYER MOVED DOWN " << std::endl; }
        return;
      }
      else if (mazeChunk == 1)
      {
        //CHANGE POS BACK TO NORMAL
        posY -= 1;
        if (testing == true) { std::cout << "\tUNABLE TO COMPLETE MOVE DOWN AS THERE IS A WALL" << std::endl; }
        return;
      }
    }
    else if (posY == y)
    {
      std::cout << "Unable to move down as it is out of bounds" << std::endl;
      return;
    }
  }

  //LEFT
  else if (move == "11")
  {
    //IF THE PLAYER IS NOT ON THE EDGE OF THE MAZE
    if (posX > 0)
    {
      //MOVE THE POSITION OF THE PLAYER
      posX -= 1;

      //FIND THE MAZE CHUNK
      int mazeChunk;
      mazeChunk = mazeArray[posY][posX];

      //CHECK CHUNK TO SEE IF ITS A PATH OR WALL (0 | 1)
      if (mazeChunk == 0 || mazeChunk == 2) //IF IT IS A PATH
      {
        if (testing == true) { std::cout << "\tPLAYER MOVED LEFT " << std::endl; }
        return;
      }
      else if (mazeChunk == 1) //IF IT HITS A WALL
      {
        //CHANGE POS BACK TO NORMAL
        posX += 1;
        if (testing == true) { std::cout << "\tUNABLE TO COMPLETE MOVE LEFT AS THERE IS A WALL" << std::endl; }
        return;
      }
    }
    else if (posX == 0)
    {
      std::cout << "Unable to move left as it is out of bounds" << std::endl;
      return;
    }
  }

  //RIGHT
  else if (move == "01")
  {

    //IF THE PLAYER IS NOT ON THE EDGE OF THE MAZE
    if (posX < x)
    {
      //MOVE THE POSITION OF THE PLAYER
      posX += 1;

      //FIND THE MAZE CHUNK
      int mazeChunk;
      mazeChunk = mazeArray[posY][posX];

      //CHECK CHUNK TO SEE IF ITS A PATH OR WALL (0 | 1)
      if (mazeChunk == 0 || mazeChunk == 2) //IF IT IS A PATH
      {
        if (testing == true) { std::cout << "\tPLAYER MOVED RIGHT " << std::endl; }
        return;
      }
      else if (mazeChunk == 1) //IF IT HITS A WALL
      {
        //CHANGE POS BACK TO NORMAL
        posX -= 1;
        if (testing == true) { std::cout << "\tUNABLE TO COMPLETE MOVE RIGHT AS THERE IS A WALL" << std::endl; }
        return;
      }
    }
    else if (posX == x)
    {
      std::cout << "Unable to move right as it is out of bounds" << std::endl;
      return;
    }
  }
  
  return;
}

//SOLUTION FOUND FUNCTION
void solutionFound(std::vector <std::string> chromosomeList, int cN, int count, int generation, double mutationChance)
{
  double seconds_since_start = difftime(time(0), start);
  std::cout << "//---------------------------------------------------//" << std::endl;
  std::cout << "        * * * SOLUTION HAS BEEN FOUND! * * *           " << std::endl;
  std::cout << "//---------------------------------------------------//" << std::endl;
  std::cout << "\n\tChromosome : " << &chromosomeList[count] << "\n" << std::endl;
  std::cout << "\n\tLenght of Chromosome = " << cN << "\n" << std::endl;
  std::cout << "\n\tGeneration : " << generation << "\n" << std::endl;
  std::cout << "\n\tMutation Chance = " << mutationChance << "\n" << std::endl;
  std::cout << "\n\tTime Taken : " << seconds_since_start << "\n" << std::endl;

  std::cout << "\n" << std::endl;
  system("Pause");
  return;
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