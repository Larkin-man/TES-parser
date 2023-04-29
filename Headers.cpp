//---------------------------------------------------------------------------
#include <stdio.h>
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
#pragma package(smart_init)
