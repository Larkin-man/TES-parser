// Copyright 2026 Larkin
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
//#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "TableLoader.h"
#include <System.IOUtils.hpp>	// Для FileExists
#include <memory.h>				// Для memset/memcpy
//---------------------------------------------------------------------------

__fastcall TableLoader::TableLoader()
{
	MemStr = NULL;
	MemInt = NULL;
	MemChar = NULL;
	MemBool = NULL;
	MemFloat = NULL;

	StrCount = 0;
	IntCount = 0;
	CharCount = 0;
	BoolCount = 0;
	FloatCount = 0;
	
	FRowCount = 0;
	FColCount = 0;
	
	IgnoreFirstString = false;
	IgnoreDelimitersPack = false;
	
	FFormat = NULL;
	FLastError = "";
	FFileName = "";
	
	Delimiter = '\t';
	EndMark = "[end]";
	
	FSections = NULL;
	FSectionCount = 0;
	SectionCapacity = 0;
}
//---------------------------------------------------------------------------

__fastcall TableLoader::~TableLoader()
{
	Clear();
}
//---------------------------------------------------------------------------

int TableLoader::RealGetSection(int SectionIdx, va_list& args)
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
	
	int intCount = 0, strCount = 0, charCount = 0, boolCount = 0, floatCount = 0;
	int** intArg;
	String** strArg;
	char** charArg;
	bool** boolArg;
	float** floatArg;
	
	for (unsigned int i = 0; i < strlen(FFormat); ++i)
	{
		switch (FFormat[i])
		{
			case 'i':
			case 'I':
				if ((intArg = va_arg(args, int**)) != NULL)
					*intArg = MemInt[intCount++] + sPos;
				break;
			case 's':
			case 'S':
				if ((strArg = va_arg(args, String**)) != NULL)
					*strArg = MemStr[strCount++] + sPos;
				break;
			case 'c':
			case 'C':
				if ((charArg = va_arg(args, char**)) != NULL)
					*charArg = MemChar[charCount++] + sPos;
				break;
			case 'b':
			case 'B':
				if ((boolArg = va_arg(args, bool**)) != NULL)
					*boolArg = MemBool[boolCount++] + sPos;
				break;
			case 'f':
			case 'F':		 // НОВОЕ: поддержка float
				if ((floatArg = va_arg(args, float**)) != NULL)
					*floatArg = MemFloat[floatCount++] + sPos;
				break;
		}
	}
	
	va_end(args);
	return sSize;
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::GetSection(String SectionName, ...)
{
	if (FFormat == NULL)
		return 0;
	
	va_list ap;
	va_start(ap, SectionName);
	
	if (!SectionName.IsEmpty())
	{
		for (int i = 0; i < FSectionCount; ++i)
		{
			if (FSections[i].Name != NULL && SectionName == *FSections[i].Name)
			{
				int result = RealGetSection(i, ap);
				return result;
			}
		}
		va_end(ap);
		return 0;
	}
	
	int result = RealGetSection(-1, ap);
	return result;
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::RegColumn(int*& Field, int ColNum, String SectionName)
{
	ColNum--; // Конвертация из 1-based в 0-based
	
	if (ColNum < 0 || ColNum >= IntCount)
		return -1;
	
	if (!SectionName.IsEmpty())
	{
		for (int i = 0; i < FSectionCount; ++i)
		{
			if (FSections[i].Name != NULL && SectionName == *FSections[i].Name)
			{
				Field = MemInt[ColNum] + FSections[i].Pos;
				return FSections[i].Size;
			}
		}
		return 0;
	}
	else
	{
		Field = MemInt[ColNum];
		return FRowCount;
	}
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::RegColumn(char*& Field, int ColNum, String SectionName)
{
	ColNum--;
	
	if (ColNum < 0 || ColNum >= CharCount)
		return -1;
	
	if (!SectionName.IsEmpty())
	{
		for (int i = 0; i < FSectionCount; ++i)
		{
			if (FSections[i].Name != NULL && SectionName == *FSections[i].Name)
			{
				Field = MemChar[ColNum] + FSections[i].Pos;
				return FSections[i].Size;
			}
		}
		return 0;
	}
	else
	{
		Field = MemChar[ColNum];
		return FRowCount;
	}
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::RegColumn(String*& Field, int ColNum, String SectionName)
{
	ColNum--;
	
	if (ColNum < 0 || ColNum >= StrCount)
		return -1;
	
	if (!SectionName.IsEmpty())
	{
		for (int i = 0; i < FSectionCount; ++i)
		{
			if (FSections[i].Name != NULL && SectionName == *FSections[i].Name)
			{
				Field = MemStr[ColNum] + FSections[i].Pos;
				return FSections[i].Size;
			}
		}
		return 0;
	}
	else
	{
		Field = MemStr[ColNum];
		return FRowCount;
	}
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::RegColumn(bool*& Field, int ColNum, String SectionName)
{
	ColNum--;
	
	if (ColNum < 0 || ColNum >= BoolCount)
		return -1;
	
	if (!SectionName.IsEmpty())
	{
		for (int i = 0; i < FSectionCount; ++i)
		{
			if (FSections[i].Name != NULL && SectionName == *FSections[i].Name)
			{
				Field = MemBool[ColNum] + FSections[i].Pos;
				return FSections[i].Size;
			}
		}
		return 0;
	}
	else
	{
		Field = MemBool[ColNum];
		return FRowCount;
	}
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::RegColumn(float*& Field, int ColNum, String SectionName)
{
	ColNum--;
	
	if (ColNum < 0 || ColNum >= FloatCount)
		return -1;
	
	if (!SectionName.IsEmpty())
	{
		for (int i = 0; i < FSectionCount; ++i)
		{
			if (FSections[i].Name != NULL && SectionName == *FSections[i].Name)
			{
				Field = MemFloat[ColNum] + FSections[i].Pos;
				return FSections[i].Size;
			}
		}
		return 0;
	}
	else
	{
		Field = MemFloat[ColNum];
		return FRowCount;
	}
}
//---------------------------------------------------------------------------

void TableLoader::Load(TStringList* list, const char* format)
{
	FLastError = "";
	
	if (list == NULL || list->Count == 0)
	{
		FLastError = "Список строк пуст";
		return;
	}
	
	if (IgnoreFirstString)
	{
		if (list->Count <= 1)
			return;
		else
			list->Delete(0);
	}
	
	if (FRowCount > 0)
		Clear();
	
	// Нужно пробежаться по файлу и удалить лишние строки и посчитать FRowCount
	FRowCount = -1;
	String str;
	bool EndIsPossible = !EndMark.IsEmpty();
	
	for (int i = 0; i < list->Count; i++)
	{
		str = list->Strings[i];
		
		if (str.IsEmpty())
		{
			list->Delete(i);
			i--;
			continue;
		}
		
		// EndMark проверяем только в начале строки
		if (EndIsPossible && (str.Pos(EndMark) == 1))
		{
			FRowCount = i;
			break;
		}
		
		// Проверка на секцию [name]
		if ((!str.IsEmpty()) && (str[1] == '[') && (str.Pos(']') > 0))
		{
			SectionEnsureCapacity(FSectionCount + 1);
			FSections[FSectionCount].Pos = i;
			FSections[FSectionCount].Name = new String(str.SubString(2, str.Pos(']') - 2));
			FSectionCount++;
			list->Delete(i);
			i--;
			continue;
		}
	}
	
	if (FRowCount < 0)
		FRowCount = list->Count;
	
	if (FRowCount == 0)
		return;
	
	// Установить размер секций
	if (FSectionCount > 0)
	{
		for (int i = 0; i < FSectionCount - 1; ++i)
			FSections[i].Size = FSections[i + 1].Pos - FSections[i].Pos;
		FSections[FSectionCount - 1].Size = FRowCount - FSections[FSectionCount - 1].Pos;
	}
	
	StrCount = 0;
	IntCount = 0;
	CharCount = 0;
	BoolCount = 0;
	FloatCount = 0;
	
	// Разбор строки формата
	FFormat = strdup(format);
	int FormatLen = strlen(format);
	
	for (int i = 0; i < FormatLen; ++i)
	{
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
			case 'F':
			case 'f': FloatCount++; break;
			default: FFormat[i] = '0';
		}
	}
	
	FColCount = IntCount + CharCount + StrCount + BoolCount + FloatCount;
	
	if (FColCount == 0)
		return;
	
	// Создание массивов по количеству строк в файле
	if (IntCount > 0)
	{
		MemInt = new int*[IntCount];
		for (int i = 0; i < IntCount; ++i)
			MemInt[i] = new int[FRowCount];
	}
	
	if (CharCount > 0)
	{
		MemChar = new char*[CharCount];
		for (int i = 0; i < CharCount; ++i)
			MemChar[i] = new char[FRowCount];
	}
	
	if (StrCount > 0)
	{
		MemStr = new String*[StrCount];
		for (int i = 0; i < StrCount; ++i)
			MemStr[i] = new String[FRowCount];
	}
	
	if (BoolCount > 0)
	{
		MemBool = new bool*[BoolCount];
		for (int i = 0; i < BoolCount; ++i)
			MemBool[i] = new bool[FRowCount];
	}
	
	if (FloatCount > 0)
	{
		MemFloat = new float*[FloatCount];
		for (int i = 0; i < FloatCount; ++i)
			MemFloat[i] = new float[FRowCount];
	}
	
	// Начало считывания файла и разбор по словам
	int p;
	AnsiString word;
	int curr = 0, currStr = 0, currInt = 0, currChar = 0, currBool = 0, currFloat = 0;
	
	for (int i = 0; i < FRowCount; ++i)
	{
		curr = 0;
		currStr = 0;
		currInt = 0;
		currChar = 0;
		currBool = 0;
		currFloat = 0;
		
		str = list->Strings[i];
		
		while (!str.IsEmpty())
		{
			// проверяем первый символ
			if (IgnoreDelimitersPack && (!str.IsEmpty()) && (str[1] == Delimiter))
			{
				str.Delete(1, 1);
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
				word = str.SubString(1, p - 1);
				str.Delete(1, p);
			}
			
			// Слово получено
			try
			{
				while (curr < FormatLen)
				{
					switch (FFormat[curr])
					{
						case 'I':
							currInt++;
							curr++;
							continue;
						case 'C':
							currChar++;
							curr++;
							continue;
						case 'S':
							currStr++;
							curr++;
							continue;
						case 'B':
							currBool++;
							curr++;
							continue;
						case 'F':
							currFloat++;
							curr++;
							continue;
							
						case 'i':
							MemInt[currInt][i] = word.ToIntDef(0);
							currInt++;
							break;
						case 'c':
							if (word.IsEmpty())
								MemChar[currChar][i] = '\0';
							else
								MemChar[currChar][i] = word[1];
							currChar++;
							break;
						case 's':
							MemStr[currStr][i] = word;
							currStr++;
							break;
						case 'b':
							if (word.IsEmpty())
								MemBool[currBool][i] = false;
							else
								MemBool[currBool][i] = (word[1] != '0');
							currBool++;
							break;
						case 'f':
							if (word.IsEmpty())
								MemFloat[currFloat][i] = 0.0f;
							else
								MemFloat[currFloat][i] = static_cast<float>(word.ToDouble());
							currFloat++;
							break;
					}
					break;
				}
			}
			catch (const Exception& e)
			{
				// Обработка ошибок парсинга с установкой значений по умолчанию
				switch (FFormat[curr])
				{
					case 'i': MemInt[currInt][i] = 0; currInt++; break;
					case 'c': MemChar[currChar][i] = '\0'; currChar++; break;
					case 's': MemStr[currStr][i] = ""; currStr++; break;
					case 'b': MemBool[currBool][i] = false; currBool++; break;
					case 'f': MemFloat[currFloat][i] = 0.0f; currFloat++; break;
				}
			}
			
			curr++;
			if (curr >= FormatLen)
				break;
		}
	}
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::LoadFromFile(String Filename, const char* format, ...)
{
	// проверка существования файла
	if (!FileExists(Filename))
	{
		FLastError = "Файл не найден: " + Filename;
		return 0;
	}
	
	// проверка формата
	if (format == NULL || strlen(format) == 0)
	{
		FLastError = "Неверный формат загрузки";
		return 0;
	}
	
	TStringList* file = NULL;
	try
	{
		file = new TStringList;
		file->LoadFromFile(Filename);
		FFileName = Filename;
		
		Load(file, format);
		
		va_list ap;
		va_start(ap, format);
		RealGetSection(-1, ap);
		// va_end вызывается в RealGetSection
	}
	__finally
	{
		if (file != NULL)
			delete file;
	}
	
	return FRowCount;
}
//---------------------------------------------------------------------------
int __cdecl TableLoader::LoadFromResource(String ResourceName, const char* format, ...)
{
	// проверка формата
	if (format == NULL || strlen(format) == 0)
	{
		FLastError = "Неверный формат загрузки";
		return 0;
	}

	TStringList* file = NULL;
	TResourceStream* resStream = NULL;
	try
	{
		// правильное создание TResourceStream
		resStream = new TResourceStream((NativeUInt)HInstance, ResourceName, reinterpret_cast<System::WideChar*>(RT_RCDATA));
		//resStream = new TResourceStream((NativeUInt)HInstance, ResourceName, (System::WideChar*)RT_RCDATA);

		file = new TStringList;
		file->LoadFromStream(resStream);

		Load(file, format);

		va_list ap;
		va_start(ap, format);
		RealGetSection(-1, ap);
	}
	__finally
	{
		if (file != NULL)
			delete file;
		if (resStream != NULL)
			delete resStream;
	}

	return FRowCount;
}
//---------------------------------------------------------------------------

int __cdecl TableLoader::LoadFromList(TStrings* list, const char* format, ...)
{
	// проверка формата
	if (format == NULL || strlen(format) == 0)
	{
		FLastError = "Неверный формат загрузки";
		return 0;
	}
	
	if (list == NULL)
	{
		FLastError = "Список строк пуст";
		return 0;
	}
	
	Load((TStringList*)list, format);
	
	va_list ap;
	va_start(ap, format);
	RealGetSection(-1, ap);
	
	return FRowCount;
}
//---------------------------------------------------------------------------

void TableLoader::SectionEnsureCapacity(int count)
{
	if (count > SectionCapacity)
	{
		int newCapacity = (SectionCapacity == 0) ? INITIAL_SECTION_CAPACITY : SectionCapacity;
		
		while (newCapacity < count)
			newCapacity *= 2;
		
		TLSection* newSections = new TLSection[newCapacity];
		
		// Копируем старые данные
		if (FSections != NULL)
		{
			for (int i = 0; i < FSectionCount; ++i)
			{
				newSections[i] = FSections[i];
			}
			delete[] FSections;
		}
		
		// Инициализируем новые элементы
		for (int i = FSectionCount; i < newCapacity; ++i)
		{
			newSections[i].Name = NULL;
			newSections[i].Pos = 0;
			newSections[i].Size = 0;
		}
		
		FSections = newSections;
		SectionCapacity = newCapacity;
	}
}
//---------------------------------------------------------------------------

void __cdecl TableLoader::Clear()
{
	if (FFormat != NULL)
	{
		free(FFormat);
		FFormat = NULL;
	}
	
	if (FSections != NULL)
	{
		for (int i = 0; i < FSectionCount; ++i)
		{
			if (FSections[i].Name != NULL)
			{
				delete FSections[i].Name;
				FSections[i].Name = NULL;
			}
		}
		delete[] FSections;
		FSections = NULL;
		SectionCapacity = 0;
		FSectionCount = 0;
	}
	
	if (IntCount > 0 && MemInt != NULL)
	{
		for (int i = 0; i < IntCount; ++i)
			delete[] MemInt[i];
		delete[] MemInt;
		MemInt = NULL;
		IntCount = 0;
	}
	
	if (CharCount > 0 && MemChar != NULL)
	{
		for (int i = 0; i < CharCount; ++i)
			delete[] MemChar[i];
		delete[] MemChar;
		MemChar = NULL;
		CharCount = 0;
	}
	
	if (BoolCount > 0 && MemBool != NULL)
	{
		for (int i = 0; i < BoolCount; ++i)
			delete[] MemBool[i];
		delete[] MemBool;
		MemBool = NULL;
		BoolCount = 0;
	}
	
	if (StrCount > 0 && MemStr != NULL)
	{
		for (int i = 0; i < StrCount; ++i)
			delete[] MemStr[i];
		delete[] MemStr;
		MemStr = NULL;
		StrCount = 0;
	}
	
	if (FloatCount > 0 && MemFloat != NULL)
	{
		for (int i = 0; i < FloatCount; ++i)
			delete[] MemFloat[i];
		delete[] MemFloat;
		MemFloat = NULL;
		FloatCount = 0;
	}
	
	FRowCount = 0;
	FColCount = 0;
	FLastError = "";
	FFileName = "";
}
//---------------------------------------------------------------------------

TLSection* __cdecl TableLoader::FindSection(String SectionName)
{
	if (FSections == NULL)
		return NULL;
	
	for (int i = 0; i < FSectionCount; ++i)
	{
		if (FSections[i].Name != NULL && SectionName == *FSections[i].Name)
			return &FSections[i];
	}
	
	return NULL;
}
//---------------------------------------------------------------------------

void __cdecl TableLoader::GetCount(int& intCount, int& charCount, int& boolCount, int& strCount, int& floatCount)
{
	intCount = IntCount;
	charCount = CharCount;
	boolCount = BoolCount;
	strCount = StrCount;
	floatCount = FloatCount;
}
//---------------------------------------------------------------------------