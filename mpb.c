#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static unsigned width;
static char spinchars[] = "|/-\\";
static unsigned spinindex = 0;
static char line[(1 << 12) + 1];
static unsigned lastfill = UINT_MAX;
static unsigned lastpercent = 101;

void
print_progress(unsigned percent, int showbar, int showspinner, int showline)
{
    unsigned i, fill;
    fill = percent * width / 100;
    if (showspinner) {
        if (percent == 100)
            spinindex = 0;
        printf(" (%c)", spinchars[spinindex++]);
        spinindex %= sizeof(spinchars) - 1;
    }
    if (showbar) {
        if (fill != lastfill) {
            printf(" [");
            for (i = 0; i < fill; i++)
                putchar('#');
            for (; i < width; i++)
                putchar('-');
            lastfill = fill;
        } else {
            printf("\x1B[%uC", width + 2);
        }
        if (percent != lastpercent) {
            printf("] %3u%%", percent);
            lastpercent = percent;
        }
        printf("\r");
        if (showline)
            printf("\x1B[B%s\r\x1B[A", line);
    } else if (showline) {
        printf(" %s", line);
    }
    printf("\r");
    fflush(stdout);
}

int
main(int argc, char *argv[])
{
    unsigned long count;
    unsigned percent;
    int i;
    char *arg;
    char *argtotal = NULL;
    int opt_showbar = 0;
    int opt_showline = 0;
    int opt_showspinner = 0;
    unsigned long total = 0;
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        if (*arg == '-') {
            while (*(++arg)) {
                switch (*arg) {
                case 'v':
                    opt_showline = 1;
                    break;
                case 's':
                    opt_showspinner = 1;
                    break;
                default:
                    fprintf(stderr, "%s: error: unknown option '-%c'\n", argv[0], *arg);
                    return 1;
                }
            }
        } else {
            argtotal = arg;
        }
    }
    if (argtotal != NULL) {
        total = (unsigned) atol(argtotal);
        if (total)
            opt_showbar = 1;
    }
    width = 32;
    print_progress(0, opt_showbar, opt_showspinner, 0);
    count = 0;
    while (scanf("%s", line) != EOF) {
        if (count < total)
            count++;
        if (total)
            percent = count * 100 / total;
        print_progress(percent, opt_showbar, opt_showspinner, opt_showline);
    }
    print_progress(100, opt_showbar, opt_showspinner, 0);
    puts("\x1B[B\x1B[2K\x1B[A");
    return 0;
}
