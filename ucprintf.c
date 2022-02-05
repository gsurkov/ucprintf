#include <float.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/** Character output function */
void _putchar(char c);

/** Optional function to trigger buffered transfers */
void __attribute__((weak)) _flush(void) {}

#define TRUE 1
#define FALSE 0

#define XTOA_BUF_SIZE 32U

#define FMT_FPREC_DEFAULT 6
#define FMT_IPREC_DEFAULT 0
#define FMT_WIDTH_DEFAULT 0

#define _min(x, y) (x < y ? x : y)
#define _max(x, y) (x > y ? x : y)

#define _ctoi(c) (c - '0')

#define _isdigit(c) \
    ((c >= '0') && (c <= '9'))

#define _isalphalc(c) \
    ((c >= 'a') && (c <= 'z'))

#define _tolowerc(c) \
    (c - ('a' - 'A'))

#define FLOAT_SIGN_MASK (1UL << 31)
static inline  bool _isnegf(float v)
{
    union {
        float f;
        uint32_t i; // Has to be exactly 32 bits wide
    } word = {v};

    return (word.i & FLOAT_SIGN_MASK);
}

/** Rounds positive floating point numbers */
static inline uintmax_t _proundf(float x)
{
    return (uintmax_t)x + (x - (uintmax_t)x >= 0.5f ? 1U : 0U);
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
        if(_isalphalc(*p)) {
            *p = _tolowerc(*p);
        }
    }

    return s;
}

