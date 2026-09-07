// Copyright 2026 Larkin
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

/***********************************************************************
* TableLoader - Загрузка таблиц из текстовых файлов. Версия 2.1
* Добавлена поддержка типа float, исправлены критические ошибки
***********************************************************************/

#ifndef TableLoaderH
#define TableLoaderH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

// Константы для инициализации
const int INITIAL_SECTION_CAPACITY = 4;

struct TLSection
{
	String* Name;		// Имя секции
	int Pos;				// Позиция секции с 0
	int Size;			// Количество строк
};

class TableLoader
{
private:
	char* FFormat;
	String FLastError;	// Последняя ошибка
	String FFileName;	// Имя загруженного файла
	
	// Mem обязательно [Count][FRowCount]
	String** MemStr;
	int** MemInt;
	char** MemChar;
	bool** MemBool;
	float** MemFloat;
	
	int StrCount, IntCount, CharCount, BoolCount, FloatCount;
	int FRowCount;
	int FColCount;
	
protected:
	TLSection* FSections;
	void SectionEnsureCapacity(int count);
	int SectionCapacity;
	int FSectionCount;
	void Load(TStringList* list, const char* format);
	int RealGetSection(int SectionIdx, va_list& args);
	
public:
	__fastcall TableLoader();
	__fastcall ~TableLoader();

	__property char* Format = {read = FFormat};
	__property int RowCount = {read = FRowCount};
	__property int ColCount = {read = FColCount};
	__property int SectionCount = {read = FSectionCount};
	__property TLSection* Sections = {read = FSections};
	__property String LastError = {read = FLastError};
	__property String FileName = {read = FFileName};
	
	TLSection* FindSection(String SectionName);
	
	bool IgnoreFirstString;	 // Игнорировать первую строку в файле. default=false
	bool IgnoreDelimitersPack; // Игнорировать разделители, идущие подряд. default=false
	char Delimiter;			// Разделитель столбцов (табуляция)
	String EndMark;			// "[end]"
	
	// Загрузка, format: i-int, c-char, s-String, b-bool, f-float
	// ... список ссылок на переменные
	int __cdecl LoadFromFile(String Filename, const char* format, ...);
	int __cdecl LoadFromResource(String ResourceName, const char* format, ...);
	int __cdecl LoadFromList(TStrings* list, const char* format, ...);
	
	// GetSection: ... это список ссылок, также как в LoadFrom
	// Возвращает кол-во строк в секции
	int __cdecl GetSection(String SectionName, ...);
	
	// RegColumn - устанавливает ссылку на столбец в секции
	// ColNum - Номер столбца соотв типа. Возвращает кол-во строк в секции
	int __cdecl RegColumn(int*& Field, int ColNum, String SectionName);
	int __cdecl RegColumn(char*& Field, int ColNum, String SectionName);
	int __cdecl RegColumn(String*& Field, int ColNum, String SectionName);
	int __cdecl RegColumn(bool*& Field, int ColNum, String SectionName);
	int __cdecl RegColumn(float*& Field, int ColNum, String SectionName);
	
	void GetCount(int& intCount, int& charCount, int& boolCount, int& strCount, int& floatCount);
	void __cdecl Clear();
};

#endif