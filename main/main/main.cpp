#include <iostream>
#include <cstring>
#include <Windows.h>

#pragma warning (disable : 4996) //Без этого ругается на fopen и strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define FIO_WITH 32
#define BASE_NAME "polyclinicBase.txt"

using namespace std;

struct Polyclinic
{
	static int totalRecords;
	int specialty;     //специальность (1-терапевт, 2-кардиолог, 3-стоматолог)
	int qualification; //квалификация (0-высшая, 1-первая, 2-вторая)
	char* fio;         //фио врача
};

int Polyclinic::totalRecords = 0; //Инициализация static переменной выше  это сделать нельзя

//протоипы функций
Polyclinic* addNewRecord(Polyclinic*, Polyclinic); //выделение памяти под новую структу и заполнение ее данными
void enterNewRecord(Polyclinic&); //ввод новой записи в единичную структуру потом она будет записана в addNewRecord()
void showAllRecords(Polyclinic*); //показывает все записи в памяти
void exitAndSave(Polyclinic*); // запись базы из памяти на диск при выходе
Polyclinic* loadRecords(Polyclinic*); //загрузка данных из файлаб вывод их на экран и загрузка в пямять для работы
void qualificationFilter(Polyclinic*);  //фильтрует вывод по квалификации
void specialtyFilter(Polyclinic*); //фильтрует вывод по специальности
void showRecordHeader(const char*); //вспомогательная функция - печать заголовка таблицы
void showOneRecord(Polyclinic, int); //вспомогательная функция - печать одной записи
int menu(); //Меню функций базы


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int choice;

	Polyclinic* myPolyclinic = nullptr;
	Polyclinic temp;
	temp.fio = new char[FIO_WITH];

	myPolyclinic = loadRecords(myPolyclinic);

	do {
		choice = menu(); // реализация меню

		switch (choice)
		{
		case 1:
			enterNewRecord(temp);
			myPolyclinic = addNewRecord(myPolyclinic, temp);
			break;
		case 2:
			showAllRecords(myPolyclinic);
			break;
		case 3:
			qualificationFilter(myPolyclinic);
			break;
		case 4:
			specialtyFilter(myPolyclinic);
			break;

		default:
			break;
		}

	} while (choice != 0);

	exitAndSave(myPolyclinic); // запись базы из памяти на диск при выходе

	delete[] temp.fio;
} // main()

void qualificationFilter(Polyclinic* P)  //Фильтрует вывод по квалификации
{ 
	int qual;

	do
	{
		cout << "=====================================================" << endl;
		cout << "Код квалификации (0-высшая, 1-первая, 2-вторая): ";
		cin >> qual;
		cin.get();
	} while (qual < 0 || qual > 2);
	cout << "=====================================================" << endl;

	showRecordHeader("Фильтр по квалификации:");

	for (int i = 0; i < P->totalRecords; ++i) {
		if (qual == P[i].qualification) {
			showOneRecord(P[i], i + 1);
		}
	} //for
	cout << "===================================================================" << endl;
} //qualificationFilter()

void specialtyFilter(Polyclinic* P)  //Фильтрует вывод по специальности
{
	int spec;

	do
	{
		cout << "=====================================================" << endl;
		cout << "Код специальности (1-терапевт, 2-кардиолог, 3-стоматолог): ";
		cin >> spec;
		cin.get();
	} while (spec < 0 || spec > 2);
	cout << "=====================================================" << endl;

	showRecordHeader("Фильтр по специальности:");

	for (int i = 0; i < P->totalRecords; ++i) {
		if (spec == P[i].specialty) {
			showOneRecord(P[i], i + 1);
		}
	} //for
	cout << "===================================================================" << endl;
} //qualificationFilter()

void exitAndSave(Polyclinic* P) // запись базы из памяти на диск при выходе
{
	FILE* FP = fopen(BASE_NAME, "w");

	for (int i = 0; i < P->totalRecords; ++i) {
		fprintf(FP, "%s\n%d\n%d\n", P[i].fio, P[i].specialty, P[i].qualification);
	}

	if (FP != NULL)
	{
		fclose(FP);
	}
} //exitAndSave()

