// Copyright 2022 Larkin
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.
/******************************************************************\
* TableLoader - Çàãðóçêà òàáëèö èç òåêñòîâûõ ôàéëîâ. Âåðñèÿ 1.7    *
\******************************************************************/
#ifndef TableLoaderH
#define TableLoaderH

struct TLSection
{
	String *Name;
	int Pos;	//Ïîçèöèÿ ñåêöèè ñ 0
	int Size;//Êîë-âî ñòðîê
};

class TableLoader
{
private:
   char *FFormat;
	//Mem îáÿçàòåëüíî [Count][FRowCount]
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
	bool IgnoreFirstString; //Èãíîðèðîâàòü ïåðâóþ ñðîêó â ôàéëå. defaul=true
	bool IgnoreDelimitersPack; //Èãíîðèðîâàòü ðàçäåëèòåëè, èäóùèå ïîäðÿä. defauly=true
	Char Delimiter;      //Ðàçäåëèòåëü ñòîëáöîâ (òàáóëÿöèÿ)
   String EndMark; 		//"[end]"
	//Çàãðóçêà, format: i-int c-Char s-String b-bool, ... ñïèñîê ññûëîê íà ïåðåìåííûå
	int LoadFromFile(String Filename, const char *format, ...);
	int LoadFromResource(String ResourceName, const char *format, ...);
	int LoadFromList(TStrings *list, const char *format, ...);
	//GetSection: ... ýòî ñïèñîê ññûëîê, òàêæå êàê â LoadFrom. Âîçâðàùàåò êîë-âî ñòðîê â ñåêöèè
	int GetSection(const String SectionName="", ...);
	//RegColumn - óñòàíàâëèâàåò ññûëêó íà ñòîëáåö â ñåêöèè. ColNum - Íîìåð ñòîëáöà ñîîòâ òèïà. Âîçâðàùàåò êîë-âî ñòðîê â ñåêöèè
	int RegColumn(int* &Field, int ColNum, const String SectionName = "") const;
	int RegColumn(Char* &Field, int ColNum, const String SectionName = "") const;
	int RegColumn(String* &Field, int ColNum, const String SectionName = "") const;
	int RegColumn(bool* &Field, int ColNum, const String SectionName = "") const;
	void GetCount(int &intCount, int &charCount, int &boolCount, int &strCount) const;
	void Clear();
	__fastcall ~TableLoader() { Clear(); };
};

#endif
