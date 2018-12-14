#include "GA.h"

void GA::initPlayer(int _mazeArray[20][20], int x, int y, bool test)
{
  width = x;
  height = y;

  testing = test;
  //https://stackoverflow.com/questions/18709577/stdcopy-two-dimensional-array
  for (int i = 0; i < 20; i++) {
    for (int j = 0;j < 20; j++) {
      mazeArray[i][j] = _mazeArray[i][j];
    }
  }
 
  //FIND THE START AND FINISH POINT
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
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
}

void GA::genPopulation()
{
  //For loop to go through the number of members in the list
  for (int i = 0; i < N; i++)
  {
    item it; //CREATE AN INSTANCE OF ITEM

    //CREATE A STRING CALLED MEMEBER
    std::string member;

    //CREATE A MEMBER AT THE SIZE OF N
    for (int j = 0; j < cN; j++)
    {
      int r = genRandom(1);

      if (r == 0)
      {
        std::string i = "0";
        member.insert(j, i);
      }
      else
      {
        std::string i = "1";
        member.insert(j, i);
      }
    }

    //USING ITEM TO PUT VALUES INTO VECTOR
    it.str = member;
    it.fitness = 0;

    //PUT THE FILLED STRING AT THE BACK OF THE VECTOR
    //chromosomeList[i].first = it.str;
    //chromosomeList[i].second = it.fitness;
    chromosomeList.emplace_back(it.str, it.fitness);
    //chromosomeList.emplace_back(it); //CANNOT CONVERT AS ITS NOT EXPLICIT
  }

  //FOR LOOP TO GO THROUGH ALL THE OF MEMBERS IN THE VECTOR
  if (testing == true)
  {
    std::cout << "\n\n" << "//---------------------------------------------------//" << std::endl;
    std::cout << "   VECTOR LIST:" << std::endl;
    for (int j = 0; j < chromosomeList.size(); j++)
    {
      std::cout << "\tVector [" << j << "] = " << chromosomeList[j].first << " with a fitness of " << chromosomeList[j].second << std::endl;
    }
  }
}

void GA::calcFitness()
{
  for (int count = 0; count < chromosomeList.size(); count++)
  {
    std::string strChromosome;
    strChromosome = chromosomeList[count].first; //TEMP STORE CHROMOSOME AS A STRING
    if (testing == true)
    {
      std::cout << "//---------------------------------------------------//\n" << std::endl;
      std::cout << "Chromosome in use = " << strChromosome << std::endl;
    }

    //GO THROUGH EACH MOVE IN THE CURRENT CHROMOSOME
    int movesCount = 0;
    for (int i = 0; i < cN; i += 2)
    {
      //TEMP STORE THE MOVE AS A STRING
      movesCount++;
      std::string strMove;
      strMove = strChromosome.substr(i, 2); //TEMP STORE THE MOVE INFORMATION (00/01/10/11)
      if (testing == true) { std::cout << "Move #" << movesCount << " = " << strMove << std::endl; }

      //CALL THE MOVE FUNCTION (for each move within the chromosome)
      moveFunc(strMove, strChromosome);
    }

    //CALCULATE THE FITNESS OF THE CHROMOSOME (store this as a double in a 1d array)
    double fitness;

    double dX = abs(posX - finishX);
    double dY = abs(posY - finishY);
    double dXY = (dX + dY);
    fitness = (1 / (dXY + 1));

    totalFitness += fitness;//CALCULATE TOTAL FITNESS OF ALL MEMBERS ADDED TOGETHER
    chromosomeList[count].second = fitness; //STORE IN VECTOR

    //RESET THE POSITON X AND Y TO STARTING POINTS ONCE THE MOVES FOR CHROMOSOME HAS BEEN COMPLETE
    posX = startX;
    posY = startY;

  }

  //CALCULATE THE TOTAL FITNESS PERCENTAGE
  //double percentage;
  //for (int i = 0; i < chromosomeList.size(); i++)
  //{
  //  double fitness;
  //  double individualFitness;
  //  fitness = chromosomeList[i].second; //Temp store fitness from vector
  //  if (testing == true) { std::cout << "fitness of chromosome list second [" << i << "] = " << chromosomeList[i].second << std::endl; }
  //  individualFitness = fitness / totalFitness;
  //  percentage = individualFitness * 100;
  //  if (testing == true) {std::cout << "Total Fitness Percentage = " << percentage << "%" << std::endl; }
  //}
}

