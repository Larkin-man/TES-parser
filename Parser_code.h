//---------------------------------------------------------------------------

#ifndef Parser_codeH
#define Parser_codeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ButtonGroup.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

const int INTFIELDS = 0;
const int STRFIELDS = 1;
const int FLOATFIELDS = 2;
const int WORDFIELDS = 3;
const int BYTEFIELDS = 4;
const int LENSIZE12 = 5;
////////////////////////
const int CELLNAMEDATALEN = 28;
const int CELLRGNNLEN = 8;
static int LENSTOP = 9000;

const int CHEADER = 0;
const int CSTART = 1;
const int CSIZE = 2;
const int CDATA = 3;
//const int DEFAULTLENLEN = 12;

struct MHeader
{
	char Name[4];
	int Length;
	union HeaderData
	{
		int i;
		float f;
		short w[2];
		unsigned short uw[2];
		Byte b[4];
		char c;
	} *Data;
	void Read(FILE* &file, int lensize)
	{
		fread(&Name, 4, 1, file);
		fread(&Length, lensize, 1, file);
		if ((Length > LENSTOP)||(Length < 0))
		{
			ShowMessage("Length is over "+IntToStr(LENSTOP));
			LENSTOP *= 4;
         Length = 0;
			fseek(file, -8, SEEK_CUR);
			return;
      }
		Data = (HeaderData*)realloc(Data, Length);//TODP:bug22
		fread(Data, Length, 1, file);
	}
	void Write(FILE* &file)
	{
		fwrite(&Name, 4, 1, file);
		fwrite(&Length, 4, 1, file);
		fwrite(Data, Length, 1, file);
   }
	MHeader() { Length = 0; Data=NULL; }
	~MHeader() { free(Data); }
};

typedef struct TRECORD3INT
{
	char	Name[4];//CELL
	int	i[3];
	void Write(FILE* &file)
	{
		fwrite(&Name, 4, 1, file);
		fwrite(&i, 4, 3, file);
	}
} RECORD3INT;

typedef struct TRECORD1INT1STR
{
	char	NAME	[4];
	int	Length;//	4
	char*	Data;//	4
	void 	Create() { if (Length < 1024) Data = new char[Length]; }
	void Write(FILE* &file)
	{
   	fwrite(&NAME, 4, 1, file);
		fwrite(&Length, 4, 1, file);
		fwrite(Data, Length, 1, file);
   }
	TRECORD1INT1STR() { Data = NULL; }
	~TRECORD1INT1STR() { delete [] Data; }
} RECORD1INT1STR;

typedef struct TRECORD4INT1STR : public RECORD3INT, public RECORD1INT1STR
{
	const static int SIZE = 6*4;
} RECORD4INT1STR;

typedef struct TRECORD1INT1STR3INT : public RECORD1INT1STR
{
	const static int SIZE1 = 2*4;
	const static int SIZE2 = 4*4;
	RECORD3INT Fields;
} RECORD1INT1STR3INT; //Mast

typedef struct TINTV
{
	char	INTV	[4]; //INTV
	int	INTV_Length;//	4
	int*	INTV_Data;
	TRECORD1INT1STR Data;
	void CreateINTV() { INTV_Data = new int[INTV_Length/sizeof(int)]; }
	TINTV() { INTV_Data = NULL; }
	~TINTV() { delete [] INTV_Data; }
} INTV;

struct Tes3Header : public TRECORD3INT
{
char	Header	[4]; //HEDR
int	HeaderSize;//	[4];
float	Version_Number;//	[4];
int	Unknown;	//[4];
char	Author_Name	[32];
char	Description	[256];
int   NumRecords;
} Tes3;

struct TRECORDINTMAS
{
	char	Name	[4]; //INTV
	int	Length;//	4
	int*	Data;
	void Create() { Data = new int[Length/sizeof(int)]; }
	TRECORDINTMAS() { Data = NULL; }
	~TRECORDINTMAS() { delete [] Data; }
};

struct TRECORD3ByteMas
{
	char	Name	[4];
	int	Length;
	struct BYTE3
	{
		unsigned int X : 4;
		unsigned int Y : 4;
		unsigned int Z : 4;
	} *Data;
	void Create() { Data = new BYTE3[Length/sizeof(BYTE3)]; }
	TRECORD3ByteMas() { Data = NULL; }
	~TRECORD3ByteMas() { delete [] Data; }
};

struct MData_Cell : public RECORD4INT1STR    //Это INTV на самомаделе
{
	char	Data[4]; //DATA
	int	Data_Length[2];//	16
	int	GridX, GridY;
	RECORD1INT1STR RGNN;
} Cell;

struct SPEL : public RECORD4INT1STR
{
	char	FNAM	[4];
	int	FNAMLen;//	4
	char*	FNAMData;//	4
	char	SPDT	[4];
	int	SPDTLen;//	4
	int 	Type;
	int	Cost;
	int 	Flags;
	char	ENAM	[4];
	int	ENAMLen;//	4
	short	Eff ;
	short	Eff2 ;
	int 	Range;
	int	Area;
	int 	Dura;
	int 	Min;
	int 	Max;
} Spel;

