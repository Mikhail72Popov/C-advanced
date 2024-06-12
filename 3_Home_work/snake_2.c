#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#define SNAKE_ARRAY_SIZE 310

//Библиотеки Linux
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


//Константы Linux
//Управление (клавиши со стрелками для Манджаро)
#define UP_ARROW  'A'
#define LEFT_ARROW  'D'
#define RIGHT_ARROW  'C'
#define DOWN_ARROW 'B'
#define ENTER_KEY 10

const char SNAKE_HEAD = 'X';
const char SNAKE_BODY = ' ';
const char WALL = '|';
const char CEIL = '-';
const char FOOD = '@';
const char BLANK = '*';

//Функции Linux - Эти функции эмулируют некоторые функции из заголовочного файла conio, доступного только для Windows
void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}


int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

char getch()
{
    char c;
    system("stty raw");
    c= getchar();
    system("stty sane");
    return(c);
}

void clrscr()
{
    system("clear");
    return;
}
//Завершение работы функций Linux


//Это должно быть одинаково в обеих операционных системах
#define EXIT_BUTTON 27 //ESC
#define PAUSE_BUTTON 112 //P

char waitForAnyKey(void)
{
int pressed;

while(!kbhit());

pressed = getch();
return((char)pressed);
}

int getGameSpeed()
{
int speed = 1;
clrscr();

while(speed < 1 || speed > 9){
        return(speed);
}
return 1;
}



//Эта функция проверяет, нажата ли клавиша, затем проверяет, нажата ли какая-либо из клавиш со стрелками/ p/esc. 
//Она изменяет направление в соответствии с нажатой клавишей.
int checkKeysPressed(int direction)
{
int pressed;

if(kbhit()) //Если была нажата клавиша
{
    pressed=getch();
    if (direction != pressed)
    {
        if(pressed == DOWN_ARROW && direction != UP_ARROW)
            direction = pressed;
        else if (pressed == UP_ARROW && direction != DOWN_ARROW)
            direction = pressed;
        else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
            direction = pressed;
        else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
            direction = pressed;
    }
}
return(direction);
}

//Циклически проверяя, являются ли координаты x y координатами змеи в качестве одной из этих частей
//Следует отметить одну вещь: змея длиной 4 не может столкнуться сама с собой, 
//поэтому нет необходимости вызывать эту функцию, если длина змеи <= 4
int collisionSnake (int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect)
{
int i;
for (i = detect; i < snakeLength; i++) //Проверяет, не столкнулась ли змея сама с собой
{
    if ( x == snakeXY[0][i] && y == snakeXY[1][i])
        return(1);
}
return(0);
}

//Генерирует пищу и следит за тем, чтобы еда не появлялась на змейке сверху <- 
//Иногда это приводит к задержке!!! Не такая уж большая проблема, хотя
int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{

do
{
    srand ( time(NULL) );
    foodXY[0] = rand() % (width-2) + 2;
    srand ( time(NULL) );
    foodXY[1] = rand() % (height-6) + 2;
} while (collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0)); //Это должно предотвратить появление "Еды" 
//на вершине змеи. - - Однако у еды есть шанс появиться на вершине змеи, и в этом случае змея должна ее съесть...

gotoxy(foodXY[0] ,foodXY[1]);
printf("%c", FOOD);

return(0);
}

/*
Перемещает массив змей вперед, т.е.
Этот:
x 1 2 3 4 5 6
y 1 1 1 1 1 1
Становится таким:
x 1 1 2 3 4 5
y 1 1 1 1 1 1

Затем, в зависимости от направления (в данном случае на запад - влево), оно становится:

x 0 1 2 3 4 5
y 1 1 1 1 1 1

snakeXY[0][0]--; <- если направление влево, отнять 1 от координаты x
*/
void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
int i;
for( i = snakeLength-1; i >= 1; i-- )
{
    snakeXY[0][i] = snakeXY[0][i-1];
    snakeXY[1][i] = snakeXY[1][i-1];
}

/*
поскольку мы на самом деле не знаем, какая у новой змеи голова xy,
мы должны проверить направление и добавить или отнять от него в зависимости от направления.
*/
switch(direction)
{
    case DOWN_ARROW:
        snakeXY[1][0]++;
        break;
    case RIGHT_ARROW:
        snakeXY[0][0]++;
        break;
    case UP_ARROW:
        snakeXY[1][0]--;
        break;
    case LEFT_ARROW:
        snakeXY[0][0]--;
        break;
}

return;
}

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
int x;
int y;

