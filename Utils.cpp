//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <float.h>
//#include <cmath> // Для функций std::roundf и std::fabsf
#include <math.h> // Подключаем классический заголовок Си вместо <cmath>
#pragma hdrstop
#include "TableLoader.h"
#include "Headers.h"
#include "Parser_code.h"

bool TForm1::RowComparator::operator()(const RowData& a, const RowData& b) const
{
	if (isString)
	{
		if (isAscending)
			return a.cells[col] < b.cells[col];
		else
			return a.cells[col] > b.cells[col];
	} else
	{
		int valA = a.cells[col].ToIntDef(0);
		int valB = b.cells[col].ToIntDef(0);

		if (isAscending)
			return valA < valB;
		else
			return valA > valB;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExportSPELClick(TObject *Sender)
{
	if (List->Row == -1)	return;
	Export = new TStringList;
	Export->Append("id	name	type	cost	flags	Effect1	Effect2	Range	Area	Time	Min	Max");
	LogUp = false;
	for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
	{
		int go = List->Cells[CSTART][i].ToInt();
		fseek(file, go, SEEK_SET);
		SPELreadClick(Sender);
	}
	Export->SaveToFile("EXPORT.txt");
	ShowMessage("Saved EXPORT.txt");
	List->Row = List->Row;
}
//---------------------------------------------------------------------------
// Ультра-быстрая проверка близости к сетке на битовых масках
static inline bool TrySnapBitwise(int x, int step, int tolerance, int& result)
{
    // step - это степень двойки (например, 512, 256).
    // Маска для получения остатка от деления: (step - 1)
	int mask = step - 1;
	int remainder = x & mask; // Заменяет операцию (x % step)
    int minVal = x & ~mask;   // Заменяет операцию ((x / step) * step)

	if (remainder < tolerance)
	{
		result = minVal;
        return true;
    }
	if ((step - remainder) < tolerance)
	{
        result = minVal + step;
        return true;
    }
    return false;
}

int TForm1::GetOkrugl(int x)
{
    int absX = (x < 0) ? -x : x;
	int result = 0;

    // Проверяем каскад адаптивных сеток с помощью побитовой логики
    if (TrySnapBitwise(absX, 512, 32, result)) goto sub_return;
    if (TrySnapBitwise(absX, 256, 28, result)) goto sub_return;
    if (TrySnapBitwise(absX, 128, 20, result)) goto sub_return;

    // Финальная сетка 32 (обычное округление к ближайшей степени)
    {
        int remainder = absX & 31;  // absX % 32
        int minVal = absX & ~31;    // (absX / 32) * 32
        result = (remainder <= 16) ? minVal : (minVal + 32);
    }

sub_return:
    return (x < 0) ? -result : result;
}
//---------------------------------------------------------------------------

float TForm1::Check999(float x)
{
    // Классическая формула округления к ближайшему целому (аналог round)
    // Она корректно работает как для положительных, так и для отрицательных чисел
	float rounded = (x >= 0.0f) ? floor(x + 0.5f) : ceil(x - 0.5f);
    // Используем глобальную функцию fabs для вычисления модуля разности.
    // Если расстояние до целого числа меньше 0.02, возвращаем округленное значение.
	if (::fabs(x - rounded) < 0.02)
		return rounded;
	return x;
}
//---------------------------------------------------------------------------
void TForm1::RotateBooks()
{
	TStringList *what;
	if (what == NULL)
	{
		what = new TStringList;
		what->LoadFromFile("torotate.txt");
		what->Sorted = true;
	}
	bool finded = false;
	float Data[3];
	float &x = Data[0];
	float &y = Data[1];
	float &z = Data[2];
	int Length;
	bool CanSel = false;
	if (List->Row <= -1)
		return;
	for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
	{
		ListSelectCell(NULL, 0, i, CanSel);
		int count = 0;
		for (int j = 4; j < List2->RowCount; ++j)
			if (finded)
			{
				if (List2->Cells[CHEADER][j] == "DATA")
				{
					finded = false;
					int Offset = List2->Cells[CSTART][j].ToInt();
					fseek(file, Offset + 4, SEEK_SET);
					int Length;
					fread(&Length, 4, 1, file);
					fseek(file, 12, SEEK_CUR);
					fread(Data, 4, 3, file);
					x = x * 180.0 / 3.14159265358979;
					y = y * 180.0 / 3.14159265358979;
					z = z * 180.0 / 3.14159265358979;
					if (equ(x,0) && equ(y,180)) //flat
						y=0; //z=180-z;
					else if (equ(x,180) && equ(y,0)) //flat
						x=0; //z=180-z;
					else if (equ(y,270) && x != 0) //flat
					{
						z = z + x;
						x = 0; //z=180-z;
					}
					else if (equ(y,90) && x != 0) //flat
					{
						z = 360 - x + z;
						x = 0;
						//z=180-z;
					}
					if (x < 0) x+=360;
					if (y < 0) y+=360;
					if (z < 0) z+=360;
					if (x >= 360) x-=360;
					if (y >= 360) y-=360;
					if (z >= 360) z-=360;
					if (x >= 359) x=0;
					if (y >= 359) y=0;
					if (z >= 359) z=0;
					x = x * 3.14159265358979 / 180.0;
					y = y * 3.14159265358979 / 180.0;
					z = z * 3.14159265358979 / 180.0;
					fseek(file, -12, SEEK_CUR);
					fwrite(Data, sizeof(float), 3, file);
					count++;
					//Sadrith Mora, Telvanni Council House, Entry
				}
			}
			else
			{
				if (List2->Cells[CHEADER][j] == "NAME")
					if (what->IndexOf(List2->Cells[CDATA2][j]) != -1)
						finded = true;
			}
		if (count > 0)
			tolog(IntToStr(count)+"\t"+List->Cells[CDATA][i]);
	}
}
//---------------------------------------------------------------------------
