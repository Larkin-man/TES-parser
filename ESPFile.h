//---------------------------------------------------------------------------

#ifndef ESPFileH
#define ESPFileH
//---------------------------------------------------------------------------



class ESPFile
{
	public:
		struct RECORD3INT
		{
			int i[3];
			static const int size = sizeof(int)*3;
			RECORD3INT(int f) { i[0] = f; }
			char gg[5];
		} Data;

		static struct RECORD1INT1STR
		{
			int Length;
			char* Str;
			void Read()
			{ }//fread(&Length, HS->Text.ToInt(), 1, file);      }
			void Create() { if (Length < 1024) Str = new char[Length]; }
			RECORD1INT1STR(int a) { Length = a; Str = NULL; }
			~RECORD1INT1STR() { delete [] Str; }
		} RGNN;

	FILE *file;
	ESPFile() ;
	~ESPFile();
};
#endif
