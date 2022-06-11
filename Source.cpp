// filemanager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <Windows.h>
#include <process.h>
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

int curs = 0; //текущая выбранная строка - курсор
int last_el = 0; //последний элемент в каталоге, нужен для корректной работы курсора
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
fs::path current_path = "C:\\"; //выбранная директория

//функция для ввода параметра
string set_param()
{
	string buf;
	cout << "Задайте параметр для приложения: " << endl;
	getline(cin, buf);
	return buf;
}

//переход по папкам и запуск файлов
void enter_folder_or_open_file()
{
	int count = 0;
	for (auto& p : fs::directory_iterator(current_path))
	{
		if (curs == count) //находим строку на которую показывает курсор
		{
			if (p.path().extension() == ".exe") //если выбранный файл exe, делаем это
			{
				//p.path().string() - берём полное расположение файла
				//ставим пробел
				//set_param() прибавляем параметр запуска
				string buf = p.path().string() + " " + set_param();
				char file_exe[2048]; //буферная переменная для передачи в функцию exe + параметр
				strcpy_s(file_exe, buf.c_str()); //копируем из string в char

				//создаём переменные необходимые для запуска стороннего exe
				//
				//определить оконный терминал
				//инф о недавном созданном процессе и его первичном потоке
				STARTUPINFOA si;
				PROCESS_INFORMATION pi;

				RtlZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				RtlZeroMemory(&pi, sizeof(pi));

				//создаём процесс exe + параметр
				if (!CreateProcessA(NULL, file_exe, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
					cout << "///Открываю EXE///" << endl;

				//ждём пока процесс закончит своё выполнение
				WaitForSingleObject(pi.hProcess, INFINITE);

				//прибераемся за процессом
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				system("pause");

			}
			else if (p.path().extension() == "") //если выбранна папка
			{
				current_path = p.path(); //присваеваем выбранной директории новое значение выбранной папки
				curs = 0; //обнуляем курсор
				last_el = 0;
			}
			else
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				cout << "Я пока не умею открывать данный тип файлов :(" << endl;
				system("pause");
			}
			return;
		}
		count++;
	}
}

//печать содержимого каталога
void print_struct_folder()
{
	int count = 0; //счётчик

	//не рекурсивный проход каталога
	for (auto& p : fs::directory_iterator(current_path))
	{
		if (curs == count)
		{
			//выделяем цветом строку на которую указывает курсор
			SetConsoleTextAttribute(hConsole, 63);
			cout << count << " " << p.path() << endl;
			SetConsoleTextAttribute(hConsole, 7);
		}
		else
		{
			cout << count << " " << p.path() << endl;
		}

		count++;
	}
	last_el = count - 1;
	//cout << curs << endl;
}

//печать первых двух строк - навигация
void print_navigation()
{
	//выделяем дветом строку навигации
	SetConsoleTextAttribute(hConsole, 2);
	cout << current_path << endl;
	SetConsoleTextAttribute(hConsole, 7);

	//отображение возвращения в предыдущую папку
	if (current_path != current_path.parent_path())
		cout << "<- (Esc)" << endl;
	else
		cout << "Root folder!" << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
	cout << "передвижение вверх - стрелочка верх " << endl;
	cout << "передвижение вниз - стрелочка вниз " << endl;
	SetConsoleTextAttribute(hConsole, 7);
	cout << "_______________________________________ " << endl;
}

//функция перехода к предыдущему каталогу
void up_folder()
{
	string buf = "";
	if (current_path != current_path.parent_path()) // если мы не в коренном каталоге
	{
		string s(current_path.string()); //присваиваем строке текущее расположение
		s.resize(s.rfind("\\")); //обрезаем строку вместе с первым встречающимся с конца "\\"

		if (s.size() == 2) //для коректного возвращения в корневой каталог "C:\\"
			s += "\\";

		current_path = s; //возвращаем новое значение директории
	}
}

//главная функция
void main()
{
	setlocale(LC_ALL, "RUS"); //русификация

	while (true)
	{
		print_navigation(); //печатаем первые две строки, которые помогут нам ориентироваться в файловой системе
		print_struct_folder(); //печатаем файлы и папки которые есть в каталоге

		switch (_getch())//в зависимости от нажатой клавиши управляем программой
		{
		case 72: //вверх
			if (curs > 0)
				curs--;
			else
				curs = last_el;

			break;

		case 80: //вниз
			if (curs < last_el)
				curs++;
			else
				curs = 0;

			break;

		case 13: //enter
			enter_folder_or_open_file(); //открыть выбранный каталог/файл

			break;

		case 27: //esc (назад)
			up_folder(); //возвращаемся на один каталог обратно

			break;

		default:
			break;
		}

		system("cls");
	}

	system("pause");
}