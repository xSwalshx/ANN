#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <time.h>

class GA
{
public:
  ///FUNCTIONS:
  void initPlayer(int _mazeArray[20][20], int x, int y, bool test);

  void genPopulation();
  void calcFitness();
  void selection();
  void crossover();
  void generationAdd();

  ///VARIABLES:
  time_t start;
  bool solutionFound = false;

private:
  ///ITEM
  struct item {
    std::string str;
    double fitness;
  };

  ///VECTORS:
  /* http://www.cplusplus.com/reference/utility/pair/ */
  std::vector <std::pair < std::string, double>> chromosomeList;

  ///VARIABLES:
  int generation = 1;
  int mazeArray[20][20];
  int width, height;
  int posX;     //Current position (X)
  int posY;     //Current position (Y)
  int N = 4;    //Number of individuals/members in the population
  int cN = 16;  //Current Lenght of each individual/members

  int startX;   //Start position of the player (X)
  int startY;   //Start position of the player (Y)
  int finishX;  //Goal for the player (X)
  int finishY;  //Goal for the player (Y)

  bool testing;
  bool spFound = false; //Start Point Found
  bool fpFound = false; //Finish Point Found

  double totalFitness = 0; //Total fitness of all chromosomes added together

  std::string parent1;  //Best parent #1
  std::string parent2;  //Best parent #2
  std::string child1;   //Child #1
  std::string child2;   //Child #2

  ///FUNCTIONS:
  void moveFunc(std::string move, std::string strChrome);
  void solutionFoundFunc(std::string str);

  int genRandom(int limit)
  {
    /* https://stackoverflow.com/questions/28653255/stddefault-random-engine-generates-the-same-values-even-with-changing-seed */
    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, limit);
    
    //generate r RAND between 0 and limit
    int r = distribution(generator);

    return r;
  }
};