//--------------------------------------------------------------------
//#include <stdio.h> //для printf
//#include <stdarg.h>
//#include <Dialogs.hpp>
#include <Classes.hpp>
#pragma hdrstop
#include "TableLoader.h"
//--------------------------------------------------------------------
__fastcall TableLoader::TableLoader()
{
   MemStr  = NULL;
   MemInt  = NULL;
   MemChar = NULL;
   MemBool = NULL;
   StrCount = 0; IntCount = 0; CharCount = 0; BoolCount = 0;
   FRowCount = 0;
   IgnoreFirstString = true;
   IgnoreDelimitersPack = true;
	FFormat = NULL;
   //Vars = NULL;
   Delimiter = '\t';
   FSections = NULL;
   FSectionCount = 0;
   SectionCapacity = 0;
	FColCount = 0;
	EndMark = "[end]";
}
//--------------------------------------------------------------------
int TableLoader::RealGetSection(int SectionIdx, va_list &args)
{
	int sSize, sPos;
	if (SectionIdx < 0)
	{
		sSize = FRowCount;
		sPos = 0;
	}
	else
	{
		sSize = FSections[SectionIdx].Size;
		sPos = FSections[SectionIdx].Pos;
	}
	int intCount=0, strCount=0, charCount=0, boolCount=0;
	int **intArg;  String **strArg;  Char **CharArg;  bool **boolArg;
	for (unsigned int i=0; i<strlen(FFormat); ++i)
		switch (FFormat[i])
		{
			case 'i':
			case 'I':
				if ((intArg = va_arg(args, int**)) != 0)
					*intArg = MemInt[intCount++] + sPos;
				break;
         case 's':
			case 'S':
				if ((strArg = va_arg(args, String**)) != 0)
					*strArg = MemStr[strCount++] + sPos;
				break;
			case 'c':
			case 'C':
				if ((CharArg = va_arg(args, Char**)) != 0)
					*CharArg = MemChar[charCount++] + sPos;
				break;
			case 'b':
			case 'B':
				if ((boolArg = va_arg(args, bool**)) != 0)
					*boolArg = MemBool[boolCount++] + sPos;
				break;
			//default: '0'
		}
	va_end(args);
	return sSize;
}
//--------------------------------------------------------------------
//[секция], ... это список ссылок, также как в LoadFromFile. Возвращает кол-во строк в секции
int TableLoader::GetSection(const String SectionName, ...)
{
	if (FFormat == NULL)		return 0;
   va_list ap;
	va_start(ap, SectionName);
	if (SectionName.IsEmpty() == false)
      for (int i=0; i<FSectionCount; ++i)
			if (SectionName == *FSections[i].Name)
				return RealGetSection(i, ap);
	return RealGetSection(-1, ap);
}
//--------------------------------------------------------------------
int TableLoader::RegColumn(int* &Field, int ColNum, const String SectionName) const
{
   --ColNum;
	if ((ColNum < 0)||(ColNum >= IntCount))
		return -1;
	if (SectionName.IsEmpty() == false)
   {
      for (int i=0; i<FSectionCount; ++i)
         if (SectionName == *FSections[i].Name)
         {
            Field = MemInt[ColNum] + FSections[i].Pos;
				return FSections[i].Size;
         }
      return 0;
   }
   else
		Field = MemInt[ColNum];
	return FRowCount;
}
//--------------------------------------------------------------------
int TableLoader::RegColumn(Char* &Field, int ColNum, const String SectionName) const
{
	--ColNum;
	if (ColNum < 0)      return -1;
	if (ColNum >= CharCount)      return -1;
	if (SectionName.IsEmpty() == false)
   {
      for (int i=0; i<FSectionCount; ++i)
         if (SectionName == *FSections[i].Name)
         {
            Field = MemChar[ColNum] + FSections[i].Pos;
            return FSections[i].Size;
         }
      return 0;
   }
   else
		Field = MemChar[ColNum];
	return FRowCount;
}
//--------------------------------------------------------------------
int TableLoader::RegColumn(String* &Field, int ColNum, const String SectionName) const
{
   --ColNum;
	if (ColNum < 0)      return -1;
	if (ColNum >= StrCount)      return -1;
	if (SectionName.IsEmpty() == false)
   {
      for (int i=0; i<FSectionCount; ++i)
         if (SectionName == *FSections[i].Name)
         {
            Field = MemStr[ColNum] + FSections[i].Pos;
            return FSections[i].Size;
         }
      return 0;
   }
   else
		Field = MemStr[ColNum];
	return FRowCount;
}
//--------------------------------------------------------------------
int TableLoader::RegColumn(bool* &Field, int ColNum, const String SectionName) const
{
   --ColNum;
	if (ColNum < 0)      return -1;
   if (ColNum >= BoolCount)      return -1;
   if (SectionName.IsEmpty() == false)
   {
      for (int i=0; i<FSectionCount; ++i)
         if (SectionName == *FSections[i].Name)
         {
            Field = MemBool[ColNum] + FSections[i].Pos;
            return FSections[i].Size;
         }
      return 0;
   }
   else
		Field = MemBool[ColNum];
	return FRowCount;
}
//--------------------------------------------------------------------
void TableLoader::Load(TStringList *list, const char *format)
{
	if (IgnoreFirstString)
		if (list->Count <= 1)
			return;
		else
			list->Delete(0); //Первая лишняя
	else
		if (list->Count < 1)
			return; //delete list;
	if (FRowCount > 0)
		Clear();
	//Нужно пробежаться по файлу и удалить лишние строки и посчитать FRowCount
	FRowCount = -1;
	String str; //String *pStr;
	bool EndIsPossible = !EndMark.IsEmpty();
	for (int i=0; i<list->Count; i++)
	{
		str = list->Strings[i]; //str = &(list->Strings[i]); //не пашет
      if (str.IsEmpty())  //Пустую строку удалить, будто её нет
      {  //ShowMessage("Удаляем пустую строку №"+IntToStr(i));
			list->Delete(i);
			//FRowCount--;
			i--;
			continue;  //Это если пустые строки в конце документа
		}
		if (EndIsPossible&&(str.Pos(EndMark) > 0))
      {
			FRowCount = i;
			break;
		}
		if ( (!str.IsEmpty())&&(str[1] == '[')&&(str.Pos(']') > 0) )
		{
			SectionEnsureCapacity(FSectionCount+1);
			FSections[FSectionCount].Pos = i;
			//strcpy(FSections[SectionCount].Name, g.c_str());
			FSections[FSectionCount].Name = new String(str.SubString(2, str.Pos(']')-2));
			FSectionCount++;
			list->Delete(i); //str = "";
			i--;
			continue;  //Это если пустые строки в конце документа
		}
	}
	if (FRowCount < 0)
		FRowCount = list->Count;
	if (FRowCount == 0)
		return;
	//Установить размер секций
	if (FSectionCount > 0)
   {
		for (int i=0; i<FSectionCount-1; ++i)
			FSections[i].Size = FSections[i+1].Pos - FSections[i].Pos;
      FSections[FSectionCount-1].Size = FRowCount - FSections[FSectionCount-1].Pos;
	}
   StrCount=0; IntCount=0; CharCount=0; BoolCount=0;
   //Разбор строки формата
	FFormat = strdup(format);
	for (unsigned int i=0; i<strlen(FFormat); ++i)
		switch (FFormat[i])
		{
			case 'I':
			case 'i': IntCount++; break;
			case 'C':
			case 'c': CharCount++; break;
			case 'S':
			case 's': StrCount++; break;
			case 'B':
			case 'b': BoolCount++; break;
         default: FFormat[i] = '0';
		}
	FColCount = 0;
	FColCount += IntCount; FColCount += CharCount; FColCount += StrCount; FColCount += BoolCount;
	if (FColCount == 0)
   	return;
	//----------------------
	//Создание массивов по кооличеству строк в файле
	if (IntCount > 0)
   {
		MemInt = new int*[IntCount];
		for (int i=0; i<IntCount; ++i)
			MemInt[i] = new int[FRowCount];
   }
	if (CharCount > 0)
   {
		MemChar = new Char*[CharCount];
		for (int i=0; i<CharCount; ++i)
         MemChar[i] = new Char[FRowCount];
   }
	if (StrCount > 0)
   {
		MemStr = new String* [StrCount];
		for (int i=0; i<StrCount; ++i)
         MemStr[i] = new String[FRowCount];
   }
	if (BoolCount > 0)
   {
		MemBool = new bool* [BoolCount];
		for (int i=0; i<BoolCount; ++i)
         MemBool[i] = new bool[FRowCount];
	}
	//-----------------------------------------------
	//Началось считывание файл и разбор по словам
   int p;   //Индекс разделителя
	String word;   //Это отдельные слова
	int curr, currStr, currInt, currChar, currBool;
   for (int i=0; i<FRowCount; ++i)
	{
      curr=0; currStr=0; currInt=0; currChar=0; currBool=0;
      str = list->Strings[i];
		while (str.IsEmpty() == false)  //Вот цикл отделяющий слова
      {
         if ( (IgnoreDelimitersPack)&&(str[1] == Delimiter) ) //Для отсеивания лишних разделителей
         {
            str.Delete(1,1);
            continue;
         }
         p = str.Pos(Delimiter);
         if (p == 0)
         {
            word = str;
            str = "";
         }
         else
         {
            word = str.SubString(1,p-1);
            str.Delete(1,p);
         }
			//Слово получено
			try
			{
				REPEAT:
            switch (format[curr])
				{
					case 'I': 	currInt++;
									curr++;
									goto REPEAT;
					case 'C': 	currChar++;
									curr++;
									goto REPEAT;
					case 'S': 	currStr++;
									curr++;
									goto REPEAT;
					case 'B': 	currBool++;
									curr++;
									goto REPEAT;
					case 'i' :  MemInt [currInt][i] = word.ToIntDef(0);
									currInt++;
                           break;
					case 'c' :  MemChar[currChar][i] = word[1];
                           currChar++;
									break;
					case 's' :  MemStr [currStr][i] = word;
									currStr++;
									break;
					case 'b' :  MemBool[currBool][i] = word[1] == '0' ? false : true;
									currBool++;
									break;
					//default :
				}
			}
			catch (...)
         {
				switch (format[curr])
				{
					case 'i' :  MemInt [currInt][i] = 0;  currInt++;  break;
					case 'c' :  MemChar[currChar][i]= '0';  currChar++; break;
					case 's' :  MemStr [currStr][i] = "";  currStr++; break;
					case 'b' :  MemBool[currBool][i]= false;  currBool++; break;
					//default :
				}
			}
         curr++;
      }
	}
}
//--------------------------------------------------------------------
//Загрузка из файла, format: i-int c-Char s-Ansi b-bool, ... список ссылок на переменные
int TableLoader::LoadFromFile(String Filename, const char *format, ...)
{
	TStringList *file = NULL;
	try
	{
		file = new TStringList;
		file->LoadFromFile(Filename);
		Load(file, format);
		va_list ap;
		va_start(ap, format);
		RealGetSection(-1, ap); //va_end(ap);
	}
	__finally
	{
		delete file;
	}
	return FRowCount;
}
//--------------------------------------------------------------------
int TableLoader::LoadFromResource(String ResourceName, const char *format, ...)
{
	TStringList *file = NULL;
	TResourceStream *ptRes = NULL;
	try
	{
		file = new TStringList;
		ptRes = new TResourceStream(0, ResourceName, L"RT_RCDATA"); //#define _D(__s) L ## __s
		file->LoadFromStream(ptRes);
		Load(file, format);
		va_list ap;
		va_start(ap, format);
		RealGetSection(-1, ap);	//va_end(ap);
	}
	__finally
	{
		delete file;
		delete ptRes;
	}
	return FRowCount;
}
//--------------------------------------------------------------------
int TableLoader::LoadFromList(TStrings *list, const char *format, ...)
{
	if (list)
	{
		Load((TStringList*)list, format);
		va_list ap;
		va_start(ap, format);
		RealGetSection(-1, ap);		//va_end(ap);
	}
	return FRowCount;
}

