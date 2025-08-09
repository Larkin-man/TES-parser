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
//////////List2/////////
const int CDATA2 = 4;
const int CTYPE = 3;
////////////////////////
const int INTFIELDS = 0;
const int TEXTFIELDS = 1;
const int STRFIELDS = 2;
const int FLOATFIELDS = 3;
const int WORDFIELDS = 4;
const int BYTEFIELDS = 5;
////////////////////////
const int CELLNAMEDATALEN = 28;
const int CELLRGNNLEN = 8;

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
	TButton *SwapCoord;
	TCheckBox *Wordwrap;
	TPopupMenu *PopupMenu1;
	TMenuItem *NTestMenu;
	TMenuItem *NEnableSublist;
	TMenuItem *NClearOut;
	TButton *DelTrash;
	TButton *Sel;
	TMenuItem *NEnableList2Delete;
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
	TMenuItem *NRewrites;
	TPanel *PanelSubRead;
	TMenuItem *NShowData;
	TButton *CheckConflicts;
	TStaticText *NextS;
	TMenuItem *BreakIf0Len;
	TButton *PrepareE;
	TButton *PrepareGame;
	TButton *ExportScriptsBtn;
	TButton *LoadCells;
	TButton *Button1;
	TButton *MVRF;
	TButton *DelGroupSubheaders;
	TButton *DeleteAllSubhead;
	TButton *CheckCELL;
	TMenuItem *NAutoFind;
	TComboBox *WhatFinded;
	TButton *MassDelete;
	TButton *FindCELLmast;
	TButton *FindOwners;
	TMenuItem *NAbove;
	TSplitter *Splitter3;
	TMenuItem *NExtreriorFlagsPrint;
	TButton *DevastateCell;
	TButton *DropMaster;
	void __fastcall OpenBtnClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TES3ReadClick(TObject *Sender);
	void __fastcall CellReadClick(TObject *Sender);
	void __fastcall ButtonGroup1ButtonClicked(TObject *Sender, int Index);
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
	void __fastcall SwapCoordClick(TObject *Sender);
	void __fastcall CloseClick(TObject *Sender);
	void __fastcall WordwrapClick(TObject *Sender);
	void __fastcall NTestMenuClick(TObject *Sender);
	void __fastcall NEnableSublistClick(TObject *Sender);
	void __fastcall NClearOutClick(TObject *Sender);
	void __fastcall DelTrashClick(TObject *Sender);
	void __fastcall SelClick(TObject *Sender);
	void __fastcall FindNextContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall ListKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall NEnableList2DeleteClick(TObject *Sender);
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
	void __fastcall NRewritesClick(TObject *Sender);
	void __fastcall CheckConflictsClick(TObject *Sender);
	void __fastcall PrepareEClick(TObject *Sender);
	void __fastcall PrepareGameClick(TObject *Sender);
	void __fastcall ExportScriptsBtnClick(TObject *Sender);
	void __fastcall LoadCellsClick(TObject *Sender);
	void __fastcall List2Enter(TObject *Sender);
	void __fastcall ListEnter(TObject *Sender);
	void __fastcall MVRFClick(TObject *Sender);
	void __fastcall DeleteAllSubheadClick(TObject *Sender);
	void __fastcall CheckCELLClick(TObject *Sender);
	void __fastcall OutDblClick(TObject *Sender);
	void __fastcall DelGroupSubheadersClick(TObject *Sender);
	void __fastcall WhatFindedChange(TObject *Sender);
	void __fastcall MassDeleteClick(TObject *Sender);
	void __fastcall FindCELLmastClick(TObject *Sender);
	void __fastcall FindOwnersClick(TObject *Sender);
	void __fastcall NAboveClick(TObject *Sender);
	void __fastcall List2DblClick(TObject *Sender);
	void __fastcall ListDblClick(TObject *Sender);
	void __fastcall DevastateCellClick(TObject *Sender);
	void __fastcall DropMasterClick(TObject *Sender);
	void __fastcall DropMasterContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);


private:	// User declarations
public:		// User declarations
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
	void Setup(int SubLenSize, int MainLenSize, int CountOffset);
	FILE *file, *save;
	long EoF;
	MHeader Univ;
	BITS *pbit;
	void Clear();
	void tolog(String msg)
	{
		Out->Lines->Add(msg);
	}
	void tologi(String msg, int i)
	{
		Out->Lines->Add(msg+"="+IntToStr(i));
	}
	void ToLogLen(String msg, int Len=-1)
	{
		if (LogUp) {
			if (msg.Length() > Len)
				Out->Lines->Add(msg.SetLength(Univ.Length));
			else
				Out->Lines->Add(msg); }
	}
	void ToLog(String msg, const char *param = NULL)
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
	void AddRow(char *Header, int Length, long Start);
	void RefreshData(FILE* &file, int Start=0);
	TStringList *Export, *Expo;
	bool LogUp;
	bool CompareString;
	int  SortingColumn;
	void QuickSort(int iLo, int iHi);
	void DeleteRecord(int Row);
	void Ready(bool ready);
	struct TAGTYPES
	{
		char Name[4];
		char Type;
		char MainTag[4];
	} TagTypes[TAGSS];
	int nTypes;
	void AddTagType(char *name, char type, char *maintag = NULL);
	String PluginName;
	String *THeader;
	String *TSubHeader;
	String *TType;
	String *TDescr;
	int *SubIndexes; //для поля description
	int cSubIndexes;
	//float round(float x);
	int GetOkrugl(int x);
	float Check999(float x);
	bool Opening;
	bool Find(String find);
	bool EndFind(int Row);
	bool BlockList2Sel; //int Indextt;
	void Delete2(int Row2);
	void DeleteSublist(int Row2, int MainRow);
	int FindIdx;
	String OpenedFileName;
	std::vector<DeleteItem>SubDelete;
	int RecordCount;
	int DebugL2c;
	void SetDescription(int Num, int Row);
	void DoUpdateList(bool begin);
	bool localeinstalled;
	int bloklist2;
	String CurrCell;
	std::vector<long> RefStarts;
	std::vector<long> RefEnds;
	struct SavingData
	{
		void *Data;
		int Size;
	};
	std::map<long, int> Edited;
	typedef std::map<long, int>::iterator ED;
	void PrepareFor(char SYMBS[4]);
	bool PrepareForEdit;
	int SearchingIn1;
	int SearchingIn2;
	int *SearchingIn;
	TStringGrid *SearchList;
	bool ShowAll;
	bool StringsIdent(String left, String* &right, int rightcount);
	TableLoader basecel;
	//№	Header	Name	Subheader	Size	Type	Data
	Basecell Mor;
	std::vector<Coord> Coords;
	//void TextToFloat6(String str, Coord &curr);
	TStringList *what;
	bool equ(float &value, float rhs)
	{
		return (value > rhs-3.0 && value < rhs+3.0);
	}
	struct PACK
	{
		int Size;
		int RowCount;
		TStringList *col[5];
		PACK()
		{
			col[0] = NULL;
			col[1] = NULL;
			col[2] = NULL;
			col[3] = NULL;
			col[4] = NULL;
		}
		~PACK()
		{
//       	delete col[0];
//         delete col[1];
//         delete col[2];
//         delete col[3];
//         delete col[4];
		}
	};
	typedef PACK* PPACK;
	std::map<int, PACK> ListStore;
	std::set<String> obj;
	std::vector<int>Sizes;
	std::vector<long>Ends;
	std::set<String>Loc1;
	std::set<String>LocFrmr;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