//Удалите хвост (ЭТО ДОЛЖНО БЫТЬ СДЕЛАНО ДО ПЕРЕМЕЩЕНИЯ МАССИВА!!!!! )
x = snakeXY[0][snakeLength-1];
y = snakeXY[1][snakeLength-1];

gotoxy(x,y);
printf("%c",BLANK);

//Превращает голову змеи в часть тела
gotoxy(snakeXY[0][0],snakeXY[1][0]);
printf("%c", SNAKE_BODY);

moveSnakeArray(snakeXY, snakeLength, direction);

gotoxy(snakeXY[0][0],snakeXY[1][0]);
printf("%c",SNAKE_HEAD);

gotoxy(1,1); //Избавляется от мигающего символа подчеркивания.

return;
}

//Эта функция проверяет, находится ли голова змеи на поверхности блюда, и если это так, то она принесет еще немного еды...
int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[])
{
if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
{
    foodXY[0] = 0;
    foodXY[1] = 0; //Это должно предотвратить появление неприятной ошибки (циклы), требующей проверки, существует ли ошибка по-прежнему...

    printf("\7"); //Гудок
    return(1);
}

return(0);
}
//Нужно немного здесь прибраться
int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength ) 
{
int colision = 0;
if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4)) //Checks if the snake collided wit the wall or it's self
    colision = 1;
else
    //Если змея столкнулась со стеной, нет смысла проверять, столкнулась ли она сама с собой.
    if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1)) 
        colision = 1;

return(colision);
}

void refreshInfoBar(int score, int speed)
{
gotoxy(5,23);
printf("Score: %d", score);

gotoxy(5,24);
printf("Speed: %d", speed);

gotoxy(52,23);
printf("Coders : n/a");

gotoxy(52,24);
printf("Version: 0.1");

return;
}


