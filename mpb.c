#include <stdio.h>
#include <stdlib.h>
#include <poll.h>

static unsigned width;

void
hide_cursor()
{
    printf("\x1B[?25l");
    fflush(stdout);
}

void
show_cursor()
{
    printf("\x1B[?25h");
    fflush(stdout);
}

void
print_bar(unsigned long percent)
{
    unsigned i, n;
    n = percent * width / 100;
    printf("\r[");
    for (i = 0; i < n; i++)
        putchar('#');
    for (; i < width; i++)
        putchar('-');
    printf("] %3lu%%", percent);
    fflush(stdout);
}

int
main(int argc, char *argv[])
{
    int ret = 0;
    unsigned long count, total, percent;
    if (argc != 2) {
        fprintf(stderr, "usage:\n  %s total\n", argv[0]);
        ret = 1;
        goto quit;
    }
    total = (unsigned) atol(argv[1]);
    if (total == 0) {
        fprintf(stderr, "total must be nonzero\n");
        ret = 1;
        goto quit;
    }
    width = 32;
    hide_cursor();
    print_bar(0);
    for (count = 0; count < total; count++) {
        poll(NULL, 0, 25);
        percent = count * 100 / total;
        print_bar(percent);
    }
    print_bar(100);
quit:
    show_cursor();
    puts("");
    return ret;
}