struct NAM5
{
char	NAM5	[4]; //NAM5
int	NAM5_Length;//	4
unsigned int	Red : 8;
unsigned int	Green : 8;
unsigned int	Blue : 8;
unsigned int	Null : 8;
} Nam5;

//List of FRMR records	Referenced Object List	Varies
struct NAM0
{
char	NAM0	[4]; //NAM0   //Это INTV?
int	Lengths[2];//	4
} Nam0;

struct Reference_to_an_Object
{
char	FRMR	[4]; //FRMR
int	Field_Length;//	4
int	LoadRef;//Index	4  //MEE 3байта+флаг1байт
	RECORD1INT1STR Data;
} FRMR;

struct MFRMRData
{
char	DATA	[4]; //DATA
int	FloatsLength;
float X,Y,Z,RotX,RotY,RotZ; //28byte = 7ints
} FRMRData;

struct MovedFRMR
{
	char MVRF[4];
	int Len1;
	int MovedRef;
	char CNDT[4];
	int Len2;
	int NewX;
	int NewY;
};

class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelUp;
	TButton *OpenBtn;
	TMemo *Out;
	TPanel *PanelTags;
	TLabel *Label1;
	TButtonGroup *ButtonGroup1;
	TFlowPanel *FlowPanel1;
	TButton *HEDRRead;
	TButton *MASTread;
	TButton *Read3Bytes;
	TButton *CellRead;
	TButton *Nam5Read;
	TButton *Read3INT;
	TButton *Read1STR;
	TButton *Nam0Read;
	TButton *FRMRRead;
	TButton *ReadInts;
	TButton *LTEXread;
	TButton *INTVread;
	TCheckBox *WriteMVRFB;
	TButton *DelDatas;
	TStaticText *NextS;
	TRadioGroup *LenSize;
	TLabel *Label2;
	TCheckBox *Reinter;
	TPanel *PanelList;
	TPanel *Panel4;
	TLabeledEdit *ToE;
	TButton *Go;
	TButton *TestP;
	TButton *SPELread;
	TButton *Delete;
	THeaderControl *HeaderControl1;
	TStringGrid *List;
	TButton *Refresh;
	TSplitter *Splitter1;
	TUpDown *UpDown1;
	TOpenDialog *OpenDialog1;
	TButton *ExportSPEL;
	TButton *Save;
	TCheckBox *Sost;
	TButton *NextTag;
	TLabel *LDele;
	TButton *FindDELE;
	TButton *DelDials;
	TButton *ExportBtn;
	TSplitter *Splitter2;
	TPanel *Panel1;
	THeaderControl *HeaderControl2;
	TStringGrid *List2;
	TEdit *EFinds;
	TButton *FindStr;
	TBevel *Bevel1;
	TCheckBox *CheckBox1;
	TBevel *Bevel2;
	TButton *Go16;
	TEdit *SubDescript;
	TButton *SPLMread;
	TButton *PushCoord;
	TCheckBox *Rewrite;
	TCheckBox *Wordwap;
	void __fastcall OpenBtnClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall HEDRReadClick(TObject *Sender);
	void __fastcall CellReadClick(TObject *Sender);
	void __fastcall Nam5ReadClick(TObject *Sender);
	void __fastcall Nam0ReadClick(TObject *Sender);
	void __fastcall FRMRReadClick(TObject *Sender);
	void __fastcall MASTreadClick(TObject *Sender);
	void __fastcall LTEXreadClick(TObject *Sender);
	void __fastcall INTVreadClick(TObject *Sender);
	void __fastcall Read3INTClick(TObject *Sender);
	void __fastcall Read1STRClick(TObject *Sender);
	void __fastcall ReadIntsClick(TObject *Sender);
	void __fastcall Read3BytesClick(TObject *Sender);
	void __fastcall ButtonGroup1ButtonClicked(TObject *Sender, int Index);
	void __fastcall DelDatasClick(TObject *Sender);
	void __fastcall NextSClick(TObject *Sender);
	void __fastcall NextTagClick(TObject *Sender);
	void __fastcall GoClick(TObject *Sender);
	void __fastcall ToESubLabelClick(TObject *Sender);
	void __fastcall SPELreadClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall HeaderControl1SectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall RefreshClick(TObject *Sender);
	void __fastcall ListSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall ListKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall HeaderControl1SectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall HeaderControl1Resize(TObject *Sender);
	void __fastcall UpDown1ChangingEx(TObject *Sender, bool &AllowChange, short NewValue,
          TUpDownDirection Direction);
	void __fastcall TestPClick(TObject *Sender);
	void __fastcall ExportSPELClick(TObject *Sender);
	void __fastcall rplusbClick(TObject *Sender);
	void __fastcall FindDELEClick(TObject *Sender);
	void __fastcall DelDialsClick(TObject *Sender);
	void __fastcall ExportBtnClick(TObject *Sender);
	void __fastcall FindStrClick(TObject *Sender);
	void __fastcall NextSContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall List2SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall Go16Click(TObject *Sender);
	void __fastcall SPLMreadClick(TObject *Sender);
	void __fastcall PushCoordClick(TObject *Sender);
	void __fastcall CloseClick(TObject *Sender);
	void __fastcall RewriteClick(TObject *Sender);
	void __fastcall WordwapClick(TObject *Sender);