void GA::moveFunc(std::string move, std::string strChrome)
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
        std::string str = strChrome;
        solutionFoundFunc(str);
      }
    }
    else if (posY == 0)
    {
      if (testing == true) { std::cout << "Unable to move up as it is out of bounds" << std::endl; }
      return;
    }
  }

  //DOWN
  else if (move == "10")
  {
    //IF THE PLAYER IS NOT ON THE EDGE OF THE MAZE
    int yEdge = (height -= 1);
    if (posY < yEdge)
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
      else if (mazeChunk == 3)
      {
        std::string str = strChrome;
        solutionFoundFunc(str);
      }
    }
    else if (posY == height)
    {
      if (testing == true) { std::cout << "Unable to move down as it is out of bounds" << std::endl; }
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
      else if (mazeChunk == 3)
      {
        std::string str = strChrome;
        solutionFoundFunc(str);
      }
    }
    else if (posX == 0)
    {
      if (testing == true) { std::cout << "Unable to move left as it is out of bounds" << std::endl; }
      return;
    }
  }

  //RIGHT
  else if (move == "01")
  {
    //IF THE PLAYER IS NOT ON THE EDGE OF THE MAZE
    int xEdge = (width -= 1);
    if (posX < xEdge)
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
      else if (mazeChunk == 3)
      {
        std::string str = strChrome;
        solutionFoundFunc(str);
      }
    }
    else if (posX == width)
    {
      if (testing == true) { std::cout << "Unable to move right as it is out of bounds" << std::endl; }
      return;
    }
  }
}

void GA::selection()
{
  std::sort(chromosomeList.begin(), chromosomeList.end(), [](auto &a, auto &b) { return a.second > b.second; });

  //GET THE 2 BEST PARENTS AFTER THAT HAVE BEEN SORTED WITH HIGHEST FITNESS AT THE BEGINIING
  parent1 = chromosomeList[0].first;
  parent2 = chromosomeList[1].first;

  return;
}

void GA::crossover()
{
  child1 = "";
  child2 = "";
  ///CROSSOVER
  //Find a random point in the string
  int crossOverPoint = genRandom(parent1.length() - 1);

  for (int i = 0; i < parent1.length(); i++)
  {
    if (i < crossOverPoint)
    {
      child1.push_back(parent1.at(i)); // Making 2 children based on
      child2.push_back(parent2.at(i)); // the crossOverPoint of the 2 parents
    }
    else if (i >= crossOverPoint)
    {
      child1.push_back(parent2.at(i));
      child2.push_back(parent1.at(i));
    }
  }

  int chromeListSize = chromosomeList.size();

  chromosomeList[chromeListSize - 1].first = child1;
  chromosomeList[chromeListSize - 2].first = child2;

  //MUTATION:
  for (int i = 0; i < chromosomeList.size(); i++)
  {
    for (int j = 0; j < chromosomeList[i].first.length(); j++)
    {
      //Get the generation chance
      int random = genRandom(1000);

      //IF MUTATION IS TRUE THEN MUTATE THE NUMBER IN POS [i][j]
      if (random <= 10)
      {
        //GENERATE RANDOM NUMBER BETWEEN 1 AND 0
        int randomNum = genRandom(1);
        if (randomNum == 0)
        {
          char str = '0';
          chromosomeList[i].first.at(j) = str; 
        }
      }
    }
  }
}

void GA::generationAdd()
{
  generation += 1; //Increase the generation by 1
  cN += 2; //Increase the chromosome length by 1 move (2 bits)
  std::string temp;

  //alex knowl
  for (int i = 0; i < chromosomeList.size();i++) {
    temp = "";
    temp.push_back(genRandom(1) + '0');
    temp.push_back(genRandom(1) + '0');

    chromosomeList[i].first.append(temp);
  }

}

void GA::solutionFoundFunc(std::string str)
{
  solutionFound = true;

    double seconds_since_start = difftime(time(0), start);
    std::cout << "//---------------------------------------------------//" << std::endl;
    std::cout << "        * * * SOLUTION HAS BEEN FOUND! * * *           " << std::endl;
    std::cout << "//---------------------------------------------------//" << std::endl;
    std::cout << "\n\tChromosome : " << str << "\n" << std::endl;
    std::cout << "\n\tLenght of Chromosome = " << cN << "\n" << std::endl;
    std::cout << "\n\tGeneration : " << generation << "\n" << std::endl;
    std::cout << "\n\tMutation Chance = " << "10/1000" << "\n" << std::endl;
    std::cout << "\n\tTime Taken : " << seconds_since_start << "\n" << std::endl;
  

}
