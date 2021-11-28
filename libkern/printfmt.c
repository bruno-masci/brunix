// Based on https://github.com/guilleiguaran/xv6

// Stripped-down primitive printf-style formatting routines,
// used in common by printf, sprintf, fprintf, etc.


#include <stdint.h> // for uintptr_t
#include <stdarg.h>
#include <stddef.h> // for NULL

#include <string.h>
#include <brunix/defs.h>            // for PRIVATE, EXPORT


EXPORT void vprintfmt(void (*putch)(char, void *), void *putdat, const char *fmt, va_list ap);


//=============================================================================
//=============================================================================


/*
 * Print a number (base <= 16) in reverse order, using specified putch function
 * and associated pointer putdat.
 */
PRIVATE void printnum(void (*putch)(char, void *), void *putdat,
                     unsigned long long num, unsigned base,
                     int width, int padc) {
    // first recursively print all preceding (more significant) digits
    if (num >= base) {
        printnum(putch, putdat, num / base, base, width - 1, padc);
    } else {
        // print any needed pad characters before first digit
        while (--width > 0)
            putch((char) padc, putdat);
    }

    // then print this (the least significant) digit
    putch("0123456789abcdef"[num % base], putdat);
}

// Get an unsigned int of various possible sizes from a varargs list,
// depending on the lflag parameter.
PRIVATE unsigned long long getuint(va_list *ap, int lflag) {
    if (lflag >= 2)
        return va_arg(*ap, unsigned long long);
    else if (lflag)
        return va_arg(*ap, unsigned long);
    else
        return va_arg(*ap, unsigned int);
}

// Same as getuint but signed - can't use getuint
// because of sign extension
PRIVATE long long getint(va_list *ap, int lflag) {
    if (lflag >= 2)
        return va_arg(*ap, long long);
    else if (lflag)
        return va_arg(*ap, long);
    else
        return va_arg(*ap, int);
}

void vprintfmt(void (*putch)(char, void *), void *putdat, const char *fmt, va_list ap) {
    const char *p;
    char ch;
    unsigned long long num;
    int lflag, width, precision, altflag;
    unsigned int base;
    char padc;

    while (1) {
        while ((ch = *( char *) fmt++) != '%') {
            if (ch == '\0')
                return;
            putch(ch, putdat);
        }

        // Process a %-escape sequence
        padc = ' ';
        width = -1;
        precision = -1;
        lflag = 0;
        altflag = 0;
        reswitch:
        switch (ch = *(char *) fmt++) {

            // flag to pad on the right
            case '-':
                padc = '-';
                goto reswitch;

                // flag to pad with 0's instead of spaces
            case '0':
                padc = '0';
                goto reswitch;

                // width field
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                for (precision = 0;; ++fmt) {
                    precision = precision * 10 + ch - '0';
                    ch = *fmt;
                    if (ch < '0' || ch > '9')
                        break;
                }
                goto process_precision;

            case '*':
                precision = va_arg(ap, int);
                goto process_precision;

            case '.':
                if (width < 0)
                    width = 0;
                goto reswitch;

            case '#':
                altflag = 1;
                goto reswitch;

            process_precision:
                if (width < 0)
                    width = precision, precision = -1;
                goto reswitch;

                // long flag (doubled for long long)
            case 'l':
                lflag++;
                goto reswitch;

                // character
            case 'c':
                putch((char) (va_arg(ap, int)), putdat);
                break;

                // string
            case 's':
                if ((p = va_arg(ap, char *)) == NULL)
                p = "(null)";
                if (width > 0 && padc != '-')
                    for (width -= strnlen(p, (size_t) precision); width > 0; width--)
                        putch(padc, putdat);
                for (; (ch = *p++) != '\0' && (precision < 0 || --precision >= 0); width--)
                    if (altflag && (ch < ' ' || ch > '~'))
                        putch('?', putdat);
                    else
                        putch(ch, putdat);
                for (; width > 0; width--)
                    putch(' ', putdat);
                break;

                // (signed) decimal
            case 'd':
                num = (long long unsigned int) getint(&ap, lflag);
                if ((long long) num < 0) {
                    putch('-', putdat);
                    num = -num;
                }
                base = 10;
                goto number;

                // unsigned decimal
            case 'u':
                num = getuint(&ap, lflag);
                base = 10;
                goto number;

                // (unsigned) binary
            case 'b':
                num = getuint(&ap, lflag);
                base = 2;
                goto number;

                // (unsigned) octal
            case 'o':
                num = getuint(&ap, lflag);
                base = 8;
                goto number;

                // pointer
            case 'p':
                putch('0', putdat);
                putch('x', putdat);
                num = (unsigned long long)
                        (uintptr_t) va_arg(ap, void *);
                base = 16;
                goto number;

                // (unsigned) hexadecimal
            case 'x':
                num = getuint(&ap, lflag);
                base = 16;
            number:
                printnum(putch, putdat, num, base, width, padc);
                break;

            case 'k':
                num = getuint(&ap, lflag);
                base = 16;
                printnum(putch, putdat, num, base, width, padc);
                putch('<', putdat);
                printnum(putch, putdat, num / 1024, 10, -1, ' ');
                putch(' ', putdat);
                putch('K', putdat);
                putch('B', putdat);
                putch('>', putdat);
                break;

                // escaped '%' character
            case '%':
                putch(ch, putdat);
                break;

                // unrecognized escape sequence - just print it literally
            default:
                putch('%', putdat);
                for (fmt--; fmt[-1] != '%'; fmt--)
                    /* do nothing */;
                break;
        }
    }
}
