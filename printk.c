//#include <asm/asm.h>
//#include <screen.h>
//#include <util.h>
#include <asm/asm.h>
#include <stdarg.h>
#include <monitor.h>

void printk (char *format, ...)
{
    int i, j, size, buflen, neg;
    va_list ap;
    char buf[16];

    unsigned char c;
    int ival;
    unsigned int uival;

    va_start (ap, format);

    while ((c = *format++))
    {
        size = neg = 0;

        if (c == 0)
            break;
        else if (c == '%')
        {
            c = *format++;
            if (c >= '0' && c <= '9')
            {
                size = c - '0';
                c = *format++;
            }

            if (c == 'd')
            {
                ival = va_arg (ap, int);
                if (ival < 0)
                {
                    uival = 0 - ival;
                    neg++;
                }
                else uival = ival;

                itoa (buf, uival, 10);
                monitor_write(buf);
                buflen = strlen (buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0; --i, --j)
                        buf[i] = (j >= 0) ? buf[j] : '0';

                if (neg)
                    printk ("-%s", buf);
                else
                    printk (buf);
            }
            else if (c == 'u')
            {
                uival = va_arg (ap, int);
                itoa (buf, uival, 10);

                buflen = strlen (buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0; --i, --j)
                        buf[i] = (j >= 0) ? buf[j] : '0';

                printk (buf);
            }
            else if (c == 'x' || c == 'X')
            {
                uival = va_arg (ap, int);
                itoa (buf, uival, 16);

                buflen = strlen (buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0; --i, --j)
                        buf[i] = (j >= 0) ? buf[j] : '0';

                printk ("0x%s", buf);
            }
            else if (c == 'p')
            {
                uival = va_arg (ap, int);
                itoa (buf, uival, 16);
                size = 8;

                buflen = strlen (buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0; --i, --j)
                        buf[i] = (j >= 0) ? buf[j] : '0';

                printk ("0x%s", buf);
            }
            else if (c == 's')
            {
                printk ((char *) va_arg (ap, int));
            }
            else if (c == 'c')
            {
                monitor_put (va_arg (ap, int));
            }
        }
        else
            monitor_put (c);
    }
    return;
}

void panic (const char *file, int line, char *msg)
{
    //extern char kattr;
    //kattr = 0x0C;
    printk ("PANIC: %s:%d : %s\n", file, line, msg);
    halt ();
}

void panic_assert (const char *file, int line, char *msg)
{
    //extern char kattr;
    //kattr = 0x0C;
    printk ("ASSERTION-FAILED: %s:%d : %s\n", file, line, msg);
    halt ();
}