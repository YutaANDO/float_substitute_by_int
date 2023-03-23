#include <stdio.h>
#include <string.h>
#include <math.h>

#define POW_2_30 1073741824
#define MUSK_UPPER 0x8000FFFF

// 数字をfloater.man / POW_2_30 * 2^floater.expと表現する
typedef struct float_coverd_int
{
  // 仮数部 mantissa 例えば1.0のときは 1*2^30を入れる
  // 先頭bitは必ず1なので，本来であれば省略できるが実装を簡単にするため，省略しない
  int man;

  // 指数部 exponent 例えば1.0のときは 0を入れる
  int exp;
} floater;

void floater_ini(floater *val)
{
  (*val).man = 0;
  (*val).exp = 0;
}

// 動作確認用のコード 組み込み系には入れない
// floatをfloaterに変換するコード
void float2floater(floater *val, float arg)
{
  (*val).exp = 0;
  if (arg != 0)
  {
    while (1)
    {
      if (arg <= -2 || 2 <= arg)
      {
        (*val).exp++;
        arg /= 2;
      }
      else if (-1 < arg && arg < 1)
      {
        (*val).exp--;
        arg *= 2;
      }
      else
      {
        break;
      }
    }
    (*val).man = arg * POW_2_30;
  }
  else
  {
    (*val).man = 0;
  }
}

// 動作確認用のコード 組み込み系には入れない
// floatをfloaterに変換するコード
float floater2float(floater val)
{
  return (float)val.man / POW_2_30 * pow(2, val.exp);
}

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

// c=a+b
void f_add(floater *c, floater a, floater b)
{

  if (a.exp > b.exp)
  {
    b.exp = a.exp - b.exp;
    b.man = b.man >> (a.exp - b.exp);
  }
  else
  {
    a.exp = b.exp - a.exp;
    a.man = a.man >> (b.exp - a.exp);
  }
  (*c).exp = ++a.exp;

  (*c).man = (a.man >> 1) + (b.man >> 1);
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
  printf("bman=%d, %d\n", b.man, b.exp);
  printf("bman.d %d\n", (((b.man >> 16) * ((a.man & MUSK_UPPER) >> 2)) >> 14));
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

int main(int argc, char const *argv[])
{
  floater val;
  floater val2;
  floater_ini(&val);

  float2floater(&val, 81);
  float2floater(&val2, 9);

  printf("int = %d\n", floater2int(val));
  printf("%d, %d, \n", val.man, val.exp);
  printf("%20.10f\n", floater2float(val));

  // f_reciprocal(&val2, val2);
  // f_mul(&val, val, val2);
  f_div(&val, val, val2);

  printf("%d, %d, \n", val.man, val.exp);
  printf("%20.10f\n", floater2float(val));

  int a = 5;

  return 0;
}
