/* Testing Code */

#include <limits.h>
#include <math.h>

/* Routines used by floation point test code */

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
    union {
        unsigned u;
        float f;
    } a;
    a.u = u;
    return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
    union {
        unsigned u;
        float f;
    } a;
    a.f = f;
    return a.u;
}
//2
long test_copyLSB(long x)
{
  return (x & 0x1L) ? -1 : 0;
}
long test_dividePower2(long x, long n)
{
    long p2n = 1L<<n;
    return x/p2n;
}
long test_distinctNegation(long x) {
    return (long) (x != -x);
}
long test_anyEvenBit(long x) {
  int i;
  for (i = 0; i < 64; i+=2)
      if (x & (1L<<i))
          return 1L;
  return 0L;
}
//3
long test_isLessOrEqual(long x, long y)
{
    return (long) (x <= y);
}
long test_replaceByte(long x, long n, long c)
{
    switch(n) {
    case 0:
      x = (x & 0xFFFFFFFFFFFFFF00L) | c;
      break;
    case 1:
      x = (x & 0xFFFFFFFFFFFF00FFL) | (c << 8);
      break;
    case 2:
      x = (x & 0xFFFFFFFFFF00FFFFL) | (c << 16);
      break;
    case 3:
      x = (x & 0xFFFFFFFF00FFFFFFL) | (c << 24);
      break;
    case 4:
      x = (x & 0xFFFFFF00FFFFFFFFL) | (c << 32);
      break;
    case 5:
      x = (x & 0xFFFF00FFFFFFFFFFL) | (c << 40);
      break;
    case 6:
      x = (x & 0xFF00FFFFFFFFFFFFL) | (c << 48);
      break;
    default:
      x = (x & 0x00FFFFFFFFFFFFFFL) | (c << 56);
      break;
    }
    return x;
}
long test_conditional(long x, long y, long z)
{
  return x?y:z;
}
long test_bitMask(long highbit, long lowbit)
{
  long result = 0L;
  int i;
  for (i = lowbit; i <= highbit; i++)
    result |= 1L << i;
  return result;
}
//4
long test_isPalindrome(long x) {
    long result = 1L;
    int i;
    long mask = 0xFFFFFFFFL;
    long xlo = x & mask;
    long xhi = (x >> 32) & mask;
    for (i = 0; i < 32; i++) {
        int flipi = 31-i;
        long bhigh = (xhi >> i) & 0x1L;
        long blow = (xlo >> flipi) & 0x1L;
        result = result && (long) (bhigh == blow);
    }
    return result;
}
long test_trueFiveEighths(long x)
{
  return (long) (((__int128) x * 5)/8);
}
long test_logicalNeg(long x)
{
  return !x;
}
