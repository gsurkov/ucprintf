#include <float.h>
#include <stdarg.h>
#include <stdbool.h>

/* Character output function */
void _putchar(char c);

/* Optional function to trigger buffered transfers */
void __attribute__((weak)) _flush(void) {}

#define XTOA_BUF_SIZE 32U

#define FMT_FPREC_DEFAULT 6
#define FMT_IPREC_DEFAULT 0
#define FMT_WIDTH_DEFAULT 0
#define FMT_SIGN_DEFAULT false

#define _min(x, y) (x < y ? x : y)
#define _max(x, y) (x > y ? x : y)

#define _ctoi(c) (c - '0')

#define _isdigit(c) \
    ((c >= '0') && (c <= '9'))

union dword {
    float f;
    unsigned int i;
};

static char buf[XTOA_BUF_SIZE];

#define FLOAT_SIGN_MASK (1U << 31)
static inline  bool _isnegf(float v)
{
    union dword word = {v};
    return (word.i & FLOAT_SIGN_MASK);
}

static inline unsigned int _proundf(float x)
{
    return (unsigned int)x + (x - (unsigned int)x >= 0.5f ? 1U : 0U);
} 

static void _puts(const char *s)
{
    for(; *s; s++) {
        _putchar(*s);
    }
}

static char *_to_upper(char *s) {
    char *p = s;

    for(; *p; p++) {
        if((*p >= 'a') && (*p <= 'z')) {
            *p -= ('a' - 'A');
        }
    }

    return s;
}

static int _strlen(const char *s)
{
    const char *p = s;
    for(; *p; p++) {}
    return p - s;
}

static char *_strrev(char *s)
{
    for(char *f = s, *b = s + _strlen(s) - 1; b > f; f++, b--) {
        char tmp = *f;
        *f = *b;
        *b = tmp;
    }

    return s;
}

static char *_strjoin(char *s, char c)
{
    s += _strlen(s);
    *s++ = c;
    *s = '\0';

    return s;
}

static char *_strnfill(char *buf, char c, int n)
{
    for(int i = 0; i < n; ++i) {
        buf[i] = c;
    }
    
    buf[n] = '\0';

    return buf;
}

static char *_strcpy(char *dst, const char *src)
{
    char *ret = dst;
    for(; *src; dst++, src++) {
        *dst = *src;
    }

    *dst = '\0';

    return ret;
}

static char _itoc(int v)
{
    if(v < 10) {
        return v + 48;
    } else {
        return v + 87;
    }
}

static char *_xitoa(unsigned int v, char *buf, int maxlen, int base, int prec, int width, bool is_neg, bool is_sign)
{
    const int numlen = maxlen - (is_neg || is_sign ? 2U : 1U);

    int i = 0;

    do {
        buf[i++] = _itoc(v % base);
        v = v / base;

    } while((v > 0) && (i < numlen));

    if(prec > i) {
        prec = _min(prec - i, numlen - i);
        _strnfill(buf + i, '0', prec);
        i += prec;
    }
 
    if(is_neg) {
        buf[i++] = '-';
    } else if(is_sign) {
        buf[i++] = '+';
    }

    if(width > i) {
        width = _min(width - i, maxlen - i - 1);
        _strnfill(buf + i, ' ', width);
        i += width;
    }

    buf[i] = '\0';

    return _strrev(buf);
}

static char *_itoa(int v, char *buf, int maxlen, int base, int prec, int width, bool is_sign)
{
    const bool is_negative = v < 0;
    return _xitoa(is_negative ? -v : v, buf, maxlen, base, prec, width, is_negative, is_sign);
}

/* Internal to printf, handles only non-negative dec numbers */
static int _atoi_dp(const char *s, int *nchar)
{
    int i;
    int val = 0;
    for(i = 0; _isdigit(s[i]); ++i) {
        val = val * 10 + _ctoi(s[i]);
    }

    if(nchar) {
        *nchar = i;
    }

    return val;
}

static char *_uitoa(unsigned int v, char *buf, int maxlen, int base, int prec, int width, bool is_sign)
{
    return _xitoa(v, buf, maxlen, base, prec, width, false, is_sign);
}

