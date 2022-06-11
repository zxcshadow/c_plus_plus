#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>
#include <cmath>
#include "string.h"
#include <codecvt>
using namespace std;


struct wav_header_t
{
	char                chunkId[4];                 // Информация о формате файла (RIFF), Содержит символы “RIFF” в ASCII кодировке;
	unsigned long       ChunkSize;                  // Размер без  chunkId[4];
	char                format[4];                  // Формат потоковых данных (WAVE);
	char                subchunk1Id[4];             // Описание параметров WAV-файла (fmt-chunk);
	unsigned long       subchunk1Size;              // Размер подструктуры  subchunk1  (16 байт);
	unsigned short      audioFormat;                // Аудио формат (PCM = 1); PCM - импульсная кодовая модуляция
	unsigned short      nChannels;                  // Количество каналов (Моно = 1, Стерео = 2);
	unsigned long       SamplesRate;                // Частота дискретизации в Гц;
	unsigned long       ByteRate;                   // Кол-во передаваемых байт в секунду воспроизведения;
	unsigned short      blockAlign;                 // Размер сэмпла в байтах 16 бит = 2 байта моно, 32 бита = 4 байта стерео (включая все каналы);
	unsigned short      BitsPerSample;              // Количество бит в сэмпле. Так называемая “глубина” или точность звучания. 8 бит, 16 бит и т.д. /// битов на отсчет
	char                Subchunk2ID[4];             // Символы "Data", начало чанка данных;
	unsigned long       Subchunk2Size;              // Размер области данных в байтах;
													// продолжительность = Subchunk2Size/ByteRate
													// пойнтер стоит на дает сразу после структуры  
};

struct WAVdata {
	short int data1;
};

//struct chunk_t
//{
//    char ID[4]; //"data" = 0x61746164
//    unsigned long size; //Chunk data bytes фрагмент данных байт
//};

void SetCursorPosition(short x, short y)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x , y };
	SetConsoleCursorPosition(output, pos);
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
};

void navigation ()
{
	
}

void graphik() {
	system("cls");
	double l = 0;
	for (int i = 1; i < 7; i++) {
		SetCursorPosition(17, 16 - i);
		cout << "|";
	}
	SetCursorPosition(16, 10);
	cout << "/\\";
	for (int i = 0; i < 5; i++) {
		SetCursorPosition(13, 15 - i);
		l += 0.2;
		cout << l;
	}
	SetCursorPosition(13, 16);
	cout << 0;
	for (int i = 0; i < 50; i++) {
		SetCursorPosition(17 + i, 16);
		cout << "=";
	}
	SetCursorPosition(0, 0);
	cout << "Q - Выход";
	SetCursorPosition(0, 2);
	cout << "A - Предыдущая гистограмма";
	SetCursorPosition(0, 3);
	cout << "D - Следующая гистограмма";
	SetCursorPosition(80, 1);
	cout << "W - Движение курсора вверх";
	SetCursorPosition(80, 2);
	cout << "S - Движение курсора вниз";


	SetCursorPosition(20, 17);     cout << "50";
	SetCursorPosition(20 + 5, 17); cout << "100";
	SetCursorPosition(25 + 5, 17); cout << "200";
	SetCursorPosition(30 + 5, 17); cout << "500";
	SetCursorPosition(35 + 5, 17); cout << "1k";
	SetCursorPosition(40 + 5, 17); cout << "2k";
	SetCursorPosition(45 + 5, 17); cout << "5k";
	SetCursorPosition(50 + 5, 17); cout << "10k";
	SetCursorPosition(55 + 5, 17); cout << "20k";
	SetCursorPosition(57 + 9, 16); cout << ">";
	SetCursorPosition(62 + 5, 17); cout << "Гц";
}
void SetHistogram(unsigned int x, unsigned int y, short znak, short height, short column) {
	x = x + 5 * column;
	for (int i = 0; i < height; i++) {
		SetCursorPosition(x, y - i);
		cout << (char)znak;
	}
}
void commandW(unsigned short* column) {
	(*column)--;									//вверх
	SetCursorPosition(90, *column);
	cout << ">";
	SetCursorPosition(90, *column + 1);
	cout << " ";
	SetCursorPosition(10, 0);
}
void commandS(unsigned short* column) {
	(*column)++;									//вниз
	SetCursorPosition(90, *column - 1);
	cout << " ";
	SetCursorPosition(90, *column);
	cout << ">";
	SetCursorPosition(10, 0);
}
void commandE(unsigned short* exit) {
	system("cls");
	(*exit)--;
	SetCursorPosition(10, 0);
}
void column_9_Enter(unsigned short* exit, wav_header_t obj) {
	system("cls");
	SetCursorPosition(50, 8);
	(*exit)++;
	SetCursorPosition(10, 0);
}
void column_10_Enter(unsigned short* exit) {
	system("cls");
	SetCursorPosition(70, 8);
	graphik();
	(*exit)++;
	SetCursorPosition(10, 0);
}

