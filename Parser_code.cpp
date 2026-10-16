//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
//#include <float.h>
#include "Headers.h"
#include <cmath> // Для функций std::roundf и std::fabsf
#include "TableLoader.h"
#pragma hdrstop
#include "Parser_code.h"

//---------------------------------------------------------------------------
//#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)	: TForm(Owner)//,TAGSS(6)
{
	Tes3 = true;
	file = NULL;
	save = NULL;//ShowMessage
	Out->Clear();
	Setup(4, 12, 320); //TES3
	for (char c = 'A'; c <= 'Z'; ++c)
		TagSymb << c;
	for (char c = '0'; c <= '9'; ++c)
		TagSymb << c;
	TagSymb << '_';
	if (List->Font->Height >= 0)
		HeaderControl1->Sections->Items[0]->Width += List->Font->Height / 2;
	else
		HeaderControl1->Sections->Items[0]->Width -= List->Font->Height / 2;
	HeaderControl2->Sections->Items[0]->Width = HeaderControl1->Sections->Items[0]->Width;
	for (int i = 0; i < HeaderControl1->Sections->Count; ++i)
	{
		List->ColWidths[i] = HeaderControl1->Sections->Items[i]->Width - 1;
		List2->ColWidths[i] = HeaderControl2->Sections->Items[i]->Width - 1;
	}
	List2->ColWidths[4] = HeaderControl2->Sections->Items[4]->Width - 1;
	OpenDialog1->InitialDir = GetCurrentDir();
	Export = NULL;
	//Out->Lines->Add(sizeof());
	Ready(false);
	CreateTags();
	types.IgnoreFirstString = true;
	types.IgnoreDelimitersPack = false;
	types.LoadFromResource("TEXTRES","ssss",&THeader,&TSubHeader,&TType,&TDescr);
	//for (int i = 0; i < types.RowCount; ++i)
	//	Out->Lines->Add(TDescr[i]);
	cSubIndexes = 32;
	SubIndexes = new int[cSubIndexes];
	Opening = false;
	Expo = NULL;
	Clear();
	//ToLog(sizeof(MHeader::HeaderData));
	localeinstalled = false;
	Univ.Capacity = 0;
	PanelPRO->Visible = ProModeCK->Checked;
	//SortingColumn = CSTART;
	WhatFindedChange(NULL);
	ListEnter(NULL);
	ShowAll = true;
	what = NULL;
	if (List->Font->Height >= 0)
		List->DefaultRowHeight = List->Font->Height + 5;
	else
		List->DefaultRowHeight = -(List->Font->Height - 5);
	if (List2->Font->Height >= 0)
		List2->DefaultRowHeight = List2->Font->Height + 5;
	else
		List2->DefaultRowHeight = -(List2->Font->Height - 5);
	LastSortingColumn = -1;
	AscendingOrder = true;
}
//---------------------------------------------------------------------------
void TForm1::Clear()
{
	// Проходим по всем строкам таблицы и освобождаем метаданные
	// Очищаем весь вектор метаданных. Память освобождается автоматически!
	RecordData.clear();
	for (std::vector<DeleteItem>::iterator el = SubDelete.begin(); el != SubDelete.end(); ++el)
		if (el->Addon)
			delete [] el->Addon;
	Deleted.clear();
	DeletedSize = 0;
	LogUp = true;
	FindIdx = -1;
	SubDelete.clear();
	RefStarts.clear();
	RefEnds.clear();
	Edited.clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (file) { fclose(file); file = NULL; }
	if (save) { fclose(save); save = NULL; }
	// Очищаем весь вектор метаданных. Память освобождается автоматически!
	RecordData.clear();
	delete []SubIndexes;
	if (Expo)
	Expo->SaveToFile("Expo.txt");
	delete Expo;
	delete Export;
	delete what;
	delete [] SecretData;
}
//---------------------------------------------------------------------------
void TForm1::Ready(bool ready)
{
	PanelSubRead->Enabled = false;
	FlowPanel1->Enabled = ready;
	List->Enabled = ready;
	Panel4->Enabled = ready;
	List2->Enabled = ready;
	NEnableList2Delete->Enabled = ready;
	if (ready == false)
	{
		fclose(file);
		file = NULL;
	}
}
//---------------------------------------------------------------------------

