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
void __fastcall TForm1::MassDelete2Click(TObject *Sender)
{
	if (List->Row == -1)
		return;
	int type = 0;
	Wordwrap->Checked = false;

	String Info = Out->Lines->CommaText;
	if (Info.Length() > 1024)
	{
		Info.SetLength(1024);
		Info +="...";
	}
	String ss(L"To remove all from "+WhatFinded->Text
		+L" conterminous with the {"+Info.w_str()+L"}?");
	if ( (type=Application->MessageBox(ss.w_str()
		, L"Mass deleting", MB_YESNOCANCEL+MB_ICONQUESTION))== ID_CANCEL)
			return;
	if (type == ID_NO)
	{
		ss = (L"To remove all "+WhatFinded->Text
		+L" except for conterminous with the {"+Info.w_str()+L"}?");
		if ( (type=Application->MessageBox(ss.w_str()
		, L"Mass deleting", MB_YESNOCANCEL+MB_ICONQUESTION))!= ID_YES)
			return;
		type = 888;
	}
	std::set<String> heat;
	for (int i = 0; i < Out->Lines->Count; i++)
		heat.insert(Out->Lines->Strings[i]);
	std::set<String> heat2(heat);
	bool CanSel = true;
	for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
	{
		ListSelectCell(Sender, 0, i, CanSel);
		if (type != 888)
		{
			for (int j = 0; j < List2->RowCount; ++j)
				if (heat.find(List2->Cells[SearchingIn2][j]) != heat.end())
				{
					DeleteSublist(j, i);
					heat2.erase(List->Cells[SearchingIn2][j]);
				}
		}
		else //all Except find
			for (int j = 0; j < List2->RowCount; ++j)
				if (heat.find(List2->Cells[SearchingIn2][j]) == heat.end())
					DeleteSublist(j, i);
				else
					heat2.erase(List2->Cells[SearchingIn2][j]);
	}
	if (heat2.size() > 0)
	{
		Out->Lines->Append("The lines were not found:");
		for (std::set<String>::iterator el = heat2.begin(); el != heat2.end(); ++el)
			Out->Lines->Append(*el);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DropMasterClick(TObject *Sender)
{
	if (List->Cells[CHEADER][List->Row] != "CELL")
		return;
	int type = 0;
	if ( (type=Application->MessageBox(L"FRMR indexes - 0:New; 1:M; 2:T; 3:B; 4+: Another esm. To replace 4+ to 0?"
		, L"FRMR Indexes->0", MB_YESNOCANCEL)) == ID_CANCEL)
			return;
	if (type == ID_YES)
		type = 400;
	else
	if ( (type=Application->MessageBox(L"FRMR indexes 0:New; 1:M; 2:T; 3:B; 4+: Another esm. To replace ALL to 0?"
		, L"FRMR Indexes->0", MB_YESNOCANCEL)) == ID_CANCEL)
			return;
	if (type == ID_YES)
		type = 1;
	else
		type = 4;
	int frmr, mast;
	for (int j = 1; j < List2->RowCount; j++)
		if (List2->Cells[CHEADER][j].Compare("FRMR") == 0)
		{
			int Offset = List2->Cells[CSTART][j].ToInt() + 8;
			fseek(file, Offset, SEEK_SET);
			int Data;
			fread(&Data, 4, 1, file);
			mast = Data / 16777216;
			frmr = Data % 16777216;
			if (mast >= type)
			{
				Edited.insert(std::pair<long, int>(Offset, frmr));
				tolog(IntToStr(frmr)+" "+IntToStr(mast)+" = drop.");
			}
			else
				tolog(IntToStr(frmr)+" "+IntToStr(mast)+" = ignored.");
		}
	if (Edited.size() > 0)
	{
		tolog("Done. "+IntToStr((int)Edited.size())+" substitutions. Click Save to complete.");
		Save->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DropMasterContextPopup(TObject *Sender, TPoint &MousePos,
	  bool &Handled)
{
	for (ED el = Edited.begin(); el != Edited.end(); ++el)
		tolog(IntToStr((int)el->first)+" "+IntToStr(el->second));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DevastateCellClick(TObject *Sender)
{
	bool CanSelect = true;
	ShowAll = false;
	NShowData->Checked = false;
	for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
		if (List->Cells[CHEADER][i].Compare("CELL") == 0)
		{
			ListSelectCell(Sender, 0, i, CanSelect);
			int Row = 3;
			if (List2->RowCount >= 3)
				if (List2->Cells[CHEADER][3].Compare("AMBI") == 0)
					Row = 4;
			for (; Row < List2->RowCount; ++Row)
				DeleteSublist(Row, i);
		}
	ShowAll = true;
	LDele->Caption = "Deleted Size="+IntToStr(DeletedSize)+" Count="+Deleted.size();
	NShowData->Checked = true;
	LDele->Visible = true;
	Save2->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FindOwnersClick(TObject *Sender)
{
	if (NEnableList2Delete->Checked==false && Out->Lines->Count <= 0)
		return;
	Wordwrap->Checked = false;
	if (NEnableList2Delete->Checked)
	{
		if (obj.size() <= 0)
		{
			for (int i = 0; i < Out->Lines->Count; i++)
				obj.insert(Out->Lines->Strings[i]);
			Out->Lines->Clear();
			ShowMessage("Мы готовы удалять "+IntToStr((int)obj.size())+" объектов. Только укажи локацию.");
			return;
		}
	}
	else
	{
		obj.clear();
		for (int i = 0; i < Out->Lines->Count; i++)
			obj.insert(Out->Lines->Strings[i]);
	}
	bool CanSelect = true;
	Out->Lines->BeginUpdate();
	Out->Lines->Clear();
	int i = 0;
	int end = List->RowCount;
	if (NEnableList2Delete->Checked)
	{
		i = List->Row;
		end = i+1;
	}
	for (; i < end; i++)
		if (List->Cells[CHEADER][i].Compare("CELL") == 0)
		{
			ListSelectCell(Sender, 0, i,CanSelect);
			int nameidx = -1;
			for (int j = 1; j < List2->RowCount; j++)
			{
				if (nameidx < 0)
				{
					if (List2->Cells[CHEADER][j].Compare("NAME") == 0)
						if (obj.find(List2->Cells[CDATA2][j]) != obj.end())
							nameidx = j;
				} else
				if (List2->Cells[CHEADER][j] == "DATA")
					nameidx = -2;
				else
				{
					if (NEnableList2Delete->Checked)
					{
						String Head = List2->Cells[CHEADER][j];
						if (Head=="ANAM" || Head=="INTV" || Head=="NAM9"
							|| Head=="CNAM" || Head=="INDX" )
							Delete2(j);
					}
					else
						if ( (List2->Cells[CHEADER][j].Compare("CNAM") == 0)
							||(List2->Cells[CHEADER][j].Compare("ANAM") == 0))
							Out->Lines->Append(List->Cells[CDATA][i]+"\t"
								+List2->Cells[CDATA2][nameidx]);
				}
			}
		}
	Out->Lines->EndUpdate();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::MassDeleteClick(TObject *Sender)
{
	int type = 0;
	Wordwrap->Checked = false;
	std::set<String> heat;
	String Info = Out->Lines->CommaText;
	if (Info.Length() > 1024)
	{
		Info.SetLength(1024);
		Info +="...";
	}
	String ss(L"To remove all from "+WhatFinded->Text
		+L" conterminous with the {"+Info.w_str()+L"}?");
	if ( (type=Application->MessageBox(ss.w_str()
		, L"Mass deleting", MB_YESNOCANCEL+MB_ICONQUESTION))== ID_CANCEL)
			return;
	if (type == ID_NO)
	{
		ss = (L"To remove all "+WhatFinded->Text
		+L" except for conterminous with the {"+Info.w_str()+L"}?");
		if ( (type=Application->MessageBox(ss.w_str()
		, L"Mass deleting", MB_YESNOCANCEL+MB_ICONQUESTION))!= ID_YES)
			return;
		type = 888;
	}
	for (int i = 0; i < Out->Lines->Count; i++)
		heat.insert(Out->Lines->Strings[i]);
	std::set<String> heat2(heat);
	if (type != 888)
	{
		for (int i = 0; i < List->RowCount; ++i)
			if (heat.find(List->Cells[WhatFinded->ItemIndex][i]) != heat.end())
			{
				DeleteRecord(i);
				heat2.erase(List->Cells[WhatFinded->ItemIndex][i]);
			}
	}
	else //all Except find
		for (int i = 0; i < List->RowCount; ++i)
			if (heat.find(List->Cells[WhatFinded->ItemIndex][i]) == heat.end())
				DeleteRecord(i);
			else
				heat2.erase(List->Cells[WhatFinded->ItemIndex][i]);
	if (heat2.size() > 0)
	{
		Out->Lines->Append("The lines were not found:");
		for (std::set<String>::iterator el = heat2.begin(); el != heat2.end(); ++el)
			Out->Lines->Append(*el);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DelGroupSubheadersClick(TObject *Sender)
{
	if (List2->Row < 0)
		return;
	String Find = List2->Cells[CHEADER][List2->Row];
	Delete2(List2->Row);
	for (int i = List2->Selection.Top + 1; i <= List2->RowCount; ++i)
		if (List2->Cells[CHEADER][i].Compare(Find) == 0)
			return;
		else
			Delete2(i);
}
//---------------------------------------------------------------------------
//Проверяет равенство строки left с любой из массива right
bool TForm1::StringsIdent(String left, String* &right, int rightcount)
{
	for (int i = 0; i < rightcount; ++i)
		if (left.Compare(right[i]) == 0)
			return true;
	return false;
//}
//bool TForm1::StringsIdent(const String& left, TStringList* right)
//{
//	if (right == NULL) return false;
//
//	// IndexOf возвращает индекс строки или -1, если строка не найдена
//	return (right->IndexOf(left) != -1);
//}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteAllSubheadClick(TObject *Sender)
{
	if (Out->Lines->Count < 3)
	{
		Application->MessageBox(L"Enter three headings: 1) base, which it is necessary to search; 2) the heading of a sublist with which needs to be removed; 3) it is necessary to remove heading of a sublist, up to which (inclusive)"
		, L"Mass deleting", MB_OK+MB_ICONEXCLAMATION);
		return;
	}
	for (int i = 0; i < 3; i++)
		if (Out->Lines->Strings[i].Length() % 5 != 4)
		{
			Application->MessageBox(String(Out->Lines->Strings[i]+ L" is not heading or list of headings through a blank.").w_str()
			, L"Mass deleting", MB_OK+MB_ICONHAND);
			return;
		}
	String *header[3];
	int count[3];
	bool CanSelect = true;
	for (int i = 0; i < 3; i++)
	{
		count[i] = Out->Lines->Strings[i].Length() / 5 + 1;
		header[i] = new String[count[i]];
		for (int j = 0; j < count[i]; j++)
			header[i][j] = Out->Lines->Strings[i].SubString(j*5+1, 4);
	}
	for (int i = 0; i < List->RowCount; ++i)
	{
		if (StringsIdent(List->Cells[CHEADER][i], header[0], count[0]) == false)
			continue;
		ListSelectCell(Sender, 0, i, CanSelect);
		bool HasStart = false;
		for (int Row = 0; Row < List2->RowCount; ++Row)
		{
			if (StringsIdent(List2->Cells[CHEADER][Row], header[1], count[1]))
				HasStart = true;
			if (HasStart)
			{
				DeleteSublist(Row, i);
				if (StringsIdent(List2->Cells[CHEADER][Row], header[2], count[2]))
					HasStart = false;
			}
		}
	}
	for (int i = 0; i < 3; i++)
		delete [] header[i];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PrepareEClick(TObject *Sender)
{
	PrepareForEdit = true;
	PrepareFor("@#{}");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PrepareGameClick(TObject *Sender)
{
	PrepareForEdit = false;
	PrepareFor("{}@#");
}
//---------------------------------------------------------------------------
void TForm1::PrepareFor(char SYMBS[4])
{
	if (Deleted.empty() == false)
		return;
	RefStarts.clear();
	RefEnds.clear();
	char 	Name[5];	Name[4] = '\0';
	int 	Len;
	char 	curr;
	int 	kol = 0;
	for (int i = 0; i < List->RowCount; i++)
	{
		if (List->Cells[CHEADER][i].Compare("INFO") == 0)
		{
			int size = Sizes[i];
			if (size <= MNAMETOSUBLEN)
				continue;
			int end = List->Cells[CSTART][i].ToInt();
			if (end > EoF)
				break;
			fseek(file, end, SEEK_SET);
			fseek(file, MLENTOSLEN, SEEK_CUR);
			end += size;
			bool nach = false;
			while (ftell(file) < end)
			{
				fread(Name, 4, 1, file);
				fread(&Len, SLENSIZE, 1, file);
				if (strncmp(Name,"NAME", 4)==0)
				{
					for (int sym = 0; sym < Len; sym++)
					{
						fread(&curr, 1, 1, file);
						if (curr == SYMBS[0])
						{
							if (nach)
								tolog("Warning, double of { in " + List->Cells[CDATA][i] +" offset= "+ IntToStr((int)ftell(file)));
							nach = true;
							RefStarts.push_back(ftell(file)-1);
							kol++;
						} else
						if (curr == SYMBS[1])
						{
							if (!nach)
								tolog("Warning, double of } in " + List->Cells[CDATA][i] +" offset= "+IntToStr((int)ftell(file)));
							nach = false;
							RefEnds.push_back(ftell(file)-1);
							kol++;
						}
					}
					if (nach)
						tolog("Warning, none of } in " + List->Cells[CDATA][i] +" offset= "+ IntToStr((int)ftell(file)));
				} else
					fseek(file, Len, SEEK_CUR);	//Tell2 = ftell(file);
			}
		}
	}
	if (kol > 0)
	{
		tolog("Done. "+IntToStr(kol)+" substitutions. Click Save to complete.");
		Save->Enabled = true;
	}
	else
		tolog("No "+String(SYMBS[0]));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckConflictsClick(TObject *Sender)
{
	if (OpenDialog1->Execute() != ID_OK)
		return;
	FILE* conf = NULL;
	conf = _wfopen(OpenDialog1->FileName.w_str(), L"rb");
	if (!conf)
		return ShowMessage( "Cannot open binary file.");
	fseek(conf, 0, SEEK_END);
	int EoC = ftell(conf);
	if (EoC < 48)
		return ShowMessage( "File is empty.");
	Clear();
	Save->Enabled = false;
	LDele->Visible = false;
	for (int i=OpenDialog1->FileName.Length(); i>1; --i)
		if (OpenDialog1->FileName[i] == '\\')
		{
			PluginName = PluginName + " + "+OpenDialog1->FileName.SubString(i+1, OpenDialog1->FileName.Length());
			Form1->Caption = PluginName + " - TES parser";
			break;
		}
	bool dele = false;
	dele = (Application->MessageBox(L"Delete conflicts?", L"Option", MB_YESNO) == ID_YES);
	Opening = true;
	TES3Read->Enabled = true;
	if (NClearOut->Checked)
		Out->Lines->Clear();
	Out->Lines->Add("Check conflicts for " + PluginName);
	Out->Lines->Add("Size="+IntToStr(EoC));
	int Hard = 0;
	if ( (Hard=Application->MessageBox(L"Check full data?", L"Option", MB_YESNOCANCEL))== ID_CANCEL)
		return;
	char 	Name[5];	Name[4] = '\0';
	int 	Len;
	char 	sName[5]; sName[4] = '\0';
	long 	Tell;
	DoUpdateList(true);
	AddedRow = List->RowCount;
	int StartCon = List->RowCount;
	fseek(conf, 0, SEEK_SET);
	while ((Tell = ftell(conf)) < EoC)
	{
		fread(Name, 4, 1, conf);
		fread(&Len, 4, 1, conf);
		fseek(conf, MOVERLENTOSNAME, SEEK_CUR);
			if (Tes3 || strncmp(Name,"GRUP", 4)!=0 )
				fseek(conf, Len, SEEK_CUR);
			else //its GRUP
			{
				List->Cells[CSIZE][AddedRow-1] = MLENTOSLEN;
				Sizes[AddedRow-1] = MLENTOSLEN;
			}
		AddRow(Name, Len, Tell);
	}
	Out->Lines->Add("End on "+IntToStr((int)ftell(conf)));
	///закончен список
	//fseek(conf, POSNRECORDS, SEEK_SET);
	//fread(&RecordCount, 4, 1, conf);
	fseek(conf, 0, SEEK_SET);
	if (NShowData->Checked && Hard==ID_NO)
		RefreshData(conf, StartCon);
	if (List->RowCount > AddedRow)
		List->RowCount = AddedRow;
	int nConf = 0;
	Out->WordWrap = false;
	Wordwrap->Checked = false;
	if (Hard == ID_YES)
	{
		int MainLen;
		byte mainbuf[4096];
		byte buf[4096];
		for (int i = StartCon+1; i < List->RowCount; i++)
		{	//по конфликтному
			MainLen = Sizes[i];
			String Hed = List->Cells[CHEADER][i];
			if (MainLen > 4096)
				tolog(Hed+StrToInt(MainLen)+" too long");
			mainbuf[0] = 0;
			for (int j = 1; j < StartCon; ++j)
			{
				Len = Sizes[j];
				if (MainLen != Len)
					continue;
				if (Hed.Compare(List->Cells[CHEADER][j]) != 0)
					continue;
				if (mainbuf[0] == 0 && Len > 0)
				{
					fseek(conf, List->Cells[CSTART][i].ToInt() + MLENTOSLEN, SEEK_SET);
					fread(mainbuf, MainLen>4096?4096:MainLen, 1, conf);
				}
				fseek(file, List->Cells[CSTART][j].ToInt() + MLENTOSLEN, SEEK_SET);
				fread(buf, Len>4096?4096:Len, 1, file);
				int b = 0;
				for (; b < Len; b++)
					if (buf[b] != mainbuf[b])
						break;
				if (b == Len)
				{
					nConf++;
					if (Len == 4096)
						tolog(List->Cells[CSTART][i]+" MAYBIIDEN "+List->Cells[CSTART][j]);
					else
						tolog(List->Cells[CSTART][i]+" IDEN "+List->Cells[CSTART][j]);
				}
			}
		}
	}
	else
	{
		for (int i = StartCon+1; i < List->RowCount; i++)
		{
			//Out->Lines->Add(List->Cells[CDATA][i]+List->Cells[CHEADER][i]);
			String Dat = List->Cells[CDATA][i];
			String Hed = List->Cells[CHEADER][i];
			for (int j = 1; j < StartCon; ++j)
				if ( Dat.Compare(List->Cells[CDATA][j])==0)
				{
					if (Hed.Compare(List->Cells[CHEADER][j])==0)
					{
						Out->Lines->Add( Hed + "\t" + Dat +"\t"+List->Cells[CSTART][j]+"\t"+List->Cells[CSIZE][j] );
						nConf++;
						if (dele)
							DeleteRecord(j);
					}
					else
						Out->Lines->Add("Same identifiers\t"+Dat+"\t"+List->Cells[CHEADER][j]+"\t"+Hed+"\t"+List->Cells[CSTART][j] );
				}
		}
	}
	if (nConf == 0)
		Out->Lines->Add("No conflicts.");
	else
		Out->Lines->Add("--------"+IntToStr(nConf)+" conflicts.");
	DoUpdateList(false);
	NEnableList2Delete->Enabled = false;
	Save2->Visible = false;
	Save->Visible = false;
	Delete->Enabled = false;
	DelTrash->Enabled= false;
	Opening = false;
	fclose(conf);
}
//---------------------------------------------------------------------------
