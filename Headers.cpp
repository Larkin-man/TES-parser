//---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <Dialogs.hpp>
#pragma hdrstop

#include "Headers.h"
//---------------------------------------------------------------------------
void MHeader::Read(FILE* &file, int lensize)
{
	fread(&Name, 4, 1, file);
	fread(&Length, lensize, 1, file);
	if (Length > LENSTOP)
	{
		ShowMessage("Length is over "+IntToStr((int)LENSTOP));
		Length = LENSTOP / 4;
		LENSTOP *= 2;
		//fseek(file, -(4+lensize), SEEK_CUR);
		//return;
	}
	if (Length > Capacity)
	{
		Data = realloc(Data, Length);//TODO:bug22
		Capacity = Length;
	}
	fread(Data, Length, 1, file);
	if (Capacity > Length)
		//Data[Length].c = 0;
		//(char*)(Data)[Length] = 0;
		reinterpret_cast<char*>(Data)[Length] = 0;
}
//---------------------------------------------------------------------------
void MHeader::Write(FILE* &file)
{
	fwrite(&Name, 4, 1, file);
	fwrite(&Length, 4, 1, file);
	fwrite(Data, Length, 1, file);
}

void TRECORD3INT::Write(FILE* &file)
{
	fwrite(&Name, 4, 1, file);
	fwrite(&i, 4, 3, file);
}

bool TRECORD1INT1STR::Create()
{
	if (Length <= 1024) Data = new char[Length];
	else return false;
	return true;
}
void TRECORD1INT1STR::Write(FILE* &file)
{
	fwrite(&NAME, 4, 1, file);
	fwrite(&Length, 4, 1, file);
	fwrite(Data, Length, 1, file);
}
void FRMR::Set(FILE* &file, int FRMRoffset)
{
	fseek(file, FRMRoffset + 8, SEEK_SET);
	int Data;
	fread(&Data, 4, 1, file);
	mast = Data / 16777216;
	frmr = Data % 16777216;
}
//---------------------------------------------------------------------------
bool Coord6::TextToFloat3(String str)
{
	if (str.Length() < 2)
		return true;
	int p;
	str = str.Trim();
	while ((p = str.Pos('.')) > 0)
		str[p] = ',';
	while ((p = str.Pos("  ")) > 0)
		str.Delete(p,1);
	//EFinds->Text = str;
	p = str.Pos(' ');
	if (p <= 0)
		return true;
	String Word = str.SubString(1,p-1);
	x = Word.ToDouble();
	str.Delete(1,p);
	p = str.Pos(' ');
	if (p <= 0)
		return true;
	Word = str.SubString(1,p-1);
	y = Word.ToDouble();
	str.Delete(1,p);
	z = str.ToDouble();
	return false;
}
//---------------------------------------------------------------------------
void Coord6::TextToFloat6(String str)
{
	int st = 1;
	int coi = 0;
	for (int j = 2; j <= str.Length(); j++)
		if (str[j] == ' ')
		{
			all[coi] = str.SubString(st, j-st).ToDouble();
			st = j+1;
			coi++;
		}
	rz = str.SubString(st, str.Length()-st+1).ToDouble();
}
//---------------------------------------------------------------------------
void Coord6::Round()
{
	for (int i = 0; i < 3; i++)
		all[i] = round(all[i]);
}
//---------------------------------------------------------------------------
float Coord6::round(float x)
{
	if (x >= 0)
		return floor(x + 0.5); //в сторону меньшего
	return ceil(x - 0.5); //в сторону большего
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
