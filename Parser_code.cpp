//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
//#include <float.h>
#include "Headers.h"
#pragma hdrstop
#include "TableLoader.cpp"
int const TAGSS = 33;
#include "Parser_code.h"

//---------------------------------------------------------------------------
//#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
//typedef char TAG[4];
//typedef Set<int, 0, 2147483647> TAGSET;
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
	for (int i = 0; i < 4; ++i)
	{
		List->ColWidths[i] = HeaderControl1->Sections->Items[i]->Width - 1;
		List2->ColWidths[i] = HeaderControl2->Sections->Items[i]->Width - 1;
	}
	List2->ColWidths[4] = HeaderControl2->Sections->Items[4]->Width - 1;
	OpenDialog1->InitialDir = GetCurrentDir();
	Export = NULL;
	//Out->Lines->Add(sizeof());
	Ready(false);
	nTypes = 0;//int const TAGSS = 18<-их колво 11 !!!!!!!!!!!
	AddTagType("INTV",'4'); AddTagType("NAME",'t');
	AddTagType("NAM5",'1'); AddTagType("FLTV",'g');
	AddTagType("MVRF",'i'); AddTagType("SPDT",'4');
	AddTagType("NAM0",'i'); AddTagType("NPDT",'4');
	AddTagType("CNDT",'g'); AddTagType("SCRI",'t');
	AddTagType("XSCL",'g'); AddTagType("MODL",'t');
	AddTagType("MAST",'t'); AddTagType("SCTX",'t');
	AddTagType("RGNN",'t'); AddTagType("SCHD",'t');
	AddTagType("AIDT",'t'); AddTagType("NPCS",'t');
	//skyrim
	AddTagType("SCVR",'t'); AddTagType("EDID",'t');
	AddTagType("FULL",'t'); AddTagType("MOD2",'t');
	AddTagType("MOD3",'t'); AddTagType("ICON",'t');
	AddTagType("NIFZ",'t'); AddTagType("MICO",'t');
	AddTagType("NAM1",'t'); AddTagType("MOD4",'t');
	AddTagType("ICO2",'t'); AddTagType("DESC",'t');
	AddTagType("ITXT",'t'); AddTagType("RDMP",'t');
	AddTagType("XATO",'t');
	if (nTypes != TAGSS)
		tolog("REPLACE TAGGS COUNT!!!!!!");
	types.IgnoreFirstString = false;
	types.IgnoreDelimitersPack = false;
	types.LoadFromResource("TEXTRES","ssss",&THeader,&TSubHeader,&TType,&TDescr);
	//for (int i = 0; i < types.RowCount; ++i)
	//	Out->Lines->Add(THeader[i]);
	cSubIndexes = 32;
	SubIndexes = new int[cSubIndexes];
	Opening = false;
	Expo = NULL;
	Clear();
	//ToLog(sizeof(MHeader::HeaderData));
	localeinstalled = false;
	Univ.Capacity = 0;
	PanelPRO->Visible = ProModeCK->Checked;
	SortingColumn = CSTART;
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
}
//---------------------------------------------------------------------------
void TForm1::Clear()
{
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
	Sizes.clear();
	Ends.clear();
	Edited.clear();
}
//---------------------------------------------------------------------------

void TForm1::AddTagType(char *name, char type, char *maintag)
{
	//TagTypes = new TAGTYPES[nTypes+1];
	strncpy(TagTypes[nTypes].Name, name, 4);
	TagTypes[nTypes].Type = type;
	if (maintag != NULL)
		strncpy(TagTypes[nTypes].MainTag, maintag, 4);
	nTypes++;
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
	if (ready == false && ListStore.size() > 0)
		for (std::map<int, PACK>::iterator el = ListStore.begin(); el != ListStore.end(); ++el)
		{
			delete el->second.col[0];
			delete el->second.col[1];
			delete el->second.col[2];
			delete el->second.col[3];
			delete el->second.col[4];
		}
	if (ready == false)
		ListStore.clear();
}
//---------------------------------------------------------------------------

