//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <set>
#include <vector>
#include <algorithm>
#pragma hdrstop
#include "TableLoader.cpp"
int const  TAGSS = 18;
#include "Parser_code.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
//typedef char TAG[4];
//typedef Set<int, 0, 2147483647> TAGSET;
__fastcall TForm1::TForm1(TComponent* Owner)	: TForm(Owner)//,TAGSS(6)
{
	Tes4 = false;
	file = NULL;
	Moved.MVRF[0]='M';Moved.MVRF[1]='V';Moved.MVRF[2]='R';Moved.MVRF[3]='F';// "MVRF";
	Moved.Len1 = 4;
	Moved.CNDT[0]='C'; Moved.CNDT[1]='N'; Moved.CNDT[2]='D'; Moved.CNDT[3]='T';//"CNDT";
	Moved.Len2 = 8;
	save = NULL;
	Out->Clear();
//	ti.LoadFromFile("HEADERS.txt", "sicci", &tab.Name, &tab.Tes3
//		, &tab.Sost, &tab.Type, &tab.Tes4);
//	tab.TagCount = ti.RowCount - 1;
//	tab.LenLen = tab.Tes3;
	//ShowMessage(ti.RowCount);  	ShowMessage(Data.Name[2]);
	LENSIZE = 4;
	DEFAULTLENLEN = 12;
	LENTODEF = DEFAULTLENLEN - LENSIZE;
	LENSIZE2 = LENSIZE;
	for (char c = 'A'; c <= 'Z'; ++c)
		TagSymb << c;
	for (char c = '0'; c <= '9'; ++c)
		TagSymb << c;
	TagSymb << '_';
	//TAG tg;
	//tg[0] = 'f';
	//Out->Lines->Add(int(Moved.MVRF));
	//TAGSET fff;
	//fff << 22222;
	//fff.Contains((int)Moved.MVRF);
//	TextTags = new TStringList;
//	TextTags->LoadFromFile("TEXTTAGS.txt");
//   TextTags->Sorted = true;
//	TextTags->Duplicates = dupIgnore;
	for (int i = 0; i < 4; ++i)
	{
		List->ColWidths[i] = HeaderControl1->Sections->Items[i]->Width;
		List2->ColWidths[i] = HeaderControl1->Sections->Items[i]->Width;
	}
	OpenDialog1->InitialDir = GetCurrentDir();
	Export = NULL;
	LogUp = true;
	//Out->Lines->Add(sizeof());
	Ready(false);
	nTypes = 0;   //int const  TAGSS = 12<-их колво
	AddTagType("INTV",'4');
	AddTagType("NAME",'t');
	AddTagType("NAM5",'1'); AddTagType("FLTV",'f');
	AddTagType("MVRF",'4'); AddTagType("SPDT",'4');
	AddTagType("NAM0",'4'); AddTagType("NPDT",'4');
	AddTagType("CNDT",'f'); AddTagType("SCRI",'t');
	AddTagType("XSCL",'f'); AddTagType("MODL",'t');
	AddTagType("MAST",'t'); AddTagType("SCTX",'t');
	AddTagType("RGNN",'t'); AddTagType("SCHD",'t');
	AddTagType("AIDT",'t'); AddTagType("NPCS",'t');
	types.IgnoreFirstString = false;
	types.IgnoreDelimitersPack = false;
	types.LoadFromFile("TYPES.txt","ssss",&THeader,&TSubHeader,&TType,&TDescr);
	//for (int i = 0; i < types.RowCount; ++i)
	//	Out->Lines->Add(THeader[i]);
	SubIndexes = new int[4];
	cSubIndexes = 4;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenBtnClick(TObject *Sender)
{
	if (OpenDialog1->Execute() != ID_OK)
		return;
	if (file)
		fclose(file);
	if (Rewrite->Checked)
	{
		file = fopen (OpenDialog1->FileName.c_str(), "r+b");
		Save->Enabled = true;
	}
	else
		file = fopen (OpenDialog1->FileName.c_str(), "rb");
	if (!file)
		return;
	Save->Enabled = false;
	LDele->Visible = false;
	DeletedSize = 0;
//	String FileName = OpenDialog1->FileName;
	for (int i=OpenDialog1->FileName.Length(); i>1; --i)
		if (OpenDialog1->FileName[i] == '\\')
		{
			PluginName = OpenDialog1->FileName.SubString(i+1, OpenDialog1->FileName.Length());
			Form1->Caption = PluginName + " - TES parseer";
			break;
		}
	//Form1->Caption = FileName.SubString(1, FileName.Length()-4) + " - TES file parseer";
	if (!file)
		return ShowMessage( "Cannot open binary file");
//	if (rplusb->Checked)
//		save = fopen ((espname->Text+"2").c_str(), "w+b");
	//Save->Enabled = rplusb->Checked;
	HEDRRead->Enabled = true;
	fseek(file, 0, SEEK_END);
	EoF = ftell(file);
	fseek(file, 0, SEEK_SET);
	Out->Lines->Add("Size="+IntToStr(EoF));

	char 	Name[5];	Name[4] = '\0';
	fread(&Name, 4, 1, file);
	if (String(Name) == "TES4")
	{
		LENSIZE = 4;
		DEFAULTLENLEN = 16;
		LENTODEF = DEFAULTLENLEN - LENSIZE;
		//tab.LenLen = tab.Tes4;
		LENSIZE2 = 2;
		Tes4 = true;
	}
	fseek(file, 0, SEEK_SET);
	List->ScrollBars = ssNone; //List->Items->BeginUpdate();
	if (String(Name) == "TES3")
	{
		int 	Len, Len2;
		char 	sName[5]; sName[4] = '\0';
		long 	Tell, Tell2;
		row = 0;
		while ((Tell = ftell(file)) < EoF)
		{
			fread(&Name, 4, 1, file);
			fread(&Len, LENSIZE, 1, file);
			AddRow(Name, Len, Tell);
			//List->Items->Add(String(Name) +"[" + IntToStr(Len)+"]"+IntToStr((int)Tell) );
			fseek(file, LENTODEF, SEEK_CUR);
			if (Sost->Checked)
			{
				Tell2 = ftell(file);
				long Stop = ftell(file) + Len;
				while (Tell2 < Stop)
				{
					fread(&Name, 4, 1, file);
					fread(&Len2, LENSIZE, 1, file);
					AddRow(Name, Len2, Tell2);
					//List->Items->Add("-"+String(Name) +"[" + IntToStr(Len2)+"]"+IntToStr((int)Tell2) );
					fseek(file, Len2, SEEK_CUR);
					Tell2 = ftell(file);
				}
			} else
				fseek(file, Len, SEEK_CUR);
			//Tell = ftell(file);
		}
	}
	else //Не TES3
	{
		//uni = new TStringList;
		//uni->Sorted = true;
		//uni->Duplicates = dupIgnore;
		int end = EoF;
		while (ftell(file) < EoF)
			NextTagClick(Sender);
		EoF = end;
		//Out->Lines->AddStrings(uni);
	}
	Out->Lines->Add("End on "+IntToStr((int)ftell(file)));
	fseek(file, 0, SEEK_SET);
	if (List->RowCount > row)
		List->RowCount = row;
	List->ScrollBars = ssVertical; //List->Items->EndUpdate(); List->ScrollBars = ssNone;

	NextSClick(Sender);
	Deleted.clear();
	RefreshData();
	Ready(true);
	if (Rewrite->Checked)
		Rewrite->Enabled = false;
	else
   	Rewrite->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	fclose(file);
	fclose(save);
	//delete uni;
	delete TextTags;
	delete []SubIndexes;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::HEDRReadClick(TObject *Sender)
{  //(TES3 + HEDR ++?)
	fread(&Tes3, 324, 1, file);
	char4ToLog(Tes3.Name);
	ToLog(Tes3.i[0],"i[0]");
	ToLog(Tes3.i[1],"i[1]");
	ToLog(Tes3.i[2],"i[2]");
	char4ToLog(Tes3.Header);
	ToLog(Tes3.HeaderSize, "HEDR size");
	ToLog(Tes3.Version_Number,"Version number");
	ToLog(Tes3.Unknown,"Unknown");
	ToLog(Tes3.Author_Name,"Автор");
	ToLog(Tes3.Description,"Описание");  //*/
	ToLog(Tes3.NumRecords,"NumRecords");
	if (save)
		fwrite(&Tes3, sizeof(Tes3), 1, save);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MASTreadClick(TObject *Sender)
{
	fread(&Mast, Mast.SIZE1, 1, file);
	char4ToLog(Mast.NAME); //MAST
	ToLog(Mast.Length,"HMF length");  //*/
	Mast.Create();
	fread(Mast.Data, Mast.Length, 1, file);
	ToLog(Mast.Data);  //*/
	fread(&Mast.Fields, Mast.SIZE2, 1, file);
	char4ToLog(Mast.Fields.Name);
	ToLog(Mast.Fields.i[0], "Length");  //*/
	ToLog(Mast.Fields.i[1],"FileSize");  //*/
	ToLog(Mast.Fields.i[2]);  //*/
	if (save)
	{
		Mast.RECORD1INT1STR::Write(save);
		Mast.Fields.Write(save);
	}
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CellReadClick(TObject *Sender)
{
	fread(&Cell, Cell.SIZE, 1, file);
	char4ToLog(Cell.Name);
	ToLog(Cell.i[0],"AllFieldsLengtg");
	ToLog(Cell.i[1],"Unc0");
	ToLog(Cell.i[2],"Unc0");
	char4ToLog(Cell.NAME); //NAME
	ToLog(Cell.Length,"Length");
	Cell.Create();
	fread(Cell.Data, Cell.Length, 1, file);
	ToLog(Cell.Data);
	fread(&Cell.MData_Cell::Data, 4, 5, file);
	char4ToLog(Cell.MData_Cell::Data);
	ToLog(Cell.Data_Length[0],"Length");
	ToLog(Cell.Data_Length[1],"Это вроде за *");
	ToLog(Cell.GridX,"GridX");
	ToLog(Cell.GridY,"GridY");

	int Stop = Cell.i[0] - CELLNAMEDATALEN - Cell.Length;
	if (Stop == 0)
	{
		Out->Lines->Add("NO RGNN!");
		return;
	}
	else
		Read1STRClick(Sender); //RGNN
	Stop -= CELLRGNNLEN;
	Stop -= Stri.Length;
	if (Stop == 0)
		Out->Lines->Add("END!");
	else
		Out->Lines->Add(Stop);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Nam5ReadClick(TObject *Sender)
{
	fread(&Nam5, 12, 1, file);
	char4ToLog(Nam5.NAM5);
	ToLog(Nam5.NAM5_Length,"Length");
	String dd = "Colors:";
	dd += Nam5.Red; dd += ' ';
	dd += Nam5.Green; dd += ' ';
	dd += Nam5.Blue; dd += ' ';
	dd += Nam5.Null;
	ToLog(dd);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Nam0ReadClick(TObject *Sender)
{
	fread(&Nam0, 12, 1, file);
	char4ToLog(Nam0.NAM0); //NAM0
	ToLog(Nam0.Lengths[0],"Lengths");
	ToLog(Nam0.Lengths[1],"NumObjects");
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FRMRReadClick(TObject *Sender)
{
	fread(&FRMR, 3*4, 1, file);
	char4ToLog(FRMR.FRMR); //FRMR
	ToLog(FRMR.Field_Length,"Length");
	ToLog(FRMR.LoadRef,"LoadRef");
	if (WriteMVRFB->Checked)
	{
		fseek(file, -3*4, SEEK_CUR);
		Moved.MovedRef = FRMR.LoadRef;
		Moved.NewX = Cell.GridX;
		Moved.NewY = Cell.GridY + 2;
		fwrite(&Moved, sizeof(Moved), 1, file);
		//fseek(file, 3*4, SEEK_CUR);
		fseek(file, 0, SEEK_CUR);
	}
	Read1STRClick(Sender);
	/////OPTIONAL
	fread(&FRMRData.DATA, 4, 1, file);
	fseek(file, -4, SEEK_CUR);
	if ((FRMRData.DATA[0]=='X')&&(FRMRData.DATA[1]=='S')&&(FRMRData.DATA[2]=='C')&&(FRMRData.DATA[3]=='L') )
	{
		Xscl.Read(file, 4);
		char4ToLog(Xscl.Name);
		ToLog(Xscl.Length,"Length");
		ToLog(Xscl.Data[0].f,"Scale");
	}
	fread(&FRMRData, 8*4, 1, file);
	char4ToLog(FRMRData.DATA); //FRMR
	ToLog(FRMRData.FloatsLength,"Length");
	ToLog(FRMRData.X,"X"); ToLog(FRMRData.Y,"Y"); ToLog(FRMRData.Z,"Z");
	ToLog(FRMRData.RotX,"RotX"); ToLog(FRMRData.RotY,"RotY"); ToLog(FRMRData.RotZ,"RotZ");
	if (WriteMVRFB->Checked)
	{
		//fseek(file, -6*4, SEEK_CUR);
		//FRMRData.X
		//FRMRData.Y += 8192*2;
		//fwrit
	}
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LTEXreadClick(TObject *Sender)
{
	fread(&Ltex, Ltex.SIZE, 1, file);
	char4ToLog(Ltex.Name);
	ToLog(Ltex.i[0],"Length всех полей"); ToLog(Ltex.i[1]); ToLog(Ltex.i[2]);
	char4ToLog(Ltex.NAME);
	ToLog(Ltex.Length,"Length");
	Ltex.Create();
	fread(Ltex.Data, Ltex.Length, 1, file);
	ToLog(Ltex.Data,"Texture");
	ButtonGroup1ButtonClicked(Sender, INTFIELDS);
	ButtonGroup1ButtonClicked(Sender, STRFIELDS);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::INTVreadClick(TObject *Sender)
{
	fread(&Intv, 2*4, 1, file);
	char4ToLog(Intv.INTV);
	ToLog(Intv.INTV_Length,"INTV_Length");
	Intv.CreateINTV();
	fread(Intv.INTV_Data, Intv.INTV_Length, 1, file);
	for (int i = 0; i < Intv.INTV_Length/4; ++i)
		ToLog(Intv.INTV_Data[i]);
	NextSClick(Sender);
	//Read1STRClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Read3INTClick(TObject *Sender)
{
	fread(&Land, sizeof(RECORD3INT), 1, file);
	char4ToLog(Land.Name);
	ToLog(Land.i[0],"Length?");
	ToLog(Land.i[1]);
	ToLog(Land.i[2]);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Read1STRClick(TObject *Sender)
{
	fread(&Stri, 2*4, 1, file);
	char4ToLog(Stri.NAME);
	ToLog(Stri.Length,"Length");
	Stri.Create();
	fread(Stri.Data, Stri.Length, 1, file);
	ToLog(Stri.Data);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ReadIntsClick(TObject *Sender)
{
	fread(&Ints, 2*4, 1, file);
	char4ToLog(Ints.Name);
	ToLog(Ints.Length,"Length");
	Ints.Create();
	fread(Ints.Data, Ints.Length, 1, file);
	for (size_t i = 0; i < Ints.Length/sizeof(int); ++i)
		ToLog(Ints.Data[i]);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Read3BytesClick(TObject *Sender)
{
	fread(&Bytes3, 2*4, 1, file);
	char4ToLog(Bytes3.Name);
	ToLog(Bytes3.Length,"Length");
	Bytes3.Create();
	fread(Bytes3.Data, Bytes3.Length, 1, file);
	//for (size_t i = 0; i < Bytes3.Length/sizeof(int); ++i)
	//	ToLog(Bytes3.Data[i].X+" "+Bytes3.Data[i].Y+" "+Bytes3.Data[i].Z);
  	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonGroup1ButtonClicked(TObject *Sender, int Index)
{
	static int LENSmall = 4;
	static int LENBig = 12;
	if (UpDown1->Visible)
	{
		LENSmall = LenSize->Items->Strings[0].ToIntDef(4);
		LENBig = LenSize->Items->Strings[1].ToIntDef(12);
   }
	if (!Reinter->Checked)
	{
		if (LenSize->ItemIndex == 1)
			Univ.Read(file, LENBig);
		else
			Univ.Read(file, LENSmall); //TODO:bug2
	}
	char4ToLog(Univ.Name);
	ToLog(Univ.Length,"Length");
	switch (Index)
	{
		case INTFIELDS:
			for (unsigned int i = 0; i < Univ.Length/sizeof(int); ++i)
				ToLog(Univ.Data[i].i);
			break;
		case STRFIELDS:
			if (!Reinter->Checked)
				ToLog(&Univ.Data[0].c);
			else
			{
				for (unsigned int i = 0; i < Univ.Length/sizeof(char); ++i)
					ToLog(Univ.Data[i].c);
			}
			break;
		case FLOATFIELDS:
			for (unsigned int i = 0; i < Univ.Length/sizeof(float); ++i)
				ToLog(Univ.Data[i].f);
			break;
		case WORDFIELDS:
			for (int i = 0; i < Univ.Length/4; ++i)
			{
				ToLog((unsigned int)Univ.Data[i].uw[0]);
				ToLog((unsigned int)Univ.Data[i].uw[1]);
			}
			break;
		case BYTEFIELDS:
			//for (Byte *i = Univ.Data->b; i < Univ.Length; ++i)
			//	ToLog((unsigned int)i);
			for (int i = 0; i < Univ.Length/4; ++i)
			{
				ToLog((unsigned int)Univ.Data[i].b[0]);
				ToLog((unsigned int)Univ.Data[i].b[1]);
				ToLog((unsigned int)Univ.Data[i].b[2]);
				ToLog((unsigned int)Univ.Data[i].b[3]);
			}
			break;
	}
	NextSClick(Sender);
}
//---------------------------------------------------------------------------
void TForm1::DeleteRecord(int Row)
{
	if (Row >= 0)
	{
		int Offset = List->Cells[CSTART][Row].ToInt();
		if	(Deleted.find(Offset) == Deleted.end())
		//if (List->Cells[CSIZE][List->Row] != "-X-")
		{
			Deleted.insert(Offset);
			DeletedSize += List->Cells[CSIZE][Row].ToIntDef(0);
			List->Cells[CSIZE][Row] = "-X-";
			LDele->Visible = true;
			LDele->Caption = "Deleted Size="+IntToStr(DeletedSize)+" Count="+Deleted.size();
			Save->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteClick(TObject *Sender)
{
	if (List->Row != -1)
		for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
			DeleteRecord(i);
	//Out->Lines->Add(List->Selection.Top);
	//Out->Lines->Add(List->Selection.Bottom);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DelDatasClick(TObject *Sender)
{
	TStringList *Report = new TStringList;
	Report->Capacity = Out->Lines->Count;
	for (int i = 0; i < Out->Lines->Count; ++i)
	{
		int Row = List->Cols[CDATA]->IndexOf(Out->Lines->Strings[i]);
		if (Row >= 0)
		{
			int Offset = List->Cells[CSTART][Row].ToInt();
			if (DELEStarts.find(Offset) != DELEStarts.end())
			{
				Report->Add("FIND DELE:"+Out->Lines->Strings[i]);
				continue;
			}
			if (Deleted.find(Offset) == Deleted.end())
			{
				Deleted.insert(Offset);
				DeletedSize += List->Cells[CSIZE][Row].ToIntDef(0);
				List->Cells[CSIZE][Row] = "delete";
				Report->Add(Out->Lines->Strings[i]);
			}
		}
		else
			Report->Add("NO DATA:"+Out->Lines->Strings[i]);
	}
	Save->Enabled = true;
	LDele->Caption = "Deleted Size="+IntToStr(DeletedSize)+" Count="+Deleted.size();
	LDele->Visible = true;
	Out->Lines->Assign(Report);
	delete Report;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextSClick(TObject *Sender)
{
	char Header[4];
	//Out->Lines->Append(ftell(file));
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
		fread(&len, 4, 1, file);
		NextS->Caption = NextS->Caption + IntToStr(len) + "]";
		fseek(file, -8, SEEK_CUR);
	}
	else
	{
		NextS->Caption = "Next null or end.";
		fseek(file, -4, SEEK_CUR);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextTagClick(TObject *Sender)
{
	if (ftell(file) >= EoF)
	{
		NextS->Caption = "End";
		return;
	}
	char 	Name[5];	Name[4] = '\0';
	int 	Len, Len2;
	char 	sName[5]; sName[4] = '\0';
	long 	Tell = ftell(file), Tell2;


	fread(&Name, 4, 1, file);
	fread(&Len, LENSIZE, 1, file);
	AddRow(Name, Len, Tell);
	//List->Items->Add(String(Name) +"[" + IntToStr(Len)+"]"+IntToStr((int)Tell) );

	fseek(file, LENTODEF, SEEK_CUR);
	if (Name[0]=='G'&&Name[1]=='R'&&Name[2]=='U'&&Name[3]=='P')
		return;
	if (Sost->Checked)
	{
		Tell2 = ftell(file);
		long Stop = ftell(file) + Len;
		while (Tell2 < Stop)
		{
			fread(&Name, 4, 1, file);
			Len2 = 0;
			fread(&Len2, LENSIZE2, 1, file);
			AddRow(Name, Len2, Tell2);
			//List->Items->Add("-"+String(Name) +"[" + IntToStr(Len2)+"]"+IntToStr((int)Tell2) );
			fseek(file, Len2, SEEK_CUR);
			Tell2 = ftell(file);
		}
	} else
		fseek(file, Len, SEEK_CUR);
	//uni->Add(Name);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GoClick(TObject *Sender)
{
	fseek(file,ToE->Text.ToIntDef(0) - ftell(file), SEEK_CUR);
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
	fread(&Spel, 4*4, 1, file);
	char4ToLog(Spel.Name);
	ToLog(Spel.i[0],"Length");
	fread(&Spel.NAME, 2*4, 1, file);
	char4ToLog(Spel.NAME);
	ToLog(Spel.Length,"Length");
	Spel.Create();
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
	else
		Export->Add(exp);
	NextSClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveClick(TObject *Sender)
{
	if (Deleted.empty())
		return;
	Save->Enabled = false;
	String Nam = Form1->Caption;
	Nam = Nam.SubString(1, Nam.Pos(" - TES"));
	Nam = "CLEAR_" + Nam;
	save = fopen (Nam.c_str(), "wb");
	if (!save)
		return ShowMessage( "Cannot open binary file");
   fseek(file, 0, SEEK_END);
	EoF = ftell(file);
	Deleted.insert(EoF);
	fseek(file, 0, SEEK_SET);

	byte *mem;
	std::set<int>::iterator el = Deleted.begin();
	int cap = *el;
	mem = new byte[cap];
	fseek(file, 0, SEEK_SET);
	fread (mem, cap, 1, file);
	fwrite(mem, cap, 1, save);
	for (el++; el != Deleted.end(); ++el)
	{
		//Out->Lines->Add(*el);
		int Len;
		fseek(file, 4, SEEK_CUR);
		fread (&Len, LENSIZE, 1, file);
		fseek(file, LENTODEF+Len, SEEK_CUR);
		Len = *el - ftell(file);
		if (Len > cap)
		{
			mem = new byte[Len];
			cap = Len;
		}
		fread (mem, Len, 1, file);
		fwrite(mem, Len, 1, save);
	}
	delete [] mem;
	fclose (save);
	ToLog("Сохранено: "+Nam);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HeaderControl1SectionResize(THeaderControl *HeaderControl,
          THeaderSection *Section)
{
	List->ColWidths[Section->Index] = Section->Width;
	List2->ColWidths[Section->Index] = Section->Width;
	//if (Section->Index == CDATA-1)
	//	List->ColWidths[CDATA] = HeaderControl1->Sections->Items[CDATA]->Width;
		//List->DefaultRowHeight--;  testheight
		//Out->Lines->Add(List->DefaultRowHeight);
	//Out->Lines->Add(Section->Width);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RefreshClick(TObject *Sender)
{
	RefreshData();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
	static int LENSmall = 4;
	static int LENBig = 12;
	SubDescript->Visible = false;
	if (Tes4)
	{
      LENSmall = 2;
		LENBig = 16;
	}
	char 	Name[5];	Name[4] = '\0';
	//List2->Selection.
	if (ARow >= 0)
	{
		ToE->Text = List->Cells[CSTART][ARow];
		GoClick(Sender);

		//long end = ARow + 1 >= List->RowCount ? EoF : List->Cells[CSTART][ARow+1].ToInt();
		long end;
		if (List->Cells[CSIZE][ARow] == "-X-")
		{
			if (ARow < List->RowCount -1)
				end = List->Cells[CSTART][ARow+1].ToInt();
		}
		else
			end = List->Cells[CSTART][ARow].ToInt()+List->Cells[CSIZE][ARow].ToInt() + LENSmall + LENBig;
		//Out->Lines->Append(end);
		//List2->Cols->BeginUpdate();
		fseek(file,4+DEFAULTLENLEN,SEEK_CUR);
		//Out->Lines->Append(ftell(file));
		long Pos = ftell(file);
		int Row = 0;
		while (Pos < end)
		{
      	if (List2->RowCount < Row+1)
				List2->RowCount++;
			List2->Cells[CSTART][Row] = Pos;
			fread(&Name, 4, 1, file);
			fread(&Univ.Length, LENSmall, 1, file);
			List2->Cells[CHEADER][Row] = Name;
			List2->Cells[CSIZE][Row] = Univ.Length;
			List2->Cells[CDATA][Row] = "";
			char *st = file->curp;
			if (Name[1]=='N'&&Name[2]=='A'&&Name[3]=='M')
			{
				List2->Cells[CDATA][Row] = st;
			}
			else
			{
			int Len = Univ.Length > 64 ? 64 : Univ.Length;
			for (int i = 0; i < nTypes; ++i)
				if (strncmp(TagTypes[i].Name, Name, 4) == 0)
					switch (TagTypes[i].Type)
					{
						case 't': List2->Cells[CDATA][Row] = st;  break;  //ToLog(&Name[8]);
						case 'f': for (unsigned int i = 0; i < Univ.Length/sizeof(float); ++i)
								List2->Cells[CDATA][Row] = List2->Cells[CDATA][Row] + FloatToStr(st[i*4])+" ";
							break;
						case '4':
							for (unsigned int i = 0; i < Univ.Length/sizeof(int); ++i)
								List2->Cells[CDATA][Row] = List2->Cells[CDATA][Row] + IntToStr(st[i*4])+" ";
							break;
						case '2':
							for (int i = 0; i < Univ.Length/2; ++i)
								List2->Cells[CDATA][Row] = List2->Cells[CDATA][Row] + IntToStr(WORD(st[i*2]))+" ";
							break;
					}
			}
			//const int CDATA = 3;
			fseek(file,Univ.Length,SEEK_CUR);
			Pos = ftell(file);
			Row++;
		}
		List2->RowCount = Row;
      fseek(file,ToE->Text.ToIntDef(0) - ftell(file), SEEK_CUR);
	}
	//TYPES TABLES
	if (cSubIndexes < List2->RowCount)
	{
		SubIndexes = new int[List2->RowCount];
		cSubIndexes = List2->RowCount;
	}
	for (int i = 0; i < List2->RowCount; ++i)
		SubIndexes[i] = -1;
	String find;
	find = List->Cells[CHEADER][ARow];
	int m1=-1,m2=-1; //определим начало и конец для оптимизации
	for (int t = 0; t < types.RowCount; ++t)
		if (THeader[t].Compare(find) == 0)
		{
			if (m1 == -1)
				m1 = t;
			m2 = t;
		} else if (m1 != -1)
			break;
	if (m2 == -1)
		return;

	String Interpret;
	for (int sub = 0; sub < List2->RowCount; ++sub)
	{
		find = List2->Cells[CHEADER][sub];
		for (int t = m1; t <= m2; ++t)
			if (TSubHeader[t].Compare(find) == 0)
			{
				SubIndexes[sub] = t;
				//////REINTERPRET
				if (TType[t].Length() >= 1)
				{
					Interpret = "";
					fseek(file,
					(List2->Cells[CSTART][sub].ToInt() + 4), SEEK_SET);
					unsigned int Len;
					fread(&Len, LENSmall, 1, file);
					//Interpret = Len;
					//Len = List2->Cells[CSIZE][sub].ToInt();
					char *st = file->curp;
					switch (TType[t][1])
					{
						//case 't': List2->Cells[CDATA][Row] = st;  break;  //ToLog(&Name[8]);
						case 'f': for (unsigned int i = 0; i < Len/sizeof(float); ++i)
								Interpret = Interpret + FloatToStr(st[i*4])+".";
								List2->Cells[CDATA][sub] = Interpret;
							break;
						case 'i':
							for (unsigned int i = 0; i < Len/sizeof(int); ++i)
								Interpret = Interpret + IntToStr(st[i*4])+",";
								List2->Cells[CDATA][sub] = Interpret;
							break;
						case 'b':
							for (unsigned int i = 0; i < Len; ++i)
								Interpret = Interpret + IntToStr(st[i])+" ";
								List2->Cells[CDATA][sub] = Interpret;
							break;
						case '1':
							for (unsigned int i = 0; i < Len/2; ++i)
								Interpret = Interpret + IntToStr(WORD(st[i*2]))+"!";
								List2->Cells[CDATA][sub] = Interpret;
							break;
					}
				}
			}
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_DELETE)
		DeleteClick(Sender);
}
//---------------------------------------------------------------------------
void TForm1::QuickSort(int iLo, int iHi)  //рекурсивная
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
			//t = M[Lo];
			//M[Lo] = M[Hi];
			//M[Hi] = t;
      }
      Lo++;
		Hi--;
    }
  }
  while (Lo <= Hi);
  bool fool = false;

  if (Hi > iLo)
  {
	  fool=true;
	  QuickSort(iLo, Hi);
	  //Out->Lines->Add("more ");
  }
  if (Lo < iHi)
  {  //if (fool) Out->Lines->Add(" FOOL ");
	 QuickSort(Lo, iHi); }
   //Out->Lines->Add("end;\n");
}

void __fastcall TForm1::HeaderControl1SectionClick(THeaderControl *HeaderControl,
			 THeaderSection *Section)
{
	Tick = ::GetTickCount();
	//Out->Lines->Add(Section->Index);
	//TStrings *Curr = List->Rows[1];
	//int a,f;
	//Out->Lines->Exchange(1,2);
	//List->Rows->Exchange(1,3);  ythf,jnftn
	//List->Rows[1] = List->Rows[2];
	//List->Rows[2] = Curr;    неработает обмен
	SortingColumn = Section->Index;
	CompareString = (SortingColumn==CHEADER||SortingColumn==CDATA);
	//String  t;
	List->ScrollBars = ssNone;
	QuickSort(0,List->RowCount-1);
	Tick = ::GetTickCount() - Tick;  //Вычислить время расчета
	Out->Lines->Add("Время расчета : "+FloatToStr(Tick)+" миллисек.");
	List->ScrollBars = ssVertical;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HeaderControl1Resize(TObject *Sender)
{
	List->ColWidths[CDATA] = HeaderControl1->Sections->Items[CDATA]->Width-22;
	List2->ColWidths[CDATA] = HeaderControl1->Sections->Items[CDATA]->Width-22;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown1ChangingEx(TObject *Sender, bool &AllowChange, short NewValue,
			 TUpDownDirection Direction)
{
	if (Direction == updUp)
	{
		LenSize->Items->Strings[LenSize->ItemIndex] = UpDown1->Position;
	} else
	if (Direction == updDown)
	{
		LenSize->Items->Strings[LenSize->ItemIndex] = UpDown1->Position;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TestPClick(TObject *Sender)
{
	UpDown1->Visible = true;
	for (int i = 0; i < nTypes; ++i)
		Out->Lines->Add(TagTypes[i].Name + IntToStr(TagTypes[i].Type));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExportSPELClick(TObject *Sender)
{
	Export = new TStringList;
	LogUp = false;
	for (int i = List->Row; i < List->RowCount; ++i)
	{
		int go = List->Cells[CSTART][i].ToInt();
		fseek(file, go - ftell(file), SEEK_CUR);
		SPELreadClick(Sender);
	}
	Export->SaveToFile("EXPORT.txt");
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

void __fastcall TForm1::FindDELEClick(TObject *Sender)
{
	//if (List->Row < 0)
	//	return;
	std::vector<int>Offsets;
	Offsets.reserve(List->RowCount);
	for (int i = 0; i < List->RowCount; ++i)
		Offsets.push_back(List->Cells[CSTART][i].ToIntDef(0));//	 List->Cols[CSTART][i].ToInt() ;
	std::sort(Offsets.begin(),Offsets.end());
	//for (std::vector<int>::iterator el = Offsets.begin(); el != Offsets.end(); ++el)
	//	Out->Lines->Add(*el);
	char Find[] = "DELE";
	//int eof = feof(file);
	Out->Lines->Add("Finding:"+String(Find));
	Out->Lines->Add("EoF="+IntToStr(EoF));
	fseek(file, 0, SEEK_SET);
	//Out->Lines->Add("Size="+IntToStr(EoF));
	Out->ScrollBars = ssNone;
	Out->Lines->BeginUpdate();
	DELEStarts.clear();
	int prevOffset = -1;//Чтоб не выводжить повторно

	char 	c1,c2,c3,c4;
	while (ftell(file)<EoF)
	{
		fread(&c1, 1, 1, file);
		if (c1==Find[0])
		{
			//Out->Lines->Add(c1);
			fread(&c2, 1, 1, file);
			if (c2==Find[1])
			{
				//Out->Lines->Add(c2);
				fread(&c3, 1, 1, file);
				if (c3==Find[2])
				{
					//Out->Lines->Add(c3);
					fread(&c4, 1, 1, file);
					if (c4==Find[3])
					{
						int p = ftell(file)-4;
						int max = 0;
						for (std::vector<int>::iterator el = Offsets.begin(); el != Offsets.end(); ++el)
							if (*el > p)
							{
								el--;
								if (prevOffset != *el)
									Out->Lines->Add("DELE:"+IntToStr(*el)+". on pos:"+IntToStr(p));
								prevOffset = *el;
								DELEStarts.insert(*el);
								break;
							}
						//fread(&c2, 1, 1, file);
					}
				}
			}
		}
	}
	Out->ScrollBars = ssVertical;
	Out->Lines->EndUpdate();
	Out->Lines->Add("DELE Count="+IntToStr((int)DELEStarts.size()));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DelDialsClick(TObject *Sender)
{
	if (List->Row < 0)
		return;
	//int DialStart;
	int DialRow;
	bool NeedDel = false;
	for (int Row = List->Row; Row < List->RowCount; ++Row)
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
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExportBtnClick(TObject *Sender)
{
	Export = new TStringList;
	LogUp = false;
	if (List->Row == -1)
		return;
	int num = 1;
	Export->Add("№\tHeader\tSubheader\tOffset\tSize\tData");
	for (int i = List->Selection.Top; i <= List->Selection.Bottom; ++i)
	{
		//int go = List->Cells[CSTART][i].ToInt();
		//fseek(file, go - ftell(file), SEEK_CUR);
		//SPELreadClick(Sender);
		bool stup = true;
		String Head = IntToStr(num)+"\t"+List->Cells[0][i]+"\t";
		ListSelectCell(Sender, 0, i, stup);  //(TObject *Sender, int ACol, int ARow, bool &CanSelect)
		for (int j = 0; j < List2->RowCount; ++j)
			Export->Add(Head + List2->Cells[0][j] + "\t"+List2->Cells[1][j]
				+"\t"+List2->Cells[2][j]+"\t"+List2->Cells[3][j]);
		num++;
	}
	Export->SaveToFile(PluginName+".txt");
	ShowMessage("Saved:"+PluginName+".txt");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FindStrClick(TObject *Sender)
{
	String find = EFinds->Text;
	if (find.Length() <= 0)
		return;
	int row = 0;
	if (List->Row <= 0)
	{
		row = List->Cols[3]->IndexOf(find);
		if (row > -1)
		{
			List->Row = row;
			return;
		}
		row = 0;
	}
	else
		row = List->Row+1;
	for (int i = row; i < List->RowCount; ++i)
		if (List->Cols[3]->Strings[i].Pos(find) == 1)
		{
			List->Row = i;
			return;
		}
	for (int i = row; i > 0; --i)
		if (List->Cols[3]->Strings[i].Pos(find) == 1)
		{
			List->Row = i;
			return;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NextSContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled)

{
	Out->Lines->Add(ftell(file));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::List2SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
	if (ARow >= 0)
	{
		ToE->Text = List2->Cells[CSTART][ARow];
		if (SubIndexes[ARow] != -1)
		{
			SubDescript->Text = TDescr[SubIndexes[ARow]];
			SubDescript->Visible = true;
		} else
			SubDescript->Visible = false;

		GoClick(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
	PanelTags->Visible = CheckBox1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Go16Click(TObject *Sender)
{
	char uName[4];
	int uInt[3];
	fread(&uName, 4, 1, file);
	//fread(&uLength, LENBig, 1, file);//НЕльзя 12 а то сбивается uName
	fread(&uInt[0], LENSIZE, 1, file);
	fread(&uInt[1], LENSIZE, 1, file);
	fread(&uInt[2], LENSIZE, 1, file);
	char4ToLog(uName);
	ToLog(uInt[0],"Размер всех полей");
	Out->Lines->Add("??=("+IntToStr(uInt[1])+","+IntToStr(uInt[2])+")");
	NextSClick(Sender);
	ToE->Text = ftell(file);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SPLMreadClick(TObject *Sender)
{
	Go16Click(Sender);
	Byte NAM0, XNAM;
	char NAME[4], Name[4];
	int Length;
	String Data;
	Interpret Store;
	Out->Lines->BeginUpdate();
	int Coun[5];
	for (int i = 0; i < 5; ++i)
   	Coun[i] = 0;
	for (int i = 0; i < List2->RowCount; ++i)
	{
		fread(&Name, 4, 1, file);
		fread(&Length, 4, 1, file);
		char *st = file->curp;
		if (strncmp(Name, "NAME", 4) == 0)
		{
			Data = IntToStr(st[0]);
			//int ddd = st[0];
			//unsigned int nam2 = (unsigned int)st[0];
			//if (ddd < 0)
			// ToLogS(Data, IntToStr(i)+")NAME");
			List2->Cells[CDATA][i] = Data;//)+" "+IntToStr(int(nam2));
			Out->Lines->Add("");
			ToLogS(Data, IntToStr(i)+")NAME");
			Coun[0]++;
		}
		else if (strncmp(Name, "NAM0", 4) == 0)
		{
			Data = IntToStr(Byte(st[0]));
			List2->Cells[CDATA][i] = Data;
			ToLogS(Data, IntToStr(i)+")NAM0");
			Coun[1]++;
		}
		else if (strncmp(Name, "XNAM", 4) == 0)
		{
			Data = IntToStr(Byte(st[0]));
			List2->Cells[CDATA][i] = Data;
			ToLogS(Data, IntToStr(i)+")XNAM");
			Coun[2]++;
		}
		else if (strncmp(Name, "SPDT", 4) == 0)
		{
			int SPDT1 = (int)st[0];
			String Sub1(&st[4]);
			int SPDT2 = (int)st[44];
			int SPDT3 = (int)st[48];

			//void *flo = file->curp;
			//float SPDT11 = ((float*)&file->curp[0])[0];
			//float SPDT21 = ((float*)&file->curp[44])[0];
			//float SPDT31 = ((float*)&file->curp[48])[0];
			String Sub2(&st[52]);
			List2->Cells[CDATA][i] = Sub1;
			ToLogS(IntToStr(SPDT1), IntToStr(i)+")SPDT");
			ToLog(IntToStr(SPDT2));
			ToLog(IntToStr(SPDT3));
			ToLog(Sub1);
			ToLog(Sub2);
			Coun[3]++;
		}
		else if (strncmp(Name, "NPDT", 4) == 0)
		{
			String Sub1(&st[0]);
			int NPDT1 = st[44];
			int NPDT2 = st[48];
			int NPDT3 = st[52];
			List2->Cells[CDATA][i] = Sub1;
			ToLogS(NPDT1, IntToStr(i)+")NPDT");
			ToLog(NPDT2);
			ToLog(NPDT3);
			ToLog(Sub1);
			Coun[4]++;
		}
		fseek(file, Length, SEEK_CUR);
	}
	Out->Lines->Add("Count of NAME="+IntToStr(Coun[0])+" NAM0="
		+IntToStr(Coun[1])+" XNAM="+IntToStr(Coun[2])+" SPDT="
		+IntToStr(Coun[3])+" NPDT="+IntToStr(Coun[4]));
	Out->Lines->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PushCoordClick(TObject *Sender)
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
	Base = EFinds->Text;
	if (Base.Length() < 2)
		return ShowMessage("Поле ввода должно содержать координаты X Y Z через пробел из редактора");
	int p;
	Base = Base.Trim();
	while ((p = Base.Pos('.')) > 0)
		Base[p] = ',';
	while ((p = Base.Pos("  ")) > 0)
		Base.Delete(p,1);
	EFinds->Text = Base;

	p = Base.Pos(' ');
	if (p <= 0)
		return ShowMessage("Поле ввода должно содержать координаты X Y Z");
	String Word = Base.SubString(1,p-1);
	float x = Word.ToDouble();
	Base.Delete(1,p);
	p = Base.Pos(' ');
	if (p <= 0)
		return ShowMessage("Отсутствует координата Z");
	Word = Base.SubString(1,p-1);
	float y = Word.ToDouble();
	Base.Delete(1,p);
	float z = Base.ToDouble();

	Out->Lines->Add(FloatToStr(x)+FloatToStr(y)+FloatToStr(z));
	//Out->Lines->Add(IntToStr(GetOkrugl(x))+" "+IntToStr(int(x-GetOkrugl(x))));	
	if (IsCell)
	{
		float xyz[3];
		float xm1 = x-1; float xp1 = x+1;
		float ym1 = y-1; float yp1 = y+1;
		float zm1 = z-1; float zp1 = z+1;
		for (int i = 0; i < List2->RowCount; ++i)
			if (List2->Cells[CHEADER][i] == "DATA")
				if (List2->Cells[CSIZE][i].ToInt() == 24)
				{
					int adr = List2->Cells[CSTART][i].ToInt();
					fseek(file, adr + 8, SEEK_SET);
					fread(xyz, sizeof(float), 3, file);
					if (xyz[0] > xm1 && xyz[0] < xp1)
						xyz[0] = 0;
					else
						xyz[0] = Check999(xyz[0] - x);

					if (xyz[1] > ym1 && xyz[1] < yp1)
						xyz[1] = 0;
					else
						xyz[1] = Check999(xyz[1] - y);

					if (xyz[2] > zm1 && xyz[2] < zp1)
						xyz[2] = 0;
					else
						xyz[2] = Check999(xyz[2] - z);
					if (Wordwap->Checked)
						Out->Lines->Add(IntToStr(adr)+":"+FloatToStr(xyz[0])+"*"+FloatToStr(xyz[1])+"*"+FloatToStr(xyz[2]));
					fseek(file, -12, SEEK_CUR);
					fwrite(xyz, sizeof(float), 3, file);
				}
	}
	else
	{
		int xyzpop[3];
		xyzpop[0] = x; xyzpop[1] = y; xyzpop[2] = z;  
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
					if (Wordwap->Checked)
						Out->Lines->Add(IntToStr(p)+" To "+IntToStr(xyzh[0])+"*"+IntToStr(xyzh[1])+"*"+IntToStr(xyzh[2]));
					fseek(file, -12, SEEK_CUR); //БЕЗ ФСЕКА МЕЖДУ ЧТЕНИЕМ И ЗАПИСЬЮ НЕЛЬЗЯ
					fwrite(xyzh, sizeof(int), 3, file);
					//ddd = ftell(file); //Out->Lines->Add(IntToStr(p)+" : "+IntToStr(ddd));
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
		return otr ? -min : min;   //макс расхожд 16
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

void __fastcall TForm1::RewriteClick(TObject *Sender)
{
	PushCoord->Visible = Rewrite->Checked;
	EFinds->Width = 340;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::WordwapClick(TObject *Sender)
{
	Out->WordWrap = Wordwap->Checked;
}
//---------------------------------------------------------------------------

