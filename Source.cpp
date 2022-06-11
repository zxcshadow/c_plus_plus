#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>
using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

//enum class color : unsigned short
//{
//	black, blue, green, cyan, red, magenta, brown, yellow, white
//};


FILE* file;
char* namefile = NULL;
int fileSize = 0;

long int numByte[256];

double limit = 0;
int viewByte = 0;
int allByte = 0;

void ChangeCoord(int, int);
void ChangeWindow(int, int);
void ChangeBufer(int a, int b);

//считывание
void readFile()
{
	fopen_s(&file, namefile, "rb");
	for (int i = 0; i < 256; i++)
	{
		numByte[i] = 0;
	}
	fseek(file, 0, 2);
	fileSize = ftell(file);
	fseek(file, 0, 0);
	unsigned char oneByte;
	for (int i = 0; i < fileSize; i++){
		fread_s(&oneByte, 1, 1, 1, file);
		numByte[int(oneByte)] += 1;
		allByte += 1;
	}
	fclose(file);
}

//создаем таблицу
void creatTable() 
{
	ChangeBufer(6 * 18 + 3, 51);
	ChangeWindow(6 * 18 + 3, 51);

	SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE | FOREGROUND_RED);
	for (int i = 0; i < 10; i++)
		cout << "      " << i;
	for (int i = 0; i < 6; i++)
		cout << "      " << char(int('A') + i);
	for (int i = 0; i < 10; i++)
		cout << '\n' << '\n' << '\n' << i;
	for (int i = 0; i < 6; i++)
		cout << '\n' << '\n' << '\n' << char(int('A') + i);

	SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE);
	ChangeCoord(0, 1);
	for (int i = 0; i < 6 * 18 + 3; i++) cout << '_';
	for (int i = 0; i < 52; i++)
	{
		ChangeCoord(2, i);
		cout << '|';
	}
	for (int i = 0; i < 6 * 18 + 3; i++)
	{
		ChangeCoord(i, 50);
		cout << '_';
	}
	for (int i = 0; i < 52; i++)
	{
		ChangeCoord(114, i);
		cout << '|';
	}

	SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

//заполняем таблицу
void fillTable()
{
	for (int i = 0; i < 256; i++){
		int x = i % 16;
		int y = i / 16;
		ChangeCoord(6 + 7 * x, 3 + 3 * y);

		double prec = ((double)numByte[i] / allByte) * 100;
		prec = round(prec * 100) / 100;

		if (prec > limit && prec != 0) SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN); //лимит
		if (viewByte % 2 == 0) cout << numByte[i];
		else{
			cout << prec << '%';
		}
		SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
}

void ChangeWindow(int a, int b) {
	COORD crd = { a, b };
	SMALL_RECT src = { 0, 0, crd.X, crd.Y };
	SetConsoleWindowInfo(hStdOut, true, &src);
}

void ChangeBufer(int a, int b){
	COORD crd = { a, b };
	SetConsoleScreenBufferSize(hStdOut, crd);
}

void GetLimit(char* argv){
	int lenLim = strlen(argv);
	switch (lenLim)
	{
	case 1:
		limit = int(argv[0]) - int('0');
		break;
	case 2:
		limit = (int(argv[0]) - int('0')) * 10 + (int(argv[1]) - int('0'));
		break;
	case 4:
		limit = int(argv[0]) - int('0') + double(argv[2] - int('0')) / 10 + double(argv[3] - int('0')) / 100;
		break;
	case 5:
		limit = (int(argv[0]) - int('0')) * 10 + (int(argv[1]) - int('0')) + double(argv[3] - int('0')) / 10 + double(argv[4] - int('0')) / 100;
	}
}

void ChangeCoord(int x, int y){
	COORD pos = { x, y };
	SetConsoleCursorPosition(hStdOut, pos);
}

int main(int argc, char* argv[])
{
	setlocale(0, "");
	system("cls");
	namefile = argv[1];
	GetLimit(argv[2]);
	readFile();
	int code;
	do
	{
		creatTable();
		fillTable();
		code = _getch();
		system("cls");
		if (code == int('1')) viewByte += 1;
	} while (code != 13);
	return 0;
}