#include <string>

namespace io {

static const int buf_size = 1 << 22;
char buf[buf_size];
int len = 0, pos = 0;

inline bool is_blank(char c);
inline bool skip_blanks();

inline char get_char();
inline char peek_char();
inline char read_char();

template <class T = int> 
inline void read_int(T&);
inline void read_double(double&);
inline void read_string(std::string& s);

inline void write_char(char x);
template <class T> 
inline void write_int(T x, char end = 0);
inline void write_word(const char* s);
inline void write_double(double x, int digits);
inline void write_string(const std::string& s);

inline char get_char() {
    if (pos == len)
        pos = 0, len = fread(buf, 1, buf_size, stdin);
    if (pos == len)
        return -1;
    return buf[pos++];
}

inline char peek_char() {
    return buf[pos];
}

inline bool is_blank(char c) { return (c <= ' ' && c); }

inline bool skip_blanks() {
    while (is_blank(peek_char())) {
        get_char();
    }
    return peek_char() != 0;
}

inline char read_char() {
    char c = get_char();
    while (c <= 32)
        c = get_char();
    return c;
}

template <class T>
    inline void read_int(T& x) {
        int s = 1, c = read_char();
        x = 0;
        if (c == '-')
            s = -1, c = get_char();
        while ('0' <= c && c <= '9')
            x = x * 10 + c - '0', c = get_char();
        x = (s == 1) ? x : -x;
    }

inline void read_double(double& x) {
    std::string s;
    read_string(s);
    sscanf(s.c_str(), "%lf", &x);
}

inline void read_string(std::string& s) {
    if (skip_blanks()) {
        s.clear();
        while (!is_blank(peek_char())) {
            s += peek_char();
            get_char();
        }
    }
}

static int write_pos = 0;
static char write_buf[buf_size];

inline void write_char(char x) {
    if (write_pos == buf_size)
        fwrite(write_buf, 1, buf_size, stdout), write_pos = 0;
    write_buf[write_pos++] = x;
}

template <class T>
    inline void write_int(T x, char end) {
        if (x < 0)
            write_char('-'), x = -x;
        char s[24];
        int n = 0;
        while (x || !n)
            s[n++] = '0' + x % 10, x /= 10;
        while (n--)
            write_char(s[n]);
        if (end)
            write_char(end);
    }

inline void write_word(const char* s) {
    while (*s)
        write_char(*s++);
}

inline void write_double(double x, int digits) {
    char buffer[digits + 2];
    sprintf(buffer, "%lf", x);
    write_word(buffer);
}

inline void write_string(const std::string& s) {
    for (auto& c : s) {
        write_char(c);
    }
}

struct Flusher {
    ~Flusher() {
        if (write_pos)
            fwrite(write_buf, 1, write_pos, stdout), write_pos = 0;
    }
} flusher;

} // namespace io
