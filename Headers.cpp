//---------------------------------------------------------------------------
#pragma hdrstop

#include "Headers.h"
//---------------------------------------------------------------------------
void MHeader::Read(FILE* &file, int lensize)
{
	// 1. Читаем имя тега и его длину из файла
	fread(&Name, 4, 1, file);
    unsigned int actualLength = 0;
    fread(&actualLength, lensize, 1, file);
	Length = actualLength;
    // 2. Исправление Бага 3: Обработка превышения лимита
	if (Length > LENSTOP)
	{
        ShowMessage("Предупреждение: Длина блока (" + IntToStr((int)Length) +
                    ") превышает лимит " + IntToStr((int)LENSTOP));
        // Расширяем лимит, чтобы программа не падала на больших файлах
		while (LENSTOP <= Length)
			LENSTOP *= 2;
	}
    // 3. Исправление Багов 1 и 2: Выделение памяти с запасом под нуль-терминатор (+1)
    if (Length + 1 > Capacity)
    {
        // Выделяем память временному указателю, чтобы не потерять старый при ошибке
        void* newData = realloc(Data, Length + 1);
        if (newData == NULL)
		{
			ShowMessage("Критическая ошибка: Не удалось выделить " + IntToStr((int)(Length + 1)) + " байт памяти!");
			return; // Выходим, сохранив старый Data целым
		}
        Data = newData;
        Capacity = Length + 1;
	}
    // 4. Читаем ровно столько байт, сколько написано в файле
	if (Length > 0)
		fread(Data, Length, 1, file);
    // 5. Безопасно ставим нуль-терминатор. Теперь под него ТОЧНО есть место в рамках Capacity
    reinterpret_cast<char*>(Data)[Length] = '\0';
}
//---------------------------------------------------------------------------
void MHeader::Write(FILE* &file)
{
	fwrite(&Name, 4, 1, file);
	fwrite(&Length, 4, 1, file);
	fwrite(Data, Length, 1, file);
}

void TRECORD3INT::Write(FILE* &file)
{
	fwrite(&Name, 4, 1, file);
	fwrite(&i, 4, 3, file);
}

bool TRECORD1INT1STR::Create()
{
	if (Length <= 1024) Data = new char[Length];
	else return false;
	return true;
}
void TRECORD1INT1STR::Write(FILE* &file)
{
	fwrite(&NAME, 4, 1, file);
	fwrite(&Length, 4, 1, file);
	fwrite(Data, Length, 1, file);
}
void FRMR::Set(FILE* &file, int FRMRoffset)
{
	fseek(file, FRMRoffset + 8, SEEK_SET);
	int Data;
	fread(&Data, 4, 1, file);
	mast = Data / 16777216;
	frmr = Data % 16777216;
}
//---------------------------------------------------------------------------
bool Coord6::TextToFloat3(String str)
{
	if (str.IsEmpty()) return true;
	// Подготавливаем настройки формата, чтобы точка ВСЕГДА воспринималась как разделитель
	TFormatSettings settings = TFormatSettings::Create();
	settings.DecimalSeparator = L'.';
	// Если в вашей системе гарантированно используется запятая, поменяйте на L','
	// Но если в строке могут быть и точки, и запятые, то нормализуем строку ОДНИМ проходом:
	wchar_t* pStart = str.c_str();
	// Быстрая замена запятых на точки (или наоборот) за один проход по памяти
	for (int i = 0; pStart[i] != L'\0'; i++)
		if (pStart[i] == L',') pStart[i] = L'.';
	wchar_t* pEnd;
	// Использование wcstod автоматически пропускает любые группы пробелов!
	// Она считывает число, а в pEnd записывает указатель на место, где оно закончилось.
	x = static_cast<float>(std::wcstod(pStart, &pEnd));
	if (pStart == pEnd) return true; // Не удалось прочитать X
	pStart = pEnd;
	y = static_cast<float>(std::wcstod(pStart, &pEnd));
	if (pStart == pEnd) return true; // Не удалось прочитать Y
	pStart = pEnd;
	z = static_cast<float>(std::wcstod(pStart, &pEnd));
	if (pStart == pEnd) return true; // Не удалось прочитать Z
	return false; // Успешно прочитано
}
//---------------------------------------------------------------------------

void Coord6::TextToFloat6(String str)
{
	wchar_t* pStart = str.c_str(); // Получаем указатель на строку
	for (int i = 0; i < 5; i++)
	{
		// _wtof считывает float/double до первого пробела
		all[i] = static_cast<float>(_wtof(pStart));
		// Сдвигаем указатель за пробел к следующему числу
		while (*pStart && *pStart != L' ')
			pStart++;
		if (*pStart == L' ')
			pStart++; // Пропускаем сам пробел
	}
	// Записываем последнее 6-е число
	rz = static_cast<float>(_wtof(pStart));
}
//---------------------------------------------------------------------------
void Coord6::Round()
{
	for (int i = 0; i < 3; i++)
		all[i] = round(all[i]);
}
//---------------------------------------------------------------------------
float Coord6::round(float x)
{
	if (x >= 0)
		return floor(x + 0.5f); //в сторону меньшего
	return ceil(x - 0.5f); //в сторону большего
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
