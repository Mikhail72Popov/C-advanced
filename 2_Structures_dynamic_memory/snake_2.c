/*
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;


int main() {
	srand(time(0)); // запуск генератора случайных чисел
	system("title Snake Game");
	system("mode con cols=70 lines=31"); // установка размеров окна консоли
	MoveWindow(GetConsoleWindow(), 50, 50, 2000, 2000, true); // установка стартовой позиции окна консоли (50 и 50 - это пиксели
	// относительно верхнего левого угла монитора
	const int width = 50, height = 30; // размеры поля, по которому бегает змейка
	const int max_length = 50; // установка максимальной длины "змейки"
	int array_X[max_length]; // массив,хранящий абсциссы звеньев "змейки"
	int array_Y[max_length]; // массив, хранящий ординаты звеньев "змейки"
	int length = 1; // переменная длины "змейки"
 array_X[0] = width / 2; // установка стартовой абсциссы "змейки"
 array_Y[0] = height / 2; // установка стартовой ординаты "змейки"
	int dx = 1, dy = 0; // создание смещений по осям для движения "змейки"
	int X_apple; // абсцисса "яблока"
	int Y_apple; // ордината "яблока"
	do // цикл ставит координаты яблока случанйм образом - но чтобы они не совпадали со "змейкой"
	{
 X_apple = rand() % (width - 2) + 1;
 Y_apple = rand() % (height - 2) + 1;
 } while (X_apple != array_X[length - 1] && Y_apple != array_Y[length - 1]);

	int sleep_time = 100; // переменная частоты кадров 

	char snake = '*'; // символ для отображения тела "змейки"
	char apple = 'o'; // символ для отображения "яблока"
	char head = 1; // символ для отображения головы "змейки"
 COORD c; // объект для хранения координат
 HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // создание хендла потока вывода
 CONSOLE_CURSOR_INFO cci = { sizeof(cci), false }; // создание параметров на отображение курсора
	SetConsoleCursorInfo(h, &cci); //связывание параметров и хендла

	SetConsoleTextAttribute(h, 4); // установка цвета, которым рисуется рамка поля
	for (int y = 0; y < height; y++) // стандартный двойной цикл на отрисовку рамки
	{
		for (int x = 0; x < width; x++)
		{
			char s;
			if (x == 0 && y == 0) // в верхнем левом углу поля - символ соответствующего угла
 s = '#'; // 218
			else if (x == 0 && y == height - 1) // нижний левый угол
 s = '#'; // 192
			else if (y == 0 && x == width - 1) // верхний правый угол
 s = '#'; // 191
			else if (y == height - 1 && x == width - 1) // нижний правый угол
 s = '#'; // 217
			else if (y == 0 || y == height - 1) // верхняя и нижняя граница поля
 s = '#'; // 196
			else if (x == 0 || x == width - 1) // левая и правая граница поля
 s = '#'; // 179
			else s = ' '; // во всех остальных случаях должен быть просто пробел (означает пустую область поля)
 cout << s; // выводим символ
		}
 cout << "\n";
	}

 c.X = X_apple; // связываем объект координат с позициями "яблока"
 c.Y = Y_apple;
	SetConsoleCursorPosition(h, c); // отправляем курсор на позицию "яблока"
	SetConsoleTextAttribute(h, 12); // устанавливаем красный цвет для отрисовки "яблока"
 cout << apple; // отображаем символ "яблока"

 c.X = array_X[0]; // связываем объект координат со стартовой позицией "змейки"
 c.Y = array_Y[0];
	SetConsoleCursorPosition(h, c); // отправляем курсор на позицию головы "змейки"
	SetConsoleTextAttribute(h, 10); // устанавливаем зеленый цвет для отрисовки "змейки"
 cout << head; // отображаем символ головы "змейки"

	bool flag = true; // переменная для управления ходом цикла
	do // собственно цикл игры
	{
		Sleep(sleep_time); // задержка потока программы на заданный ранее интервал

		if (_kbhit()) // проверяем, была ли нажата какая-либо клавиша и запускаем её обработку в случае ИСТИНЫ
		{
			int k = _getch(); // считываем код клавиши из буфера
			if (k == 0 || k == 224) // если первый код - вспомогательный, считываем второй код
 k = _getch();
			switch (k) // пропускаем код нажатой клавиши внутрь оператора выбора
			{
			case 80: // если была нажата клавиша вниз
 dy = 1; // то приращение по оси ординат будет положительным
 dx = 0; // по оси абсцисс приращение нулевое
				break;
			case 72: // если вверх
 dy = -1; // аналогично согласно геометрической логике
 dx = 0;
				break;
			case 75: // если влево
 dy = 0;
 dx = -1;
				break;
			case 77: // если вправо
				dy = 0;
 dx = 1;
				break;
			case 27: // если была нажата клавиша ESC
 flag = false; // устанавливаем флажок на ЛОЖЬ, чтоб закончить показ движения
				break;
			}
		}

		int X = array_X[length - 1] + dx; // определяем значение абсциссы головы "змейки" после смещения
		int Y = array_Y[length - 1] + dy; // то же самое для ординаты
		if (X == 0 || X == width - 1 || Y == 0 || Y == height - 1) // проверка на достижение границ поля
		{
 flag = false; // пока что - просто установка управляющей переменной цикла
		}
		else if (X == X_apple && Y == Y_apple) // проверка на достижение "яблока"
		{
 c.X = array_X[length - 1]; // установка в объект координат позиции головы "змейки"
 c.Y = array_Y[length - 1];
			SetConsoleCursorPosition(h, c); // установка курсора в эту позицию
 cout << snake; // отображение символа тела "змейки"

 length++; // увеличение длины "змейки" (яблоко проглочено)
 c.X = array_X[length - 1] = X; // установка в массивы позиции нового звена "змейки"
 c.Y = array_Y[length - 1] = Y;
			SetConsoleCursorPosition(h, c); // установка туда курсора
 cout << head; // и отображение там символа головы "змейки"

			if (length == max_length) // проверка, достигла ли длина "змейки" своего максимального значения
			{
				break; // пока что - просто прерываем цикл 
			}

			int i; // переменная для подсчета количества звеньев "змейки", не совпадающих с позицией "яблока"
			do {
 X_apple = rand() % (width - 2) + 1; // установка новых координат "яблока"
 Y_apple = rand() % (height - 2) + 1;
 i = 0; // обнуление числа несовпадающих координат
				for (; i < length; i++) // запуск цикла на сверку совпадений
					if (X_apple == array_X[i] && Y_apple == array_Y[i]) // если совпадение найдено
						break; // то прерываем цикл for
 } while (i < length); // поиск новых координат продолжается, пока число несовпадающих координат меньше длины "змейки"

 c.X = X_apple; // установка в объект координат новой корректной позиции "яблока"
 c.Y = Y_apple;
			SetConsoleCursorPosition(h, c); // отправка туда курсора
			SetConsoleTextAttribute(h, 12); // установка цвета в красный
 cout << apple; // отображение символа "яблока"
			SetConsoleTextAttribute(h, 10); // обратная установка цвета в зеленый - для дальнейшего отображения "змейки"
		}
		else // случай, когда голова "змейки" оказалась на новой пустой позиции
		{
			int i = 1; // переменная на количество звеньев, не совпадающих с новой позицией - кроме хвоста "змейки"
			for (; i < length; i++)
				if (X == array_X[i] && Y == array_Y[i]) // если совпадение найдено в цикле - прерываемся
					break;
			if (i < length) // если число несовпадающих звеньев меньше длины "змейки" - то прерываем основной цикл игры
				break;
			else // а иначе запускаем обработку сдвига "змейки"
			{
 c.X = array_X[0]; // устанавливаем в объект координат позицию хвоста "змейки"
 c.Y = array_Y[0];
				SetConsoleCursorPosition(h, c); // двигаем туда курсор
 cout << ' '; // и отображаем пробел (затирка хвоста)

				if (length > 1) // если длина змейки больше 
				{
 c.X = array_X[length - 1]; // устанавливаем в объект координат предыдущую позицию головы "змейки"
 c.Y = array_Y[length - 1];
					SetConsoleCursorPosition(h, c); // двигаем туда курсор
					putchar(snake); // выводим символ тела "змейки"
				}

				for (int i = 0; i < length - 1; i++) // запускаем цикл свдига координат звеньев "змейки"
				{
 array_X[i] = array_X[i + 1]; // обрабатываем все звенья - кроме последнего
 array_Y[i] = array_Y[i + 1];
				}

 c.X = array_X[length - 1] = X; // устанавливаем новую позицию головы "змейки"
 c.Y = array_Y[length - 1] = Y;
				SetConsoleCursorPosition(h, c); // двигаем туда курсора
 cout << head; // отображаем символ головы "змейки"
			}
		}
 } while (flag); // выходим из цикла, если сброшена управляющая переменная
	system("cls"); // очищаем экран
 cout << "GAME OVER\n"; // выводим сообщение о конце игры
}
*/