#include "snake.h"

snakeGame::position::position(){
  x = 0;
  y = 0;
} //default position constructor

snakeGame::position::position(int col, int row){
  x = col;
  y = row;
}


snakeGame::snakeGame(){
  initializeMenuScreen();
  snakeBody = 'o'; //symbol to represent our snake
  screenEdge = char(219); //a rectangle according to ascii table
  food =  '*'; //symbol to represent food
  for(int i = 0; i < 2; ++i)
    snake.push_back(position(40+i, 10)); //use a for loop to initialize our snake

  score = 0; // initialize our score to 0.
  snakeSpeed = 100000; //snakeSpeed for our sleep.
  obtainFood = false;
  direction = 'r'; //default loc. snake will start moving.
  srand(time((0))); //random number generator.
  spawnFood(); //puts the food somewhere randomly in the window
  for(int i = 0; i < maxScreenWidth - 1; ++i){
    move(0, i);
    addch(screenEdge);
  } //this forloop creates the horizontal edge on the top of our window.

  for(int i = 0; i < maxScreenWidth - 1; ++i){
    move(maxScreenHeight-2, i);
    addch(screenEdge);
  } //this forloop creates the horizontal edge on the bottom of our window.

  for(int i = 0; i < maxScreenHeight - 1; ++i){
    move(i, 0);
    addch(screenEdge);
  } //this forloop creates the vertical edge on the left of our window.

  for(int i = 0; i < maxScreenHeight - 1; ++i){
    move(i, maxScreenWidth-1);
    addch(screenEdge);
  } //this forloop creates the vertical edge on the right of our window.

  for(int i = 0; i < snake.size(); ++i){
    move(snake[i].y, snake[i].x);
    addch(snakeBody);
  } //create our actual snake

  move(maxScreenHeight-1, 0);
  printw("Score: %d", score);
  move(foodLocation.y, foodLocation.x);
  addch(food);
  refresh();
}

snakeGame::~snakeGame(){
  nodelay(stdscr, false);
  getch();
  endwin();
}

void snakeGame::startGame(){
  while(1){
    if(collision() == true ){
      refresh();
      move(maxScreenHeight/2, maxScreenWidth/2 - 4);
      printw("GAME OVER");
      std::ofstream fout("scores.txt", std::ios::app);
      fout << score << std::endl;
      break;
    }
    moveSnake();
    if(direction == 'q')
      break;
    usleep(snakeSpeed);
 }
}

void snakeGame::spawnFood(){
  while(1){
    int tempX = rand() % maxScreenWidth+1;
    int tempY = rand() % maxScreenHeight+1;
    for(int i = 0; i < snake.size(); ++i){
    if(snake[i].x == tempX && snake[i].y == tempY){
        continue;
      } //if the food generates in the location of the snake create new food location
    }
    if (tempX >= maxScreenWidth -2 || tempY >= maxScreenHeight - 3)
      continue; //make sure our food doesn't appear in a location off the map.

    foodLocation.x = tempX;
    foodLocation.y = tempY;
    break;
  }
    move(foodLocation.y, foodLocation.x);
    addch(food);
    refresh();
}

void snakeGame::moveSnake(){
  int keyPress = getch();
  switch(keyPress){
    case KEY_LEFT:
      if(direction != 'r'){
        direction = 'l';
      }
      break;
    case KEY_UP:
      if(direction != 'd'){
        direction = 'u';
      }
      break;
    case KEY_RIGHT:
      if(direction != 'l'){
        direction = 'r';
      }
      break;
    case KEY_DOWN:
      if(direction !=  'u'){
        direction = 'd';
      }
      break;
    case KEY_BACKSPACE:
        direction = 'q';
      break;
    } //switch statement for button presses.


    if(!obtainFood){
      move(snake[snake.size()-1].y, snake[snake.size()-1].x);
      addch(' ');
      refresh();
      snake.pop_back();
    } //while snake is moving we need to continuously edit the snake parts

    if(direction == 'l'){
      snake.insert(snake.begin(), position(snake[0].x - 1, snake[0].y));
    } else if(direction ==  'r') {
      snake.insert(snake.begin(), position(snake[0].x + 1, snake[0].y));
    } else if(direction ==  'u') {
      snake.insert(snake.begin(), position(snake[0].x, snake[0].y - 1));
    } else if(direction ==  'd') {
      snake.insert(snake.begin(), position(snake[0].x, snake[0].y + 1));
    }

    move(snake[0].y, snake[0].x);
    addch(snakeBody);
    refresh();
}

void snakeGame::initializeMenuScreen(){
	initscr();			/* Start curses mode 		*/
  move(5, 30);
  noecho();
  curs_set(0);
  printw("WELCOME TO SNAKE");
  getmaxyx(stdscr, maxScreenHeight, maxScreenWidth); //get the dimensions of the terminal window
	menu = newwin(10, maxScreenWidth - 12, maxScreenHeight - 15, 5);
  box(menu, 0, 0);
  refresh();
  wrefresh(menu);
  keypad(menu, true);

  std::string choices[2] = {"Play Game", "High Scores"};
  int choice;
  int highlight = 0;
  bool m = true;

  while(m){
    for(int i = 0; i < 2; ++i){
        if(i == highlight){
          wattron(menu, A_REVERSE);
        }
        mvwprintw(menu, i+1, 28, choices[i].c_str());
        wattroff(menu, A_REVERSE);
    }
    choice = wgetch(menu);

    switch(choice){
      case KEY_UP:
        highlight--;
        if(highlight == -1){
          highlight = 1;
        }
        break;
      case KEY_DOWN:
        highlight++;
        if(highlight == 2){
          highlight = 0;
        }
        break;
    }

    if(choice == 10 && highlight == 0){
      initializeGameScreen();
      m = false;
    } else if(choice == 10 && highlight == 1) {
      highScoreList();
      refresh();
    } else {
      continue;
    }
  }

}

void snakeGame::initializeGameScreen(){
  erase();
  refresh();
  nodelay(stdscr, true); //make sure the game continues without waiting for a button press
  keypad(stdscr, true); //allows you to use arrow keys
  noecho(); //make sure no key press is outputted to the console
  curs_set(0); //make sure the cursor is invisible;
  getmaxyx(stdscr, maxScreenHeight, maxScreenWidth); //get the dimensions of the terminal window
  mvprintw(50, 50, "HEY");
  refresh();
}

void snakeGame::highScoreList(){
  system("gedit /home/rahulvig/Programs/Snake\\ C++/Snake/scores.txt");
}

bool snakeGame::collision(){
  if(snake[0].x == 0 || snake[0].x == maxScreenWidth - 1 || snake[0].y == 0 || snake[0].y == maxScreenHeight-2){
    return true;
  } //if our snake hits the edge of the screen, collision is true

  for(int i = 2; i < snake.size(); ++i){
    if((snake[0].x == snake[i].x && snake[0].y == snake[i].y)){
      return true;
    }
  } //if the snake collides with itself, collision is true
  //start at 2 because its impossible to collide with the part next to the head

  if(snake[0].x == foodLocation.x && snake[0].y == foodLocation.y){
    obtainFood = true;
    spawnFood(); //generate a new food location
    score+=10; //obtained food so we increase our score by 10.
    move(maxScreenHeight-1, 0); //move cursor to update the score
    printw("Score: %d", score);
    if(score % 100 == 0){
      snakeSpeed -=10000; //increase the speed of the snake every 100 score you get.
    } //the snake has collided with the food.
    return false;
  } else{
    obtainFood = false;
    return false;
  } //the food is not obtained.
}
