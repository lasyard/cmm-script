#ifndef _CMM_CHAR_UTILS_H_
#define _CMM_CHAR_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

static inline bool is_line_end(int c)
{
    return c == '\0' || c == '\r' || c == '\n';
}

static inline bool is_space(int c)
{
    return c == ' ' || c == '\t';
}

static inline bool is_digit(int c)
{
    return '0' <= c && c <= '9';
}

static inline int digit_value(char c)
{
    return c & 0x0F;
}

static inline int hex_value(char c)
{
    return (c & 0x0F) + 9;
}

static inline bool is_hex(int c)
{
    return ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

static inline bool is_hex_digit(char c)
{
    return is_digit(c) || is_hex(c);
}

static inline bool is_alpha(int c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static inline bool is_id_init(int c)
{
    return is_alpha(c) || c == '_' || c == '@' || c == '\\';
}

static inline bool is_id_body(int c)
{
    return is_id_init(c) || is_digit(c);
}

#ifdef __cplusplus
}
#endif

#endif /* _CMM_CHAR_UTILS_H_ */