static char *_ftoa(float v, char *buf, int maxlen, int prec, int width, bool is_sign)
{
    static const float pow[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    prec = _min(prec, (int)(sizeof(pow)/sizeof(float)));

    if(v != v) {
        _strcpy(buf, "nan");
    } else if(v < -FLT_MAX) {
        _strcpy(buf, "-inf");
    } else if(v > FLT_MAX) {
        static const char inf[] = "+inf";
        _strcpy(buf, is_sign ? inf : inf+1);

    } else {
        const bool is_neg = _isnegf(v);
        
        if(is_neg) {
            v = -v;
        }

        const unsigned int integer = (unsigned int)v;
        const unsigned int decimal = _proundf((v - integer) * pow[prec]);
        const int padding = width > prec ? width - prec - 1 : 0;
        
        if(decimal != 0) {
            _xitoa(decimal, _strjoin(_xitoa(integer, buf, maxlen, 10, 0, padding, is_neg, is_sign), '.'), maxlen, 10, 0, 0, false, false);
        } else {
            _strnfill(_strjoin(_xitoa(integer, buf, maxlen, 10, 0, padding, is_neg, is_sign), '.'), '0', FMT_FPREC_DEFAULT);
        }
    }

    return buf;
}

void ucprintf(const char *fmt, ...)
{
    enum parser_state {
        NORMAL,
        FORMAT
    };

    va_list args;
    va_start(args, fmt);

    enum parser_state state = NORMAL;
    
    int prec = FMT_IPREC_DEFAULT;
    int fprec = FMT_FPREC_DEFAULT;
    int width = FMT_WIDTH_DEFAULT;
    bool is_sign = FMT_SIGN_DEFAULT;

    for(; *fmt; fmt++) {
        if(state == NORMAL) {
            if(*fmt == '%') {
                state = FORMAT;
            } else {
                _putchar(*fmt);
            }

            continue;

        } else if(state == FORMAT) {
            if(*fmt == '+') {
                is_sign = true;
                continue;

            } else if(*fmt == '.') {
                int len;
                prec = _atoi_dp(fmt + 1, &len);
                fprec = len ? prec : FMT_FPREC_DEFAULT;
                /* Changing loop counter here! */
                fmt += len;
                continue;

            } else if(_isdigit(*fmt)) {
                int len;
                width = _atoi_dp(fmt, &len);
                /* Changing loop counter here! */
                fmt += len - 1;
                continue;

            } else if(*fmt == '%') {
                _putchar('%');
            } else if(*fmt == 's') {
                _puts(va_arg(args, char*));
            } else if(*fmt == 'd') {
                _puts(_itoa(va_arg(args, int), buf, XTOA_BUF_SIZE, 10, prec, width, is_sign));
            } else if(*fmt == 'u') {
                _puts(_uitoa(va_arg(args, unsigned int), buf, XTOA_BUF_SIZE, 10, prec, width, is_sign));
            } else if(*fmt == 'x') {
                _puts(_uitoa(va_arg(args, unsigned int), buf, XTOA_BUF_SIZE, 16, prec, width, is_sign));
            } else if(*fmt == 'X') {
                _puts(_to_upper(_uitoa(va_arg(args, unsigned int), buf, XTOA_BUF_SIZE, 16, prec, width, is_sign)));
            } else if(*fmt == 'f') {
                _puts(_ftoa(va_arg(args, double), buf, XTOA_BUF_SIZE, fprec, width, is_sign));
            } else if(*fmt == 'e') {
                _puts("<exp>");
            } else if(*fmt == 'E') {
                _puts("<EXP>");
            } else if(*fmt == 'o') {
                _puts(_uitoa(va_arg(args, unsigned int), buf, XTOA_BUF_SIZE, 8, prec, width, is_sign));
            }  else {
                /* No match - going to reset */
            }
        }

        /* Reset and wait for next identifier */
        state = NORMAL;
        prec = FMT_IPREC_DEFAULT;
        fprec = FMT_FPREC_DEFAULT;
        width = FMT_WIDTH_DEFAULT;
        is_sign = FMT_SIGN_DEFAULT;
    }

    va_end(args);
    
    _flush();
}