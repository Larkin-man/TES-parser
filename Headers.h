//---------------------------------------------------------------------------

#ifndef HeadersH
#define HeadersH
//---------------------------------------------------------------------------
static unsigned int LENSTOP = 6000;

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
	void Read(FILE* &file, int lensize);
	void Write(FILE* &file);
	MHeader() { Length = 0; Data=NULL; }
	~MHeader() { free(Data); }
};

typedef struct TRECORD3INT
{
	char	Name[4];//CELL
	int	i[3];
	void 	Write(FILE* &file);
} RECORD3INT;

typedef struct TRECORD1INT1STR
{
	char	NAME	[4];
	int	Length;//	4
	char*	Data;//	4
	bool 	Create();
	void Write(FILE* &file);
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
};      //(1+3+1+1+1+1)*4+32+256

struct MData_Cell : public RECORD4INT1STR    //Это INTV на самомаделе
{
	char	Data[4]; //DATA
	int	Data_Length[2];//	16
	int	GridX, GridY;
	RECORD1INT1STR RGNN;
} ;

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
} ;

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
} ;

union Interpret
{
	int i;
	float f;
	short w[2];
	unsigned short uw[2];
	Byte b[4];
	char c;
} ;

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
	//~DeleteItem()	{	}
};

#endif
