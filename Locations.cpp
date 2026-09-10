//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <float.h>
#pragma hdrstop
#include "TableLoader.h"
#include "Headers.h"
#include "Parser_code.h"
//---------------------------------------------------------------------------

void __fastcall TForm1::SwapCoordClick(TObject *Sender)
{
	if (List->Row < 0)
		return;
	String base = List->Cells[CHEADER][List->Row];
	bool isCell = false;
	if (base == L"CELL")
		isCell = true;
	else if (base == L"PGRD")
		isCell = false;
	else
		return ShowMessage(L"Нужно выделить ячейку CELL или PGRD");
	if (EFinds->Text.Length() < 2)
		return ShowMessage(L"Поле ввода должно содержать координаты X Y Z через пробел из редактора");
	Coord6 in;
	if (in.TextToFloat3(EFinds->Text))
		return ShowMessage(L"Поле ввода должно содержать координаты X Y Z");
	// Красивый и быстрый вывод начальных координат
	Out->Lines->Add(Format(L"Входные: %g, %g, %g", ARRAYOFCONST((in.x, in.y, in.z))));
	// Кэшируем количество строк, чтобы VCL не пересчитывал RowCount в цикле
	int list2RowCount = List2->RowCount;
	bool isLogNeeded = Wordwrap->Checked;

	if (isCell)
	{
		float rawXYZ[3]; // Массив для безопасного чтения/записи без оверхеда структур
		float xm1 = in.x - 1.0f; float xp1 = in.x + 1.0f;
		float ym1 = in.y - 1.0f; float yp1 = in.y + 1.0f;
		float zm1 = in.z - 1.0f; float zp1 = in.z + 1.0f;

		for (int i = 0; i < list2RowCount; ++i)
		{
			// Оптимизация: сначала проверяем имя тега, чтобы лишний раз не вызывать тяжелый ToInt()
			if (List2->Cells[CHEADER][i] == L"DATA")
			{
				int cSize = List2->Cells[CSIZE][i].ToIntDef(0);
				if (cSize == 24)
				{
					int adr = List2->Cells[CSTART][i].ToIntDef(0);
					fseek(file, adr + 8, SEEK_SET);
					fread(rawXYZ, sizeof(float), 3, file);
					// Манипуляции с координатами X
					if (rawXYZ[0] > xm1 && rawXYZ[0] < xp1) rawXYZ[0] = 0.0f;
					else rawXYZ[0] = Check999(rawXYZ[0] - in.x);
					// Манипуляции с координатами Y
					if (rawXYZ[1] > ym1 && rawXYZ[1] < yp1) rawXYZ[1] = 0.0f;
					else rawXYZ[1] = Check999(rawXYZ[1] - in.y);
					// Манипуляции с координатами Z
					if (rawXYZ[2] > zm1 && rawXYZ[2] < zp1) rawXYZ[2] = 0.0f;
					else rawXYZ[2] = Check999(rawXYZ[2] - in.z);
					if (isLogNeeded)
						Out->Lines->Add(IntToStr(adr) + L":" + Format(L"\t%g\t%g\t%g", ARRAYOFCONST((rawXYZ[0], rawXYZ[1], rawXYZ[2]))));
					fseek(file, adr + 8, SEEK_SET); // Используем абсолютное смещение вместо ненадежного SEEK_CUR
					fwrite(rawXYZ, sizeof(float), 3, file);
				}
			}
		}
	}
	else
	{
		int xyzpop[3] = { static_cast<int>(in.x), static_cast<int>(in.y), static_cast<int>(in.z) };
		int xyzh[3]; // Используем размер 3, так как читаем ровно 3 int (X, Y, Z).

		for (int i = 0; i < list2RowCount; ++i)
		{
			if (List2->Cells[CHEADER][i] == L"PGRP")
			{
				int adr = List2->Cells[CSTART][i].ToIntDef(0);
				int size = List2->Cells[CSIZE][i].ToIntDef(0);
				int end = adr + 8 + size;
				// Позиционируемся на начало блока данных ВСЕГО ОДИН РАЗ перед циклом
				fseek(file, adr + 8, SEEK_SET);
				for (int p = adr + 8; p < end; p += 16)
				{
					// fseek(file, p, SEEK_SET); -> УДАЛЕНО! Поток файла сдвигается сам при fread/fwrite
					fread(xyzh, sizeof(int), 3, file);
					xyzh[0] = GetOkrugl(xyzh[0] - xyzpop[0]);
					xyzh[1] = GetOkrugl(xyzh[1] - xyzpop[1]);
					xyzh[2] = GetOkrugl(xyzh[2] - xyzpop[2]);
					if (isLogNeeded)
						Out->Lines->Add(IntToStr(p) + L" To " + IntToStr(xyzh[0]) + L"*" + IntToStr(xyzh[1]) + L"*" + IntToStr(xyzh[2]));
					// Возвращаемся ровно на 12 байт назад (3 прочитанных int по 4 байта), чтобы перезаписать их
					fseek(file, -12, SEEK_CUR);
					fwrite(xyzh, sizeof(int), 3, file);
					// Пропускаем оставшиеся 4 байта структуры шага сетки (16 байт шаг минус 12 байт записанных данных)
					fseek(file, 4, SEEK_CUR);
				}
			}
		}
	}
	EFinds->SelectAll();
	EFinds->SetFocus();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
