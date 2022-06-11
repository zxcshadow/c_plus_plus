// filemanager.cpp : ���� ���� �������� ������� "main". ����� ���������� � ������������� ���������� ���������.
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

int curs = 0; //������� ��������� ������ - ������
int last_el = 0; //��������� ������� � ��������, ����� ��� ���������� ������ �������
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
fs::path current_path = "C:\\"; //��������� ����������

//������� ��� ����� ���������
string set_param()
{
	string buf;
	cout << "������� �������� ��� ����������: " << endl;
	getline(cin, buf);
	return buf;
}

//������� �� ������ � ������ ������
void enter_folder_or_open_file()
{
	int count = 0;
	for (auto& p : fs::directory_iterator(current_path))
	{
		if (curs == count) //������� ������ �� ������� ���������� ������
		{
			if (p.path().extension() == ".exe") //���� ��������� ���� exe, ������ ���
			{
				//p.path().string() - ���� ������ ������������ �����
				//������ ������
				//set_param() ���������� �������� �������
				string buf = p.path().string() + " " + set_param();
				char file_exe[2048]; //�������� ���������� ��� �������� � ������� exe + ��������
				strcpy_s(file_exe, buf.c_str()); //�������� �� string � char

				//������ ���������� ����������� ��� ������� ���������� exe
				//
				//���������� ������� ��������
				//��� � �������� ��������� �������� � ��� ��������� ������
				STARTUPINFOA si;
				PROCESS_INFORMATION pi;

				RtlZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				RtlZeroMemory(&pi, sizeof(pi));

				//������ ������� exe + ��������
				if (!CreateProcessA(NULL, file_exe, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
					cout << "///�������� EXE///" << endl;

				//��� ���� ������� �������� ��� ����������
				WaitForSingleObject(pi.hProcess, INFINITE);

				//����������� �� ���������
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				system("pause");

			}
			else if (p.path().extension() == "") //���� �������� �����
			{
				current_path = p.path(); //����������� ��������� ���������� ����� �������� ��������� �����
				curs = 0; //�������� ������
				last_el = 0;
			}
			else
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				cout << "� ���� �� ���� ��������� ������ ��� ������ :(" << endl;
				system("pause");
			}
			return;
		}
		count++;
	}
}

//������ ����������� ��������
void print_struct_folder()
{
	int count = 0; //�������

	//�� ����������� ������ ��������
	for (auto& p : fs::directory_iterator(current_path))
	{
		if (curs == count)
		{
			//�������� ������ ������ �� ������� ��������� ������
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

//������ ������ ���� ����� - ���������
void print_navigation()
{
	//�������� ������ ������ ���������
	SetConsoleTextAttribute(hConsole, 2);
	cout << current_path << endl;
	SetConsoleTextAttribute(hConsole, 7);

	//����������� ����������� � ���������� �����
	if (current_path != current_path.parent_path())
		cout << "<- (Esc)" << endl;
	else
		cout << "Root folder!" << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED);
	cout << "������������ ����� - ��������� ���� " << endl;
	cout << "������������ ���� - ��������� ���� " << endl;
	SetConsoleTextAttribute(hConsole, 7);
	cout << "_______________________________________ " << endl;
}

//������� �������� � ����������� ��������
void up_folder()
{
	string buf = "";
	if (current_path != current_path.parent_path()) // ���� �� �� � �������� ��������
	{
		string s(current_path.string()); //����������� ������ ������� ������������
		s.resize(s.rfind("\\")); //�������� ������ ������ � ������ ������������� � ����� "\\"

		if (s.size() == 2) //��� ���������� ����������� � �������� ������� "C:\\"
			s += "\\";

		current_path = s; //���������� ����� �������� ����������
	}
}

//������� �������
void main()
{
	setlocale(LC_ALL, "RUS"); //�����������

	while (true)
	{
		print_navigation(); //�������� ������ ��� ������, ������� ������� ��� ��������������� � �������� �������
		print_struct_folder(); //�������� ����� � ����� ������� ���� � ��������

		switch (_getch())//� ����������� �� ������� ������� ��������� ����������
		{
		case 72: //�����
			if (curs > 0)
				curs--;
			else
				curs = last_el;

			break;

		case 80: //����
			if (curs < last_el)
				curs++;
			else
				curs = 0;

			break;

		case 13: //enter
			enter_folder_or_open_file(); //������� ��������� �������/����

			break;

		case 27: //esc (�����)
			up_folder(); //������������ �� ���� ������� �������

			break;

		default:
			break;
		}

		system("cls");
	}

	system("pause");
}