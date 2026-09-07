//---------------------------------------------------------------------------

#ifndef HeadersH
#define HeadersH
//---------------------------------------------------------------------------

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
	unsigned int LENSTOP;
	void *Data;
	unsigned int Capacity;
	void Read(FILE* &file, int lensize);
	void Write(FILE* &file);
	MHeader()
	{
		Length = 0;
		Data = NULL;
		LENSTOP = 6000;
		Capacity = 0;
	}
	// Безопасный деструктор
	~MHeader()
	{
		if (Data)
		{
			free(Data);
			Data = NULL; // Зануляем указатель во избежание dangling pointer
		}
	}

private:
	// Запрещаем копирование структуры, чтобы компилятор Borland сразу выдал ошибку,
	// если вы случайно попытаетесь скопировать объект и вызвать двойное освобождение памяти
	MHeader(const MHeader&);
	MHeader& operator=(const MHeader&);
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
};  //(1+3+1+1+1+1)*4+32+256

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
	unsigned char b[4];
	char c;
} ;

struct DeleteItem
{
	int MainLenOffset;
	int MainLen;
	int Offset;
	int Size; //size of deleting block
	unsigned char *Addon;
	DeleteItem(int mlo, int ml, int o, int s)
	{
		MainLenOffset = mlo;
		MainLen = ml;
		Offset = o;
		Size = s;
		Addon = NULL;
	}
};

struct Exterior
{
	int X,Y;
	int MainLenOffset;
	int MainLen;
	int PasteOffset;
};

struct Basecell
{
	int* N;
	String* Name;
	String* Subheader;
	int* Size;
	char* Type;
	String* Data;
	int* CoordRef;
};

struct Coord6
{
	// Анонимное объединение: все поля делят одну память
	union
	{
		float all[6];
		struct
		{
			float x;
			float y;
			float z;
			float rx;
			float ry;
			float rz;
		};
	};
	Coord6()
	{
		memset(all, 0, sizeof(all));
	}
	bool TextToFloat3(String str);
	void TextToFloat6(String str);
	String ToStr()
	{
		// Оптимизация: Format работает быстрее, чем куча сложений строк через "+"
		return Format(L"\t%g\t%g\t%g", ARRAYOFCONST((x, y, z)));
		//return String("\t"+FloatToStr(x)+"\t"+FloatToStr(y)+"\t"+FloatToStr(z));
	}
	static float round(float x);
	void Round();
};

struct Coord: public Coord6
{
	int FRMR;
	String Name;
	String Dodt;
	Coord& operator=(const Coord &rhs)
	{
		for (int i = 0; i < 6; i++)
			all[i] = rhs.all[i];
		FRMR = rhs.FRMR;
		Name = rhs.Name;
		Dodt = rhs.Dodt;
		return *this;
	}
};

struct FRMR
{
	int frmr;
	int mast;
	void Set(FILE* &file, int FRMRoffset);
};

#endif