//--------------------------------------------------------------------
void TableLoader::SectionEnsureCapacity(int count)
{
   if (count > SectionCapacity)
   {
      SectionCapacity = 4;
      while ( SectionCapacity < count )
         SectionCapacity *= 2;
      if (FSections)
			FSections = (TLSection*) realloc (FSections, SectionCapacity*sizeof(TLSection));
      else
         FSections = (TLSection*) malloc (SectionCapacity*sizeof(TLSection));
   }
}
//--------------------------------------------------------------------
void TableLoader::Clear()
{
	if (FFormat)
   {
		free (FFormat);
		FFormat = NULL;
   }
   if (FSections)
   {
      for (int i=0; i<FSectionCount; ++i)
         delete FSections[i].Name;
      free (FSections);
      FSections = NULL;
      SectionCapacity = 0;
      FSectionCount = 0;
   }
   if (IntCount > 0)
   {
      for (int i=0; i<IntCount; ++i)
         delete []MemInt[i];
      delete []MemInt;
      MemInt = NULL;
      IntCount = 0;
   }
   if (CharCount > 0)
   {
      for (int i=0; i<CharCount; ++i)
         delete []MemChar[i];
      delete []MemChar;
      MemChar = NULL;
      CharCount = 0;
   }
   if (BoolCount > 0)
   {
      for (int i=0; i<BoolCount; ++i)
         delete []MemBool[i];
      delete []MemBool;
      MemBool = NULL;
      BoolCount = 0;
   }
   if (StrCount)
   {
      for (int i=0; i<StrCount; ++i)
         delete []MemStr[i];
      delete []MemStr;
      MemStr = NULL;
		StrCount = 0;
   }
   FRowCount = 0;
	FColCount = 0;
}
//--------------------------------------------------------------------
TLSection* TableLoader::FindSection(String SectionName)
{
   for (int i=0; i<FSectionCount; ++i)
      if (SectionName == *FSections[i].Name)
         return &FSections[i];
   return NULL;
}
//--------------------------------------------------------------------
void TableLoader::GetCount(int &intCount, int &charCount, int &boolCount, int &strCount) const
{
	strCount = StrCount;
	intCount = IntCount;
	charCount = CharCount;
	boolCount = BoolCount;
}
//--------------------------------------------------------------------
//__fastcall TableLoader::~TableLoader()
//{
//}
//-----------------------------------------------------------------------
#pragma package(smart_init)
