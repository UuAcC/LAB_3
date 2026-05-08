// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len <= 0) throw negativeSize;
    BitLen = len;
    MemLen = (BitLen + BIT_TELEM_SIZE - 1) >> deg2; 
    pMem = new TELEM[MemLen];
    memset(pMem, 0, TELEMsize * MemLen);
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, TELEMsize * MemLen);
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return (n >> deg2);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int shift = n & ((1 << deg2) - 1); // n & ((1 << (int)log2(sizeof(TELEM) * 8)) - 1)
    return 1 << shift;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) throw negativeIndex;
    if (n > this->BitLen) throw tooLargeIndex;
    this->pMem[this->GetMemIndex(n)] |= this->GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0) throw negativeIndex;
    if (n > BitLen) throw tooLargeIndex;
    this->pMem[this->GetMemIndex(n)] &= ~(this->GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0) throw negativeIndex;
    if (n > BitLen) throw tooLargeIndex;
    return this->pMem[this->GetMemIndex(n)] & this->GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, TELEMsize * MemLen);
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (size_t i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    int min_len = GetMemIndex((BitLen > bf.BitLen) ? bf.BitLen : BitLen) + 1;
    TBitField res = TBitField(max_len);
    size_t i = 0;
    for (; i < min_len; i++)
        res.pMem[i] = pMem[i] | bf.pMem[i];
    for (; i < res.MemLen; i++)
        res.pMem[i] |= bf.pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    int min_len = GetMemIndex((BitLen > bf.BitLen) ? bf.BitLen : BitLen) + 1;
    TBitField res = TBitField(max_len);
    for (size_t i = 0; i < min_len; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(*this);
    for (size_t i = 0; i < res.MemLen; i++)
        res.pMem[i] = ~(res.pMem[i]);
    int rest = res.BitLen & ((1 << deg2) - 1);
    res.pMem[res.MemLen - 1] &= ((1 << rest) - 1);
    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    /*int size; istr << size;
    bf = TBitField(size);*/
    int n, elem; istr >> n;
    for (size_t i = 0; i < n; i++)
    {
        istr >> elem;
        bf.SetBit(elem);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    ostr << "BitLen: " << bf.BitLen << std::endl;
    ostr << "Memory: "; bool flag = false;
    for (size_t i = 0; i < bf.MemLen; i++)
    {
        if (flag) ostr << ", ";
        else flag = true;
        for (size_t j = 0; j < BIT_TELEM_SIZE; j++)
        {
            if (bf.pMem[i] & (1 << j)) ostr << "1";
            else ostr << "0";
        }
    }
    ostr << endl;
    return ostr;
}
