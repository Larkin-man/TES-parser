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
