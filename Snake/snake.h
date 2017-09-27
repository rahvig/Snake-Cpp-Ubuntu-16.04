#ifndef SNAKE_H
#define SNAKE_H

#include <iostream> //needed for srand(time(0)) function
#include <ncurses.h> //needed to interact with terminal
#include <cstdlib> //needed for rand() function
#include <unistd.h> //needed for usleep() function
#include <vector> //needed to construct actual snake
#include <fstream>

class snakeGame{
  private:
    int score, snakeSpeed, maxScreenWidth, maxScreenHeight;
    //score will represent the users game score
    //snake speed represents how fast our snake will go
    //maxScreenWidth will take on the value of the the max width of our terminal
    //maxScreenHeight will take on the value of the max height of our terminal
    char snakeBody, screenEdge, food, direction;
    //snakeBody is the char used to construct the body of the snake
    //screenEdge is the char used to construct the boundaries of our terminal window
    //food is the char used to represent the food the snake eats to get points
    //direction is used to represent the current direction the snake is moving in

    bool obtainFood;
    //obtainFood is set to true when snake has eaten the food object

    WINDOW* menu;

    struct position{
      int x, y;
      position(int col, int row); //modified constructor sets the position of a game object
      position(); //defualt constructor for our snake structure
    }; //we define a structure to keep track of the positon of game objects.

    position foodLocation; //location of our food object
    std::vector<position> snake; //vector of positions representing the location of our snake.

    void spawnFood(); //private method to spawn food randomly
    void moveSnake(); //private method detailing snake movement
    bool collision();
    void initializeMenuScreen(); //private method to initialize the menu screen
    void initializeGameScreen(); //private method to initialize game screen
    void highScoreList();

  public:
    snakeGame(); //when snakeGame object created, all our private data will be initialized
    ~snakeGame(); //Desructor to deallocate any memory the game takes up after it has ended
    void startGame(); //starts the whole game

};
#endif
