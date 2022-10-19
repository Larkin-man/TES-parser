/******************************************************************\
* TableLoader - Загрузка таблиц из текстовых файлов. Версия 1.7    *
\******************************************************************/
#ifndef TableLoaderH
#define TableLoaderH

struct TLSection
{
	String *Name;
	int Pos;	//Позиция секции с 0
	int Size;//Кол-во строк
};

class TableLoader
{
private:
   char *FFormat;
	//Mem обязательно [Count][FRowCount]
	String **MemStr;
   int **MemInt;
	Char **MemChar;
   bool **MemBool;
	int StrCount, IntCount, CharCount, BoolCount;
   int FRowCount;
	int FColCount;

protected:    
	TLSection *FSections;
   void SectionEnsureCapacity(int count);
   int SectionCapacity;
	int FSectionCount;
	void Load(TStringList *list, const char *format);
	int RealGetSection(int SectionIdx, va_list &args);

public:     
	__fastcall TableLoader();
	__property char* Format = {read = FFormat};
	__property int RowCount = {read = FRowCount};
	__property int ColCount = {read = FColCount};
	__property int SectionCount = {read = FSectionCount};
	__property TLSection* Sections = {read = FSections};
	TLSection* FindSection(String SectionName);
	bool IgnoreFirstString; //Игнорировать первую сроку в файле. defaul=true
	bool IgnoreDelimitersPack; //Игнорировать разделители, идущие подряд. defauly=true
	Char Delimiter;      //Разделитель столбцов (табуляция)
   String EndMark; 		//"[end]"
	//Загрузка, format: i-int c-Char s-String b-bool, ... список ссылок на переменные
	int LoadFromFile(String Filename, const char *format, ...);
	int LoadFromResource(String ResourceName, const char *format, ...);
	int LoadFromList(TStrings *list, const char *format, ...);
	//GetSection: ... это список ссылок, также как в LoadFrom. Возвращает кол-во строк в секции
	int GetSection(const String SectionName="", ...);
	//RegColumn - устанавливает ссылку на столбец в секции. ColNum - Номер столбца соотв типа. Возвращает кол-во строк в секции
	int RegColumn(int* &Field, int ColNum, const String SectionName = "") const;
	int RegColumn(Char* &Field, int ColNum, const String SectionName = "") const;
	int RegColumn(String* &Field, int ColNum, const String SectionName = "") const;
	int RegColumn(bool* &Field, int ColNum, const String SectionName = "") const;
	void GetCount(int &intCount, int &charCount, int &boolCount, int &strCount) const;
	void Clear();
	__fastcall ~TableLoader() { Clear(); };
};

#endif
