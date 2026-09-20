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

void __fastcall TForm1::CheckCoordClick(TObject *Sender)
{
	int Param[3];
	bool Ext;
	float max, min;
	int maxi, mini;
	String maxs, mins;
	int isx, isy;
	float Data[6];
	bool CanSel = false;
	if (List->Row <= -1)
		return;
	for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
	{
		if (List->Cells[CHEADER][i] != "CELL")
			continue;
		maxi = -1;
		ListSelectCell(Sender, 0, i, CanSel);
		Out->Lines->Add("-------"+List->Cells[CDATA][i]);
		for (int j = 0; j < List2->RowCount; ++j)
			if (List2->Cells[CHEADER][j] == "DATA")
			{
				int Offset = List2->Cells[CSTART][j].ToInt();
				fseek(file, Offset + 4, SEEK_SET);
				int Length;
				fread(&Length, 4, 1, file);
				if (List2->Cells[CSIZE][j].ToInt() == 12) //location
				{
					fread(Param, 4, 3, file);
					pbit = reinterpret_cast<BITS*> (&(Param[0]));
					if ((pbit) && (pbit->b1 == 1)) //interior
						Ext = false;
					else
						Ext = true;
					min = INT_MAX;
					max = INT_MIN;
				} //coord
				else
				{
					fread(Data, 4, 6, file);
					tolog(FloatToStr(Data[0])+" "+FloatToStr(Data[1])+" "+FloatToStr(Data[2])+" "
						+FloatToStr(Data[3])+" "+FloatToStr(Data[4])+" "+FloatToStr(Data[5]));
					if (Ext)
					{
						isx = static_cast<int>(Data[0] / 8192.0f);
						if (Data[0] < 0)	isx--;
						if (isx != Param[1])
							Out->Lines->Add(IntToStr(isx)+"!!!X:"+List2->Cells[CSTART][j]+"\t"+FloatToStr(Data[0]/8192)+"\t"+List2->Cells[CDATA2][j]);
						isy = static_cast<int>(Data[1] / 8192.0f);
						if (Data[1] < 0)	isy--;
						if (isy != Param[2])
							Out->Lines->Add(IntToStr(isy)+" !!Y:"+List2->Cells[CSTART][j]+"\t"+FloatToStr(Data[1]/8192)+"\t"+List2->Cells[CDATA2][j]);
					}
					if (Data[2] > max)
					{
						max = Data[2];
						maxi = List2->Cells[CSTART][j].ToInt();
						if (List2->Cells[CHEADER][j-1] == "NAME")
							maxs = List2->Cells[CDATA2][j-1];
						else
							maxs = List2->Cells[CDATA2][j-2];
					}
					if (Data[2] < min)
					{
						min = Data[2];
						mini = List2->Cells[CSTART][j].ToInt();
						if (List2->Cells[CHEADER][j-1] == "NAME")
							mins = List2->Cells[CDATA2][j-1];
						else
							mins = List2->Cells[CDATA2][j-2];
					}
				}
			}
		if (maxi != -1 && maxi != mini)
		{
			Out->Lines->Add(maxs+"\tMax:"+FloatToStr(max)+" in "+IntToStr(maxi));
			Out->Lines->Add(mins+"\tMin:"+FloatToStr(min)+" in "+IntToStr(mini));
			Out->Lines->Add("Z Diff="+IntToStr((int)max-(int)min));
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckCoordContextPopup(TObject *Sender, TPoint &MousePos,
			 bool &Handled)
{
	if (CheckCoord->Tag == 0)
	{
		CheckCoord->Tag = 1;
		Out->Lines->Add("Auto check coordinates.");
	}
	else
		CheckCoord->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CellReadClick(TObject *Sender)
{
	MData_Cell Cell;
	fseek(file, ToE->Text.ToIntDef(0), SEEK_SET);
	fread(&Cell, Cell.SIZE, 1, file);
	char4ToLog(Cell.Name);
	ToLog(Cell.i[0],"AllFieldsLengtg");
	ToLog(Cell.i[1],"Unc1");
	ToLog(Cell.i[2],"Unc2");
	char4ToLog(Cell.NAME); //NAME
	ToLog(Cell.Length,"Length");
	if	(Cell.Create() == false)
		return;
	fread(Cell.Data, Cell.Length, 1, file);
	ToLog(Cell.Data);
	fread(&Cell.MData_Cell::Data, 4, 5, file);
	char4ToLog(Cell.MData_Cell::Data);
	ToLog(Cell.Data_Length[0],"Length");
	ToLog(Cell.Data_Length[1],"This is *");
	ToLog(Cell.GridX,"GridX");
	ToLog(Cell.GridY,"GridY");
	int Stop = Cell.i[0] - CELLNAMEDATALEN - Cell.Length;
	if (Stop == 0)
	{
		Out->Lines->Add("NO RGNN!");
		return;
	}
	Stop -= CELLRGNNLEN;
//	Stop -= Stri.Length; RECORD1INT1STR Stri;
	if (Stop == 0)
		Out->Lines->Add("END!");
	else
		Out->Lines->Add(Stop);
	List->Row = List->Row;
	NextSClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckCELLClick(TObject *Sender)
{
	if (List->Row < 0)
		return;
	if (List->Cells[CHEADER][List->Row].Compare("CELL") != 0)
		return;
	String CellName = List->Cells[CDATA][List->Row];
	int Idx;
	if ((Idx=CellName.Pos(')')) > 0)
		CellName.SetLength(Idx);
	Idx = 0;
	int Start = -1;
	int End = basecel.RowCount;
	if (LocFrmr.size() > 0) //Для округления координат Locationname/FRMR
	{
		if (Loc1.find(CellName) == Loc1.end())
			return;
	}
	else
	{
		for (int i = 0; i < basecel.RowCount; i++)
			if (Mor.N[i] != Idx) //нашел первую строку новой ячеки
			{
				Idx = Mor.N[i];
				if (Mor.Name[i].Compare(CellName) == 0) //ее название совпало
				{
					Start = i;
					i++;
					for (; i < basecel.RowCount; i++)
						if (Mor.N[i] != Idx)
						{
							End = i;
							break;
						}
					break;
				}
			}
		if (Start == -1)
			return tolog("Nothing");
		tolog("\t"+CellName);
		tolog("\t"+IntToStr(Start)+" do "+IntToStr(End));
	}
	Coord curr;

	std::set<String>::iterator currFrmr;
	for (int Row = 2; Row < List2->RowCount; Row++)
	{
		if (List2->Cells[CHEADER][Row].Compare("FRMR") == 0)
		{
			curr.FRMR = List2->Cells[CDATA2][Row].SubString(1, List2->Cells[CDATA2][Row].Pos(' ')-1).ToInt();
			if (curr.Dodt.Length() > 0)
				curr.Dodt.SetLength(0);
			if (LocFrmr.size() > 0)
				currFrmr = LocFrmr.find(CellName+curr.FRMR);
		} else
		if (List2->Cells[CHEADER][Row].Compare("NAME") == 0)
		{
			curr.Name = List2->Cells[CDATA2][Row];
		} else
		if (List2->Cells[CHEADER][Row].Compare("DATA") == 0
			&& List2->Cells[CSIZE][Row].ToInt() == 24)
		{
			if (LocFrmr.size() > 0)
			{
				if	(currFrmr != LocFrmr.end())
				{
					int adr = List2->Cells[CSTART][Row].ToInt();
					fseek(file, adr + 8, SEEK_SET);
					fread(&curr, sizeof(float), 3, file);
					curr.Round();
					fseek(file, -3 * sizeof(float), SEEK_CUR);
					fwrite(&curr, sizeof(float), 3, file);
					tolog(IntToStr(curr.FRMR)+curr.Name+curr.ToStr());
				}
				continue;
			}
			curr.TextToFloat6(List2->Cells[CDATA2][Row]);
			for (int i = Start; i < End; i++)
			{
				if (Mor.Size[i] == 24 && Mor.Subheader[i].Compare("DATA") == 0)
				{
					if (Coords[Mor.CoordRef[i]].FRMR == curr.FRMR)
					{
						String str = IntToStr(curr.FRMR)+" "+curr.Name;
						float sum = 0;
						for (int co = 0; co < 6; co++)
						{
							curr.all[co] -= Coords[Mor.CoordRef[i]].all[co];
							str += " " + FloatToStrF(curr.all[co], ffGeneral, 6, 6);
							if (co >= 3)
								curr.all[co] * 180 / 3.141593;
							sum += (curr.all[co] >= 0 ? curr.all[co] : -curr.all[co]);
						}
						if (curr.Dodt.Length() > 0 && Coords[Mor.CoordRef[i]].Dodt != curr.Dodt)
						{
							str += "\t"+Coords[Mor.CoordRef[i]].Dodt;
							str += ";\t\t"+curr.Dodt;
						}
						if (sum < 0.2)
							str = "NO CHANGED\t"+ str;
						else if (sum <= 2.0)
							str = "<2.0!\t"+ str;
						else
							str = ">"+FloatToStrF(sum, ffGeneral, 6, 6)+"\t"+ str;
						if (Coords[Mor.CoordRef[i]].Name != curr.Name)
							str = "   >>>"+Coords[Mor.CoordRef[i]].Name +" " + str;
						tolog(str);
					}
				}
			}
		} else
		if (List2->Cells[CHEADER][Row].Compare("DODT") == 0)
		{
			curr.Dodt = List2->Cells[CDATA2][Row];
			if (List2->Cells[CHEADER][Row+1].Compare("DNAM") == 0)
			{
				Row++;
				curr.Dodt = curr.Dodt + List2->Cells[CDATA2][Row];
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FindCELLmastClick(TObject *Sender)
{
	bool CanSelect = true;
	for (int i = 0; i < List->RowCount; ++i)
		if (List->Cells[CHEADER][i].Compare("CELL") == 0)
		{
			ListSelectCell(Sender, 0, i, CanSelect);
			for (int Row = 0; Row < List2->RowCount; ++Row)
				if	(List2->Cells[CHEADER][Row].Compare("FRMR") == 0)
				{
					int p = List2->Cells[CDATA][Row].Pos(' ');
					int master = List2->Cells[CDATA][Row].SubString(p+1, List2->Cells[CDATA][Row].Length() - p).ToInt();
					if (master > 3)
					{
						tolog(List->Cells[CDATA][i]+List2->Cells[CDATA][Row+1]);
						break;
					}
				}
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadCellsClick(TObject *Sender)
{
	if (FileExists("BASECELLS.txt"))
	{
		int p;
		for (int i = 0; i < Out->Lines->Count; i++)
		{
			p = Out->Lines->Strings[i].Pos('\t');
			String L(Out->Lines->Strings[i].SubString(1, p-1));
			String F(Out->Lines->Strings[i].SubString(p+1, Out->Lines->Strings[i].Length()-p));
			Loc1.insert(L);
			LocFrmr.insert(L+F);
		}
		tolog("Total "+IntToStr((int)LocFrmr.size()));
		return;
	}
	basecel.IgnoreFirstString = true;
	basecel.IgnoreDelimitersPack = false;
	//№	!Header!	Name	Subheader	Size	Type	Data
	basecel.LoadFromFile("BASECELLS.txt", "i0ssicsI", &Mor.N, &Mor.Name, &Mor.Subheader
		, &Mor.Size, &Mor.Type, &Mor.Data, &Mor.CoordRef);
	//tolog(Mor.Cell[32]); tolog(Mor.Subheader[32]); tolog(Mor.Data[32]); tolog(Mor.Size[32]);
	Coords.clear();
	Coords.reserve(basecel.RowCount / 5);
	Coord curr;
	///float *co[6] = {&xyz.x, &xyz.y, &xyz.z, &xyz.rx, &xyz.ry, &xyz.rz};
	int CurIdx = 0;
	int StartCell;
	for (int i = 0; i < basecel.RowCount; i++)
	{
		Mor.CoordRef[i] = -1;
		if (Mor.N[i] != CurIdx) //Для Mor.Name[i] экстерьеров
		{
			if ( (StartCell=Mor.Name[i].Pos(')')) > 0 )
				Mor.Name[i].SetLength(StartCell);
			CurIdx = Mor.N[i];
		}
		if (Mor.Size[i] == 4 && Mor.Subheader[i].Compare("FRMR") == 0)
		{
			curr.FRMR = Mor.Data[i].SubString(1, Mor.Data[i].Pos(' ')-1).ToInt();
			if (curr.Dodt.Length() > 0)
				curr.Dodt.SetLength(0);
		} else
		if (Mor.Subheader[i].Compare("NAME") == 0)
		{
			curr.Name = Mor.Data[i];
		} else
		if (Mor.Size[i] == 24 && Mor.Subheader[i].Compare("DATA") == 0)
		{
			curr.TextToFloat6(Mor.Data[i]);
			Mor.CoordRef[i] = Coords.size();
			Coords.push_back(curr);
		} else
		if (Mor.Size[i] == 24 && Mor.Subheader[i].Compare("DODT") == 0)
		{
			curr.Dodt = Mor.Data[i];
			if (Mor.Subheader[i+1] == "DNAM")
			{
				i++;
				curr.Dodt = curr.Dodt + Mor.Data[i];
			}
		}
	}
	tolog("Total "+IntToStr((int)basecel.RowCount)+String(" loaded."));
	tolog("FRMR count = "+IntToStr((int)Coords.size()));
//	for (std::vector<Coord>::iterator el=Coords.begin(); el != Coords.end(); ++el)
//		Out->Lines->Add(IntToStr(el->FRMR)+el->Name+FloatToStr(el->x)+"="+FloatToStr(el->all[0]));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MVRFClick(TObject *Sender)
{
	int ende = 0;
	ShowAll = false;
	bool CanSelect = true;
	String tcell("CELL"); String tmvrf("MVRF");
	tolog("Finding MVRF & CNDT ...");
	std::vector<Exterior> Locs;
	Exterior Curr;
	Locs.reserve(16);
	for (int i = 0; i < List->RowCount; ++i)
	{
		if (ende > 999)
			return;
		if (List->Cells[CHEADER][i].Compare(tcell) != 0)
			continue;
		Curr.X = List->Cells[CDATA][i].Pos('(');
		if (Curr.X == 0)
			continue; //interior
		String mdata = List->Cells[CDATA][i].SubString(Curr.X+1, 128);
		Curr.Y = mdata.Pos(',');
		Curr.X = mdata.SubString(1, Curr.Y-1).ToInt();
		Curr.Y = mdata.SubString(Curr.Y+1, mdata.Pos(')')-Curr.Y-1).ToInt();
		Curr.MainLenOffset = List->Cells[CSTART][i].ToInt()+4;
		Curr.MainLen = Sizes[i];
		Curr.PasteOffset = -1;
		ListSelectCell(Sender, 0, i, CanSelect);
		bool HasMvrf = false;
		for (int Row = 0; Row < List2->RowCount; ++Row)
		{
			if (Curr.PasteOffset == -1 && List2->Cells[CHEADER][Row].Compare("RGNN") == 0)
			{
				if (List2->RowCount > Row+1)
				{
					if (List2->Cells[CHEADER][Row+1].Compare("NAM5") == 0)
					{
						Row++;
						if (List2->RowCount <= Row+1)
						{
							Curr.PasteOffset = 4+LENSIZE + List2->Cells[CSTART][Row].ToInt()+List2->Cells[CSIZE][Row].ToInt();
							continue;
						}
					}
					if (List2->Cells[CHEADER][Row+1].Compare("NAM0") == 0)
					{
						Curr.PasteOffset = 4+LENSIZE + List2->Cells[CSTART][Row+1].ToInt()+List2->Cells[CSIZE][Row+1].ToInt();
						continue;
					}
				}
				Curr.PasteOffset = 4+LENSIZE + List2->Cells[CSTART][Row].ToInt()+List2->Cells[CSIZE][Row].ToInt();
				continue;
			}
//		continue;
			if	(List2->Cells[CHEADER][Row].Compare(tmvrf) == 0)
			{
				ende++;
				HasMvrf = true;
				Out->Lines->Append("");
				Out->Lines->Add(List->Cells[CDATA][i]+"\t"+List2->Cells[CDATA2][Row]);
				continue;
			}
			if (HasMvrf && List2->Cells[CHEADER][Row].Compare("NAME") == 0)
				Out->Lines->Add(List2->Cells[CDATA2][Row]);
			if (HasMvrf && List2->Cells[CHEADER][Row].Compare("CNDT") == 0)
				Out->Lines->Add(List2->Cells[CDATA2][Row]);

			if (HasMvrf && List2->Cells[CHEADER][Row].Compare("DELE") == 0)
			{
				if (NEnableList2Delete->Checked)
					if (Reinter->Checked)
					{
						DeleteItem ea(List->Cells[CSTART][i].ToInt()+4 , List->Cells[CSIZE][i].ToInt()
							, List2->Cells[CSTART][Row].ToInt(), -32); //int mlo, int ml, int o, int s)
						byte *store = new byte[32];
						//memcpy(store, Data, 32);
						ea.Addon = store;
						SubDelete.push_back(ea);
					}
					else
						DeleteSublist(Row, i);
				HasMvrf = false;
				continue;
			} else
			if (HasMvrf && List2->Cells[CHEADER][Row].Compare("DATA") == 0)
			{
				if (NShowData->Checked == false)
					continue;
				Out->Lines->Add(List2->Cells[CDATA2][Row]);
				if (NEnableList2Delete->Checked) //у CNDT z+=1000
				{
					fseek(file, List2->Cells[CSTART][Row].ToInt()+16, SEEK_SET);
					float z;
					fread(&z, sizeof(float), 1, file);
					z += 1000.0f;
					fseek(file, List2->Cells[CSTART][Row].ToInt()+16, SEEK_SET);
					fwrite(&z, sizeof(float), 1, file);
				}
				HasMvrf = false;
				continue;
			}
		}
		if (Curr.PasteOffset == -1)
			tolog("Error in CELL, no RGNN:" + List->Cells[CDATA][i]);
		else
			Locs.push_back(Curr);
	}
	for (std::vector<Exterior>::iterator el = Locs.begin(); el != Locs.end(); ++el)
		tolog(IntToStr(el->X)+" "+IntToStr(el->Y)+" "+IntToStr(el->MainLenOffset)+" "
			+IntToStr(el->MainLen)+" "+IntToStr(el->PasteOffset));
	tolog("Construct Exteriors ...");
	ShowAll = true;
	if (NEnableList2Delete->Checked)
	{
		LDele->Visible = true;
		Save2->Enabled = true;
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
