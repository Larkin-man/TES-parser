//---------------------------------------------------------------------------

#ifndef SpellParseerH
#define SpellParseerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
const int Skill[] = {0, 1, 2, 3, 4, 3, 4, 5};
const int Limit[] = {13,38,56,68,84,89,100};
const int G = sizeof(Limit)/sizeof(Limit[0]); //7
const UnicodeString SCHOOL[] = {"Изменение","Разрушение","Иллюзии","Мистицизм","Восстановление","Колдунство"};

union SPDT
{
	Byte Field[12];
	struct
	{
		unsigned int Type;
		unsigned int Cost;
		unsigned int Flags;
	};
};

union ENAM
{
	Byte Field[24];
	struct
	{
		Byte Type[4];
		unsigned int Range;
		unsigned int Area;
		unsigned int Duration;
		unsigned int MagnMin;
		unsigned int MagnMax;
	};
};


class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TButton *Button1;
	TListBox *Out;
	TEdit *Edit2;
	TEdit *Edit4;
	TEdit *Edit3;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	TStringList *file;
	TStringList *res;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
