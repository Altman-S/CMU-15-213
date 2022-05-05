#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define TMin LONG_MIN
#define TMax LONG_MAX

#include "bits.h"
#include "btest.h"

test_rec test_set[] = {
//2
 {"copyLSB", (funct_t) copyLSB, (funct_t) test_copyLSB, 1,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"dividePower2", (funct_t) dividePower2, (funct_t) test_dividePower2, 2,
    "! ~ & ^ | + << >>", 15, 2,
  {{TMin, TMax},{0,62},{TMin,TMax}}},
 {"distinctNegation", (funct_t) distinctNegation, (funct_t) test_distinctNegation, 1, "! ~ & ^ | +", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"anyEvenBit", (funct_t) anyEvenBit, (funct_t) test_anyEvenBit, 1,
    "! ~ & ^ | + << >>", 14, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
//3
 {"isLessOrEqual", (funct_t) isLessOrEqual, (funct_t) test_isLessOrEqual, 2,
    "! ~ & ^ | + << >>", 24, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"replaceByte", (funct_t) replaceByte, (funct_t) test_replaceByte, 3,
    "! ~ & ^ | + << >>", 10, 3,
  {{TMin, TMax},{0,3},{0,255}}},
 {"conditional", (funct_t) conditional, (funct_t) test_conditional, 3, "! ~ & ^ | << >>", 16, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"bitMask", (funct_t) bitMask, (funct_t) test_bitMask, 2,
    "! ~ & ^ | + << >>", 16, 3,
  {{0, 63},{0,63},{TMin,TMax}}},
//4
{"isPalindrome", (funct_t) isPalindrome, (funct_t) test_isPalindrome, 1, "! ~ & ^ | + << >>", 70, 4,
        {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"trueFiveEighths", (funct_t) trueFiveEighths, (funct_t) test_trueFiveEighths, 1,
    "! ~ & ^ | + << >>", 20, 4,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
 {"logicalNeg", (funct_t) logicalNeg, (funct_t) test_logicalNeg, 1,
    "~ & ^ | + << >>", 12, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
{
    "", NULL, NULL, 0, "", 0, 0, {
        {0, 0}, {0, 0}, {
            0, 0
        }
    }
}
}
;
