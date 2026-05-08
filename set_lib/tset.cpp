// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(1)
{
    if (mp < 0) throw negativeSize;
    this->MaxPower = mp;
    this->BitField = TBitField(MaxPower);
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(1)
{
    this->MaxPower = s.MaxPower;
    this->BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(1)
{
    this->MaxPower = bf.GetLength();
    this->BitField = bf;
}

TSet::operator TBitField()
{
    return this->BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return this->MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return this->BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    this->BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    this->BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (*this == s) return *this;
    this->BitField = s.BitField;
    this->MaxPower = s.MaxPower;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (this->MaxPower != s.MaxPower) return 0;
    return this->BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if (this->MaxPower != s.MaxPower) return 1;
    return this->BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    int mp = (this->MaxPower > s.MaxPower) ? this->MaxPower : s.MaxPower;
    TSet res = TSet(mp);
    res.BitField = this->BitField | s.BitField;
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    // ифы сюда
    TSet res = TSet(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    // ифы сюда
    TSet res = TSet(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int mp = (this->MaxPower > s.MaxPower) ? this->MaxPower : s.MaxPower;
    TSet res = TSet(mp);
    res.BitField = this->BitField & s.BitField;
    return res;
}

TSet TSet::operator~(void) // дополнение
{
    TSet res = TSet(*this);
    res.BitField = ~(res.BitField);
    return res;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int size; istr >> size;
    s = TSet(size);
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << s.BitField;
    return ostr;
}