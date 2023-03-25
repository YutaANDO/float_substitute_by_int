#include <stdio.h>
#include <string.h>
#include <math.h>
#include "float2int.h"

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

int main(int argc, char const *argv[])
{
  floater val1;
  floater val2;
  floater ans;

  float2floater(&val1, 1.1);
  float2floater(&val2, 0.9);
  printf("val1 = %f, val2 = %f\n", floater2float(val1), floater2float(val2));

  f_add(&ans, val1, val2);
  printf("val1 + val2 = %f\n", floater2float(ans));
  f_sub(&ans, val1, val2);
  printf("val1 - val2 = %f\n", floater2float(ans));
  f_mul(&ans, val1, val2);
  printf("val1 * val2 = %f\n", floater2float(ans));
  f_div(&ans, val1, val2);
  printf("val1 / val2 = %f\n", floater2float(ans));

  return 0;
}
