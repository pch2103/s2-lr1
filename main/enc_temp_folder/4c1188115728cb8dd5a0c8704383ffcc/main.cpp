#include <iostream>
#include <cstring>
#include <Windows.h>

#pragma warning (disable : 4996) //��� ����� �������� �� fopen � strcpy https://stackoverflow.com/questions/14386/fopen-deprecated-warning

#define FIO_WITH 32
#define BASE_NAME "polyclinicBase.txt"

using namespace std;

struct Polyclinic
{
	static int totalRecords;
	int specialty;     //������������� (1-��������, 2-���������, 3-����������)
	int qualification; //������������ (0-������, 1-������, 2-������)
	char* fio;         //��� �����
};

int Polyclinic::totalRecords = 0; //������������� static ���������� ����  ��� ������� ������

//�������� �������
Polyclinic* addNewRecord(Polyclinic*, Polyclinic); //��������� ������ ��� ����� ������� � ���������� �� �������
void enterNewRecord(Polyclinic&); //���� ����� ������ � ��������� ��������� ����� ��� ����� �������� � addNewRecord()
void showAllRecords(Polyclinic*); //���������� ��� ������ � ������
void exitAndSave(Polyclinic*); // ������ ���� �� ������ �� ���� ��� ������
Polyclinic* loadRecords(Polyclinic*); //�������� ������ �� ������ ����� �� �� ����� � �������� � ������ ��� ������
int menu(); //���� ������� ����


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
		choice = menu(); // ���������� ����

		switch (choice)
		{
		case 1:
			enterNewRecord(temp);
			myPolyclinic = addNewRecord(myPolyclinic, temp);
			break;
		case 2:
			showAllRecords(myPolyclinic);
			break;

		default:
			break;
		}

	} while (choice != 0);

	exitAndSave(myPolyclinic); // ������ ���� �� ������ �� ���� ��� ������

	delete[] temp.fio;
} // main()

void exitAndSave(Polyclinic* P) // ������ ���� �� ������ �� ���� ��� ������
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

Polyclinic* loadRecords(Polyclinic* P) //�������� ������ �� ������ ����� �� �� ����� � �������� � ������ ��� ������
{
	char name[FIO_WITH];
	int s, q;
	Polyclinic T;
	T.fio = new char[FIO_WITH];

	FILE* FP = fopen(BASE_NAME, "r");

	if (FP != NULL) {
		cout << endl << "+++++++++++++++++++++++++++++++++++++" << endl;
		cout << "�������� ������� ����:" << endl;
		while (fscanf(FP, "%99[^\n]%d\n%d\n", T.fio, &T.specialty, &T.qualification) != EOF) { //
			cout << T.fio << " " << T.specialty << " " << T.qualification << endl;
			P = addNewRecord(P, T);
		}
		cout << endl << "+++++++++++++++++++++++++++++++++++++" << endl;
		fclose(FP);
	}

	return P;
} //loadRecords()

void showAllRecords(Polyclinic* P) //���������� ��� ������ � ������

{
	cout << endl << "===================================================================" << endl;
	cout << "��� ������ � ���� �����������:";
	cout << endl << "===================================================================" << endl;
	cout.setf(ios::left);
	cout.width(6);
	cout << " �";
	cout.setf(ios::left);
	cout.width(FIO_WITH);
	cout << "��� �����";
	cout.setf(ios::left);
	cout.width(16);
	cout << "C������������";
	cout.setf(ios::left);
	cout.width(16);
	cout << "������������";
	cout << endl << "===================================================================" << endl;
	for (int i = 0; i < P->totalRecords; ++i) {
		cout.setf(ios::left);
		cout << " ";
		cout.width(5);
		cout << i + 1;
		cout.setf(ios::left);
		cout.width(FIO_WITH);
		cout << P[i].fio;
		cout.setf(ios::left);
		cout.width(16);
		cout << P[i].specialty;
		cout.setf(ios::left);
		cout.width(16);
		cout << P[i].qualification;
		cout << endl;
	}
	cout << "===================================================================" << endl;
}// showAllRecords()

void enterNewRecord(Polyclinic& newRecord) //���� ����� ������ � ��������� ���������
{
	char buffer[48];

	cout << "==================================" << endl;
	cout << "���������� ����� ������:" << endl;
	cout << "==================================" << endl;

	cout << "��� �����: ";
	cin.getline(buffer, FIO_WITH);
	newRecord.fio = new char[strlen(buffer) + 1];
	strcpy_s(newRecord.fio, strlen(buffer) + 1, buffer);

	do
	{
		cout << "C������������ (1-��������, 2-���������, 3-����������): ";
		cin >> newRecord.specialty;
		cin.get();
	} while (newRecord.specialty < 1 || newRecord.specialty > 3);

	do
	{
		cout << "������������ (0-������, 1-������, 2-������): ";
		cin >> newRecord.qualification;
		cin.get();
	} while (newRecord.qualification < 0 || newRecord.qualification > 2);
} // enterNewRecord()

Polyclinic* addNewRecord(Polyclinic* P, Polyclinic newRecord) //��������� ������ ��� ����� ������� � ���������� �� �������
{
	int amount = P->totalRecords;

	if (amount == 0)
	{
		P = new Polyclinic[amount + 1]; // ��������� ������ ��� ������ ���������
	}
	else
	{
		Polyclinic* tempObj = new Polyclinic[amount + 1];

		for (int i = 0; i < amount; i++)
		{
			tempObj[i] = P[i]; // �������� �� ��������� ������
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

int menu() //���� ������� ����
{
	int select;
	cout << "==================================" << endl;
	cout << "����:" << endl;
	cout << "==================================" << endl;
	cout << "1 - �������� ����� ������" << endl;
	cout << "2 - �������� ��� ������" << endl;
	cout << "0 - ����� �� ���������" << endl;
	cout << "----------------------------------" << endl;

	do
	{
		cout << "������� �����: ";
		cin >> select;
		cin.get();
	} while (select < 0 || select > 2);

	return select;
} // menu()