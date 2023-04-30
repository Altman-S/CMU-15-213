# Lab2 BombLab

In this lab, we need to understand how the assembly code works.

Somtimes, we need to have a overview of all the assembly codes without paying more attention to details. We should try to understand the logic of each command in detail, not command details.

Since there are so many registers involved and as they are not given meaningful name, you will quickly get overwhelmed.


## Phase 1

Input a string, and compare it with the string at 0x402400. Bomb defused if two strings are equal.

```
Border relations with Canada have never been better.
```

## Phase 2

Input 6 numbers from stdin and compares with 1, 2, 4, 8, 16, 32.

```
1 2 4 8 16 32
```

## Phase 3

Input 2 integers. Understand the jump instruction to find the result

```
6 682
```

## Phase 4

Input 2 integers. The first integer has to be below or equal 0xe, and the second integer should be 0. Phase 4 calls subroutine func4, whose return value should be 0, requiring first integer to be 7.

```
7 0
```

## Phase 5

Try many times to find a character map

"abcdef"->"aduier", "ghijkl"->"snfotv", "mnopqr"->"bylmad", "stuvwx"->"uiersn", "yzabcd"->"foadui"

Input 6 characters, and the output should be "flyers" (0x6038c0)

```
ionefg
```

## Phase 6

Input 6 distinct numbers between 1-6. Form a singly-linked list that is sorted, based on the order of input.

```
6 5 4 3 2 1

(gdb) print *(int *) 0x6032d0
$34 = 332
(gdb) print *(int *) *(0x6032d0+0x8)
$35 = 168
(gdb) print *(int *) *(*(0x6032d0+0x8)+0x8)
$36 = 924
(gdb) print *(int *) *(*(*(0x6032d0+0x8)+0x8)+0x8)
$37 = 691
(gdb) print *(int *) *(*(*(*(0x6032d0+0x8)+0x8)+0x8)+0x8)
$38 = 477
(gdb) print *(int *) *(*(*(*(*(0x6032d0+0x8)+0x8)+0x8)+0x8)+0x8)
$39 = 443
```

3 4 5 6 1 2 -> 4 3 2 1 6 5

```
4 3 2 1 6 5
```