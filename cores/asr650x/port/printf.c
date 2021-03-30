

#include <stdarg.h>
#include <project.h>
#include <uart_port.h>
#include <stdio.h>

static int tiny_print( char **out, const char *format, va_list args, unsigned int buflen );

static void printchar(char **str, int c, char *buflimit)
{

	if (str) {
		if( buflimit == ( char * ) 0 ) {
			/* Limit of buffer not known, write charater to buffer. */
			**str = (char)c;
			++(*str);
		}
		else if( ( ( unsigned long ) *str ) < ( ( unsigned long ) buflimit ) ) {
			/* Withing known limit of buffer, write character. */
			**str = (char)c;
			++(*str);
		}
	}
	else
	{
		//(void)uart_put_char(c);
        UART_1_UartPutChar(c);
	}
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad, char *buflimit)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar, buflimit);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printchar (out, *string, buflimit);
		++pc;
	}
	for ( ; width > 0; --width) {
		printchar (out, padchar, buflimit);
		++pc;
	}

	return pc;
}

/* the following should be enough for 64 bit int */
#define PRINT_BUF_LEN_ll 20

static int printi(char **out, long long i, int b, int sg, int width, int pad, int letbase,int isll, char *buflimit)
{
	char print_buf[PRINT_BUF_LEN_ll];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned long long u = (unsigned int)i;
	if(isll)
		u = (unsigned long long)i;
	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad, buflimit);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		if(isll)
			u = (unsigned long long)-i;
		else
			u = (unsigned int)-i;
	}

	s = print_buf + PRINT_BUF_LEN_ll-1;
	*s = '\0';

	while (u) {
		if(isll)
			t = (unsigned long long)u % b;
		else
			t = (unsigned int)u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = (char)(t + '0');
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printchar (out, '-', buflimit);
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad, buflimit);
}

static int tiny_print( char **out, const char *format, va_list args, unsigned int buflen )
{
	register int width, pad;
	register int pc = 0;
	char scr[2], *buflimit;

	if( buflen == 0 ){
		buflimit = ( char * ) 0;
	}
	else {
		/* Calculate the last valid buffer space, leaving space for the NULL
		terminator. */
		buflimit = ( *out ) + ( buflen - 1 );
	}

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = (char *)va_arg( args, int );
				pc += prints (out, s?s:"(null)", width, pad, buflimit);
				continue;
			}
			if( *format == 'l' && *(format+1) == 'l'){
				if(*(format+2) == 'd'){
					format +=2;
					pc += printi (out, va_arg( args, long long ), 10, 1, width, pad, 'a',1,buflimit);
					continue;
				}
				if(*(format+2) == 'u'){
					format +=2;
					pc += printi (out, va_arg( args, unsigned long long ), 10, 0, width, pad, 'a',1,buflimit);
					continue;
				}
			}
			if( *format == 'd' || *format == 'i' ) {
				pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a',0, buflimit);
				continue;
			}
			if( *format == 'x' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a',0, buflimit);
				continue;
			}
			if( *format == 'X' ) {
				pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A',0, buflimit);
				continue;
			}
			if( *format == 'u' ) {
				pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a',0, buflimit);
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char)va_arg( args, int );
				scr[1] = '\0';
				pc += prints (out, scr, width, pad, buflimit);
				continue;
			}
		}
		else {
		out:
			printchar (out, *format, buflimit);
			++pc;
		}
	}
	if (out) **out = '\0';
	va_end( args );
	return pc;
}
#ifdef CONSOLE_LOG_BUFFER
char log_buf[UART_CONSOLE_SIZE];
struct circ_buf log_cb = {
    .buf = log_buf,
    .head = 0,
    .tail = 0,
};
char logs[256];
#endif
int __wrap_printf(const char *format, ...)
{
        va_list args;

        va_start( args, format );
        #ifndef CONSOLE_LOG_BUFFER
        return tiny_print( 0, format, args, 0 );
        #else
        uint16_t log_len, circ_space_len;
        char *p = logs;
        log_len = tiny_print(&p, format, args, 0);
        circ_space_len = CIRC_SPACE_TO_END(log_cb.head, log_cb.tail, UART_CONSOLE_SIZE);
        if (log_len < circ_space_len) {
            circ_space_len = log_len;
        }
        memcpy(log_cb.buf + log_cb.head, logs, circ_space_len);
        log_cb.head = (log_cb.head + circ_space_len) & (UART_CONSOLE_SIZE - 1);
        return log_len;
        #endif
}

int __wrap_sprintf(char *out, const char *format, ...)
{
        va_list args;

        va_start( args, format );
        return tiny_print( &out, format, args, 0 );
}


int __wrap_snprintf( char *buf, unsigned int count, const char *format, ... )
{
        va_list args;

        ( void ) count;

        va_start( args, format );
        return tiny_print( &buf, format, args, count );
}
int csp_printf(const char *format, ...)
{
    va_list args;

    va_start( args, format );
    return tiny_print( 0, format, args, 0 );
}
int __wrap_fflush(FILE* fp)
{
    return 0;
}
#ifdef TEST_PRINTF
int main(void)
{
	char *ptr = "Hello world!";
	char *np = 0;
	int i = 5;
	unsigned int bs = sizeof(int)*8;
	int mi;
	char buf[80]={0};

	mi = (1 << (bs-1)) + 1;
	printf_std("%s\n", ptr);
	printf_std("printf_std test\n");
	printf_std("%s is null pointer\n", np);
	printf_std("%d = 5\n", i);
	printf_std("%d = - max int\n", mi);
	printf_std("char %c = 'a'\n", 'a');
	printf_std("hex %x = ff\n", 0xff);
	printf_std("hex %02x = 00\n", 0);
	printf_std("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	printf_std("%d %s(s)%", 0, "message");
	printf_std("\n");
	printf_std("%d %s(s) with %%\n", 0, "message");
	sprintf_std(buf, "justif: \"%-10s\"\n", "left"); 
	printf_std("%s", buf);
	sprintf_std(buf, "justif: \"%10s\"\n", "right"); 
	printf_std("%s", buf);
	sprintf_std(buf, " 3: %04d zero padded\n", 3); 
	printf_std("%s", buf);
	sprintf_std(buf, " 3: %-4d left justif.\n", 3); 
	printf_std("%s", buf);
	sprintf_std(buf, " 3: %4d right justif.\n", 3); 
	printf_std("%s", buf);
	sprintf_std(buf, "-3: %04d zero padded\n", -3); 
	printf_std("%s", buf);
	sprintf_std(buf, "-3: %-4d left justif.\n", -3); 
	printf_std("%s", buf);
	sprintf_std(buf, "-3: %4d right justif.\n", -3); 
	printf_std("%s", buf);

	return 0;
}

/*
 * if you compile this file with
 *   gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 32bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -2147483647 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
 */

#endif

#if 0
/* To keep linker happy. */
int	write( int i, char* c, int n)
{
	(void)i;
	(void)n;
	(void)c;
	return 0;
}
#endif
