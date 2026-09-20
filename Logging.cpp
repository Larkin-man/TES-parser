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

void TForm1::tolog(String msg)
{
	Out->Lines->Add(msg);
}
void TForm1::tologi(String msg, int i)
{
	Out->Lines->Add(msg+"="+IntToStr(i));
}
void TForm1::ToLogLen(String msg, int Len)
{
	if (LogUp) {
		if (msg.Length() > Len)
			Out->Lines->Add(msg.SetLength(Univ.Length));
		else
			Out->Lines->Add(msg); }
}
void TForm1::ToLog(String msg, const char *param)
{
	if (LogUp) {
	if (param)
	{
		msg = "="+msg;
		Out->Lines->Add(param+msg);
	}
	else
		Out->Lines->Add(msg); }
}
void TForm1::ToLogS(String msg, String param)
{
	if (LogUp)
	if (param.IsEmpty() == false)
	{
		msg = "="+msg;
		Out->Lines->Add(param+msg);
	}
	else
	Out->Lines->Add(msg);
}
void TForm1::char4ToLog(char* msg, const char *param)
{
	if (LogUp)
		ToLog(String(msg).SetLength(4), param);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExportBtnClick(TObject *Sender)
{
	if (List->Row == -1)
		return ShowMessage(L"No one selected"); // Используем Юникод-префикс L

	// Названия окон для Unicode-окружения RAD Studio
	String expoTitle = (List->Selection.Top == 0 && List->Selection.Bottom == List->RowCount - 1)
					   ? L"Export all" : L"Export";
	bool expAll = (List->Selection.Top == 0 && List->Selection.Bottom == List->RowCount - 1);

	int type = ID_NO;
	if (List->Selection.Top != List->Selection.Bottom)
	{
		// Используем стандартный MessageBox без суффикса 'A' для поддержки Юникода
		type = Application->MessageBox(L"Export subheaders to string?", expoTitle.c_str(), MB_YESNOCANCEL);
		if (type == ID_CANCEL) return;
	}

	int expTab = ID_NO;
	if (expAll && type == ID_YES)
	{
		expTab = Application->MessageBox(L"Export only table?", expoTitle.c_str(), MB_YESNOCANCEL);
		if (expTab == ID_CANCEL) return;
	}
	TStringList* exportList = new TStringList();
	ExportBtn->Tag = 1;

	try
	{
		// СЦЕНАРИЙ 1: Экспорт только главной таблицы
		if (expTab == ID_YES)
		{
			exportList->Append(L"Header\tOffset\tSize\tData");
			for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
			{
				exportList->Append(List->Cells[CHEADER][i] + L"\t" +
								   List->Cells[CSTART][i]  + L"\t" +
								   List->Cells[CSIZE][i]   + L"\t" +
								   List->Cells[CDATA][i]);
			}
			exportList->SaveToFile(PluginName + L".txt");
			ShowMessage(L"Saved: " + PluginName + L".txt");

			delete exportList; // Освобождаем память
			ExportBtn->Tag = 0;
			return;
		}

		// Опрашиваем опции для детального экспорта подзаписей
		int expOff = Application->MessageBox(L"Export Offset?", expoTitle.c_str(), MB_YESNOCANCEL);
		if (expOff == ID_CANCEL) { delete exportList; ExportBtn->Tag = 0; return; }

		int expSize = Application->MessageBox(L"Export Size?", expoTitle.c_str(), MB_YESNOCANCEL);
		if (expSize == ID_CANCEL) { delete exportList; ExportBtn->Tag = 0; return; }

		LogUp = false;
		bool stup = true;
		bool hasOffset = (expOff == ID_YES);
		bool hasSize = (expSize == ID_YES);

		// СЦЕНАРИЙ 2: Ровная таблица (type == ID_NO)
		if (type == ID_NO)
		{
			// Формируем шапку один раз на основе флагов без лесенки if-else
			String headerStr = L"№\tHeader\tName\tSubheader";
			if (hasOffset) headerStr += L"\tOffset";
			if (hasSize)   headerStr += L"\tSize";
			headerStr += L"\tType\tData";
			exportList->Append(headerStr);

			int num = 1;
			int totalLinesInFile = 1;
			int fileIndex = 1;

			for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
			{
				String head = IntToStr(num) + L"\t" + List->Cells[CHEADER][i] + L"\t" + List->Cells[CDATA][i] + L"\t";

				// Вызываем функцию формирования строк во втором гриде (List2)
				ListSelectCell(Sender, 0, i, stup);

				int list2Rows = List2->RowCount;
				for (int j = 0; j < list2Rows; ++j)
				{
					// Собираем строку подзаписи динамически
					String rowStr = head + List2->Cells[0][j]; // Subheader
					if (hasOffset) rowStr += L"\t" + List2->Cells[1][j];
					if (hasSize)   rowStr += L"\t" + List2->Cells[2][j];

					rowStr += L"\t" + List2->Cells[3][j] + L"\t" + List2->Cells[CDATA2][j];
					exportList->Append(rowStr);
				}

				num++;
				totalLinesInFile += list2Rows;

				// Если накопилось больше 50 000 строк — сбрасываем в файл-чанк
				if (totalLinesInFile >= 50000)
				{
					String partName = PluginName + L"_part_" + IntToStr(fileIndex++) + L".txt";
					exportList->SaveToFile(partName);
					exportList->Clear();
					exportList->Append(headerStr); // возвращаем шапку в новый файл
					tolog(L"Saved partial: " + partName);
					totalLinesInFile = 1;
				}
			}
		}
		// СЦЕНАРИЙ 3: Экспорт в одну строчку (Subheaders в строку)
		else
		{
			String headerStr = L"Header\tName";
			if (hasOffset && hasSize)  headerStr += L"\tSubheader[Offset]{Size}";
			else if (hasOffset)		headerStr += L"\tSubheader[Offset]";
			else if (hasSize)		  headerStr += L"\tSubheader{Size}";
			else					   headerStr += L"\tSubheader";
			headerStr += L"\tData";
			exportList->Append(headerStr);

			for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
			{
				String mainRowStr = List->Cells[CHEADER][i] + L"\t" + List->Cells[CDATA][i];
				ListSelectCell(Sender, 0, i, stup);

				int list2Rows = List2->RowCount;
				for (int j = 0; j < list2Rows; ++j)
				{
					mainRowStr += L"\t" + List2->Cells[0][j];
					if (hasOffset) mainRowStr += L"[" + List2->Cells[1][j] + L"]";
					if (hasSize)   mainRowStr += L"{" + List2->Cells[2][j] + L"}";
					mainRowStr += L"\t" + List2->Cells[CDATA2][j];
				}
				exportList->Append(mainRowStr);
			}
		}

		// Сохраняем финальный файл, только если в буфере что-то осталось (защита от перезаписи пустотой)
		if (exportList->Count > 1)
		{
			exportList->SaveToFile(PluginName + L".txt");
			ShowMessage(L"Saved: " + PluginName + L".txt");
		}
	}
	catch (...)
	{
		ShowMessage(L"Критическая ошибка во время экспорта!");
	}

	// ГАРАНТИРОВАННОЕ очищение ресурсов
	LogUp = true;
	delete exportList;
	ExportBtn->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TES3ReadClick(TObject *Sender)
{	//(TES3 + HEDR ++?)
	Tes3Header hTes3;
	if (ftell(file) > 0)
		fseek(file, 0, SEEK_SET);
	fread(&hTes3, 324, 1, file);
	char4ToLog(hTes3.Name);
	ToLog(hTes3.i[0],"i[0]");
	ToLog(hTes3.i[1],"i[1]");
	ToLog(hTes3.i[2],"i[2]");
	char4ToLog(hTes3.Header);
	ToLog(hTes3.HeaderSize, "HEDR size");
	ToLog(hTes3.Version_Number,"Version number");
	ToLog(hTes3.Unknown,"Unknown");
	ToLog(hTes3.Author_Name,"Author");
	ToLog(hTes3.Description,"Description"); //*/
	ToLog(hTes3.NumRecords,"Num Records");
	if (save)
		fwrite(&hTes3, sizeof(hTes3), 1, save);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GMDTReadClick(TObject *Sender)
{
	char 	Name[5];	Name[4] = '\0';
	int 	Len;
	float Cord[6];
	char whe[68];
	char Player[32];
	fseek(file, ToE->Text.ToIntDef(0), SEEK_SET);
	fread(Name, 4, 1, file);
	fread(&Len, SLENSIZE, 1, file);
	fread(Cord, 4, 6, file);
	fread(whe, 1, 68, file);
	fread(Player, 1, 32, file);
	ToLog(String(Name)+"("+IntToStr(Len)+")");
	for (int i = 0; i < 6; ++i)
		ToLog(Cord[i]);
	ToLog(whe);
	ToLog(Player);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::rplusbClick(TObject *Sender)
{
	if (file)
	{
		ToLog("file->curp",reinterpret_cast<const char*>(file->curp));
		ToLog("file->buffer",reinterpret_cast<const char*>(file->buffer));
		ToLog(file->level,"level");
		ToLog(file->bsize,"bsize");
		ToLog(file->istemp,"bsize");
		ToLog(file->flags,"flags");
		ToLog(file->hold,"hold");
		ToLog(file->fd,"fd");
		ToLog(file->token,"token");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SPLMreadClick(TObject *Sender)
{
	//Go16Click(Sender);
	char Name[4];
	int Length;
	String Data;
	struct SPDT //вроде это спелл который чтото дает
	{
		int i1; //int
		char str[32]; //enchant or spell
		float a; //tochno float
		float b;
		float hz;
		int in; //int
		char str2[32]; //na kogo
		char str3[32]; //kakoi predmet eto daet
		float fl;
		int nuls[10];

	} s;
	struct NPDT //это эффекты которые дает тот спелл
	{
		char who[32]; //na kogo
		int a; //0 для первого нпдт, 1 для второго итд
		int b;
		int c; //сила спелла
		float fl; //вроде одно для всех нпдт
		int end[2];

	} n;
	Out->Lines->BeginUpdate();
	int Coun[5];
	for (int i = 0; i < 5; ++i)
		Coun[i] = 0;
	ToLog("------------------->"+List2->Cells[CSTART][List2->Row]);
	for (int i = List2->Row; i < List2->RowCount; ++i)
	{
		fseek(file, List2->Cells[CSTART][i].ToInt(), SEEK_SET);
		fread(Name, 4, 1, file);
		fread(&Length, 4, 1, file);
		unsigned char *st = file->curp;
		if (strncmp(Name, "NAME", 4) == 0)
		{
			Data = IntToStr(*(int*)&st[0]);
			ToLogS(Data, "NAME");
			Coun[0]++;
		}
		else if (strncmp(Name, "NAM0", 4) == 0)
		{
			Data = IntToStr(Byte(st[0]));
			//List2->Cells[CDATA2][i] = Data;
			ToLogS(Data, "NAM0");
			Coun[1]++;
		}
		else if (strncmp(Name, "XNAM", 4) == 0)
		{
			Data = IntToStr(Byte(st[0]));
			//List2->Cells[CDATA2][i] = Data;
			ToLogS(Data, "XNAM");
			Coun[2]++;
			break;
		}
		else if (strncmp(Name, "SPDT", 4) == 0)
		{
			fread(&s, 160, 1, file);
			ToLog("SPDT");
			ToLog(s.i1);
			ToLog(s.str);
			ToLog(s.a); ToLog(s.b);
			ToLog(s.hz);
			ToLog(s.in);
			ToLog(s.str2);
			ToLog(s.str3);
			ToLog(s.fl);
			for (int un = 0; un < 10; ++un)
				ToLog(s.nuls[un]);
			Coun[3]++;
			List2->Cells[CDATA2][i] = s.str;
		}
		else if (strncmp(Name, "NPDT", 4) == 0)
		{
			fread(&n, 56, 1, file);
			ToLog("NPDT");
			ToLog(n.who);
			//ToLog(n.vozms[0]); ToLog(n.vozms[1]);
			ToLog(n.a, "№");
			ToLog(n.b);
			ToLog(n.c, "Strength");
			ToLog(n.fl, "Time");
			ToLog(n.end[0]); ToLog(n.end[1]);
			Coun[4]++;
		}
		else if (strncmp(Name, "TNAM", 4) == 0)
		{
			ToLogS((char*)st, "TNAM");
		}
		else
			return ShowMessage(Name);
		fseek(file, Length, SEEK_CUR);
	}
	//Out->Lines->Add("Count of NAME="+IntToStr(Coun[0])+" NAM0="
	//	+IntToStr(Coun[1])+" XNAM="+IntToStr(Coun[2])+" SPDT="
	//	+IntToStr(Coun[3])+" NPDT="+IntToStr(Coun[4]));
	Out->Lines->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExportScriptsBtnClick(TObject *Sender)
{
	int expAll = ID_YES;
	if (List->Row != -1 || List->Selection.Bottom - List->Selection.Top + 1 != List->RowCount)
		if ( (expAll=Application->MessageBox(L"Export all scripts?", L"Export", MB_YESNOCANCEL))== ID_CANCEL)
			return;
	int i = ( expAll == ID_YES)? 0 : List->Selection.Top;
	int end = ( expAll == ID_YES)? List->RowCount : List->Selection.Bottom + 1;
	int len;
	char 	Name[5];	Name[4] = '\0';
	FILE *scpt = NULL;
	int cap = 1024;
	char *buf = new char[cap];
	for (; i < end; i++)
	{
		if (List->Cells[0][i] == "SCPT")
		{
			int ende = List->Cells[CSTART][i].ToInt();
			fseek(file, ende + MLENTOSLEN, SEEK_SET);
			ende += Sizes[i];
			len = 0;
			do
			{
				fseek(file, len, SEEK_CUR);
				if (ftell(file) > ende)
					break;
				fread(Name, 4, 1, file);
				fread(&len, LENSIZE, 1, file);
			} while (strncmp(Name, "SCTX", 4) != 0);

			if (ftell(file) <= ende)
			{
				if (len > cap)
				{
					cap = len;
					buf = new char[cap];
				}
				scpt = _wfopen((List->Cells[CDATA][i]+".txt").w_str(), L"wb");
				fread(buf, len, 1, file);
				fwrite(buf, len, 1, scpt);
				fclose(scpt);
			}
		}
	}
	delete []buf;
}
//---------------------------------------------------------------------------
