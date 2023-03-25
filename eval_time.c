#include <stdio.h>
#include <time.h>
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

void main()
{
  int tic, toc;
  float ans = 0;
  float val1 = 1;
  float val2 = 1;

  floater ans_;
  ans_.man = 0;
  ans_.exp = 0;
  floater val1_;
  val1_.man = POW_2_30;
  val1_.exp = 0;
  floater val2_;
  val2_.man = POW_2_30;
  val2_.exp = 0;

  floater cons1;
  float2floater(&cons1, 1.1);
  floater cons2;
  float2floater(&cons2, 0.9);

  tic = clock();
  for (int i = 0; i <= 10000; i++)
  {
    f_add(&ans_, ans_, val1_);
    f_sub(&ans_, ans_, val2_);
    f_mul_precise(&val1_, val1_, cons1);
    f_mul_precise(&val1_, val1_, val2_);
    f_mul_precise(&val2_, val2_, cons2);
  }
  toc = clock();
  printf("floater time:%d\n", toc - tic);

  tic = clock();
  for (int i = 0; i <= 10000; i++)
  {
    ans += val1 - val2;
    val1 *= 1.1;
    val1 *= val2;
    val2 *= 0.9;
  }
  toc = clock();
  printf("float time:%d\n", toc - tic);

  printf("1Ans:%f, val1:%f, val2:%f \n", floater2float(ans_), floater2float(val1_), floater2float(val2_));
  printf("2Ans:%f, val1:%f, val2:%f \n", ans, val1, val2);
  return;
}