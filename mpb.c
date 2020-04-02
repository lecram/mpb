#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static unsigned width;
static char spinchars[] = "|/-\\";
static unsigned spinindex;
static char line[(1 << 12) + 1];
static unsigned lastfill = UINT_MAX;
static unsigned lastpercent = 101;

void
print_bar(unsigned percent, int showspinner, int showline)
{
    unsigned i, fill;
    fill = percent * width / 100;
    if (showspinner) {
        if (percent % 100 == 0)
            spinindex = 0;
        printf(" (%c)", spinchars[spinindex++]);
        spinindex %= sizeof(spinchars) - 1;
    }
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
    fflush(stdout);
}

int
main(int argc, char *argv[])
{
    unsigned long count, total;
    unsigned percent;
    int i;
    char *arg;
    char *argtotal = NULL;
    int opt_showline = 0;
    int opt_showspinner = 0;
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
    if (argtotal == NULL) {
        fprintf(stderr, "%s: error: no total given\n", argv[0]);
        return 1;
    }
    total = (unsigned) atol(argtotal);
    if (total == 0) {
        fprintf(stderr, "%s: error: total must be nonzero\n", argv[0]);
        return 1;
    }
    width = 32;
    print_bar(0, opt_showspinner, 0);
    count = 0;
    while (scanf("%s", line) != EOF) {
        if (count < total)
            count++;
        percent = count * 100 / total;
        print_bar(percent, opt_showspinner, opt_showline);
    }
    print_bar(100, opt_showspinner, 0);
    puts("\x1B[B\x1B[2K\x1B[A");
    return 0;
}
