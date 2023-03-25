#define POW_2_30 1073741824

// 数字をfloater.man / POW_2_30 * 2^floater.expと表現する
typedef struct float_coverd_int
{
  // 仮数部 mantissa 例えば1.0のときは 1*2^30を入れる
  // 先頭bitは必ず1なので，本来であれば省略できるが実装を簡単にするため，省略しない
  int man;

  // 指数部 exponent 例えば1.0のときは 0を入れる
  int exp;
} floater;

void f_adjust(floater *val);

void floater_ini(floater *val);

void f_add(floater *c, floater a, floater b);

void f_sub(floater *c, floater a, floater b);

void f_mul(floater *c, floater a, floater b);

void f_mul_precise(floater *c, floater a, floater b);

void f_div(floater *c, floater a, floater b);

void f_div_precise(floater *c, floater a, floater b);

int floater2int(floater val);