private:	// User declarations
public:		// User declarations
	//ESPFile esp;
	__fastcall TForm1(TComponent* Owner);
	FILE *file, *save;
	RECORD3INT Land;
	RECORD4INT1STR Ltex;
	RECORD1INT1STR3INT Mast;
	INTV Intv;
	RECORD1INT1STR Stri;
	TRECORDINTMAS Ints;
	TRECORD3ByteMas Bytes3;
	MHeader Univ;
	MHeader Xscl;
	MovedFRMR Moved;
	RECORD4INT1STR CURR;
	void ToLog(String msg, const char *param = NULL)
	{
   	if (LogUp)
		if (param)
		{
      	msg = "="+msg;
			Out->Lines->Add(param+msg);
		}
		else
       	Out->Lines->Add(msg);
	}
	void ToLogS(String msg, String param)
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
	void char4ToLog(char* msg, const char *param = NULL)
	{
		if (LogUp)
			ToLog(String(msg).SetLength(4), param);
	}
//	TableLoader ti;
//	struct HEADERSDATA
//	{
//		String *Name;
//		int *Tes3;
//		Char *Sost;
//		Char *Type;
//		int *Tes4;
//		int *LenLen;
//		int TagCount;
//	} tab;
	int EoF;
	TStringList *uni;
	int LENSIZE;
	int DEFAULTLENLEN;
	int LENTODEF;
   int LENSIZE2;
	Set <char, 0, 255> TagSymb;
	TStringList *TextTags;
	std::set<int> Deleted;
	int DeletedSize;
	bool Tes4;
	TableLoader types;

	int 	row;
	void AddRow(char *Header, int Length, long Start)
	{
		if (List->RowCount <= row)
			List->RowCount++;
		List->Cells[CHEADER][row] = Header;
		List->Cells[CSIZE][row] = Length;
		List->Cells[CSTART][row] = Start;
		//List->Cells[CDATA][row] = Header;
		row++;
	}
	void RefreshData()
	{
		for (int i = 0; i < List->RowCount; ++i)
		{
			int start = List->Cells[CSTART][i].ToInt();
			fseek(file, start, SEEK_SET);
			static char DATA[64];
			if (List->Cells[CHEADER][i] == "CELL")
			{
				fseek(file, 20, SEEK_CUR);
				int Length;
				fread(&Length, 4, 1, file);
				fread(DATA, Length, 1, file);
				int Data[5];
				fread(Data, 4, 5, file);
				List->Cells[CDATA][i] = String(DATA)+"("+IntToStr(Data[3])+","+IntToStr(Data[4])+")"+IntToStr(Data[2]);
			}
			else
			{
				fread(&CURR, CURR.SIZE, 1, file);
				if (CURR.Length == 1)
				{
					fread(DATA, 64, 1, file);
					//int a = reinterpret_cast<int>(&(DATA[9]));
					List->Cells[CDATA][i] = DATA;//&(DATA[9]);
				}
				else
				{
					fread(DATA, 64, 1, file);
					List->Cells[CDATA][i] = DATA;
				}
			}
      }
	}
	TStringList *Export;
	bool LogUp;
	bool CompareString;
	int  SortingColumn;
	bool ListCompare(const String& lhs, const String& rhs)
	{
		if (CompareString)
			return lhs > rhs;
		else
			return lhs.ToIntDef(0) > rhs.ToIntDef(0);
	}
	void QuickSort(int iLo, int iHi);
	DWORD Tick;
	std::set<int> DELEStarts;
	void DeleteRecord(int Row);
	void Ready(bool ready)
	{
		PanelTags->Enabled = ready;
		FlowPanel1->Enabled = ready;
		List->Enabled = ready;
		Panel4->Enabled = ready;
		List2->Enabled = ready;
	}
	struct TAGTYPES
	{
		char Name[4];
		char Type;
		char MainTag[4];
	} TagTypes[TAGSS];
	int nTypes;
	void AddTagType(char *name, char type, char *maintag = NULL)
	{
		//TagTypes = new TAGTYPES[nTypes+1];
		strncpy(TagTypes[nTypes].Name, name, 4);
		TagTypes[nTypes].Type = type;
		if (maintag != NULL)
			strncpy(TagTypes[nTypes].MainTag, maintag, 4);
		nTypes++;
	}
	String PluginName;
	String *THeader;
	String *TSubHeader;
	String *TType;
	String *TDescr;
	int *SubIndexes;
	int cSubIndexes;
	union Interpret
	{
		int i;
		float f;
		short w[2];
		unsigned short uw[2];
		Byte b[4];
		char c;
	};
	int GetOkrugl(int x);
	float Check999(float x);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
