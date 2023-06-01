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
	TMenuItem *NFindHeader;
	TMenuItem *NRewrites;
	TPanel *PanelSubRead;
	TMenuItem *NShowData;
	TButton *CheckConflicts;
	TStaticText *NextS;
	TMenuItem *BreakIf0Len;
	TButton *PrepareE;
	TButton *PrepareGame;
	TButton *ExportScriptsBtn;
	TButton *DelOffsets;
	TButton *LoadCells;
	TMenuItem *NSearchinData;
	TMenuItem *NSearchinOffset;
	TMenuItem *NSearchinSize;
	TMenuItem *N1;
	TButton *Button1;
	TButton *MVRF;
	TButton *Button2;
	TButton *DeleteAllSubhead;
	TButton *CheckCELL;
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
	void __fastcall NFindHeaderClick(TObject *Sender);
	void __fastcall NRewritesClick(TObject *Sender);
	void __fastcall CheckConflictsClick(TObject *Sender);
	bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
	void __fastcall PrepareEClick(TObject *Sender);
	void __fastcall PrepareGameClick(TObject *Sender);
	void __fastcall ExportScriptsBtnClick(TObject *Sender);
	void __fastcall DelOffsetsClick(TObject *Sender);
	void __fastcall LoadCellsClick(TObject *Sender);
	void __fastcall NSearchinDataClick(TObject *Sender);
	void __fastcall NSearchinOffsetClick(TObject *Sender);
	void __fastcall NSearchinSizeClick(TObject *Sender);
	void __fastcall List2Enter(TObject *Sender);
	void __fastcall ListEnter(TObject *Sender);
	void __fastcall MVRFClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall DeleteAllSubheadClick(TObject *Sender);
	void __fastcall CheckCELLClick(TObject *Sender);



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
	Tes3Header hTes3;
	MData_Cell Cell;
	SPEL Spel;
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
	BITS bit, *pbit;
	void ClearDele()
	{
		for (std::vector<DeleteItem>::iterator el = SubDelete.begin(); el != SubDelete.end(); ++el)
			if (el->Addon)
				delete [] el->Addon;
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
		PanelSubRead->Enabled = false;
		FlowPanel1->Enabled = ready;
		List->Enabled = ready;
		Panel4->Enabled = ready;
		List2->Enabled = ready;
		NEnableList2Delete->Enabled = ready;
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
	Interpret ish;
	int GetOkrugl(int x);
	float Check999(float x);
	bool Opening;
	void EndFind(int Row);
	String SelMainHedr;
	bool BlockList2Sel;
	int Indextt;
	void Delete2(int Row2);
	void DeleteSublist(int Row2, int MainRow);
	int FindIdx;
	String OpenedFileName;
	//TStringList *FindList;
	std::vector<DeleteItem>SubDelete;
	int RecordCount;
	int DebugL2c;
	void SetDescription(int Num, int Row);
	void DoUpdateList(bool begin)
	{
		if (begin)
		{
			List->Row = 0; //Tick = ::GetTickCount();
			Out->SetFocus();
      	List->Cols[0]->BeginUpdate();
			List->Cols[1]->BeginUpdate();
			List->Cols[2]->BeginUpdate();
			List->Cols[3]->BeginUpdate();
			List->ScrollBars = ssNone; //List->Items->BeginUpdate();
		}
		else
		{
			List->Cols[0]->EndUpdate();
			List->Cols[1]->EndUpdate();
			List->Cols[2]->EndUpdate();
			List->Cols[3]->EndUpdate();
			List->ScrollBars = ssVertical;
			HeaderControl1Resize(NULL);
         //HeaderControl2Resize(NULL);
		}
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
	int SearchingIn1;
	int SearchingIn2;
	int *SearchingIn;
	TStringGrid *SearchList;
	bool ShowAll;
	void RetMes();
	bool StringsIdent(String left, String* &right, int rightcount);
	TableLoader basecel;
	//№	Header	Name	Subheader	Size	Type	Data
	Basecell Mor;
	std::vector<Coord> Coords;
	//Coord TextToFloat6(String &str)//, float** &mas)
	void TextToFloat6(String str, Coord &curr)
	{
		int st = 1;
		int coi = 0;
    	for (int j = 2; j <= str.Length(); j++)
		{
			if (str[j] == ' ')
			{
				curr.all[coi] = str.SubString(st, j-st).ToDouble();
				st = j+1;
				coi++;
			}
		}
		curr.rz = str.SubString(st, str.Length()-st+1).ToDouble();
		//return xyz;
	}

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