void TForm1::Setup(int SubLenSize, int MainLenSize, int CountOffset)
{
	LENSIZE = 4;						//Mo Ob Sk
	MAINLENSIZE = MainLenSize;			//12 16 20
	MOVERLENTOSNAME = MAINLENSIZE - 4;	//8 12 16 main overlen to subname
	SLENSIZE = SubLenSize;				//4 2 2
	MLENTOSLEN = LENSIZE + MAINLENSIZE;	//16 20 24
	MNAMETOSUBLEN = MLENTOSLEN + 4; 	//20 24 28
	SecretLen = MOVERLENTOSNAME / 4;
	if (SecretData)
	{
		delete [] SecretData;
		SecretData = NULL;
	}
	SecretData = new int[SecretLen];
	POSNRECORDS = CountOffset; //Позиция где записано количество записей файла
}
//---------------------------------------------------------------------------
void TForm1::AddRow(char *Header, int Length, long Start)
{
	if (List->RowCount <= AddedRow)
		List->RowCount += 8;

	List->Cells[CHEADER][AddedRow] = Header;
	List->Cells[CSIZE][AddedRow]  = Length;
	List->Cells[CSTART][AddedRow] = Start;

	long calculatedEnd = Start + Length + 4 + MAINLENSIZE;

	// Создаем структуру метаданных
	TRowMetadata meta;
	meta.GridRow  = AddedRow;
	meta.Size	 = Length;
	meta.StartPos = Start;
	meta.EndPos   = calculatedEnd;
	meta.Header = Header;
	// Клади её в конец вектора
	RecordData.push_back(meta);
	// Получаем индекс только что добавленного элемента
	int vectorIndex = RecordData.size() - 1;
	// Сохраняем ИНДЕКС в Objects строки грида (упаковываем int в указатель)
	List->Objects[0][AddedRow] = reinterpret_cast<TObject*>(vectorIndex);
	AddedRow++;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenBtnClick(TObject *Sender)
{
	if (OpenDialog1->Execute() != ID_OK)
		return;

	FILE* tempFile = NULL;
	if (NRewrites->Checked)
	{
		tempFile = _wfopen(OpenDialog1->FileName.w_str(), L"r+b");
		Save->Enabled = true;
	}
	else
	{
		tempFile = _wfopen(OpenDialog1->FileName.w_str(), L"rb");
	}

	if (!tempFile)
	{
		return ShowMessage("Cannot open binary file.");
	}

	fseek(tempFile, 0, SEEK_END);
	long tempEoF = ftell(tempFile);
	if (tempEoF < 48)
	{
		fclose(tempFile);
		return ShowMessage("File is empty.");
	}

	// --- УСПЕШНО ---
	if (file)
	{
		Ready(false); // Закрытие файла должно быть внутри Ready или выполняться явно: fclose(file);
	}

	file = tempFile;
	EoF = tempEoF;

	Clear();

	Save->Enabled = false;
	LDele->Visible = false;

	OpenedFileName = OpenDialog1->FileName;
	PluginName = ExtractFileName(OpenedFileName);
	Form1->Caption = PluginName + " - TES parser";

	Opening = true;
	if (Out->Lines->Count > 2)
		Out->Lines->Add("----------------------");
	Out->Lines->Add(PluginName + ", size=" + IntToStr((int)EoF));

	char	Name[5];	Name[4] = '\0';
	int		Len;
	long	Pos = 0;
	float	Ver = 0.0f;

	fseek(file, 0, SEEK_SET);
	fread(Name, 4, 1, file);
	fread(&Len, 4, 1, file);
	NextS->Caption = "Next=" + String(Name) + "[" + IntToStr(Len) + "]";
	long End;

	if (strncmp(Name, "TES3", 4) == 0)
	{
		Setup(4, 12, 320); // TES3 Morrowind
		fseek(file, MLENTOSLEN, SEEK_CUR);
		fread(&Ver, 4, 1, file);
		ToLog("Elder Scrolls 3 Morrowind file structure.");
		Tes3 = true;
		End = 4 + MAINLENSIZE + Len;

		// Вместо жесткого SEEK_SET возвращаемся на начало подзаписей (байт 16)
		fseek(file, MLENTOSLEN, SEEK_SET);
	}
	else
	{
		Tes3 = false;
		Pos = FindTextInFile(file, "HEDR", 0, 512);
		if (Pos < 0)
		{
			tolog("Unknown file structure(" + String(Name) + ") or HEDR missing.");
			Ready(false);
			return;
		}
		int mls = Pos - 4;

		fseek(file, Pos + 4, SEEK_SET);
		int NewLenSize = 2;
		fread(&Len, 2, 1, file);
		fread(&Ver, 4, 1, file);

		// ИСПРАВЛЕНО: корректный логический оператор ИЛИ (||)
		if (Ver <= 0.0f || Ver > 9000.0f || Len < 4 || Len > 9000)
		{
			NewLenSize = 4;
			fseek(file, Pos + 4, SEEK_SET);
			fread(&Len, 4, 1, file);
			fread(&Ver, 4, 1, file);
			if (Ver <= 0.0f || Ver > 9000.0f || Len < 4 || Len > 9000)
			{
				ToLog("Unknown file type. HEDR pos =" + IntToStr((int)Pos) +" Version="+FloatToStr(Ver));
				Ready(false);
				return;
			}
		}

		// ИСПРАВЛЕНО: Вызываем Setup ДО того, как переменная SLENSIZE потребуется в fseek!
		if (Ver > 0.0f && Ver <= 0.97f)
		{
			ToLog("Fallout 3 file structure.");
			Setup(NewLenSize, mls, Pos + 4 + NewLenSize + 4);
		}
		else if (Ver > 0.97f && Ver <= 1.15f)
		{
			if (mls >= 20)
			{
				ToLog("Fallout 4 file structure.");
				Setup(NewLenSize, mls, Pos + 4 + NewLenSize + 4);
			}
			else
			{
				ToLog("Elder Scrolls 4 Oblivion file structure.");
				Setup(NewLenSize, mls, Pos + 4 + NewLenSize + 4);
			}
		}
		else if (Ver > 1.32f && Ver <= 1.5f)
		{
			ToLog("Fallout New Vegas file structure.");
			Setup(NewLenSize, mls, Pos + 4 + NewLenSize + 4);
		}
		else if (Ver > 1.5f && Ver <= 1.9f)
		{
			ToLog("Elder Scrolls 5 Skyrim file structure.");
			Setup(NewLenSize, mls, Pos + 4 + NewLenSize + 4);
		}
		else
		{
			ToLog("Elder Scrolls ??? file structure. Version =" + FloatToStr(Ver));
			Setup(NewLenSize, mls, Pos + 4 + NewLenSize + 4);
		}

		End = 4 + Len + MAINLENSIZE;
		// Теперь SLENSIZE гарантированно инициализирован правильно
		fseek(file, Pos + 4 + SLENSIZE + Len, SEEK_SET);
	}

	TES3Read->Enabled = Tes3;
	String Req = " ";

	// Безопасное выделение памяти под MAST
	char* tempBuf = new char[64];
	int tempLen = 64;

	while (ftell(file) < End)
	{
		if (fread(Name, 4, 1, file) != 1) break;
		if (fread(&Len, SLENSIZE, 1, file) != 1) break;

		if (strncmp(Name, "MAST", 4) == 0)
		{
			if (Len >= tempLen)
			{
				delete[] tempBuf;
				tempBuf = new char[Len + 1];
				tempLen = Len + 1;
			}
			fread(tempBuf, Len, 1, file);
			tempBuf[Len] = '\0';
			Req += String(tempBuf) + ", ";
		}
		else
		{
			fseek(file, Len, SEEK_CUR);
		}
	}

	if (Req.Length() > 1)
	{
		Req.SetLength(Req.Length() - 2);
		tolog("Required:" + Req);
	}

	delete[] tempBuf;

	fseek(file, POSNRECORDS, SEEK_SET);
	fread(&RecordCount, 4, 1, file);

	if (RecordCount > 0 && RecordCount < 2000000)
	{
		// Выделяем память под весь вектор ОДНИМ блоком заранее!
		RecordData.reserve(RecordCount + 1);
	}

	List->ScrollBars = ssNone;
	List->Perform(WM_SETREDRAW, FALSE, 0);
	List->Row = 0;
	Out->SetFocus();
	List2->RowCount = 1;
	List2->Rows[0]->Clear();

	fseek(file, 0, SEEK_SET);
	AddedRow = 0;

	while ((Pos = ftell(file)) < EoF)
	{
		fread(Name, 4, 1, file);
		fread(&Len, 4, 1, file);

		// Добавляем строку (индекс вектора запишется в Objects автоматически)
		AddRow(Name, Len, Pos);

		fseek(file, MOVERLENTOSNAME, SEEK_CUR);
		if (Tes3 || strncmp(Name, "GRUP", 4) != 0)
		{
			fseek(file, Len, SEEK_CUR);
		}
		else // Это GRUP в TES4+ (корректируем размер)
		{
			List->Cells[CSIZE][AddedRow - 1] = MLENTOSLEN;

			// Достаем индекс из Objects для текущей добавленной строки
			int vIdx = reinterpret_cast<int>(List->Objects[0][AddedRow - 1]);

			// Корректируем данные прямо в векторе по индексу
			RecordData[vIdx].Size = MLENTOSLEN;
			RecordData[vIdx].EndPos = Pos + MLENTOSLEN + 4 + MAINLENSIZE;
		}

		if (Len <= 0 && BreakIf0Len->Checked)
		{
			BreakIf0Len->Checked = false;
			break;
		}
	}

	if (ftell(file) != EoF)
		Out->Lines->Add("End on " + IntToStr((int)ftell(file)) + ". This file may be corrupt.");

	if (List->RowCount > AddedRow)
		List->RowCount = AddedRow;

	if (NShowData->Checked)
	{
		if (List->RowCount >= 1)
			List->Cells[CDATA][0] = "Number of records: " + IntToStr(RecordCount);
		RefreshData(file, 1);
		NShowData->Tag = 1;
	}
	else
	{
		if (NShowData->Tag == 1)
		{
			NShowData->Tag = 0;
			for (int i = 0; i < List->RowCount; i++)
				List->Cells[CDATA][i] = "";
		}
	}

	HeaderControl1Resize(NULL);
	List->Perform(WM_SETREDRAW, TRUE, 0);
	List->ScrollBars = ssVertical;
	List->Invalidate();

	fseek(file, 0, SEEK_SET);
	if (RecordCount != List->RowCount - 1)
		ToLog("Wrong Record Count " + IntToStr(RecordCount) + "(" + IntToStr(List->RowCount - 1) + ")");

	Ready(true);

	if (NRewrites->Checked)
		NRewrites->Enabled = false;
	else
		NRewrites->Visible = false;

	if (NClearOut->Checked)
		Out->Lines->Clear();

	Opening = false;
}
//---------------------------------------------------------------------------
void TForm1::RefreshData(FILE* &file, int Start)
{
	const int MAXSIZE = 64;
	static char DATA[MAXSIZE + 1];

	// Временный буфер для безопасного считывания заголовков подзаписей (4 байта имя + 4 байта длина)
	char subName[5]; subName[4] = '\0';
	unsigned int subLen = 0;

	for (int i = Start; i < List->RowCount; ++i)
	{
		int start = List->Cells[CSTART][i].ToInt();
		fseek(file, start, SEEK_SET);
		String Header = List->Cells[CHEADER][i];

		if (Header == "GRUP")
		{
			fseek(file, LENSIZE, SEEK_CUR);
			int Data[5] = {0,0,0,0,0};
			fread(&Data[0], 4, 1, file);
			fread(DATA, 4, 1, file); DATA[4] = 0;

			if (MOVERLENTOSNAME == 12) // Oblivion
				fread(&Data[1], 4, 2, file);
			else					// Skyrim / Fallout
				fread(&Data[1], 4, 3, file);

			Data[4] = start + Data[0];
			List->Cells[CDATA][i] = String(DATA) + "[" + IntToStr(Data[0]) +
				"](" + IntToStr(Data[1]) + "," + IntToStr(Data[2]) + "," + IntToStr(Data[3]) + ") To " + IntToStr(Data[4]);
			continue;
		}

		if (Header == "CELL" && Tes3)
		{
			// Прыгаем за заголовок записи CELL (20 байт для TES3) к первой подзаписи (обычно NAME)
			fseek(file, MNAMETOSUBLEN, SEEK_CUR);

			// Считываем реальную длину строки имени ячейки
			int nameLength = 0;
			fread(&nameLength, 4, 1, file);

			int readLength = (nameLength > MAXSIZE) ? MAXSIZE : nameLength;
			fread(DATA, readLength, 1, file);
			DATA[readLength] = '\0'; // Безопасный нуль-терминатор!

			// Если имя было длиннее MAXSIZE, аккуратно допропускаем остаток строки в файле
			if (nameLength > MAXSIZE)
				fseek(file, nameLength - MAXSIZE, SEEK_CUR);

			// Теперь ищем подзапись флагов "DATA" внутри ячейки CELL
			bool dataFound = false;
			// Ограничиваем поиск, чтобы не уйти в бесконечный цикл
			for (int check = 0; check < 5; ++check)
			{
				fread(subName, 4, 1, file);
				fread(&subLen, 4, 1, file);
				if (strncmp(subName, "DATA", 4) == 0)
				{
					dataFound = true;
					break;
				}
				fseek(file, subLen, SEEK_CUR); // Пропускаем чужие подзаписи (например, flags)
			}

			if (dataFound && subLen >= 12) // DATA в CELL содержит как минимум 12 байт (флаги + координаты)
			{
				unsigned int cellFlags = 0;
				int cellX = 0, cellY = 0;

				fread(&cellFlags, 4, 1, file); // Читаем 4 байта флагов из файла
				fread(&cellX, 4, 1, file);
				fread(&cellY, 4, 1, file);

				// Просто берем адрес переменной cellFlags, приводим к указателю на BITS
				// и работаем напрямую. Теперь данные гарантированно на месте!
				BITS* pbit = reinterpret_cast<BITS*>(&cellFlags);

				String Str = "";
				bool isInterior = (cellFlags & 0x01) != 0;
				if (NExtreriorFlagsPrint->Checked)
				{
					if (pbit->b2 == 1) Str += 'w'; // Вода
					if (pbit->b3 == 1) Str += 'i'; // Сон — преступление
				}

				if (isInterior == false)
					List->Cells[CDATA][i] = String(DATA) + " (" + IntToStr(cellX) + "," + IntToStr(cellY) + ")" + Str;
			}
			else
			{
				List->Cells[CDATA][i] = String(DATA) + " [No DATA subrecord]";
			}
			continue;
		}

		if (Header == "LAND" && Tes3)
		{
			fseek(file, start + MNAMETOSUBLEN, SEEK_SET);

			// Ищем подзапись координат ландшафта INTV
			fread(subName, 4, 1, file);
			fread(&subLen, 4, 1, file);

			if (strncmp(subName, "INTV", 4) == 0)
			{
				int gridX = 0, gridY = 0;
				fread(&gridX, 4, 1, file);
				fread(&gridY, 4, 1, file);
				List->Cells[CDATA][i] = "(" + IntToStr(gridX) + "," + IntToStr(gridY) + ")";
			}
			else
			{
				List->Cells[CDATA][i] = "LAND data";
			}
			continue;
		}

		if (Header == "PGRD" && Tes3)
		{
			fseek(file, start + MNAMETOSUBLEN, SEEK_SET);
			// Читаем сетку путей (Pathgrid) DATA подзапись
			fread(subName, 4, 1, file);
			fread(&subLen, 4, 1, file);

			if (subLen >= 12)
			{
				int gridX = 0, gridY = 0;
				short pointCount = 0;
				fread(&gridX, 4, 1, file);
				fread(&gridY, 4, 1, file);
				fread(&pointCount, 2, 1, file);
				List->Cells[CDATA][i] = "(" + IntToStr(gridX) + "," + IntToStr(gridY) + ") Points: " + IntToStr((int)pointCount);
			}
			continue;
		}

		if (Header == "SKIL" || Header == "MGEF")
		{
			fseek(file, start + MNAMETOSUBLEN + SLENSIZE, SEEK_SET);
			int INDX = 0;
			fread(&INDX, 4, 1, file);
			List->Cells[CDATA][i] = IntToStr(INDX);
			continue;
		}

		// Блок Else (Для всех дефолтных подзаписей)
		{
			fseek(file, start + MNAMETOSUBLEN, SEEK_SET);
			int actualLen = 0;
			fread(&actualLen, 4, 1, file); // Считываем реальную длину подзаписи

			int readLen = (actualLen > MAXSIZE) ? MAXSIZE : actualLen;
			if (readLen > 0)
			{
				fread(DATA, readLen, 1, file);
				DATA[readLen] = '\0';
				List->Cells[CDATA][i] = DATA;
			}
			else
			{
				List->Cells[CDATA][i] = "";
			}
		}
	}
	for (int i = Start; i < List->RowCount; ++i)
		RecordData[i].Data = List->Cells[CDATA][i];
}
//---------------------------------------------------------------------------
// Возвращает указатель (чтобы внутри проверить границы)
TForm1::TRowMetadata* TForm1::GetRowData(int Row)
{
	if (Row < 0 || Row >= List->RowCount)
		return NULL;
	void* pObj = List->Objects[0][Row];
	if (pObj == NULL)
		return NULL;
	// Безопасно для Win32/Win64 извлекаем индекс
//	int *pIdx = reinterpret_cast<int*>(List->Objects[0][Row]);
//	if (pIdx == NULL)
//		return NULL;
	int vIdx = static_cast<int>(reinterpret_cast<INT_PTR>(pObj));
	if (vIdx >= 0 && static_cast<size_t>(vIdx) < RecordData.size())
		return &RecordData[vIdx];
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonGroup1ButtonClicked(TObject *Sender, int Index)
{
	if (!Reinter->Checked)
		Univ.Read(file, SLENSIZE); //TODO:bug2
	char4ToLog(Univ.Name);
	ToLog(Univ.Length,"Length");
	switch (Index)
	{
		case INTFIELDS:
			for (unsigned int i = 0; i < Univ.Length/sizeof(int); ++i)
				tolog( ((int*)Univ.Data)[i] );
			break;
		case TEXTFIELDS:
				ToLogLen( (char*)Univ.Data , Univ.Length );
			break;
		case STRFIELDS:
			if (localeinstalled)
			{
				wchar_t* mas = new wchar_t[Univ.Length+1];
				mbstowcs(mas, (char*)(Univ.Data), Univ.Length+1);
				for (unsigned int i = 0; i < Univ.Length; ++i)
					tolog(String ( mas[i]));
				delete []mas;
			}
			else
				for (unsigned int i = 0; i < Univ.Length; ++i)
					tolog( ((char*)Univ.Data)[i] );
			break;
		case FLOATFIELDS:
			for (unsigned int i = 0; i < Univ.Length/sizeof(float); ++i)
				tolog( ((float*)Univ.Data)[i]);
			break;
		case WORDFIELDS:
			for (unsigned int i = 0; i < Univ.Length/sizeof(short); ++i)
				tolog( ((short*)Univ.Data)[i]);
			break;
		case BYTEFIELDS:
			for (unsigned int i = 0; i < Univ.Length; ++i)
				tolog( ((byte*)Univ.Data)[i]);
			break;
	}
	if (PanelList2->Visible)
		if (List2->Row >= 0)
			if (!Reinter->Checked)
			{
				if (bloklist2 == 1)
					return NextSClick(Sender);
				if (List2->Row + 1 >= List2->RowCount)
					List2->Row = 0;
				else
				{
					List2->Row++;
					NextSClick(Sender);
				}
			}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteClick(TObject *Sender)
{
	if (NEnableList2Delete->Checked == false)
	{
		if (List->Row != -1)
			for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
				DeleteRecord(i);
	}
	else
		if (List2->Row != -1)
			for (int i = List2->Selection.Top; i <= List2->Selection.Bottom; ++i)
				Delete2(i);
}
//---------------------------------------------------------------------------

void TForm1::DeleteRecord(int Row)
{
	if (Row >= 0)
	{
		if (RefStarts.empty() == false || RefEnds.empty() == false)
			return;

		TRowMetadata* meta = GetRowData(Row);
		if (meta == NULL)
			return;
		long Offset = meta->StartPos; // Вместо List->Cells[CSTART][i].ToInt() или векторов
		if	(Deleted.find(Offset) == Deleted.end())
		{
			Deleted.insert(Offset);
			int  Size  = meta->Size;
			DeletedSize += Size;
			DeletedSize += MLENTOSLEN;
			List->Cells[CSIZE][Row] = "-X-";
			LDele->Caption = "Deleted Size="+IntToStr(DeletedSize)+" Count="+Deleted.size();
			LDele->Visible = true;
			Save->Enabled = true;
			NEnableList2Delete->Enabled = false;
			NEnableList2Delete->Checked = false;
			if (CheckCoord->Tag == 1)
				Out->Lines->Add("DELETE:"+List->Cells[CDATA][Row]);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextSClick(TObject *Sender)
{
	char Header[4];
	if (ftell(file) >= EoF)//(feof(file))
	{
		NextS->Caption = "End";
		return;
	}
	fread(&Header, 4, 1, file);
	if (Header)
	{
		String hedr = "Next=0123[";
		hedr[6] = Header[0];
		hedr[7] = Header[1];
		hedr[8] = Header[2];
		hedr[9] = Header[3];
		NextS->Caption = hedr;
		int len = 0;
		fread(&len, SLENSIZE, 1, file);
		NextS->Caption = NextS->Caption + IntToStr(len) + "]";
		fseek(file, -(4+SLENSIZE), SEEK_CUR);
	}
	else
	{
		NextS->Caption = "Next null or end.";
		fseek(file, -4, SEEK_CUR);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GoClick(TObject *Sender)
{
	int pos = ToE->Text.ToIntDef(0);
	if (pos > EoF)	pos = EoF;
	if (pos < 0)	pos = 0;
	long start;
	long end = -1;
	if (List->Row >= 0)
	{
		TRowMetadata* meta = GetRowData(List->Row);
		if (meta)
		{
			start = meta->StartPos;
			end   = meta->EndPos;
		}
		if (pos > end || pos < start)
			end = -1;
	}
	int i;
	if (List->Row < 0 || end < 0)
		for (i = 1; i < List->RowCount; ++i)
			if (pos < List->Cells[CSTART][i].ToIntDef(0))
			{
				List->Row = i-1;
				break;
			}
	if (i == List->RowCount)
		List->Row = List->RowCount-1;
	//Нашли List.
	for (i = 1; i < List2->RowCount; ++i)
		if (pos < List2->Cells[CSTART][i].ToIntDef(0))
		{
			Opening = true;
			BlockList2Sel = true;
			List2->Row = i-1;
			BlockList2Sel = false;
			Opening = false;
			break;
		}
	if (i == List2->RowCount)
		List2->Row = List2->RowCount-1;
	ToE->Text = pos;
	fseek(file, pos, SEEK_SET);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToESubLabelClick(TObject *Sender)
{
	ToE->Hint = ftell(file);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveClick(TObject *Sender)
{
	if (RefStarts.empty() && Deleted.empty() && RefEnds.empty() && Edited.empty())
		return;
	FILE *curr = NULL;
	String Nam = "CLEAR_" + PluginName;
	byte *mem = NULL;
	long cap = 0;
	int Len;
	if (RefStarts.empty() == false || RefEnds.empty() == false || Edited.empty() == false)
	{
		if (Deleted.empty())
			curr = _wfopen(Nam.w_str(), L"wb");
		else
			curr = _wfopen(Nam.w_str(), L"w+b");
		if (!curr)
			return ShowMessage( "Cannot open binary file.");
		Save->Enabled = false;
		fseek(file, 0, SEEK_END);
		EoF = ftell(file);
		//curr = копия file
		fseek(file, 0, SEEK_SET);
		fseek(curr, 0, SEEK_SET);
		cap = 65536;
		mem = new byte[cap];
		for (Len = cap; Len < EoF; Len += cap)
		{
			fread (mem, cap, 1, file);
			fwrite(mem, cap, 1, curr);
		}
		Len = EoF+cap-Len;
		fread (mem, Len, 1, file);
		fwrite(mem, Len, 1, curr);
		//.
		for (ED ele = Edited.begin(); ele != Edited.end(); ++ele)
		{
			fseek(curr, ele->first, SEEK_SET);
			fwrite(&ele->second, 4, 1, curr);
		}
		char symb = PrepareForEdit ? '{' : '@';
		std::vector<long>::iterator ele = RefStarts.begin();
		for (; ele != RefStarts.end(); ++ele)
		{
			fseek(curr, *ele, SEEK_SET);
			fwrite(&symb, 1, 1, curr);
		}
		symb = PrepareForEdit ? '}' : '#';
		ele = RefEnds.begin();
		for (; ele != RefEnds.end(); ++ele)
		{
			fseek(curr, *ele, SEEK_SET);
			fwrite(&symb, 1, 1, curr);
		}
		if (Deleted.empty())
		{
			fclose (curr);
			delete [] mem;
			ToLog("Сохранено: "+Nam);
			return;
		}
		else
			Nam = "DONE_" + PluginName;
	}
	else
		curr = file;
	Deleted.insert(EoF);
	save = _wfopen(Nam.w_str(), L"wb");
	if (!save)
		return ShowMessage( "Cannot open binary file.");
	Save->Enabled = false;
	std::set<long>::iterator el = Deleted.begin();
	fseek(save, 0, SEEK_SET);

	fseek(curr, 0, SEEK_SET);
	Len = *el;
	if (Len > cap)
	{
		mem = new byte[Len];
		cap = Len;
	}
	fread (mem, Len, 1, curr);
	fwrite(mem, Len, 1, save);
	for (el++; el != Deleted.end(); ++el)
	{
		//Out->Lines->Add(*el);
		fseek(curr, 4, SEEK_CUR);
		fread (&Len, LENSIZE, 1, curr);
		fseek(curr, MOVERLENTOSNAME+Len, SEEK_CUR);
		Len = *el - ftell(curr);
		if (Len > cap)
		{
			mem = new byte[Len];
			cap = Len;
		}
		fread (mem, Len, 1, curr);
		fwrite(mem, Len, 1, save);
	}
	//Запись Record Count
	Len = List->RowCount - 1 - Deleted.size() + 1;
	//fseek(save, (1+3+1+1+1+1)*4+32+256, SEEK_SET);
	fseek(save, POSNRECORDS, SEEK_SET);
	fwrite(&Len, 4, 1, save);
	delete [] mem;
	fclose (save);
	fflush(curr);
	ToLog("Сохранено: "+Nam);
}
//---------------------------------------------------------------------------
//header resize , auto run with start
void __fastcall TForm1::HeaderControl1Resize(TObject *Sender)
{
	List->ColWidths[CDATA] = List->ClientWidth
		- HeaderControl1->Sections->Items[0]->Width
		- HeaderControl1->Sections->Items[1]->Width
		- HeaderControl1->Sections->Items[2]->Width - 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HeaderControl1SectionResize(THeaderControl *HeaderControl,
			 THeaderSection *Section)
{
	List->ColWidths[Section->Index] = Section->Width - 1;
	//if (HeaderControl1->Sections->Items[CDATA]->Right >= List->Left + List->ClientWidth - 1)
	if (Section->Index != CDATA)	//{
		HeaderControl1->Sections->Items[CDATA]->Width = List->ClientWidth
		- HeaderControl1->Sections->Items[0]->Width
		- HeaderControl1->Sections->Items[1]->Width
		- HeaderControl1->Sections->Items[2]->Width - 1;
	List->ColWidths[CDATA] = HeaderControl1->Sections->Items[CDATA]->Width - 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HeaderControl2Resize(TObject *Sender)
{
	List2->ColWidths[CDATA2] = List2->ClientWidth
		- HeaderControl2->Sections->Items[0]->Width
		- HeaderControl2->Sections->Items[1]->Width
		- HeaderControl2->Sections->Items[2]->Width
		- HeaderControl2->Sections->Items[3]->Width - 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HeaderControl2SectionResize(THeaderControl *HeaderControl,
			 THeaderSection *Section)
{
	List2->ColWidths[Section->Index] = Section->Width - 1;
	//if (HeaderControl2->Sections->Items[CDATA2]->Right >= List2->Left + List2->ClientWidth - 1)
	if (Section->Index != CDATA2) //	{
		HeaderControl2->Sections->Items[CDATA2]->Width = List2->ClientWidth
		- HeaderControl2->Sections->Items[0]->Width
		- HeaderControl2->Sections->Items[1]->Width
		- HeaderControl2->Sections->Items[2]->Width
		- HeaderControl2->Sections->Items[3]->Width - 1; //	}
	List2->ColWidths[CDATA2] = HeaderControl2->Sections->Items[CDATA2]->Width - 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListSelectCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow, bool &CanSelect)
{
	if (Opening)
		return;
	TRowMetadata* meta = GetRowData(ARow);
	if (meta == NULL)
		return;
	long Offset = meta->StartPos; // Вместо List->Cells[CSTART][i].ToInt() или векторов
	long end   = meta->EndPos;
	//1 Чекнем секрет поле maina
	if (ShowAll)
	{
		fseek(file, Offset + 4 + LENSIZE, SEEK_SET);
		fread(SecretData, 4, SecretLen, file);
		Secr->Visible = false;
		for (int i = 0; i < SecretLen; ++i)
			if (SecretData[i] != 0)
			{
				Secr->Caption = "";
				for (int j = 0; j < SecretLen; ++j)
					Secr->Caption = Secr->Caption + IntToStr(SecretData[j]) + ",";
				Secr->Caption = Secr->Caption.SetLength(Secr->Caption.Length()-1);
				Secr->Visible = true;
				break;
			}
	}
	//------------
	if (PanelList2->Visible == false)
		return;
	SubDescript->Visible = false;
	//std::map<int, PACK>::iterator Curr = ListStore.find(Offset);
	char 	Name[5];	Name[4] = '\0';
	String InterpretStr, find;
	unsigned int Len;
	unsigned char *st = NULL;
	//void *st = NULL;
	DebugL2c = 0;
	if (ARow < 0)
		return;
	String HEAD = List->Cells[CHEADER][ARow];
	// TYPES TABLE найдем HEAD от и до
	// Определим начало и конец для оптимизации
	int m1=-1, m2=-2;
	if (NShowData->Checked)
	{
		if (Tes3==false && HEAD=="NPC_")
		{
			List2->RowCount = 0;
			return;
		}
		for (int t = 0; t < types.RowCount; ++t)
			if (HEAD.Compare(THeader[t]) == 0)
			{
				if (m1 == -1)
					m1 = t;
				m2 = t;
			} else if (m1 != -1)
				break;
	}
	//--------------
	ToE->Text = List->Cells[CSTART][ARow];
	BlockList2Sel = true;
	// Сбросить номер строки
	static String SelMainHedr = "";
	if (SelMainHedr != HEAD)
	{
		List2->Row = 0;
		SelMainHedr = HEAD;
	}
	//tolog("size="+IntToStr(Sizes[ARow])+"end="+IntToStr(Ends[ARow]));
	// Разбор файла
	//List2->Cols->BeginUpdate();
	fseek(file, Offset + 4 + MAINLENSIZE, SEEK_SET);
	//Out->Lines->Append(ftell(file));
	long Pos = ftell(file);
	int Row = 0;
	Interpret ish;

	if (meta->m_SavedGridCache.empty() == false)
	{
		int totalRows = meta->m_SavedGridCache.size();
		// 1. ЗАЩИТА: Блокируем скроллбары и перерисовку Windows
		TScrollStyle originalScrollBars = List2->ScrollBars;
		List2->ScrollBars = ssNone;
		List2->Perform(WM_SETREDRAW, FALSE, 0);
		try
		{
			// 2. Сразу выставляем финальный размер таблицы [1, 2]
			List2->RowCount = totalRows;
			List2->ColCount = 5;
			// 3. Выводим данные из кэша памяти в ячейки
			for (int i = 0; i < totalRows; i++)
			{
				const List2Row &cacheRow = meta->m_SavedGridCache[i];
				// VCL автоматически и очень быстро создаст String из wchar_t*
				List2->Cells[0][i] = cacheRow.col0;
				List2->Cells[1][i] = cacheRow.col1;
				List2->Cells[2][i] = cacheRow.col2;
				List2->Cells[3][i] = cacheRow.col3;
				List2->Cells[4][i] = cacheRow.col4;
			}
		}
		__finally {
			// 4. ВОССТАНОВЛЕНИЕ: Включаем интерфейс обратно
			List2->Perform(WM_SETREDRAW, TRUE, 0);
			List2->ScrollBars = originalScrollBars;
			List2->Invalidate(); // Принудительный рендер обновленной таблицы
		}
	} else
	while (Pos < end)
	{
		if (List2->RowCount < Row+1)
			List2->RowCount++;
		List2->Cells[CSTART][Row] = Pos;
		fread(Name, 4, 1, file);
		fread(&Univ.Length, SLENSIZE, 1, file);
		List2->Cells[CHEADER][Row] = Name;
		List2->Cells[CSIZE][Row] = Univ.Length;
		List2->Cells[CDATA2][Row] = "";
		List2->Cells[CTYPE][Row] = "";
		if (ExportBtn->Tag != 1)
			Len = Univ.Length > 64 ? 64 : Univ.Length; ////TODO: shririna
		else
			Len = Univ.Length;
		//Интерпретирование данных
		if (NShowData->Checked)
		{
			st = NULL;
			InterpretStr = "";
			//1 сначала ищем в TYPES.txt
			find = List2->Cells[CHEADER][Row];//= List2->Cells[CHEADER][sub];
			//SetDescription(-1, Row);
			for (int t = m1; t <= m2; ++t)
				if (find.Compare(TSubHeader[t]) == 0)
				{	//нашли
					if (TDescr[t].Length() > 0)
						SetDescription(t, Row); //	SubIndexes[Row] = t;
					else
						SetDescription(-1, Row);
					st = file->curp;
					List2->Cells[CTYPE][Row] = TType[t][1];
					switch (TType[t][1])
					{
						//case 't': List2->Cells[CDATA2][Row] = st; break; //ToLog(&Name[8]);
						case 's': List2->Cells[CDATA2][Row] = String((char*)st); break;
						case 'i':
						for (unsigned int i = 0; i < Len; i+=sizeof(int))
								InterpretStr += IntToStr(*(int*)&st[i])+" ";
							break;
						case 'b':
							for (unsigned int i = 0; i < Len; ++i)
								InterpretStr += IntToStr(*(byte*)&st[i])+" ";
							break;
						case 'f': for (unsigned int i = 0; i < Len; i+=sizeof(float))
								InterpretStr += FloatToStrF(*(float*)&st[i], ffGeneral, 8, 8)+" ";
							break;
						case 'w': for (unsigned int i = 0; i < Len; i+=sizeof(short))
								InterpretStr += IntToStr(*(short*)&st[i])+" ";
							break;
						case 'F': //FRMR
							ish.i = 0; ish.b[0] = *(byte*)&st[3];
							InterpretStr = IntToStr(ish.i);
							ish.b[0] = *(byte*)&st[0]; ish.b[1] = *(byte*)&st[1]; ish.b[2] = *(byte*)&st[2];
							InterpretStr = IntToStr(ish.i) + " " + InterpretStr +" ";
							break;
						default :
							for (unsigned int i = 0; i < Len; ++i)
								InterpretStr += IntToStr(*(byte*)&st[i])+" ";
					}
					if (InterpretStr.Length() > 0)
						List2->Cells[CDATA2][Row] = (InterpretStr.SetLength(InterpretStr.Length()-1));
				}
			//Не нашли в TYPES.txt
			if	(List2->Cells[CTYPE][Row] == "")
			{
				SetDescription(-1, Row); //ETOSHTO
				unsigned int pe;
				//2 Не нашли. Поищем в встроеных
				std::map<String, TagInfo>::const_iterator it = tagMap.find(Name);
				//for (int i = 0; i < nTypes; ++i)
				if (it != tagMap.end())
				{
					const TagInfo& currentTag = it->second; // Получаем найденные свойства тега
					// Записываем тип в ячейку StringGrid (автоматически приведется к String)
					st = file->curp;
					List2->Cells[CTYPE][Row] = currentTag.type;

					// Превращаем mainTag (std::string) в VCL String для метода tolog
					String vclMainTag = currentTag.mainTag.c_str();
					switch (currentTag.type)
					{
						case 't':
							//check pechatn symbols
							for (pe = 0; pe < Univ.Length-1; pe++)
								if (st[pe] < 32)
									break;
							if (pe >= Univ.Length-1)
							{
								find = (char*)st;
								if ((unsigned int)find.Length() > Univ.Length)
									List2->Cells[CDATA2][Row] = find.SetLength(Univ.Length);
								else
									List2->Cells[CDATA2][Row] = find;
								break;
							}
						case '1':
							for (unsigned int i = 0; i < Len; i+=1)
								InterpretStr += IntToStr(*(byte*)&st[i])+" ";
							break;
						case 'g': for (unsigned int i = 0; i < Len; i+=sizeof(float))
								InterpretStr += FloatToStr(*(float*)&st[i])+" ";
							break;
						case '4':
							for (unsigned int i = 0; i < Len; i+=sizeof(int))
								InterpretStr += IntToStr(*(int*)&st[i])+" ";
							break;
						case '2':
							for (unsigned int i = 0; i < Len; i+=2)
								InterpretStr += IntToStr(*(WORD*)&st[i])+" ";
							break;
					}
					if (InterpretStr.Length() > 0)
						List2->Cells[CDATA2][Row] = (InterpretStr.SetLength(InterpretStr.Length()-1));
				}
			}
			if	(List2->Cells[CTYPE][Row] == "")
			{
				//3 опять Не нашли. го дерьмо
				if (Name[1]=='N'&&Name[2]=='A'&&Name[3]=='M')
				{
					st = file->curp;
					if (st[0] >= 32 && st[1] >= 32)
					{
						InterpretStr = (char*)st;
						InterpretStr.SetLength(Len);
						List2->Cells[CDATA2][Row] = InterpretStr;
						List2->Cells[CTYPE][Row] = "NAM";
					}
				}
			}
			if	(List2->Cells[CTYPE][Row] == "")
			{
				//4 опять Не нашли. го однобайты
				st = file->curp;
				for (unsigned int i = 0; i < Len; ++i)
					InterpretStr += IntToStr(*(byte*)&st[i])+" ";
				List2->Cells[CDATA2][Row] = (InterpretStr.SetLength(InterpretStr.Length()-1));
			}
		} //show data
		fseek(file, Univ.Length, SEEK_CUR);
		Pos = ftell(file);
		Row++;
	}
	if (NShowData->Checked && meta->m_SavedGridCache.empty()) //Первый клик
	{
		//Запишем все чтобы второй раз не перечитывать file
		int totalRows = List2->RowCount;
		// 1. Заранее выделяем память под весь вектор одной командой [1, 2]
		meta->m_SavedGridCache.resize(totalRows);
		// 2. Копируем данные из таблицы в кэш памяти
		for (int i = 0; i < totalRows; i++)
		{
			List2Row &cacheRow = meta->m_SavedGridCache[i];
			// Быстрое копирование текста во внутренние буферы
			wcsncpy(cacheRow.col0, List2->Cells[0][i].c_str(), 255);
			wcsncpy(cacheRow.col1, List2->Cells[1][i].c_str(), 255);
			wcsncpy(cacheRow.col2, List2->Cells[2][i].c_str(), 255);
			wcsncpy(cacheRow.col3, List2->Cells[3][i].c_str(), 64);
			wcsncpy(cacheRow.col4, List2->Cells[4][i].c_str(), 255);
		}
	}
	//fseek(file, ToE->Text.ToIntDef(0) - ftell(file), SEEK_CUR);
	for (Row = 0; Row < List2->RowCount; Row++)
	{
		find = List2->Cells[CHEADER][Row];//= List2->Cells[CHEADER][sub];
		for (int t = m1; t <= m2; ++t)
			if (find.Compare(TSubHeader[t]) == 0) //нашли
				if (TDescr[t].Length() > 0)
					SetDescription(t, Row); //	SubIndexes[Row] = t;
				else
					SetDescription(-1, Row);
	}
	BlockList2Sel = false;
	if (CheckCoord->Tag == 1)
	{
		int p = List->Cells[CDATA][ARow].Pos(')');
		if (p > 0)
			CurrCell = List->Cells[CDATA][ARow].SubString(1,p);
		else
			CurrCell = List->Cells[CDATA][ARow];
		Form1->Caption = CurrCell;
	}
	//if Arow daje ne 0+
	//Out->Lines->Append(DebugL2c);
	//if (Check&&Expo)
	//	FindNAMEClick(Sender);
	//fseek(file, List2->Cells[CSTART][List2->Row].ToIntDef(0) - ftell(file), SEEK_CUR);
	if (ShowAll == false)
		return;
	if (List2->RowCount >= 0)
	{
		PanelSubRead->Enabled = true;
		BlockList2Sel = true;
		List2->Selection.Top = 0;
		List2->Selection.Bottom = 0;
		BlockList2Sel = false;
		fseek(file, List->Cells[CSTART][ARow].ToInt() + 4 + MAINLENSIZE, SEEK_SET);
		NextSClick(Sender);
	}
	if (CheckCoord->Tag == 1)
		CheckCoordClick(Sender);
}
//---------------------------------------------------------------------------

void TForm1::SetDescription(int Num, int Row)
{
	if (cSubIndexes <= Row)
	{
		cSubIndexes *= 2;
		SubIndexes = new int[cSubIndexes];
	}
	SubIndexes[Row] = Num;
	//for (int i = 0; i < List2->RowCount; ++i)
	//SubIndexes[i] = -1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HeaderControl1SectionClick(THeaderControl *HeaderControl, THeaderSection *Section)
{
	DWORD tick = ::GetTickCount();
	int sortingColumn = Section->Index;
	// Определяем тип столбца
	bool compareString = (sortingColumn == CHEADER || sortingColumn == CDATA);
	int totalRows = List->RowCount;
	if (totalRows <= 1)
		return;
	// Логика Excel: если кликнули ту же колонку — меняем порядок на обратный.
	// Если кликнули новую колонку — всегда начинаем с сортировки по возрастанию.
	if (sortingColumn == LastSortingColumn)
	{
		AscendingOrder = !AscendingOrder;
	} else
	{
		AscendingOrder = true;
		LastSortingColumn = sortingColumn;
	}
	// 1. Замораживаем графику VCL
	List->Perform(WM_SETREDRAW, FALSE, 0);
	List->ScrollBars = ssNone;
	// 2. Считываем данные в память
	std::vector<RowData> rows;
	rows.reserve(totalRows);
	for (int i = 0; i < totalRows; ++i)
	{
		RowData row;
		row.cells[CHEADER]	= List->Cells[CHEADER][i];
		row.cells[CSTART]	= List->Cells[CSTART][i];
		row.cells[CSIZE]	= List->Cells[CSIZE][i];
		row.cells[CDATA]	= List->Cells[CDATA][i];
//		row.sizeVal	= Sizes[i];
 //		row.endVal	= Ends[i];	 TODO: DODELAI
		rows.push_back(row);
	}
	// 3. Стабильная сортировка (сохраняет порядок дубликатов) с учетом направления
	std::stable_sort(rows.begin(), rows.end(), RowComparator(sortingColumn, compareString, AscendingOrder));
	// 4. Записываем результат обратно
	for (int i = 0; i < totalRows; ++i)
	{
		List->Cells[CHEADER][i]	= rows[i].cells[CHEADER];
		List->Cells[CSTART][i]	= rows[i].cells[CSTART];
		List->Cells[CSIZE][i]	= rows[i].cells[CSIZE];
		List->Cells[CDATA][i]	= rows[i].cells[CDATA];
//		Sizes[i]	= rows[i].sizeVal;
//		Ends[i]		= rows[i].endVal;
	}
	// 5. Размораживаем и обновляем экран
	List->ScrollBars = ssVertical;
	List->Perform(WM_SETREDRAW, TRUE, 0);
	List->Invalidate();
	tick = ::GetTickCount() - tick;
	//String directionText = AscendingOrder ? L" (по возрастанию)" : L" (по убыванию)";
	//Out->Lines->Add("Колонка " + IntToStr(sortingColumn) + directionText + " отсортирована за: " + FloatToStr(tick) + " мс.");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TestPClick(TObject *Sender)
{
	NTestMenuClick(Sender);
	PrintAllTags();
}
//---------------------------------------------------------------------------
bool TForm1::Find(String find)
{
	if (find.Length() <= 0)
		return false;
	int row = 0;
	if (SearchList->Row <= 0)
	{
		row = SearchList->Cols[*SearchingIn]->IndexOf(find);
		if (row >= 0)
			return EndFind(row);
		row = 0;
	}
	else
		row = SearchList->Row+1;
	for (int i = row; i < SearchList->RowCount; ++i)
		if (SearchList->Cols[*SearchingIn]->Strings[i].Pos(find) == 1)
			return EndFind(i);
	for (int i = 0; i < row; ++i)
		if (SearchList->Cols[*SearchingIn]->Strings[i].Pos(find) == 1)
			return EndFind(i);
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FindStrClick(TObject *Sender)
{
	Find(EFinds->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FindNextClick(TObject *Sender)
{
	if (Out->SelLength > 0)
	{
		ListEnter(Sender);
		if (Find(Out->SelText) == false)
			Out->SelLength = 0;
		return;
	}
	ListEnter(Sender);
	if (FindIdx >= Out->Lines->Count)
		FindIdx = -1;
	FindIdx++;
	for (; FindIdx < Out->Lines->Count; ++FindIdx)
		if (Out->Lines->Strings[FindIdx].Length() > 0)
			if (Find(Out->Lines->Strings[FindIdx]) == true)
			{
				EFinds->Text = Out->Lines->Strings[FindIdx];
				FindNext->Caption = "Fin&d "+IntToStr(FindIdx);
				return;
			}
}
//---------------------------------------------------------------------------
bool TForm1::EndFind(int Row)
{
	SearchList->Row = Row;
	if (SearchList->TopRow != 0 && Row >= SearchList->TopRow + SearchList->VisibleRowCount - 3)
		SearchList->TopRow = Row - SearchList->VisibleRowCount / 2;
	if (EFinds->Text.Compare(List->Cells[*SearchingIn][Row]) == 0)
		EFinds->Font->Color = clBlack;
	else
		EFinds->Font->Color = clBlue;
	return true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextSContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	Out->Lines->Add(ftell(file));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::List2SelectCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow, bool &CanSelect)
{
	DebugL2c++;
	if (BlockList2Sel)
		return;
	if (ARow >= 0)
	{
		if (CheckCoord->Tag == 1)
			EFinds->Text = List2->Cells[CDATA2][ARow];
		if (bloklist2 == 1)
			bloklist2 = 0;
		ToE->Text = List2->Cells[CSTART][ARow];

		if (ARow < cSubIndexes && SubIndexes[ARow] > 0 && SubIndexes[ARow] < types.RowCount)
		{
			SubDescript->Text = TDescr[SubIndexes[ARow]];
			SubDescript->Visible = true;
		} else
			SubDescript->Visible = false;
		fseek(file, ToE->Text.ToIntDef(0), SEEK_SET);
		NextSClick(Sender);
		if (CheckCoord->Tag == 1)
			Form1->Caption = CurrCell + "|"+ List2->Cells[CDATA2][ARow];
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ProModeCKClick(TObject *Sender)
{
	PanelPRO->Visible = ProModeCK->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CloseClick(TObject *Sender)
{
	fclose (file);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WordwrapClick(TObject *Sender)
{
	Out->WordWrap = Wordwrap->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NTestMenuClick(TObject *Sender)
{
	int Count = 0;
	if (List->Row != -1)
		for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
			Count++;
	Out->Lines->Add("Selected count ="+IntToStr(Count));
	EFinds->Width += 20;
	GMDTRead->Visible = true;
	TestP->Visible = true;
	SPELread->Visible = true;
	SPLMread->Visible = true;
	Sel->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NEnableSublistClick(TObject *Sender)
{
	PanelList2->Visible = NEnableSublist->Checked;
	Splitter2->Visible = NEnableSublist->Checked;
	PanelSubRead->Visible = NEnableSublist->Checked;
	FindinList2->Enabled = NEnableSublist->Checked;
	FindinSublists->Enabled = NEnableSublist->Checked;
	SwapCoord->Enabled = NEnableSublist->Checked;
	DeleteExtraData->Enabled = NEnableSublist->Checked && NEnableList2Delete->Checked;
	DelGroupSubheaders->Enabled = NEnableSublist->Checked && NEnableList2Delete->Checked;
	DeleteAllSubhead->Enabled = NEnableSublist->Checked && NEnableList2Delete->Checked;
	CheckCoord->Enabled = NEnableSublist->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NClearOutClick(TObject *Sender)
{
	Out->Lines->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SelClick(TObject *Sender)
{
	bool CanSel = true;
	if (List->Row != -1)
		for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
			ListSelectCell(Sender, 0, i, CanSel);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FindNextContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	if (List->Row >= 0)
		DeleteClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_DELETE)
		if (List->Row >= 0)
			DeleteClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NEnableList2DeleteClick(TObject *Sender)
{
	static bool mWarning = true;
	if (mWarning)	ShowMessage("This can lead to a problem.");
	mWarning = false;
	Save2->Visible = NEnableList2Delete->Checked;
	Save->Visible = !NEnableList2Delete->Checked;
	DeleteExtraData->Enabled = true;
	DelGroupSubheaders->Enabled = true;
	DeleteAllSubhead->Enabled = true;
	DevastateCell->Visible = NEnableList2Delete->Checked;
	MassDelete2->Visible = NEnableList2Delete->Checked;
	MassDelete->Visible = !NEnableList2Delete->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::List2KeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (NEnableList2Delete->Checked && Key == VK_DELETE)
	{
		if (List2->Row >= 0)
			for (int i = List2->Selection.Top; i <= List2->Selection.Bottom; ++i)
				Delete2(i);
		if (CheckCoord->Tag == 1)
			Out->Lines->Add("DELETE:"+List->Cells[CDATA][List->Row]+"\t"+List2->Cells[CSTART][List2->Selection.Top]+"\t"
				+List2->Cells[CSTART][List2->Selection.Bottom]);
	}
}
//---------------------------------------------------------------------------

void TForm1::Delete2(int Row2)
{
	return DeleteSublist(Row2, List->Row);
}
//---------------------------------------------------------------------------

void TForm1::DeleteSublist(int Row2, int MainRow)
{
	TRowMetadata* meta = GetRowData(MainRow);
	if (meta == NULL)
		return;
	int  MainLen  = meta->Size;
	int Offset = List2->Cells[CSTART][Row2].ToInt();
	if (Deleted.find(Offset) == Deleted.end())
	{
		Deleted.insert(Offset);
		int del = List2->Cells[CSIZE][Row2].ToIntDef(0);
		del += SLENSIZE;
		del += 4;
		DeletedSize += del;
		long MainLenOffset = meta->StartPos + 4;

		if (ShowAll)
		{
			LDele->Visible = true;
			Save2->Enabled = true;
			LDele->Caption = "Deleted Size="+IntToStr(DeletedSize)+" Count="+Deleted.size();
			List2->Cells[CSIZE][Row2] = "-X-";
		}
//		else
//			tolog(IntToStr(Offset)+":DELE in:"+List->Cells[CDATA][MainRow]);
		SubDelete.push_back(DeleteItem(MainLenOffset, MainLen, Offset, del));
		//запишем размер header
		if (List->Cells[CHEADER][MainRow].ToIntDef(-1) == -1) //first del
			List->Cells[CHEADER][MainRow] = MainLen - del;
		else
			List->Cells[CHEADER][MainRow] = List->Cells[CHEADER][MainRow].ToInt() - del;
		//List->Cells[CHEADER][MainRow] = List->Cells[CSIZE][MainRow]; //костыль
	}
}
//---------------------------------------------------------------------------
//subheaderlen = 4+4+len
bool Compa(DeleteItem a, DeleteItem b)
{
	return a.Offset < b.Offset;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Save2Click(TObject *Sender)
{
	if (SubDelete.empty())
		return;
	String Nam = "DONE_"+PluginName;
	save = _wfopen(Nam.w_str(), L"wb");
	if (!save) return ShowMessage( "Cannot open binary file.");
	fseek(file, 0, SEEK_END);
	EoF = ftell(file);
	//if (NClearOut->Checked == false)
	std::sort(SubDelete.begin(),SubDelete.end(), Compa);
	SubDelete.push_back(DeleteItem(EoF, 0, EoF, 0));
	std::vector<DeleteItem>::iterator el;
	//удаление дубликатов
	if (NClearOut->Checked == false)
	{
		el = SubDelete.begin();
		int Offset = el->Offset;
		for (el++; el != SubDelete.end(); ++el)
			if (el->Offset == Offset)
			{
				SubDelete.erase(el);
				el = SubDelete.begin();
				Offset = el->Offset;
			}
			else
				Offset = el->Offset;
	}
	fseek(file, 0, SEEK_SET);
	int MemSize = 0;
	byte *Mem = NULL;
	fseek(file, 0, SEEK_SET);

	el = SubDelete.begin();
	for (; el != SubDelete.end(); ++el)
	{
		int GoodSize = el->Offset - ftell(file);
		if (GoodSize > MemSize)
		{
			Mem = new byte[GoodSize];
			MemSize = GoodSize;
		}
		if (GoodSize > 0)
		{
			fread (Mem, GoodSize, 1, file);
			fwrite(Mem, GoodSize, 1, save);
		}
		if (el->Addon == NULL)
			fseek(file, el->Size, SEEK_CUR); //к началу слудующего блока
		else
		{
			char test[32];
			memcpy(test, el->Addon, 32);
			fwrite(el->Addon, -(el->Size), 1, save);
		}
	}
	//Перепишем размеры main
	int AllDeletedSize = 0;
	int DeletedSize = 0;
	el = SubDelete.begin();
	for (; el != SubDelete.end(); ++el)
	{
		DeletedSize += el->Size;
		if (el+1 != SubDelete.end() && (el+1)->MainLenOffset != el->MainLenOffset )
		{
			//Out->Lines->Add(IntToStr(DeletedSize)+"dasaf");
			fseek(save, el->MainLenOffset - AllDeletedSize, SEEK_SET); //промотал TES3 char
			int NewLen = el->MainLen - DeletedSize;
			fwrite(&NewLen, SLENSIZE, 1, save);
			AllDeletedSize += DeletedSize;
			DeletedSize = 0;
		}
	}
	delete [] Mem;
	fclose (save);
	Out->Lines->Add("Сохранено: "+Nam);
	Out->Lines->Add("AllDeletedSize="+IntToStr(AllDeletedSize));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FindinList2Click(TObject *Sender)
{
	List2Enter(Sender);
	FindStrClick(Sender);
	ListEnter(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextTagContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	DeleteClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddMainFieldsClick(TObject *Sender)
{
	int fi[6];
	int count = MOVERLENTOSNAME / 4;
	for (int i = 0; i < List->RowCount; ++i)
	{
		fseek(file, List->Cells[CSTART][i].ToInt()+4+4, SEEK_SET);
		fread(fi, 4, count, file);
		for (int j = 0; j < count; ++j)
			List->Cells[CHEADER][i] = List->Cells[CHEADER][i] + " "+IntToStr(fi[j]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::setlocaleBtnClick(TObject *Sender)
{
	if (EFinds->Text.Length() <= 0)
		return ShowMessage("'Find' field is empty. Write a localization page in it. eg \".1251\"");
	String mes = L"Set the locale to \""+EFinds->Text+"\"?" ;
	switch (Application->MessageBox(mes.w_str(), L"setlocale", MB_YESNOCANCEL))
	{
	case ID_NO: break;
	case ID_CANCEL:
		setlocale(LC_ALL, "");
		localeinstalled = false;
		break;
	case ID_YES:
		//setlocale(LC_ALL, AnsiString(EFinds->Text).c_str());
		_wsetlocale(LC_ALL, EFinds->Text.w_str());
		localeinstalled = true;
		break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextCellClick(TObject *Sender)
{
	for (int i = List->Row+1; i < List->RowCount; ++i)
		if (List->Cells[CHEADER][i] == "CELL" || List->Cells[CHEADER][i] == "LAND"
			|| List->Cells[CHEADER][i] == "PGRD")
			{
				List->Row = i;
				return;
			}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FindinSublistsClick(TObject *Sender)
{
	String Find = EFinds->Text;
	bool CanSelect = true;
	for (int Row = List2->Row+1; Row < List2->RowCount; ++Row)
		if	(List2->Cells[SearchingIn2][Row].Pos(Find) == 1)
		{
			List2->Row = Row;
			return;
		}
	ShowAll = false;
	for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
	{
		ListSelectCell(Sender, 0, i, CanSelect);
		for (int Row = 0; Row < List2->RowCount; ++Row)
			if	(List2->Cells[SearchingIn2][Row].Pos(Find) == 1)
			{
				List->Row = i;
				List2->Row = Row;
				ShowAll= true;
				return;
			}
	}
	ShowAll= true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ReplaceClick(TObject *Sender)
{
	//чтение текста из буфера обмена
	String fromClipboard;//в эту переменную сохраним текст из буфера обмена
	if ( OpenClipboard(Application->Handle) )//открываем буфер обмена
	{
		HANDLE hData = GetClipboardData(CF_TEXT);//извлекаем текст из буфера обмена
		char* chBuffer= (char*)GlobalLock(hData);//блокируем память
		fromClipboard = chBuffer;
		GlobalUnlock(hData);//разблокируем память
		CloseClipboard();//закрываем буфер обмена
	}
	int p=1;
	while ((p=fromClipboard.Pos('@')) > 0)
	{
		fromClipboard = fromClipboard.Delete(p, 1);
		fromClipboard = fromClipboard.Insert('{',p);
	}
	while ((p=fromClipboard.Pos('#')) > 0)
	{
		fromClipboard = fromClipboard.Delete(p, 1);
		fromClipboard = fromClipboard.Insert('}',p);
	}
	while ((p=fromClipboard.Pos('')) > 0)
	{
		fromClipboard = fromClipboard.Delete(p, 1);
		fromClipboard = fromClipboard.Insert('*',p);
	}
	//найдем * без }
	for (int i = 1; i < fromClipboard.Length()+1; ++i)
		if (fromClipboard[i] == '*' && fromClipboard[i+1] != '}')
		{
			if (fromClipboard[i+1] == '*')
				continue;
			fromClipboard = fromClipboard.Insert('}',i+1);
			for (int j = i-1; j >= 1; --j)
				if (fromClipboard[j] == ' ')
				{
					fromClipboard = fromClipboard.Insert('{',j+1);
					break;
				}
			i += 2;
		}
	Out->Lines->Append(fromClipboard);
	String source = fromClipboard;
	if(OpenClipboard(Application->Handle))//открываем буфер обмена
	{
		HGLOBAL hgBuffer;
		char* chBuffer;
		EmptyClipboard(); //очищаем буфер
		hgBuffer= GlobalAlloc(GMEM_DDESHARE, source.Length()+1);//выделяем память
		chBuffer= (char*)GlobalLock(hgBuffer); //блокируем память
		strcpy(chBuffer, LPCSTR(source.c_str()));
		GlobalUnlock(hgBuffer);//разблокируем память
		SetClipboardData(CF_TEXT, hgBuffer);//помещаем текст в буфер обмена
		CloseClipboard(); //закрываем буфер обмена
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NRewritesClick(TObject *Sender)
{
	SwapCoord->Visible = NRewrites->Checked;
	MVRF->Visible = NRewrites->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::List2Enter(TObject *Sender)
{
	if (FindStr->ImageIndex == 2)
		FindStr->ImageIndex = 4;
	SearchingIn = &SearchingIn2;
	SearchList = List2;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListEnter(TObject *Sender)
{
	if (FindStr->ImageIndex == 4)
		FindStr->ImageIndex = 2;
	SearchingIn = &SearchingIn1;
	SearchList = List;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OutDblClick(TObject *Sender)
{
	if (NAutoFind->Checked == false)
		return;
	EFinds->Text = Out->SelText.TrimRight();
	List2Enter(Sender);
	FindStrClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WhatFindedChange(TObject *Sender)
{
	SearchingIn1 = SearchingIn2 = WhatFinded->ItemIndex;
	if (WhatFinded->ItemIndex == CDATA)
		SearchingIn2 = CDATA2;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NAboveClick(TObject *Sender)
{
	List->DefaultRowHeight++;
	List2->DefaultRowHeight++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::List2DblClick(TObject *Sender)
{
	static int idx = 2;
	if (List2->Cells[CTYPE][List2->Row].Length() >= 1)
		switch (List2->Cells[CTYPE][List2->Row][1])
		{
		case	'i':
		case	'4':	idx = 0; break;
		case	'g':
		case	'f': 	idx = 3; break;
		case	'1':
		case	'b': 	idx = 5; break;
		case	't':
		case	's':
		case 	'N':	idx = 1; break;
		}
	ButtonGroup1ButtonClicked(Sender, idx);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListDblClick(TObject *Sender)
{
	tolog(List->Cells[CDATA][List->Row]);
}
//---------------------------------------------------------------------------