static size_t _strlen(const char *s)
{
    size_t i;
    for(i = 0; s[i]; ++i) {}
    return i;
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

static char *_strnfill(char *buf, char c, size_t n)
{
    for(size_t i = 0; i < n; ++i) {
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

static char _itoc(unsigned char v)
{
    if(v < 10) {
        return v + 48;
    } else if(v < 16) {
        return v + 87;
    } else {
        return 0;
    }
}

/** Converts a string to a non-negative decimal number */
static uintmax_t _atoi_dp(const char *s, size_t *nchar)
{
    size_t i;
    uintmax_t val = 0;
    for(i = 0; _isdigit(s[i]); ++i) {
        val = val * 10 + _ctoi(s[i]);
    }

    if(nchar) {
        *nchar = i;
    }

    return val;
}

/**
 * @brief Converts a non-negative integer number to a string
 * @param[in] v Input nuber
 * @param[in] buf Pointer to output buffer
 * @param[in] maxlen Output buffer length
 * @param[in] base Base for the conversion
 * @param[in] prec If the resulting string is shorter, fill the rest with zeros
 * @param[in] width If the resulting string is shorter, fill the rest with spaces
 * @param[in] is_neg Whether the number should be negative
 * @param[in] is_sign Force the sign output
 * @return Pointer to output buffer
 */
static char *_xitoa(uintmax_t v, char *buf, size_t maxlen, unsigned char base,
                    unsigned char prec, unsigned char width, bool is_neg, bool is_sign)
{
    const size_t numlen = maxlen - (is_neg || is_sign ? 2U : 1U);

    size_t i = 0;

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

/** Wrapper for _xitoa() for handling signed integers */
static char *_itoa(intmax_t v, char *buf, size_t maxlen, unsigned char base, unsigned char prec, unsigned char width, bool is_sign)
{
    const bool is_negative = v < 0;
    return _xitoa(is_negative ? -v : v, buf, maxlen, base, prec, width, is_negative, is_sign);
}

/** Wrapper for _xitoa() for handling unsigned integers */
static char *_uitoa(uintmax_t v, char *buf, size_t maxlen, unsigned char base, unsigned char prec, unsigned char width, bool is_sign)
{
    return _xitoa(v, buf, maxlen, base, prec, width, false, is_sign);
}

/**
 * @brief Converts a floating point number to a string
 * @param[in] v Input nuber
 * @param[in] buf Pointer to output buffer
 * @param[in] maxlen Output buffer length
 * @param[in] prec The number of places after the decimal point
 * @param[in] width If the resulting string is shorter, fill the rest with spaces
 * @param[in] is_sign Force the sign output
 * @return Pointer to output buffer
 */
static char *_ftoa(float v, char *buf, size_t maxlen, unsigned char prec, unsigned char width, bool is_sign)
{
    static const float pow[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    prec = _min(prec, sizeof(pow) / sizeof(float));

    if(v != v) {
        _strcpy(buf, "nan");
    } else if(v < -FLT_MAX) {
        _strcpy(buf, "-inf");
    } else if(v > FLT_MAX) {
        _strcpy(buf, is_sign ? "+inf" : "inf");

    } else {
        const bool is_neg = _isnegf(v);

        if(is_neg) {
            v = -v;
        }

        const uintmax_t integer = (uintmax_t)v;
        const uintmax_t decimal = _proundf((v - (float)integer) * pow[prec]);
        const unsigned char padding = width > prec ? width - prec - (prec ? 1 : 0) : 0;

        if(decimal != 0) {
            _xitoa(decimal, _strjoin(_xitoa(integer, buf, maxlen, 10, 0, padding, is_neg, is_sign), '.'), maxlen, 10, 0, 0, false, false);
        } else if(prec != 0) {
            _strnfill(_strjoin(_xitoa(integer, buf, maxlen, 10, 0, padding, is_neg, is_sign), '.'), '0', prec);
        } else {
            _xitoa(integer, buf, maxlen, 10, 0, padding, is_neg, is_sign);
        }
    }

    return buf;
}

#pragma pack(push, 1)
typedef enum {
    SIZE_CHAR,
    SIZE_SHORT,
    SIZE_DEFAULT,
    SIZE_LONG,
    SIZE_LONGLONG
} SizeSpec;

typedef struct {
    unsigned char szspec;  /**< Size specifier */
    unsigned char prec;    /**< Precision */
    unsigned char width;   /**< Required width */
    struct {
        unsigned format:1; /**< Format specifier detected */
        unsigned sign:1;   /**< Force sign is set */
        unsigned prec:1;   /**< Force precision is set */
    } flags;
} ParserState;
#pragma pack(pop)

static void _ucprintf_reset(ParserState *state)
{
    state->szspec = SIZE_DEFAULT;
    state->prec = FMT_IPREC_DEFAULT;
    state->width = FMT_WIDTH_DEFAULT;

    state->flags.format = FALSE;
    state->flags.sign = FALSE;
    state->flags.prec = FALSE;
}

static intmax_t _pop_signed(va_list args, unsigned char szspec)
{
    switch(szspec) {
    case SIZE_CHAR:
    case SIZE_SHORT:
    case SIZE_DEFAULT:
        return va_arg(args, int);
    case SIZE_LONG:
        return va_arg(args, long);
    case SIZE_LONGLONG:
        return va_arg(args, long long);
    default:
        return 0;
    };
}

static uintmax_t _pop_unsigned(va_list args, unsigned char szspec)
{
    switch(szspec) {
    case SIZE_CHAR:
    case SIZE_SHORT:
    case SIZE_DEFAULT:
        return va_arg(args, unsigned int);
    case SIZE_LONG:
        return va_arg(args, unsigned long);
    case SIZE_LONGLONG:
        return va_arg(args, unsigned long long);
    default:
        return 0;
    };
}

void ucprintf(const char *fmt, ...)
{
    char buf[XTOA_BUF_SIZE];
    ParserState state;

    _ucprintf_reset(&state);

    va_list args;
    va_start(args, fmt);

    for(; *fmt; fmt++) {
        if(!(state.flags.format)) {
            if(*fmt == '%') {
                state.flags.format = TRUE;
            } else {
                _putchar(*fmt);
            }

            continue;

        } else if(*fmt == '+') {
            state.flags.sign = TRUE;
            continue;

        } else if(*fmt == '.') {
            size_t len;
            state.prec = _atoi_dp(fmt + 1, &len);
            state.flags.prec = TRUE;
            /* Changing loop counter here! */
            fmt += len;
            continue;

        } else if(_isdigit(*fmt)) {
            size_t len;
            state.width = _atoi_dp(fmt, &len);
            /* Changing loop counter here! */
            fmt += len - 1;
            continue;

        } else if(*fmt == 'h') {
            if(state.szspec == SIZE_DEFAULT) {
                state.szspec = SIZE_SHORT;
                continue;
            } else if(state.szspec == SIZE_SHORT) {
                state.szspec = SIZE_CHAR;
                continue;
            }

        } else if(*fmt == 'l') {
            if(state.szspec == SIZE_DEFAULT) {
                state.szspec = SIZE_LONG;
                continue;
            } else if(state.szspec == SIZE_LONG) {
                state.szspec = SIZE_LONGLONG;
                continue;
            }

        } else if(*fmt == '%') {
            _putchar('%');
        } else if(*fmt == 's') {
            _puts(va_arg(args, char*));
        } else if(*fmt == 'd' || *fmt == 'i') {
            _puts(_itoa(_pop_signed(args, state.szspec), buf, XTOA_BUF_SIZE, 10, state.prec, state.width, state.flags.sign));
        } else if(*fmt == 'u') {
            _puts(_uitoa(_pop_unsigned(args, state.szspec), buf, XTOA_BUF_SIZE, 10, state.prec, state.width, state.flags.sign));
        } else if(*fmt == 'x') {
            _puts(_uitoa(_pop_unsigned(args, state.szspec), buf, XTOA_BUF_SIZE, 16, state.prec, state.width, state.flags.sign));
        } else if(*fmt == 'X') {
            _puts(_to_upper(_uitoa(_pop_unsigned(args, state.szspec), buf, XTOA_BUF_SIZE, 16, state.prec, state.width, state.flags.sign)));
        } else if(*fmt == 'o') {
            _puts(_uitoa(_pop_unsigned(args, state.szspec), buf, XTOA_BUF_SIZE, 8, state.prec, state.width, state.flags.sign));
        } else if(*fmt == 'f') {
            _puts(_ftoa(va_arg(args, double), buf, XTOA_BUF_SIZE, state.flags.prec ? state.prec : FMT_FPREC_DEFAULT, state.width, state.flags.sign));
        } else if(*fmt == 'e') {
            _puts("<exp>"); /* TODO: take the argument from stack anyway */
        } else if(*fmt == 'E') {
            _puts("<EXP>"); /* TODO: take the argument from stack anyway */
        }  else {
            _putchar(*fmt);
        }

        /* Reset and wait for next identifier */
        _ucprintf_reset(&state);
    }

    va_end(args);

    _flush();
}