void Samples(int* arr_s, int j, int i, double** array)
{
	if (arr_s[i] < 0) { arr_s[i] = -arr_s[i] + 1; }
	if ((arr_s[i] >= 0) && (arr_s[i] <= 75)) { array[j][0]++; }
	if ((arr_s[i] >= 76) && (arr_s[i] <= 150)) { array[j][1]++; }
	if ((arr_s[i] >= 151) && (arr_s[i] <= 350)) { array[j][2]++; }
	if ((arr_s[i] >= 351) && (arr_s[i] <= 750)) { array[j][3]++; }
	if ((arr_s[i] >= 751) && (arr_s[i] <= 1500)) { array[j][4]++; }
	if ((arr_s[i] >= 1501) && (arr_s[i] <= 3500)) { array[j][5]++; }
	if ((arr_s[i] >= 3501) && (arr_s[i] <= 7500)) { array[j][6]++; }
	if ((arr_s[i] >= 7501) && (arr_s[i] <= 15000)) { array[j][7]++; }
	if (arr_s[i] >= 15001) { array[j][8]++; }
}
void Histogram(double** array, unsigned short j, wav_header_t obj) {
	for (int y = 0; y < 9; y++) {
		if ((array[j][y] / obj.ByteRate < 0.2) && (array[j][y] / obj.ByteRate > 0)) {
			SetHistogram(20, 15, 33, 1, y);
		}
		if ((array[j][y] / obj.ByteRate >= 0.2) && (array[j][y] / obj.ByteRate < 0.4)) {
			SetHistogram(20, 15, 33, 2, y);
		}
		if ((array[j][y] / obj.ByteRate >= 0.4) && (array[j][y] / obj.ByteRate < 0.6)) {
			SetHistogram(20, 15, 33, 3, y);
		}
		if ((array[j][y] / obj.ByteRate >= 0.6) && (array[j][y] / obj.ByteRate < 0.8)) {
			SetHistogram(20, 15, 33, 4, y);
		}
		if ((array[j][y] / obj.ByteRate >= 0.8) && (array[j][y] / obj.ByteRate < 1)) {
			SetHistogram(20, 15, 33, 5, y);
		}
		if (array[j][y] / obj.ByteRate == 1) {
			SetHistogram(20, 15, 33, 5, y);
		}
	}
}
void Clear() {
	for (int j = 15; j > 9; j--) {
		for (int i = 18; i < 90; i++) {
			SetCursorPosition(i, j);
			cout << ' ';
		}
	}
}


int main()
{
	system("cls");
	setlocale(0, "");
	FILE* file;
	errno_t error = 0;
	error = fopen_s(&file, "C://Users//HP//Downloads//lll1.wav", "rb");
	graphik();
	if (error != 0)
	{
		cout << "Не удалось открыть файл\n";
		return 0;
	}
	wav_header_t header2;
	WAVdata data322;

	fread_s(&header2, sizeof(wav_header_t), sizeof(wav_header_t), 1, file);
	double Time = header2.ChunkSize / ((header2.BitsPerSample / 8) * header2.nChannels * header2.SamplesRate);int m = header2.ByteRate;
	double** array = new double* [Time];
	for (int i = 0; i < Time; i++) {
		array[i] = new double[10];
	}
	for (int i = 0; i < Time; i++) {
		for (int j = 0; j < 10; j++) {
			array[i][j] = 0;
			//cout << m;
		}
	}


	// for (int j = 0; j < Time; j++) {
	//     cout << endl;
	 //    for (int i = 0; i < 10; i++) {
			 //cout << arrayofSamples[j][i] << "\t";
	 //    }
	// }
	unsigned short  column = 9, j = 0, exit = 0, count = 0;
	char command = ' ';
	SetCursorPosition(90, column);
	cout << ">";
	SetCursorPosition(10, 0);
	while (command != 'q') {
		if (_kbhit) {
			if ((command == 'w') && (column > 9) && (exit == 0)) {
				commandW(&column);
			}
			else if ((command == 's') && (column < 10) && (exit == 0)) {
				commandS(&column);
			}
			if ((column == 9) && (command == (char)13)) {
				column_9_Enter(&exit, header2);
			}
			if ((column == 10) && (command == (char)13)) {
				column_10_Enter(&exit);
			}
			if ((column == 10) && (command == 'd') && (j < Time)) {
				Clear();

				if (count == j) {
					int* arr_s = new int[header2.ByteRate];   // arrSamples - array of Samples
					for (int i = 0; i < (header2.ByteRate); i++) {
						fread_s(&data322, sizeof(WAVdata), sizeof(WAVdata), 1, file);
						arr_s[i] = data322.data1;
						Samples(arr_s, j, i, array);
					}
					delete[]arr_s;
					count++;
				}
				SetCursorPosition(35, 19);
				cout << j + 1 << " секунда  ";
				Histogram(array, j, header2);
				SetCursorPosition(10, 0);
				SetCursorPosition(10, 0);
				j++;
			}
			if ((column == 10) && (command == 'a') && (j > 0)) {
				SetCursorPosition(35, 19); j--;
				cout << j + 1 << " секунда  ";
				Clear();
				Histogram(array, j, header2);
				SetCursorPosition(10, 0);
			}
			if ((command == 'e') && (exit > 0)) {
				commandE(&exit);
			}

		}
		command = ' ';
		command = _getch();
	}
	for (int i = 0; i < Time; i++) {   //удаление двумерного массива
		delete[] array[i];
	}
	delete[]array;
	fclose(file);
	system("cls");
	return 0;

}
