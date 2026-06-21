//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SpellParseer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	file = NULL;
	res = NULL;
	SPDT ti;
	Out->Items->Add(sizeof (Byte));
	Out->Items->Add(sizeof (int));
	for (int i = 0; i < 12; ++i)
		ti.Field[i] = 0;
   ti.Field[4] = 255;
	Out->Items->Add(ti.Type);
	Out->Items->Add(ti.Cost);
	Out->Items->Add(ti.Flags);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (file == NULL)
	{
		file = new TStringList;
		res = new TStringList;
	}
	res->Clear();
	file->LoadFromFile("in.txt");
	UnicodeString Texts[4];	int Len[4];
	Texts[0] = Edit1->Text; Len[0] = Texts[0].Length() + 1;
	Texts[1] = Edit2->Text; Len[1] = Texts[1].Length() + 1;
	Texts[2] = Edit3->Text; Len[2] = Texts[2].Length() + 1;
	Texts[3] = Edit4->Text; Len[3] = Texts[3].Length() + 1;

	UnicodeString Spell;
	Char Id;
	SPDT spdt;
	int Find = 0;
	int Pos = 0;
	int skill = 5;
	res->Add("POS\tID\tNAME\tTYPE\tCOST\tFLAGS\tSCHOOL\tEFFECTIDX");
	for (int i = 0; i < file->Count; ++i)
		if (file->Strings[i].Pos(Texts[Find]) > 0)
		{
			Out->Items->Add(file->Strings[i]);
			switch (Find)
			{
			case 0:
				Spell = ++Pos;
				Spell += '\t';
			case 1:
				Spell += file->Strings[i].SubString(Len[Find], file->Strings[i].Length());
				break;
			case 2:
				//Out->Items->Add(file->Strings[i]);
				for (int b = 0; b < 11; ++b)
				{
					if (file->Strings[i][Len[2]+b] == ' ')
						spdt.Field[b] = 0;
					else
						spdt.Field[b] = file->Strings[i][Len[2]+b];
             	spdt.Field[11] = 0;
					//Out->Items->Add(spdt.Field[b]);
				}
				//Out->Items->Add(spdt.Cost);
				Spell += spdt.Type;  Spell += '\t';
				Spell += spdt.Cost;  Spell += '\t';
				Spell += spdt.Flags;
				break;
			case 3:
				//Out->Items->Add(Spell);
				Id = file->Strings[i][Len[2]];
            skill = 5;
				for (int g = 0; g < G; ++g)
					if (Id <= Limit[g])
					{
						skill = Skill[g];
						break;
					}
				Spell += SCHOOL[skill];
				res->Add(Spell+"\t"+IntToStr(Id));
				Find = 0;
				continue;
			default:				;
			}
			Spell += '\t';
			Find++;
		}
	res->SaveToFile("out.txt");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete file;
	delete res;
}
//---------------------------------------------------------------------------
