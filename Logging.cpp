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
