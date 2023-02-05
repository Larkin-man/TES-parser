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
#include <Menus.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
const int CHEADER = 0;
const int CSTART = 1;
const int CSIZE = 2;
const int CDATA = 3;
const int CTYPE = 4;
////////////////////////
const int INTFIELDS = 0;
const int TEXTFIELDS = 1;
const int STRFIELDS = 2;
const int FLOATFIELDS = 3;
const int WORDFIELDS = 4;
const int BYTEFIELDS = 5;
////////////////////////
const int LENSIZE12 = 5;
////////////////////////
const int CELLNAMEDATALEN = 28;
const int CELLRGNNLEN = 8;
static unsigned int LENSTOP = 6000;
//const int MAINLENSIZE = 12;

struct MHeader
{
	char Name[4];
	unsigned int Length;
//	union HeaderData
//	{
//		int i;
//		float f;
//		short w[2];
//		unsigned short uw[2];
//		Byte b[4];
//		char c;
//		unsigned char u;
//		wchar_t t;
//	}
	void *Data;
	unsigned int Capacity;
	void Read(FILE* &file, int lensize)
	{
		fread(&Name, 4, 1, file);
		fread(&Length, lensize, 1, file);
		if ((Length > LENSTOP)||(Length < 0))
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
	void 	Write(FILE* &file)
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
	bool 	Create()
	{
		if (Length <= 1024) Data = new char[Length];
		else return false;
		return true;
	}
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

struct Tes3Header : public TRECORD3INT
{
char	Header	[4]; //HEDR
int	HeaderSize;//	[4];
float	Version_Number;//	[4];
int	Unknown;	//[4];
char	Author_Name	[32];
char	Description	[256];
int   NumRecords; //этого всего записей в ФАЙЛЕ
} hTes3;      //(1+3+1+1+1+1)*4+32+256

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

class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelUp;
	TButton *OpenBtn;
	TMemo *Out;
	TPanel *PanelPRO;
	TLabel *Label1;
	TButtonGroup *ButtonGroup1;
	TFlowPanel *FlowPanel1;
	TButton *TES3Read;
	TButton *CellRead;
	TButton *GMDTRead;
	TButton *DelDatas;
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
	TSplitter *Splitter1;
	TOpenDialog *OpenDialog1;
	TButton *ExportSPEL;
	TButton *Save;
	TButton *FindNext;
	TLabel *LDele;
	TButton *DelDials;
	TButton *ExportBtn;
	TSplitter *Splitter2;
	TPanel *PanelList2;
	THeaderControl *HeaderControl2;
	TStringGrid *List2;
	TEdit *EFinds;
	TButton *FindStr;
	TBevel *Bevel1;
	TCheckBox *ProModeCK;
	TEdit *SubDescript;
	TButton *SPLMread;
	TButton *PushCoord;
	TCheckBox *Wordwap;
	TPopupMenu *PopupMenu1;
	TMenuItem *TestMenu;
	TMenuItem *NList2;
	TMenuItem *ClearOut;
	TButton *DelTrash;
	TButton *Sel;
	TMenuItem *EnableLsit2Delete1;
	TButton *FindinList2;
	TButton *DeleteExtraData;
	TButton *Save2;
	TButton *CheckCoord;
	TButton *AddMainFields;
	TButton *setlocaleBtn;
	TButton *NextCell;
	TImageList *ImageList1;
	TButton *FindinSublists;
	TButton *Replace;
	TLabel *Secr;
	TMenuItem *NFindSub;
	TMenuItem *FindHeader1;
	TMenuItem *Rewrites;
	TPanel *PanelSubRead;
	TMenuItem *ShowData1;
	TButton *CheckConflicts;
	TStaticText *NextS;
	TMenuItem *BreakIf0Len;
	TButton *PrepareE;
	TButton *PrepareGame;
	void __fastcall OpenBtnClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TES3ReadClick(TObject *Sender);
	void __fastcall CellReadClick(TObject *Sender);
	void __fastcall ButtonGroup1ButtonClicked(TObject *Sender, int Index);
	void __fastcall DelDatasClick(TObject *Sender);
	void __fastcall NextSClick(TObject *Sender);
	void __fastcall FindNextClick(TObject *Sender);
	void __fastcall GoClick(TObject *Sender);
	void __fastcall ToESubLabelClick(TObject *Sender);
	void __fastcall SPELreadClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall HeaderControl1SectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall ListSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall HeaderControl1SectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall HeaderControl1Resize(TObject *Sender);
	void __fastcall TestPClick(TObject *Sender);
	void __fastcall ExportSPELClick(TObject *Sender);
	void __fastcall rplusbClick(TObject *Sender);
	void __fastcall DelDialsClick(TObject *Sender);
	void __fastcall ExportBtnClick(TObject *Sender);
	void __fastcall FindStrClick(TObject *Sender);
	void __fastcall NextSContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall List2SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall ProModeCKClick(TObject *Sender);
	void __fastcall SPLMreadClick(TObject *Sender);
	void __fastcall PushCoordClick(TObject *Sender);
	void __fastcall CloseClick(TObject *Sender);
	void __fastcall WordwapClick(TObject *Sender);
	void __fastcall TestMenuClick(TObject *Sender);
	void __fastcall NList2Click(TObject *Sender);
	void __fastcall ClearOutClick(TObject *Sender);
	void __fastcall DelTrashClick(TObject *Sender);
	void __fastcall SelClick(TObject *Sender);
	void __fastcall FindNextContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall ListKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EnableLsit2Delete1Click(TObject *Sender);
	void __fastcall List2KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Save2Click(TObject *Sender);
	void __fastcall FindinList2Click(TObject *Sender);
	void __fastcall DeleteExtraDataClick(TObject *Sender);
	void __fastcall NextTagContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall CheckCoordClick(TObject *Sender);
	void __fastcall HeaderControl2Resize(TObject *Sender);
	void __fastcall HeaderControl2SectionResize(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall GMDTReadClick(TObject *Sender);
	void __fastcall AddMainFieldsClick(TObject *Sender);
	void __fastcall CheckCoordContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall setlocaleBtnClick(TObject *Sender);
	void __fastcall NextCellClick(TObject *Sender);
	void __fastcall FindinSublistsClick(TObject *Sender);
	void __fastcall ReplaceClick(TObject *Sender);
	void __fastcall FindHeader1Click(TObject *Sender);
	void __fastcall RewritesClick(TObject *Sender);
	void __fastcall CheckConflictsClick(TObject *Sender);
	bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
	void __fastcall PrepareEClick(TObject *Sender);
	void __fastcall PrepareGameClick(TObject *Sender);


private:	// User declarations
public:		// User declarations
	//ESPFile esp;
	__fastcall TForm1(TComponent* Owner);
					 //Morr   //podliv     sku
	int LENSIZE; //4      //sublen 2  2
	int MAINLENSIZE; //12  16         20
	int MOVERLENTOSNAME; //8
	int SLENSIZE; //4
	int MLENTOSLEN;//16
	int MNAMETOSUBLEN;//20
	int *SecretData;
	int SecretLen;
	int POSNRECORDS;
	void Setup(int SubLenSize, int MainLenSize, int CountOffset)
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
	FILE *file, *save;
	long EoF;
	RECORD3INT Land;
	RECORD4INT1STR Ltex;
	RECORD1INT1STR3INT Mast;
	//INTV Intv;
	RECORD1INT1STR Stri;
	//	TRECORDINTMAS Ints;
	//TRECORD3ByteMas Bytes3;
	MHeader Univ;
	//MHeader Xscl;
	//RECORD4INT1STR CURR;
	struct BITS //sizeof=4
	{  //если тип инт то там хранится 0 или -1
		unsigned int b1 : 1;
		unsigned int b2 : 1;
		unsigned int b3 : 1;
		unsigned int b4 : 1;
		unsigned int b5 : 1;
		unsigned int b6 : 1;
		unsigned int b7 : 1;
		unsigned int b8 : 1;
	} bit, *pbit;
	void ClearDele()
	{
		Deleted.clear();
		DeletedSize = 0;
		LogUp = true;
		FindIdx = -1;
		SubDelete.clear();
		DELEStarts.clear();
		RefStarts.clear();
		RefEnds.clear();
	}
	void tolog(String msg)
	{
		Out->Lines->Add(msg);
	}
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
	Set <char, 0, 255> TagSymb;
	std::set<long> Deleted;
	int DeletedSize;
	bool Tes3;
	TableLoader types;

	int AddedRow;
	void AddRow(char *Header, int Length, long Start)
	{
		if (List->RowCount <= AddedRow)
			List->RowCount++;
		List->Cells[CHEADER][AddedRow] = Header;
		List->Cells[CSIZE][AddedRow] = Length;
		List->Cells[CSTART][AddedRow] = Start;
		//List->Cells[CDATA][AddedRow] = Header;
		AddedRow++;
	}
	void SkipSubheader()
	{
		int Length;
		fread(&Length, SLENSIZE, 1, file);
		fseek(file, Length + 4, SEEK_SET);
		//fread(DATA, Length, 1, file);
	}

	void RefreshData(FILE* &file, int Start=0);
	TStringList *Export, *Expo;
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
	DWORD Tick, Tick2;
	std::set<int> DELEStarts;
	void DeleteRecord(int Row);
	void Ready(bool ready)
	{
		PanelPRO->Enabled = ready;
		FlowPanel1->Enabled = ready;
		List->Enabled = ready;
		Panel4->Enabled = ready;
		List2->Enabled = ready;
		EnableLsit2Delete1->Enabled = ready;
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
	int *SubIndexes; //для поля description
	int cSubIndexes;
	union Interpret
	{
		int i;
		float f;
		short w[2];
		unsigned short uw[2];
		Byte b[4];
		char c;
	} ish;
	int GetOkrugl(int x);
	float Check999(float x);
	bool Opening;
	void EndFind(int Row);
	String SelMainHedr;
	bool BlockList2Sel;
	int Indextt;
	void Delete2(int Row2);
	int FindIdx;
	String OpenedFileName;
	//TStringList *FindList;
	struct DeleteItem
	{    //DeleteItem ti(AI_W->Cells[1][0].ToInt(),AI_W->Cells[1][1].ToInt());
			//DeletingSubheaders.push_back(ti);
		DeleteItem(int m, int ml, int o, int s)
		{
			MainLenOffset = m;
			MainLen = ml;
			Offset = o;
			Size = s;
			Addon = NULL;
		}
		int MainLenOffset;
		int MainLen;
		int Offset;
		int Size; //size of deleting block
		byte *Addon;
		~DeleteItem()	{	if (Addon)	delete []Addon;		}
	};
	std::vector<DeleteItem>SubDelete;
	int RecordCount;
	int DebugL2c;
	void SetDescription(int Num, int Row);
	void Dran()
	{
		List->ScrollBars = ssVertical;
		List->Cols[0]->EndUpdate();
		List->Cols[1]->EndUpdate();
		List->Cols[2]->EndUpdate();
		List->Cols[3]->EndUpdate();
	}
	bool localeinstalled;
	int bloklist2;
	String CurrCell;
	void CheckLot(int &Length, int Max)
	{
		if (Length > Max)
			Length = Max;
	}
	std::vector<long> RefStarts;
	std::vector<long> RefEnds;
	void PrepareFor(char SYMBS[4]);
	bool PrepareForEdit;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
