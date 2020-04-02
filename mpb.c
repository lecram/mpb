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
print_bar(unsigned long percent)
{
    unsigned i, fill;
    fill = percent * width / 100;
    if (percent % 100 == 0)
        spinindex = 0;
    printf(" (%c) [", spinchars[spinindex++]);
    spinindex %= sizeof(spinchars) - 1;
    if (fill != lastfill) {
        for (i = 0; i < fill; i++)
            putchar('#');
        for (; i < width; i++)
            putchar('-');
        lastfill = fill;
    } else {
        printf("\x1B[%uC", width);
    }
    if (percent != lastpercent) {
        printf("] %3lu%%", percent);
        lastpercent = percent;
    }
    printf("\r");
    fflush(stdout);
}

int
main(int argc, char *argv[])
{
    unsigned long count, total, percent;
    if (argc != 2) {
        fprintf(stderr, "usage:\n  %s total\n", argv[0]);
        return 1;
    }
    total = (unsigned) atol(argv[1]);
    if (total == 0) {
        fprintf(stderr, "total must be nonzero\n");
        return 1;
    }
    width = 32;
    print_bar(0);
    count = 0;
    while (scanf("%s", line) != EOF) {
        if (count < total)
            count++;
        percent = count * 100 / total;
        print_bar(percent);
    }
    print_bar(100);
    puts("");
    return 0;
}