void TForm1::Setup(int SubLenSize, int MainLenSize, int CountOffset)
{
	LENSIZE = 4;               //M O S
	MAINLENSIZE = MainLenSize;//12 16 20
	MOVERLENTOSNAME = MAINLENSIZE - 4;//main overlen to subname 8 12 16
	SLENSIZE = SubLenSize;
	MLENTOSLEN = LENSIZE + MAINLENSIZE;//16 20 24
	MNAMETOSUBLEN = MLENTOSLEN + 4; //20 24 28
	SecretLen = MOVERLENTOSNAME / 4;
	SecretData = new int[SecretLen];
	POSNRECORDS = CountOffset;
}
//---------------------------------------------------------------------------
void TForm1::AddRow(char *Header, int Length, long Start)
{
	if (List->RowCount <= AddedRow)
		List->RowCount += 8;
	List->Cells[CHEADER][AddedRow] = Header;
	List->Cells[CSIZE][AddedRow] = Length;
	List->Cells[CSTART][AddedRow] = Start;
	AddedRow++;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenBtnClick(TObject *Sender)
{
	if (OpenDialog1->Execute() != ID_OK)
		return;
	if (file)
	{
		fclose(file);
		Ready(false);
	}
	if (NRewrites->Checked)
	{
		file = _wfopen(OpenDialog1->FileName.w_str(), L"r+b");
		Save->Enabled = true;
	}
	else
		file = _wfopen(OpenDialog1->FileName.w_str(), L"rb");
	if (!file)
		return ShowMessage( "Cannot open binary file.");
	fseek(file, 0, SEEK_END);
	EoF = ftell(file);
	if (EoF < 48)
		return ShowMessage( "File is empty.");
	Clear();
	Save->Enabled = false;
	LDele->Visible = false;
	for (int i=OpenDialog1->FileName.Length(); i>1; --i)
		if (OpenDialog1->FileName[i] == '\\')
		{
			OpenedFileName = OpenDialog1->FileName;
			PluginName = OpenedFileName.SubString(i+1, OpenedFileName.Length());
			Form1->Caption = PluginName + " - TES parser";
			break;
		}

	Opening = true;
	if (Out->Lines->Count > 2)
		Out->Lines->Add("----------------------");
	Out->Lines->Add(PluginName + ", size="+IntToStr((int)EoF));
	char 	Name[5];	Name[4] = '\0';
	int 	Len;
	long 	Tell=0;
	float 	Ver;
	fseek(file, 0, SEEK_SET);
	fread(Name, 4, 1, file);
	fread(&Len, 4, 1, file);
	NextS->Caption = "Next="+String(Name)+"[" + IntToStr(Len) + "]";
	//fseek(file, -(4+SLENSIZE), SEEK_CUR);
	if (strncmp(Name, "TES3", 4) == 0)
	{
		Setup(4, 12, 320); //TES3
		fseek(file, MLENTOSLEN, SEEK_CUR);
		fread(&Ver, 4, 1, file);
		ToLog("Elder Scrolls 3 Morrowind file structure.");// ("+FloatToStrF(Ver,ffGeneral, 4,4)+").");
		Tes3 = true;
	}
	else
	{
		Tes3 = false;
		if (strncmp(Name, "TES4", 4) != 0)
		{
			tolog("Unknown file structure("+String(Name)+")");
			Tell = 666; //SubLenSize="+IntToStr(SLENSIZE)+" MainLenSize="+IntToStr(MAINLENSIZE));
		}
		int mls;
		for (mls = 4; mls < 44; mls += 4)
		{
			fread(Name, 4, 1, file);
			if (strncmp(Name, "HEDR", 4) == 0)
				break;
		}
		if (mls >= 44 && Tell == 666)
		{
			Ready(false);
			fclose(file);
			file = NULL;
			return;
		}
		fseek(file, 2, SEEK_CUR);
		fread(&Ver, 4, 1, file);
		POSNRECORDS = 8;
		if (Ver > 0.1 && Ver <= 0.97 && mls == 20)
		{
			ToLog("Fallout 3 file structure.");// ("+FloatToStrF(Ver,ffGeneral, 4,4)+").");
			Setup(2, mls, 34);
		}
		if (Ver > 0.97 && Ver <= 1.15 && mls == 16)
		{
			ToLog("Elder Scrolls 4 Oblivion file structure.");// ("+FloatToStrF(Ver,ffGeneral, 4,4)+").");
			Setup(2, mls, 30);
		}
		if (Ver > 1.32 && Ver <= 1.5 && mls == 20)
		{
			ToLog("Fallout New Vegas file structure.");// ("+FloatToStrF(Ver,ffGeneral, 4,4)+").");
			Setup(2, mls, 34);
		}
		if (Ver > 1.5 && Ver <= 1.9 && mls == 20)
		{
			ToLog("Elder Scrolls 5 Skyrim file structure.");// ("+FloatToStrF(Ver,ffGeneral, 4,4)+").");
			Setup(2, mls, 34); //sky=34
		}
		if (mls == 12)
		{
			ToLog("Elder Scrolls 3 Morrowind file structure.");// ("+FloatToStrF(Ver,ffGeneral, 4,4)+").");
			Setup(4, 12, 320); //TES3
			Tes3 = true;
		}
		if (POSNRECORDS == 8)
		{
			ToLog("Unknown file type "+IntToStr(MAINLENSIZE));
		}
	}
	TES3Read->Enabled = Tes3;
	fseek(file, MLENTOSLEN-4-SLENSIZE, SEEK_SET);
	Tell = MLENTOSLEN + Len;
	String Req = " ";
	char *st = NULL;
	while (ftell(file) < Tell)
	{
		fread(Name, 4, 1, file);
		fread(&Len, SLENSIZE, 1, file);
		if (strncmp(Name,"MAST", 4)==0)
		{
			st = file->curp;
			Req += String(st)+", ";
		}
		fseek(file, Len, SEEK_CUR);	//Tell2 = ftell(file);
	}
	Req.SetLength(Req.Length()-2);
	tolog("Required:"+Req);
	DoUpdateList(true);
	List2->RowCount = 1;
	List2->Rows[0]->Clear();
	fseek(file, 0, SEEK_SET);
	AddedRow = 0;
	while ((Tell = ftell(file)) < EoF)
	{
		fread(Name, 4, 1, file);
		fread(&Len, 4, 1, file);
		AddRow(Name, Len, Tell);
		fseek(file, MOVERLENTOSNAME, SEEK_CUR);
			if (Tes3 || strncmp(Name,"GRUP", 4)!=0 )
				fseek(file, Len, SEEK_CUR);
			else //its GRUP
			{
				List->Cells[CSIZE][AddedRow-1] = MLENTOSLEN;
				Sizes[AddedRow-1] = MLENTOSLEN;
			}
		if (Len <= 0 && BreakIf0Len->Checked)
		{
			BreakIf0Len->Checked = false;
			break;
		}
	}
	if (ftell(file) != EoF)
		Out->Lines->Add("End on "+IntToStr((int)ftell(file))+". This file may be corrupt.");
	if (List->RowCount > AddedRow)
		List->RowCount = AddedRow;
	Sizes.reserve(AddedRow);
	Ends.reserve(AddedRow);
	for (int i = 0; i < List->RowCount; i++)
	{
		Len = List->Cells[CSIZE][i].ToInt();
		Sizes.push_back(Len);
		Ends.push_back(List->Cells[CSTART][i].ToInt() + Len + 4 + MAINLENSIZE);
	}
	fseek(file, POSNRECORDS, SEEK_SET);
	fread(&RecordCount, 4, 1, file);
	if (NShowData->Checked)
	{
		if (List->RowCount >= 1)
			List->Cells[CDATA][0] = "Number of records: "+IntToStr(RecordCount);
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
	DoUpdateList(false);
	fseek(file, 0, SEEK_SET);
	//NextSClick(Sender);
	if (RecordCount != List->RowCount - 1)
		ToLog("Wrong Record Count " +IntToStr(RecordCount)+"("+IntToStr(List->RowCount - 1)+")");
	//fal 3 wrong
	Ready(true);
	SortingColumn = CSTART;
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
	static char DATA[MAXSIZE+1];
	for (int i = Start; i < List->RowCount; ++i)
	{
		int start = List->Cells[CSTART][i].ToInt();
		fseek(file, start, SEEK_SET);
		String Header = List->Cells[CHEADER][i];
		if (Header == "GRUP")
		{
			fseek(file, LENSIZE, SEEK_CUR);
			int Data[5] ={0,0,0,0,0};
			fread(&Data[0], 4, 1, file);
			fread(DATA, 4, 1, file); DATA[4] = 0;
			if (MOVERLENTOSNAME == 12) //obla
				fread(&Data[1], 4, 2, file);
			else
				fread(&Data[1], 4, 3, file);
			Data[4] = start + Data[0];
			List->Cells[CDATA][i] = String(DATA)+"["+IntToStr(Data[0])+
				+ "]("+IntToStr(Data[1])+","+IntToStr(Data[2])+","+IntToStr(Data[3])+") To "+IntToStr(Data[4]);
			continue;
		} else
		if (Header == "CELL" && Tes3)
		{
			fseek(file, MNAMETOSUBLEN, SEEK_CUR);
			int Length;

			fread(&Length, 4, 1, file);
			if (Length > MAXSIZE)
				Length = MAXSIZE;
			fread(DATA, Length, 1, file);
			int Data[5];
			fread(Data, 4, 5, file);
			pbit = reinterpret_cast<BITS*> (&(Data[2]));
			String Str;// = "w2i4m6b8";
			if (NExtreriorFlagsPrint->Checked)
			{
				if	(pbit->b2 == 1) Str += 'w';
				if	(pbit->b3 == 1) Str += 'i';
				if	(pbit->b7 == 1) Str += 'm';
				if	(pbit->b8 == 1) Str += 'b';
				static int iknow = 0x38; ///111000
				int idontknow = Data[2]&iknow;
				if (idontknow != 0)
					Str += idontknow;
			}
			if ((pbit) && (pbit->b1 == 1)) //interior
				//Interpret ti; //ti.i = Data[4];ЭТО floaт ДЕНСИТИ!!!!!!!!!!!!!!!!!
				List->Cells[CDATA][i] = String(DATA);// + "["+)float+"]"+Str;
			else
				List->Cells[CDATA][i] = String(DATA) + "("+IntToStr(Data[3])+","+IntToStr(Data[4])+")"+Str;
			continue;
		} else
		if (Header == "LAND" && Tes3)
		{
			fseek(file, MNAMETOSUBLEN, SEEK_CUR);
			int Data[6];
			fread(Data, 4, 6, file);
			List->Cells[CDATA][i] = "("+IntToStr(Data[1])+","+IntToStr(Data[2])+")"+IntToStr(Data[5]);
		} else
		if (Header == "PGRD" && Tes3)
		{
			fseek(file, MNAMETOSUBLEN, SEEK_CUR); //20
			//SkipSubheader();
			//int Length;
			//fread(&Length, SLENSIZE, 1, file);
			//fseek(file, Length + 4, SEEK_SET);
			int Data[3]; short Dat[2];
			fread(Data, 4, 3, file);
			fread(Dat, 2, 2, file);
			List->Cells[CDATA][i] = "("+IntToStr(Data[1])+","+IntToStr(Data[2])+")"+IntToStr((int)Dat[1]);
		} else
//		if (Tes3==false && Header == "NPC_")
//		{
//			fseek(file, 4+SLENSIZE, SEEK_CUR); //20
//			short Data[11];
//			fread(Data, 2, 11, file);
//			List->Cells[CDATA][i] = IntToStr((int)Data[0])+","+IntToStr((int)Data[1])+","+IntToStr((int)Data[2])+","
//				+IntToStr((int)Data[3])+","+IntToStr((int)Data[4])+","+IntToStr((int)Data[5])+","+IntToStr((int)Data[6])
//				+","+IntToStr((int)Data[7])+","+IntToStr((int)Data[8])+","+IntToStr((int)Data[9])+","+IntToStr((int)Data[10]);
//		}
		if (Header == "SKIL" || Header == "MGEF")
		{
			fseek(file, MNAMETOSUBLEN + SLENSIZE, SEEK_CUR);
			int INDX;
			fread(&INDX, SLENSIZE, 1, file);
			List->Cells[CDATA][i] = INDX;
		}
		else
		{
			fseek(file, MNAMETOSUBLEN, SEEK_CUR);
			int Len = 0;
			fread(&Len, SLENSIZE, 1, file);
			if (Len > MAXSIZE)
				Len = MAXSIZE;
			fread(DATA, Len, 1, file);
			DATA[Len] = 0;
			List->Cells[CDATA][i] = DATA;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	fclose(file);
	fclose(save);
	delete []SubIndexes;
	if (Expo)
	Expo->SaveToFile("Expo.txt");
	delete Expo;
	delete Export;
	delete what;
	std::map<int, PACK>::iterator el;
	for (el = ListStore.begin(); el != ListStore.end(); ++el)
	{
		delete el->second.col[0];
		delete el->second.col[1];
		delete el->second.col[2];
		delete el->second.col[3];
		delete el->second.col[4];
	}
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
		int Offset = List->Cells[CSTART][Row].ToInt();
		if	(Deleted.find(Offset) == Deleted.end())
		{
			Deleted.insert(Offset);
			DeletedSize += Sizes[Row];
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
	if (pos > EoF)
		pos = EoF;
	int i, end = -1;
	if (List->Row >= 0)
	{
		end = Ends[List->Row];
		if (pos > end || pos < List->Cells[CSTART][List->Row].ToIntDef(0))
			end = -1;
	}
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

void __fastcall TForm1::SPELreadClick(TObject *Sender)
{
	String exp;
	SPEL Spel;
	fread(&Spel, 4*4, 1, file);
	char4ToLog(Spel.Name);
	ToLog(Spel.i[0],"Length");
	fread(&Spel.NAME, 2*4, 1, file);
	char4ToLog(Spel.NAME);
	ToLog(Spel.Length,"Length");
	if	(Spel.Create() == false)
		return;
	fread(Spel.Data, Spel.Length, 1, file);
	ToLog(Spel.Data);
	exp = Spel.Data;
	fread(&Spel.FNAM, 2*4, 1, file);
	char4ToLog(Spel.FNAM);
	ToLog(Spel.FNAMLen,"Length");
	if (Spel.FNAMLen < 1024) Spel.FNAMData = new char[Spel.FNAMLen];
	fread(Spel.FNAMData, Spel.FNAMLen, 1, file);
	ToLog(Spel.FNAMData);
	exp += '\t';exp += Spel.FNAMData;
	fread(&Spel.SPDT, 13*4, 1, file);
	char4ToLog(Spel.SPDT);	ToLog(Spel.SPDTLen,"Length");
	ToLog(Spel.Type,"Type"); ToLog(Spel.Cost,"Cost"); ToLog(Spel.Flags,"Flags");
	char4ToLog(Spel.ENAM); ToLog(Spel.ENAMLen,"ENAMLen"); ToLog(Spel.Eff,"Eff");
	ToLog(Spel.Eff2,"Eff2");
	ToLog(Spel.Range,"Range"); ToLog(Spel.Area,"Area"); ToLog(Spel.Dura,"Dura");
	ToLog(Spel.Min,"Min"); ToLog(Spel.Max,"Max");
	exp += '\t';
	exp+=Spel.Type; exp+='\t';exp+=Spel.Cost; exp+='\t';exp+=Spel.Flags;
	exp+='\t';exp+=Spel.Eff; exp+='\t';exp+=Spel.Eff2; exp+='\t';exp+=Spel.Range;
	exp+='\t';exp+=Spel.Area; exp+='\t';exp+=Spel.Dura; exp+='\t';exp+=Spel.Min;
	exp+='\t';exp+=Spel.Max;
	if (LogUp)
		Out->Lines->Add(exp);
	else if (Export)
		Export->Add(exp);
	NextSClick(Sender);
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

void __fastcall TForm1::ListSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
	if (Opening)
		return;
	//1 Чекнем секрет поле maina
	int Offset = List->Cells[CSTART][ARow].ToInt();
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
	static String SelMainHedr;
	if (SelMainHedr.Compare(HEAD) != 0)
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
	std::map<int, PACK>::iterator el;
	if ( (el=ListStore.find(Offset)) != ListStore.end())
	{
		//int r = el->second.RowCount;
//		while (List2->RowCount < r)
//			List2->RowCount++;
		List2->RowCount = el->second.RowCount;
		List2->Cols[0]->Assign(el->second.col[0]);
		List2->Cols[1]->Assign(el->second.col[1]);
		List2->Cols[2]->Assign(el->second.col[2]);
		List2->Cols[3]->Assign(el->second.col[3]);
		List2->Cols[4]->Assign(el->second.col[4]);
	} else
	while (Pos < Ends[ARow])
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
				for (int i = 0; i < nTypes; ++i)
					if (strncmp(TagTypes[i].Name, Name, 4) == 0)
					{
						st = file->curp;
						List2->Cells[CTYPE][Row] = TagTypes[i].Type;
						switch (TagTypes[i].Type)
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
									i = nTypes;	break;
								}
							case '1':
								for (unsigned int i = 0; i < Len; i+=1)
									InterpretStr += IntToStr(*(byte*)&st[i])+" ";
								i = nTypes; break;
							case 'g': for (unsigned int i = 0; i < Len; i+=sizeof(float))
									InterpretStr += FloatToStr(*(float*)&st[i])+" ";
								i = nTypes; break;
							case '4':
								for (unsigned int i = 0; i < Len; i+=sizeof(int))
									InterpretStr += IntToStr(*(int*)&st[i])+" ";
								i = nTypes; break;
							case '2':
								for (unsigned int i = 0; i < Len; i+=2)
									InterpretStr += IntToStr(*(WORD*)&st[i])+" ";
								i = nTypes; break;
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
	if (NShowData->Checked && el == ListStore.end()) //Первый клик
	{
		PACK f;
		f.RowCount = Row;
		List2->RowCount = Row;
		f.col[0] = new TStringList;
		f.col[1] = new TStringList;
		f.col[2] = new TStringList;
		f.col[3] = new TStringList;
		f.col[4] = new TStringList;
		f.col[0]->Assign(List2->Cols[0]);
		f.col[1]->Assign(List2->Cols[1]);
		f.col[2]->Assign(List2->Cols[2]);
		f.col[3]->Assign(List2->Cols[3]);
		f.col[4]->Assign(List2->Cols[4]);
		//EFinds->Text = f.col[4]->Count;
		//Out->Lines->Assign(f.col[4]);
		ListStore.insert(std::pair<int, PACK> (Offset, f));
		//tolog("coxpaHullo:"+IntToStr(f.RowCount));
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

void TForm1::QuickSort(int iLo, int iHi) //рекурсивная
{
	int Lo, Hi;
	//String* Mid;
	Lo = iLo;
	Hi = iHi;
	String Mid(List->Cells[SortingColumn][(Lo+Hi)/2]);
	int intMid;
	if (CompareString==false)
	{
		intMid = Mid.ToIntDef(0);
		Mid.SetLength(0);
	}
	//Shifts+= ((Lo+Hi)/2);
	do
	{
		//printf("DO Lo[%d]=%d Hi[%d]=%d\n",Lo, M[Lo], Hi, M[Hi] );
		if (CompareString)
		{
			while (List->Cells[SortingColumn][Lo] < Mid)//(M[Lo] < Mid)
				Lo++;
			while (List->Cells[SortingColumn][Hi] > Mid)
				Hi--;
		}
		else
		{
			while (List->Cells[SortingColumn][Lo].ToIntDef(0) < intMid)//(M[Lo] < Mid)
				Lo++;
			while (List->Cells[SortingColumn][Hi].ToIntDef(0) > intMid)
				Hi--;
		}
		//printf("PO Lo[%d]=%d Hi[%d]=%d\n",Lo, M[Lo], Hi, M[Hi] );
		if (Lo <= Hi)
		{
			if (Lo != Hi)
			{
				//printf("Lo=%d Hi=%d\n", Lo, Hi);
				//print (M[Lo], M[Hi]);
				// VisualSwap(M[Lo], M[Hi], Lo, Hi);
				List->Cols[CHEADER]->Exchange(Lo,Hi);
				List->Cols[CSTART]->Exchange(Lo,Hi);
				List->Cols[CSIZE]->Exchange(Lo,Hi);
				List->Cols[CDATA]->Exchange(Lo,Hi);
				int t = Sizes[Lo];
				Sizes[Lo] = Sizes[Hi];
				Sizes[Hi] = t;
				t = Ends[Lo];
				Ends[Lo] = Ends[Hi];
				Ends[Hi] = t;
			}
			Lo++;
			Hi--;
		}
	}
	while (Lo <= Hi);
	if (Hi > iLo)
	{
		QuickSort(iLo, Hi);
		//Out->Lines->Add("more ");
	}
	if (Lo < iHi)
		QuickSort(Lo, iHi);
	//Out->Lines->Add("end;\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HeaderControl1SectionClick(THeaderControl *HeaderControl,
			 THeaderSection *Section)
{
	//Tick = ::GetTickCount();
	//static bool HasSorting[4] ={false,false,false,false};
	//TStrings *Curr = List->Rows[1];
	//Out->Lines->Exchange(1,2);
	//List->Rows->Exchange(1,3); не работает
	//List->Rows[1] = List->Rows[2];
	//List->Rows[2] = Curr; не работает обмен
	SortingColumn = Section->Index;
	CompareString = (SortingColumn==CHEADER||SortingColumn==CDATA);
	List->ScrollBars = ssNone;
	QuickSort(0, List->RowCount-1);
	//Tick = ::GetTickCount() - Tick; //Вычислить время расчета
	//Out->Lines->Add("Время расчета : "+FloatToStr(Tick)+" миллисек.");
	List->ScrollBars = ssVertical;
	 //HasSorting[SortingColumn] = !HasSorting[SortingColumn];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TestPClick(TObject *Sender)
{
	NTestMenuClick(Sender);
	for (int i = 0; i < nTypes; ++i)
		Out->Lines->Add(String(TagTypes[i].Name) +"\t"+ String(TagTypes[i].Type));
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

void __fastcall TForm1::rplusbClick(TObject *Sender)
{
	if (file)
	{
		ToLog("file->curp",file->curp);
		ToLog("file->buffer",file->buffer);
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

void __fastcall TForm1::DelDialsClick(TObject *Sender)
{
	if (List->Row < 0)
		return;
	if (SortingColumn != CSTART)
	{
		SortingColumn = CSTART;
		CompareString = false;
		List->ScrollBars = ssNone;
		QuickSort(0, List->RowCount-1);
		List->ScrollBars = ssVertical;
	}
	int Row = 0;
	int End = List->RowCount;
	if (List->Selection.Top != List->Selection.Bottom)
	{
		Row = List->Selection.Top;
		End = List->Selection.Bottom;
		ToLog("Finding trash DIAL's from "+IntToStr(Row)+" to "+IntToStr(End));
	}
	int DialRow;
	bool NeedDel = false;
	for (; Row < End; ++Row)
	{
		if (List->Cells[CHEADER][Row] == "DIAL")
		{
			if (NeedDel)
				DeleteRecord(DialRow);
			DialRow = Row;
			NeedDel = true;
			//DialStart = List->Cells[CSTART][Row].ToInt();
		}
		else
		if (List->Cells[CHEADER][Row] == "INFO")
		{
			int Offset = List->Cells[CSTART][Row].ToInt();
			if (Deleted.find(Offset) == Deleted.end()) //его нет в списке удаления
				NeedDel = false;
		}
	}
	if (LDele->Visible == false)
		tolog("It has no trash DIAL");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExportBtnClick(TObject *Sender)
{
	if (List->Row == -1)
		return ShowMessage("No one selected");
	static Char expo1[] = L"Export";
	static Char expo2[] = L"Export all";
	Char *Expo;
	bool expAll = false;
	if (List->Selection.Top == 0 && List->Selection.Bottom == List->RowCount - 1)
	{
		Expo = expo2;
		expAll = true;
	}
	else
		Expo = expo1;
	int type = 0;
	if (List->Selection.Top == List->Selection.Bottom)
		type = ID_NO;
	else
		if ( (type=Application->MessageBoxA(L"Export subheaders to string?", Expo, MB_YESNOCANCEL))== ID_CANCEL)
			return;
	int expTab = 0;
	if (expAll && type == ID_YES)
		if ( (expTab=Application->MessageBoxA(L"Export only table?", Expo, MB_YESNOCANCEL))== ID_CANCEL)
			return;
	if (expTab == ID_YES)
	{
		ExportBtn->Tag = 1;
		Export = new TStringList;
		Export->Append("Header\tOffset\tSize\tData");
		for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
			Export->Append(List->Cells[CHEADER][i]+"\t" + List->Cells[CSTART][i]+"\t"
				+ List->Cells[CSIZE][i]+"\t" + List->Cells[CDATA][i]);
		Export->SaveToFile(PluginName+".txt");
		ShowMessage("Saved:"+PluginName+".txt");
		return;
	}
	int expOff = 0;
	if ( (expOff=Application->MessageBoxA(L"Export Offset?", Expo, MB_YESNOCANCEL))== ID_CANCEL)
		return;
	int expSize = 0;
	if ( (expSize=Application->MessageBoxA(L"Export Size?", Expo, MB_YESNOCANCEL))== ID_CANCEL)
		return;
	Export = new TStringList;
	LogUp = false;
	bool stup = true;

	if (type == ID_NO) //Ровная таблица
	{
		ExportBtn->Tag = 1;
		int num = 1;
		int count = 1;
		if (expOff == ID_YES)
			if (expSize == ID_YES)
				Export->Append("№\tHeader\tName\tSubheader\tOffset\tSize\tType\tData");
			else Export->Append("№\tHeader\tName\tSubheader\tOffset\tType\tData");
		else if (expSize == ID_YES)
				Export->Append("№\tHeader\tName\tSubheader\tSize\tType\tData");
			else Export->Append("№\tHeader\tName\tSubheader\tType\tData");
		for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
		{
			String Head = IntToStr(num)+"\t"+List->Cells[CHEADER][i]+"\t"+List->Cells[CDATA][i]+"\t";
			ListSelectCell(Sender, 0, i, stup); //(TObject *Sender, int ACol, int ARow, bool &CanSelect)
			for (int j = 0; j < List2->RowCount; ++j)
				if (expOff == ID_YES)
					if (expSize == ID_YES)
						Export->Append(Head + List2->Cells[0][j] + "\t"+List2->Cells[1][j]
							+"\t"+List2->Cells[2][j]+"\t"+List2->Cells[3][j]+"\t"+List2->Cells[CDATA2][j]);
					else
						Export->Append(Head + List2->Cells[0][j] + "\t"+List2->Cells[1][j]
							+"\t"+List2->Cells[3][j]+"\t"+List2->Cells[CDATA2][j]);
				else
					if (expSize == ID_YES)
						Export->Append(Head + List2->Cells[0][j]	+"\t"+List2->Cells[2][j]
							+"\t"+List2->Cells[3][j]+"\t"+List2->Cells[CDATA2][j]);
					else
						Export->Append(Head + List2->Cells[0][j]+"\t"+List2->Cells[3][j]+"\t"+List2->Cells[CDATA2][j]);
			num++;
			count += List2->RowCount;
			if (count >= 50000)
			{
				Export->SaveToFile(PluginName+IntToStr(i-1)+".txt");
				Export->Clear();
				tolog("Saved:"+PluginName+IntToStr(i-1)+".txt");
				count = 1;
			}
		}
	}
	else //в строку subheaders
	{
		ExportBtn->Tag = 1;
		if (expOff == ID_YES)
			if (expSize == ID_YES)
				Export->Append("Header\tName\tSubheader[Offset]{Size}\tData");
			else Export->Append("Header\tName\tSubheader[Offset]\tData");
		else if (expSize == ID_YES)
				Export->Append("Header\tName\tSubheader{Size}\tData");
			else Export->Append("Header\tName\tSubheader\tData");
		for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
		{
			String Str = List->Cells[CHEADER][i]+"\t"+List->Cells[CDATA][i];
			ListSelectCell(Sender, 0, i, stup); //(TObject *Sender, int ACol, int ARow, bool &CanSelect)
			for (int j = 0; j < List2->RowCount; ++j)
				if (expOff == ID_YES)
					if (expSize == ID_YES)
						Str += "\t"+ List2->Cells[0][j] + "["+List2->Cells[1][j]
						+"]{"+List2->Cells[2][j]+"}\t"+List2->Cells[CDATA2][j];
					else
						Str += "\t"+ List2->Cells[0][j] + "["+List2->Cells[1][j]
						+"]\t"+List2->Cells[CDATA2][j];
				else
					if (expSize == ID_YES)
						Str += "\t"+ List2->Cells[0][j]
						+"{"+List2->Cells[2][j]+"}\t"+List2->Cells[CDATA2][j];
					else
						Str += "\t"+ List2->Cells[0][j]
						+"\t"+List2->Cells[CDATA2][j];
			Export->Append(Str);
		}
	}
	LogUp = true;
	Export->SaveToFile(PluginName+".txt");
	ShowMessage("Saved:"+PluginName+".txt");
	ExportBtn->Tag = 0;
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

void __fastcall TForm1::List2SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
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

void __fastcall TForm1::SPLMreadClick(TObject *Sender)
{
	//Go16Click(Sender);
	Byte NAM0, XNAM;
	char NAME[4], Name[4];
	int Length;
	String Data;
	Interpret Store;
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

void __fastcall TForm1::SwapCoordClick(TObject *Sender)
{
	if (List->Row < 0)
		return;
	String Base(List->Cells[CHEADER][List->Row]);
	int IsCell;
	if (Base == "CELL")
		IsCell = true;
	else if (Base == "PGRD")
		IsCell = false;
	else
		return ShowMessage("Нужно выделить ячейку CELL или PGRD");
	if (EFinds->Text.Length() < 2)
		return ShowMessage("Поле ввода должно содержать координаты X Y Z через пробел из редактора");
	Coord6 in;
	if	(in.TextToFloat3(EFinds->Text))
		return ShowMessage("Поле ввода должно содержать координаты X Y Z");
	int p;
	Out->Lines->Add(FloatToStr(in.x)+FloatToStr(in.y)+FloatToStr(in.z));
	if (IsCell)
	{
		Coord6 xyz;
		float xm1 = in.x-1; float xp1 = in.x+1;
		float ym1 = in.y-1; float yp1 = in.y+1;
		float zm1 = in.z-1; float zp1 = in.z+1;
		for (int i = 0; i < List2->RowCount; ++i)
			if (List2->Cells[CHEADER][i] == "DATA")
				if (List2->Cells[CSIZE][i].ToInt() == 24)
				{
					int adr = List2->Cells[CSTART][i].ToInt();
					fseek(file, adr + 8, SEEK_SET);
					fread(&xyz, sizeof(float), 3, file);
					if (xyz.x > xm1 && xyz.x < xp1)
						xyz.x = 0;
					else
						xyz.x = Check999(xyz.x - in.x);
					if (xyz.y > ym1 && xyz.y < yp1)
						xyz.y = 0;
					else
						xyz.y = Check999(xyz.y - in.y);
					if (xyz.z > zm1 && xyz.z < zp1)
						xyz.z = 0;
					else
						xyz.z = Check999(xyz.z - in.z);
					if (Wordwrap->Checked)
						Out->Lines->Add(IntToStr(adr)+":"+xyz.ToStr());
					fseek(file, -12, SEEK_CUR);
					fwrite(&xyz, sizeof(float), 3, file);
				}
	}
	else
	{
		int xyzpop[3];
		xyzpop[0] = in.x; xyzpop[1] = in.y; xyzpop[2] = in.z;
		int xyzh[4];
		for (int i = 0; i < List2->RowCount; ++i)
			if (List2->Cells[CHEADER][i] == "PGRP")
			{
				int adr = List2->Cells[CSTART][i].ToInt();
				fseek(file, adr + 8, SEEK_SET);
				int size = List2->Cells[CSIZE][i].ToInt();
				int end = adr + 8 + size;
				for (int p = adr + 8; p < end; p += 16)
				{
					fseek(file, p, SEEK_SET);
					fread(xyzh, sizeof(int), 3, file);
					//Out->Lines->Add(IntToStr(p)+":"+IntToStr(ddd)+"f"+IntToStr(xyzh[0])+"*"+IntToStr(xyzh[1])+"*"+IntToStr(xyzh[2]));
					xyzh[0] -= xyzpop[0];
					xyzh[1] -= xyzpop[1];
					xyzh[2] -= xyzpop[2];
					xyzh[0] = GetOkrugl(xyzh[0]);
					xyzh[1] = GetOkrugl(xyzh[1]);
					xyzh[2] = GetOkrugl(xyzh[2]);
					if (Wordwrap->Checked)
						Out->Lines->Add(IntToStr(p)+" To "+IntToStr(xyzh[0])+"*"+IntToStr(xyzh[1])+"*"+IntToStr(xyzh[2]));
					fseek(file, -12, SEEK_CUR);
					fwrite(xyzh, sizeof(int), 3, file);
				}
			}
	}
	EFinds->SelectAll();
	EFinds->SetFocus();
}
//---------------------------------------------------------------------------
int TForm1::GetOkrugl(int x)
{
	int pr, min;
	bool otr = x < 0 ? true : false;
	if (otr)
		x = -x;
	pr = x / 512;
	min = pr * 512;
	if ((x - min) < 32)
		return otr ? -min : min;
	else if ((min + 512 - x) < 32)
		return otr ? -(min + 512) : (min + 512);
	pr = x / 256;
	min = pr * 256;
	if ((x - min) < 28)
		return otr ? -min : min;
	else if ((min + 256 - x) < 28)
		return otr ? -(min + 256) : (min + 256);
	pr = x / 128;
	min = pr * 128;
	if ((x - min) < 20)
		return otr ? -min : min;
	else if ((min + 128 - x) < 20)
		return otr ? -(min + 128) : (min + 128);
	pr = x / 32;
	min = pr * 32;
	if ((x - min) <= 16)
		return otr ? -min : min; //макс расхожд 16
	else
		return otr ? -(min + 32) : (min + 32);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CloseClick(TObject *Sender)
{
	fclose (file);
}
//---------------------------------------------------------------------------

float TForm1::Check999(float x)
{
	float d = x - int(x);
	if (d > 0.98)
		return int(x)+1;
	if (d < -0.98)
		return int(x)-1;
	if (d < 0.02 && d > 0)
		return int(x);
	if (d > -0.02 && d < 0)
		return int(x);
	return x;
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

void __fastcall TForm1::DelTrashClick(TObject *Sender)
{
	for (int i = 0; i < List->RowCount; ++i)
		if (List->Cells[CHEADER][i].Compare("CELL") == 0)
		{
			if (i < List->RowCount-1 && List->Cells[CHEADER][i+1].Compare("PGRD") == 0)
				continue;
			int Row = 0;
			int Offset = List->Cells[CSTART][i].ToInt();
			std::map<int, PACK>::iterator el;
			if ( (el=ListStore.find(Offset)) != ListStore.end())
				Row = el->second.RowCount;
			else
			{
				unsigned int Len;
				fseek(file, Offset + 4 + MAINLENSIZE, SEEK_SET);
				int Pos = ftell(file);
				while (Pos < Ends[i])
				{
					fseek(file, 4, SEEK_CUR); //fread(Name, 4, 1, file);
					fread(&Len, SLENSIZE, 1, file);
					fseek(file, Len, SEEK_CUR);
					Pos = ftell(file);
					Row++;
				}
			}
			if (Row <= 4) //todo: to optimal
				DeleteRecord(i);
			else if (Row == 5)
				tolog("CELL 5 string:"+List->Cells[CDATA][i]);
		}
	List->Row = 0;
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
	int Offset = List2->Cells[CSTART][Row2].ToInt();
	if (Deleted.find(Offset) == Deleted.end())
	{
		Deleted.insert(Offset);
		int del = List2->Cells[CSIZE][Row2].ToIntDef(0);
		del += SLENSIZE;
		del += 4;
		DeletedSize += del;
		int MainLenOffset = List->Cells[CSTART][MainRow].ToInt() + 4;//char4
		int MainLen = Sizes[MainRow];
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
			List->Cells[CHEADER][MainRow] = Sizes[MainRow] - del;
		else
			List->Cells[CHEADER][MainRow] = List->Cells[CHEADER][MainRow].ToInt() - del;
		//List->Cells[CHEADER][MainRow] = List->Cells[CSIZE][MainRow]; //костыль
	}
}
//---------------------------------------------------------------------------
///subheaderlen = 4+4+len
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
	int Len;
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
//Удаляет из листа 2 все до следующего заголовка FRMR NAM0 DATA
void __fastcall TForm1::DeleteExtraDataClick(TObject *Sender)
{
	bool FindingName = true;
	int Row = List2->Row + 1;
	String Head;
	for (; Row < List2->RowCount; ++Row)
		if (FindingName)
		{
			if (List2->Cells[CHEADER][Row] == "NAME")
				FindingName = false;
		}
		else if (List2->Cells[CHEADER][Row] == "DATA")
			FindingName = true;
		else
		{
			Head = List2->Cells[CHEADER][Row];
			if (Head=="ANAM" || Head=="INTV" || Head=="NAM9" || Head=="CNAM"
				|| Head=="INDX" )
				Delete2(Row);
		}
	//return FindinList2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextTagContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	DeleteClick(Sender);
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
						isx = Data[0] / 8192;
						if (Data[0] < 0)	isx--;
						if (isx != Param[1])
							Out->Lines->Add(IntToStr(isx)+"!!!X:"+List2->Cells[CSTART][j]+"\t"+FloatToStr(Data[0]/8192)+"\t"+List2->Cells[CDATA2][j]);
						isy = Data[1] / 8192;
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

void __fastcall TForm1::setlocaleBtnClick(TObject *Sender)
{
	if (EFinds->Text.Length() <= 0)
		return ShowMessage("'Find' field is empty. Write a localization page in it. eg \".1251\"");
	String mes = L"Set the locale to \""+EFinds->Text+"\"?" ;
	switch (Application->MessageBoxA(mes.w_str(), L"setlocale", MB_YESNOCANCEL))
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
	int p=1, d;
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
	dele = (Application->MessageBoxA(L"Delete conflicts?", L"Option", MB_YESNO) == ID_YES);
	Opening = true;
	TES3Read->Enabled = true;
	if (NClearOut->Checked)
		Out->Lines->Clear();
	Out->Lines->Add("Check conflicts for " + PluginName);
	Out->Lines->Add("Size="+IntToStr(EoC));
	int Hard = 0;
	if ( (Hard=Application->MessageBoxA(L"Check full data?", L"Option", MB_YESNOCANCEL))== ID_CANCEL)
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
void TForm1::DoUpdateList(bool begin)
{
	if (begin)
	{
		List->Row = 0; //Tick = ::GetTickCount();
		Out->SetFocus();
		List->Cols[0]->BeginUpdate();
		List->Cols[1]->BeginUpdate();
		List->Cols[2]->BeginUpdate();
		List->Cols[3]->BeginUpdate();
		List->ScrollBars = ssNone; //List->Items->BeginUpdate();
	}
	else
	{
		List->Cols[0]->EndUpdate();
		List->Cols[1]->EndUpdate();
		List->Cols[2]->EndUpdate();
		List->Cols[3]->EndUpdate();
		List->ScrollBars = ssVertical;
		HeaderControl1Resize(NULL);
		//HeaderControl2Resize(NULL);
	}
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

void __fastcall TForm1::ExportScriptsBtnClick(TObject *Sender)
{
	Int8 expAll = ID_YES;
	if (List->Row != -1 || List->Selection.Bottom - List->Selection.Top + 1 != List->RowCount)
		if ( (expAll=Application->MessageBoxA(L"Export all scripts?", L"Export", MB_YESNOCANCEL))== ID_CANCEL)
			return;
	int i = ( expAll == ID_YES)? 0 : List->Selection.Top;
	int end = ( expAll == ID_YES)? List->RowCount : List->Selection.Bottom + 1;
	int tall;
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
					z += 1000.0;
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
//Проверяет равеноство строки left с любой из массива right
bool TForm1::StringsIdent(String left, String* &right, int rightcount)
{
	for (int i = 0; i < rightcount; ++i)
		if (left.Compare(right[i]) == 0)
			return true;
	return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteAllSubheadClick(TObject *Sender)
{
	if (Out->Lines->Count < 3)
	{
		Application->MessageBoxA(L"Enter three headings: 1) base, which it is necessary to search; 2) the heading of a sublist with which needs to be removed; 3) it is necessary to remove heading of a sublist, up to which (inclusive)"
		, L"Mass deleting", MB_OK+MB_ICONEXCLAMATION);
		return;
	}
	for (int i = 0; i < 3; i++)
		if (Out->Lines->Strings[i].Length() % 5 != 4)
		{
			Application->MessageBoxA(String(Out->Lines->Strings[i]+ L" is not heading or list of headings through a blank.").w_str()
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
				if (Mor.Size[i] == 24 & Mor.Subheader[i].Compare("DATA") == 0)
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

void __fastcall TForm1::OutDblClick(TObject *Sender)
{
	if (NAutoFind->Checked == false)
		return;
	EFinds->Text = Out->SelText.TrimRight();
	List2Enter(Sender);
	FindStrClick(Sender);
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

void __fastcall TForm1::WhatFindedChange(TObject *Sender)
{
	SearchingIn1 = SearchingIn2 = WhatFinded->ItemIndex;
	if (WhatFinded->ItemIndex == CDATA)
		SearchingIn2 = CDATA2;
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
	if ( (type=Application->MessageBoxA(ss.w_str()
		, L"Mass deleting", MB_YESNOCANCEL+MB_ICONQUESTION))== ID_CANCEL)
			return;
	if (type == ID_NO)
	{
		ss = (L"To remove all "+WhatFinded->Text
		+L" except for conterminous with the {"+Info.w_str()+L"}?");
		if ( (type=Application->MessageBoxA(ss.w_str()
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

void __fastcall TForm1::DropMasterClick(TObject *Sender)
{
	if (List->Cells[CHEADER][List->Row] != "CELL")
		return;
	int type = 0;
	if ( (type=Application->MessageBoxA(L"FRMR indexes - 0:New; 1:M; 2:T; 3:B; 4+: Another esm. To replace 4+ to 0?"
		, L"FRMR Indexes->0", MB_YESNOCANCEL)) == ID_CANCEL)
			return;
	if (type == ID_YES)
		type = 400;
	else
	if ( (type=Application->MessageBoxA(L"FRMR indexes 0:New; 1:M; 2:T; 3:B; 4+: Another esm. To replace ALL to 0?"
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
	if ( (type=Application->MessageBoxA(ss.w_str()
		, L"Mass deleting", MB_YESNOCANCEL+MB_ICONQUESTION))== ID_CANCEL)
			return;
	if (type == ID_NO)
	{
		ss = (L"To remove all "+WhatFinded->Text
		+L" except for conterminous with the {"+Info.w_str()+L"}?");
		if ( (type=Application->MessageBoxA(ss.w_str()
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