void gameOverScreen(void)
{
int x = 17, y = 3;

gotoxy(x,y++);
printf(":'######::::::'###::::'##::::'##:'########:\n");
gotoxy(x,y++);
printf("'##... ##::::'## ##::: ###::'###: ##.....::\n");
gotoxy(x,y++);
printf(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
gotoxy(x,y++);
printf(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
gotoxy(x,y++);
printf(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
gotoxy(x,y++);
printf(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
gotoxy(x,y++);
printf(". ######::: ##:::: ##: ##:::: ##: ########:\n");
gotoxy(x,y++);
printf(":......::::..:::::..::..:::::..::........::\n");
gotoxy(x,y++);
printf(":'#######::'##::::'##:'########:'########::'####:\n");
gotoxy(x,y++);
printf("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
gotoxy(x,y++);
printf(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
gotoxy(x,y++);
printf(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
gotoxy(x,y++);
printf(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
gotoxy(x,y++);
printf(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
gotoxy(x,y++);
printf(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
gotoxy(x,y++);
printf(":.......::::::...:::::........::..:::::..::....::\n");

waitForAnyKey();
clrscr(); //очистите консоль
return;
}

//Грязно, нужно почистить эту функцию
void startGame( int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed)
{
int gameOver = 0;
clock_t endWait;

//CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
int waitMili = CLOCKS_PER_SEC-(speed)*(CLOCKS_PER_SEC/10);  //Устанавливает правильное время ожидания в соответствии с выбранной скоростью
int tempScore = 10*speed;
int oldDirection;
int canChangeDirection = 1;
//int seconds = 1;

endWait = clock() + waitMili;

do
{
    if(canChangeDirection)
    {
        oldDirection = direction;
        direction = checkKeysPressed(direction);
    }

    if(oldDirection != direction)//Временное исправление, предотвращающее столкновение змеи с самой собой
        canChangeDirection = 0;

    if(clock() >= endWait) //он перемещается в зависимости от того, насколько быстро работает компьютер...
    {

        move(snakeXY, snakeLength, direction);
        canChangeDirection = 1;


        if(eatFood(snakeXY, foodXY))
        {
            generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength); //Производите больше пищи
            snakeLength++;
            score+=10;

            if( score >= 10*speed+tempScore)
            {
                speed++;
                tempScore = score;

                if(speed <= 9)//это должно быть исправлено
                    waitMili = waitMili - (CLOCKS_PER_SEC/10);
                else
                {
                    if(waitMili >= 40) //Максимальная скорость
                        waitMili = waitMili - (CLOCKS_PER_SEC/200);

                }

            }

            refreshInfoBar(score, speed);
        }

        endWait = clock() + waitMili; //ВРЕМЕННОЕ ИСПРАВЛЕНИЕ, НУЖНО НАЙТИ СПОСОБ СБРОСИТЬ CLOCK() .. Нет, похоже, этот способ работает нормально...
    }

    gameOver = collisionDetection(snakeXY, consoleWidth, consoleHeight, snakeLength);

    if(snakeLength >= SNAKE_ARRAY_SIZE-5) //Просто чтобы убедиться, что он не станет больше размера массива и не выйдет из строя
    {
        gameOver = 2;//Вы выиграли! <- похоже, не работает - НУЖНО ИСПРАВИТЬ / ПРОТЕСТИРОВАТЬ ЭТО
        score+=1500; //Когда вы выигрываете, вы получаете дополнительные 1500 очков!!!
    }

} while (!gameOver);

switch(gameOver)
{
    case 1:
        printf("\7"); //Гудок
        printf("\7"); //Гудок

        gameOverScreen();
        break;
}

return;
}


void loadEnviroment(int consoleWidth, int consoleHeight)//Это можно сделать лучшим способом... ИСПРАВЬТЕ МЕНЯ!!!! Также я думаю, что в ubuntu это работает некорректно <- Исправлено
{
//int i;
int x = 1, y = 1;
int rectangleHeight = consoleHeight - 4;
clrscr(); //очистите консоль

gotoxy(x,y); //Верхний левый угол

for (; y < rectangleHeight; y++)
{
    gotoxy(x, y); //Левая стена
    printf("%c",WALL);

    gotoxy(consoleWidth, y); //Правая стена
    printf("%c",WALL);
}

y = 1;
for (; x < consoleWidth+1; x++)
{
    gotoxy(x, y); //Левая стена
    printf("%c",WALL);

    gotoxy(x, rectangleHeight); //Правая стена
    printf("%c",WALL);
}

return;
}


void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
int i;
/*
Во-первых, у змеи на самом деле недостаточно координат XY (только 1 - начальное местоположение), поэтому мы используем
эти координаты XY для "создания" других координат. Для этого мы можем использовать функцию, используемую для перемещения змеи.
Это помогает создать "целую" змейку вместо одной "точки", когда кто-то начинает игру.
*/
//moveSnakeArray(змея, длина змеи); //Следует обратить внимание на то, что змея начинается с одной координаты, в каком бы направлении она ни была направлена...

//Это должно привести к появлению змеи :P
for (i = 0; i < snakeLength; i++)
{
    gotoxy(snakeXY[0][i], snakeXY[1][i]);
    printf("%c", SNAKE_BODY); //Эх, в какой-то момент мне следовало бы сделать так, чтобы змея начиналась с головы...
}

return;
}


void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
int i;
int snakeX = snakeXY[0][0];
int snakeY = snakeXY[1][0];



for(i = 1; i <= snakeLength; i++)
{
    snakeXY[0][i] = snakeX + i;
    snakeXY[1][i] = snakeY;
}

return;
}

//Эта функция загружает окружение, snake и т.д
void loadGame(void)
{
int snakeXY[2][SNAKE_ARRAY_SIZE]; //Двумерный массив, первый массив предназначен для координат X, а второй массив - для координат Y

int snakeLength = 2; //Начальная длина

int direction = LEFT_ARROW; //НЕ МЕНЯЙТЕ ЭТО НА СТРЕЛКУ ВПРАВО, ЕСЛИ ВЫ ЭТО СДЕЛАЕТЕ, ИГРА БУДЕТ НЕМЕДЛЕННО ЗАКОНЧЕНА!!! <- Если только функция prepareSnakeArray не будет изменена с учетом направления....

int foodXY[] = {5,5};// Сохраняет информацию о местонахождении продуктов питания

int score = 0;
//int level = 1;

//Ширина окна * высота - в какой-то момент найдите способ получить фактические размеры консоли... <- - Кроме того, некоторые элементы отображения не учитывают эти размеры.. нужно это исправить...
int consoleWidth = 80;
int consoleHeight = 25;

int speed = getGameSpeed();

//Начальное местоположение змеи
snakeXY[0][0] = 40;
snakeXY[1][0] = 10;

loadEnviroment(consoleWidth, consoleHeight); //границы
prepairSnakeArray(snakeXY, snakeLength);
loadSnake(snakeXY, snakeLength);
generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
refreshInfoBar(score, speed); //Нижняя информационная панель. Количество очков, уровень и т.д.
startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);

return;
}


int main() //Нужно все это уладить
{
  loadGame();

  return(0);
}