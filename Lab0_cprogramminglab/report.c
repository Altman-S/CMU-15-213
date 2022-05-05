#include "report.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX(a, b) ((a) < (b) ? (b) : (a))

FILE *errfile = NULL;
FILE *verbfile = NULL;
FILE *logfile = NULL;

int verblevel = 0;
void init_files(FILE *efile, FILE *vfile) {
    errfile = efile;
    verbfile = vfile;
}

static char fail_buf[1024] = "FATAL Error.  Exiting\n";

volatile ssize_t rval = 0;

/* Default fatal function */
void default_fatal_fun(void) {
    rval = write(STDOUT_FILENO, fail_buf, strlen(fail_buf) + 1);
    if (logfile)
        fputs(fail_buf, logfile);
}

/* Optional function to call when fatal error encountered */
void (*fatal_fun)(void) = default_fatal_fun;

void set_verblevel(int level) {
    verblevel = level;
}

bool set_logfile(char *file_name) {
    logfile = fopen(file_name, "w");
    return logfile != NULL;
}

void report_event(message_t msg, const char *fmt, ...) {
    va_list ap;
    bool fatal = msg == MSG_FATAL;
    const char *msg_name = msg == MSG_WARN    ? "WARNING"
                           : msg == MSG_ERROR ? "ERROR"
                                              : "FATAL ERROR";
    int level = msg == MSG_WARN ? 2 : msg == MSG_ERROR ? 1 : 0;
    if (verblevel < level)
        return;
    if (!errfile)
        init_files(stdout, stdout);
    va_start(ap, fmt);
    fprintf(errfile, "%s: ", msg_name);
    vfprintf(errfile, fmt, ap);
    fprintf(errfile, "\n");
    fflush(errfile);
    va_end(ap);
    if (logfile) {
        va_start(ap, fmt);
        fprintf(logfile, "Error: ");
        vfprintf(logfile, fmt, ap);
        fprintf(logfile, "\n");
        fflush(logfile);
        va_end(ap);
        fclose(logfile);
    }
    if (fatal) {
        if (fatal_fun)
            fatal_fun();
        exit(1);
    }
}

void report(int level, const char *fmt, ...) {
    va_list ap;
    if (!verbfile)
        init_files(stdout, stdout);
    if (level <= verblevel) {
        va_start(ap, fmt);
        vfprintf(verbfile, fmt, ap);
        fprintf(verbfile, "\n");
        fflush(verbfile);
        va_end(ap);
        if (logfile) {
            va_start(ap, fmt);
            vfprintf(logfile, fmt, ap);
            fprintf(logfile, "\n");
            fflush(logfile);
            va_end(ap);
        }
    }
}

void report_noreturn(int level, const char *fmt, ...) {
    va_list ap;
    if (!verbfile)
        init_files(stdout, stdout);
    if (level <= verblevel) {
        va_start(ap, fmt);
        vfprintf(verbfile, fmt, ap);
        fflush(verbfile);
        va_end(ap);
        if (logfile) {
            va_start(ap, fmt);
            vfprintf(logfile, fmt, ap);
            fflush(logfile);
            va_end(ap);
        }
    }
}

void safe_report(int level, const char *msg) {
    if (level > verblevel)
        return;
    if (!errfile)
        init_files(stdout, stdout);
    fputs(msg, errfile);
    if (logfile) {
        fputs(msg, logfile);
    }
}

/* Functions denoting failures */

/* General failure */

/* Need to be able to print without using malloc */
NORETURN fail_fun(const char *format, const char *msg) {
    sprintf(fail_buf, format, msg);
    /* Tack on return */
    fail_buf[strlen(fail_buf)] = '\n';
    /* Use write to avoid any buffering issues */
    rval = write(STDOUT_FILENO, fail_buf, strlen(fail_buf) + 1);
    if (logfile) {
        /* Don't know file descriptor for logfile */
        fputs(fail_buf, logfile);
    }
    if (fatal_fun)
        fatal_fun();
    if (logfile)
        fclose(logfile);
    exit(1);
}

/* Call malloc & exit if fails */
void *malloc_or_fail(size_t bytes, const char *fun_name) {
    void *p = malloc(bytes);
    if (!p) {
        fail_fun("Malloc returned NULL in %s", fun_name);
    }
    return p;
}

/* Call calloc & exit if fails */
void *calloc_or_fail(size_t cnt, size_t bytes, const char *fun_name) {
    void *p = calloc(cnt, bytes);
    if (!p) {
        fail_fun("Calloc returned NULL in %s", fun_name);
    }
    return p;
}

/* Call realloc & exit if fails. */
void *realloc_or_fail(void *old, size_t new_bytes, const char *fun_name) {
    void *p = realloc(old, new_bytes);
    if (!p) {
        fail_fun("Realloc returned NULL in %s", fun_name);
    }
    return p;
}

char *strsave_or_fail(const char *s, const char *fun_name) {
    if (!s) {
        fail_fun("strsave(NULL) in %s", fun_name);
    }
    size_t len = strlen(s);
    char *ss = malloc(len + 1);
    if (!ss) {
        fail_fun("strsave failed in %s", fun_name);
    }
    return memcpy(ss, s, len + 1);
}

/* Initialization of timers */
void init_time(double *timep) {
    (void)delta_time(timep);
}

double delta_time(double *timep) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double current_time = (double)tv.tv_sec + 1.0E-6 * (double)tv.tv_usec;
    double delta = current_time - *timep;
    *timep = current_time;
    return delta;
}

double gigabytes(size_t n) {
    return (double)n / (1UL << 30);
}
