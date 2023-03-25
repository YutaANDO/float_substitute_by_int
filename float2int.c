#include <stdio.h>
#include "float2int.h"

#define MUSK_UPPER 0x8000FFFF

// 値を整える
void f_adjust(floater *val)
{
  if ((*val).man == 0)
  {
    (*val).exp = 0;
  }
  else
  {

    while (-POW_2_30 < (*val).man && (*val).man < POW_2_30)
    { // man<2^30
      (*val).man = (*val).man << 1;
      (*val).exp--;
    }
  }
}

void floater_ini(floater *val)
{
  (*val).man = 0;
  (*val).exp = 0;
}

// c=a+b
void f_add(floater *c, floater a, floater b)
{

  if (a.exp > b.exp)
  {
    b.man = b.man >> (a.exp - b.exp);
    (*c).exp = ++a.exp;
  }
  else
  {
    a.man = a.man >> (b.exp - a.exp);
    (*c).exp = ++b.exp;
  }

  (*c).man = (a.man >> 1) + (b.man >> 1);
  f_adjust(c);
}

// c=a-b
void f_sub(floater *c, floater a, floater b)
{

  if (a.exp > b.exp)
  {
    b.man = b.man >> (a.exp - b.exp);
    (*c).exp = ++a.exp;
  }
  else
  {
    a.man = a.man >> (b.exp - a.exp);
    (*c).exp = ++b.exp;
  }

  (*c).man = (a.man >> 1) - (b.man >> 1);
  f_adjust(c);
}

// 掛け算 精度は15bit
void f_mul(floater *c, floater a, floater b)
{
  a.man = a.man >> 16;
  b.man = b.man >> 16;
  (*c).man = a.man * b.man;
  (*c).exp = (a.exp + b.exp + 2);
  f_adjust(c);
}

// 精度の高い掛け算
void f_mul_precise(floater *c, floater a, floater b)
{
  (*c).man = (a.man >> 16) * (b.man >> 16) + (((a.man >> 16) * ((b.man & MUSK_UPPER) >> 2)) >> 14) + (((b.man >> 16) * ((a.man & MUSK_UPPER) >> 2)) >> 14);
  (*c).exp = (a.exp + b.exp + 2);
  f_adjust(c);
}

// 割り算 第2引数は非ゼロである必要がある
void f_div(floater *c, floater a, floater b)
{
  b.man = b.man >> 15;
  (*c).man = (a.man / b.man);
  (*c).exp = a.exp - b.exp + 15;
  f_adjust(c);
}

// 精度の高い割り算
void f_div_precise(floater *c, floater a, floater b)
{
  (*c).man = (a.man / (b.man >> 15)) << 15;
  (*c).exp = a.exp - b.exp;
  f_adjust(c);
}

int floater2int(floater val)
{
  int tmp = val.exp - 30;
  if (tmp >= 0)
  {
    return (val.man << tmp);
  }
  else
  {
    return (val.man >> (-tmp));
  }
}