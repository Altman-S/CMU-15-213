# Lab6 ShellLab

## Before starting

1. Reading `8.1 - 8.8` of `CSAPP: 3e` and the writeup can be very helpful;
2. Make sure you understand the content in the slides very carefully, especially 15-ecf-signals.pdf;
3. Read the writeup very carefully.

## Working on the lab

1. Some snippets in lecture slides and book are very similar to the code for the lab, you can just copy it and change some details;
2. The writeup provides strategies for most of the tricky parts in the lab, it can guide you through the lab, so make sure you read the writeup very carefully;
3. Make good use of man page.



## Some questions

1. Why the child process cannot use printf()?

I tried to use `printf()` to track the child process and find out what happened inside, but there was no output for the child process on the console.

The reason is here:

When a child process is created, it inherits a copy of the parent process's file descriptors, including the standard output file descriptor (`stdout`). However, the output buffering state is also inherited. By default, `stdout` is line-buffered, meaning the output is stored in a buffer until a newline character (`'\n'`) is encountered or the buffer is full.

In the case of a child process, if the output buffer is not flushed before the `fork()` call or if the child process doesn't explicitly flush the buffer, the buffered output will also be duplicated in the child process. This means that the child process will have its own copy of the buffered output, separate from the parent process.

If the child process attempts to print using `printf()`, the output will go to its own copy of the output buffer. However, because the buffer is not automatically flushed, the output may not immediately appear on the console or in the output stream.

To address this, you can explicitly flush the output buffer before the `fork()` call or use functions like `fflush(stdout)` to manually flush the buffer in both the parent and child processes. This ensures that the buffered output is immediately visible.

Additionally, you can use functions like `write()` instead of `printf()` for immediate output, as `write()` writes directly to the file descriptor without buffering.


2. How to check the process is no longer the foreground process in this lab?

In `waitfg()`, we use a busy loop around the sleep function (0.1s) to check the existence of the current foreground process (`check whether the current job id is in the job list`). If the fg process is terminated, the related job will be deleted from the job list and the shell program will end the busy sleep loop and immediately return.

To be more specific, when the child process is terminated, `waitpid()` will be automatically called and the parent process will reap the child process and delete its job id in the job list, as `deletejob()` in `sigchld_hanlder()`.


3. For trace05, in my implementation, other processes will wait for my running background process, but it's not for the `tshref` (run other jobs immediately without waiting). Why is there a difference? 

By default (when options = 0), `waitpid()` suspends execution of the calling process until a child process in its wait set terminates.

If we want to return it immediately, we need to set options as `WNOHANG | WUNTRACED` (you can check these options meaning in the book).