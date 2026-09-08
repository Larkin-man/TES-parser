//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <float.h>
#pragma hdrstop
#include "TableLoader.h"
#include "Headers.h"
#include "Parser_code.h"
// Вспомогательный метод для красивого и безопасного добавления
void TForm1::AddTagType(const String& name, char type, const String& maintag)
{
	TagInfo info;
	info.type = type;
	info.mainTag = maintag; // Если maintag не передан, сюда запишется пустая строка ""
	tagMap[name] = info;
}

void TForm1::CreateTags()
{
	tagMap.clear();
	AddTagType("INTV",'4'); AddTagType("NAME",'t');
	AddTagType("NAM5",'1'); AddTagType("FLTV",'g');
	AddTagType("MVRF",'i'); AddTagType("SPDT",'4');
	AddTagType("NAM0",'i'); AddTagType("NPDT",'4');
	AddTagType("CNDT",'g'); AddTagType("SCRI",'t');
	AddTagType("XSCL",'g'); AddTagType("MODL",'t');
	AddTagType("MAST",'t'); AddTagType("SCTX",'t');
	AddTagType("RGNN",'t'); AddTagType("SCHD",'t');
	AddTagType("AIDT",'t'); AddTagType("NPCS",'t');
	//skyrim
	AddTagType("SCVR",'t'); AddTagType("EDID",'t');
	AddTagType("FULL",'t'); AddTagType("MOD2",'t');
	AddTagType("MOD3",'t'); AddTagType("ICON",'t');
	AddTagType("NIFZ",'t'); AddTagType("MICO",'t');
	AddTagType("NAM1",'t'); AddTagType("MOD4",'t');
	AddTagType("ICO2",'t'); AddTagType("DESC",'t');
	AddTagType("ITXT",'t'); AddTagType("RDMP",'t');
	AddTagType("XATO",'t');
}
//---------------------------------------------------------------------------
void TForm1::PrintAllTags()
{
	// Проходим циклом по всей карте от начала до конца
	std::map<String, TagInfo>::const_iterator it;
	for (it = tagMap.begin(); it != tagMap.end(); ++it)
	{
		// it->first  — это КЛЮЧ карты (имя тега, например "INTV")
		// it->second — это ЗНАЧЕНИЕ (структура TagInfo с полями type и mainTag)
		String tagName = it->first;
		char tagType = it->second.type;
		// Выводим в Out (TMemo или TRichEdit)
		Out->Lines->Add(tagName + "\t" + String(tagType));
	}
}
//---------------------------------------------------------------------------
