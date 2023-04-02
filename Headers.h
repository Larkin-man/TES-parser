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


#endif