Polyclinic* loadRecords(Polyclinic* P) //загрузка данных из файлаб вывод их на экран и загрузка в пямять для работы
{
	char name[FIO_WITH];
	int s, q;
	Polyclinic T;
	T.fio = new char[FIO_WITH];

	FILE* FP = fopen(BASE_NAME, "r");

	if (FP != NULL) {
		cout << endl << "+++++++++++++++++++++++++++++++++++++" << endl;
		cout << "Загрузка текущей базы:" << endl;
		while (fscanf(FP, "%99[^\n]%d\n%d\n", T.fio, &T.specialty, &T.qualification) != EOF) { //
			cout << T.fio << " " << T.specialty << " " << T.qualification << endl;
			P = addNewRecord(P, T);
		}
		cout << endl << "+++++++++++++++++++++++++++++++++++++" << endl;
		fclose(FP);
	}

	return P;
} //loadRecords()

void showAllRecords(Polyclinic* P) //показывает все записи в памяти

{
	showRecordHeader("Все записи в базе поликлиники:");

	for (int i = 0; i < P->totalRecords; ++i) {
		showOneRecord(P[i], i+1);
	} //for
	cout << "===================================================================" << endl;
}// showAllRecords()

void enterNewRecord(Polyclinic& newRecord) //ввод новой записи в единичную структуру
{
	char buffer[48];

	cout << "==================================" << endl;
	cout << "Добавление новой записи:" << endl;
	cout << "==================================" << endl;

	cout << "ФИО врача: ";
	cin.getline(buffer, FIO_WITH);
	newRecord.fio = new char[strlen(buffer) + 1];
	strcpy_s(newRecord.fio, strlen(buffer) + 1, buffer);

	do
	{
		cout << "Cпециальность (1-терапевт, 2-кардиолог, 3-стоматолог): ";
		cin >> newRecord.specialty;
		cin.get();
	} while (newRecord.specialty < 1 || newRecord.specialty > 3);

	do
	{
		cout << "Квалификация (0-высшая, 1-первая, 2-вторая): ";
		cin >> newRecord.qualification;
		cin.get();
	} while (newRecord.qualification < 0 || newRecord.qualification > 2);
} // enterNewRecord()

Polyclinic* addNewRecord(Polyclinic* P, Polyclinic newRecord) //выделение памяти под новую структу и заполнение ее данными
{
	int amount = P->totalRecords;

	if (amount == 0)
	{
		P = new Polyclinic[amount + 1]; // выделение памяти для первой структуры
	}
	else
	{
		Polyclinic* tempObj = new Polyclinic[amount + 1];

		for (int i = 0; i < amount; i++)
		{
			tempObj[i] = P[i]; // копируем во временный объект
		}
		delete[] P;

		P = tempObj;
	}


	P[amount].specialty = newRecord.specialty;
	P[amount].qualification = newRecord.qualification;
	P[amount].fio = new char[strlen(newRecord.fio) + 1];
	strcpy_s(P[amount].fio, strlen(newRecord.fio) + 1, newRecord.fio);

	P->totalRecords++;

	return P;
} // addNewRecord()

int menu() //Меню функций базы
{
	int select;
	cout << "==================================" << endl;
	cout << "Меню:" << endl;
	cout << "==================================" << endl;
	cout << "1 - Добавить новую запись" << endl;
	cout << "2 - Показать все записи" << endl;
	cout << "3 - Фильтр по квалификации" << endl;
	cout << "4 - Фильтр по специальности" << endl;
	cout << "0 - Выход из программы и запись" << endl;
	cout << "----------------------------------" << endl;

	do
	{
		cout << "Введите номер пункта меню: ";
		cin >> select;
		cin.get();
	} while (select < 0 || select > 4);

	return select;
} // menu()

void showRecordHeader(const char* title) //вспомогательная функция - печать заголовка таблицы
{
	cout << endl << "===================================================================" << endl;
	cout << title;
	cout << endl << "===================================================================" << endl;
	cout.setf(ios::left);
	cout.width(6);
	cout << " №";
	cout.setf(ios::left);
	cout.width(FIO_WITH);
	cout << "ФИО врача";
	cout.setf(ios::left);
	cout.width(16);
	cout << "Cпециальность";
	cout.setf(ios::left);
	cout.width(16);
	cout << "Квалификация";
	cout << endl << "===================================================================" << endl;
}

void showOneRecord(Polyclinic Record, int number = 1) //вспомогательная функция - печать одной записи
{
	cout.setf(ios::left);
	cout << " ";
	cout.width(5);
	cout << number;
	cout.setf(ios::left);
	cout.width(FIO_WITH);
	cout << Record.fio;
	cout.setf(ios::left);
	cout.width(16);
	cout << Record.specialty;
	cout.setf(ios::left);
	cout.width(16);
	cout << Record.qualification;
	cout << endl;
}