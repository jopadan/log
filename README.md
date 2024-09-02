# log

Minimalistic single-header logging framework library in C 

![](./examples/example.svg)

## Usage

### Build
Run `cmake --install-prefix=/usr . && make install` to build and install

### Code

Investigate the `examples/example.c` example and `log.h`

```c
#ifndef LOG_TTY
#define LOG_TTY stderr
#endif
#define LOG_LEN_MIN 4096
#ifndef LOG_LEN
#define LOG_LEN LOG_LEN_MIN
#endif
bool  LOG_TIMESTAMP                    = true;
bool  LOG_FUNCNAME                     = true;
bool  LOG_FILENAME                     = true;
bool  LOG_LINENO                       = true;
bool  LOG_COLORED                      = true;
FILE* LOG_FILE                         = NULL;
const enum log_col LOG_LEVEL_COLOR[8]  = { LOG_COLOR_GREEN, LOG_COLOR_YELLOW, LOG_COLOR_BLUE, LOG_COLOR_RED, LOG_COLOR_RED, LOG_COLOR_RED, LOG_COLOR_MAGENTA };
enum log_col LOG_TIMESTAMP_COLOR       = LOG_COLOR_BLUE;
enum log_col LOG_FILENAME_COLOR        = LOG_COLOR_GREEN;
enum log_col LOG_FUNCNAME_COLOR        = LOG_COLOR_CYAN;
enum log_col LOG_LINENO_COLOR          = LOG_COLOR_MAGENTA;
char* LOG_LEVEL_STRING[8]              = { "NFO", "WRN", "TRC", "ERR", "DBG", "FTL", "SYS" };

extern inline void LOG_CLEAN();
extern inline void LOG_CLOSE();
extern inline char* LOG_COLOR(uint8_t attr, uint8_t fg, uint8_t bg, const char* msg);
extern inline char* LOG_COLOR_RGBA(rgba8888 fg, rgba8888 bg, const char* msg);
extern inline const char* LOG_LEVEL(enum log_lvl lvl);
extern inline char* LOG_GETOPT_ITH(int argc, char** argv, int i);
extern inline char* LOG_QUEUE(const char* pre, const char* time_format, const char* filename, const char* funcname, const ssize_t lineno, const char* msg);
extern inline char* LOG_PUTS(const char* msg);
extern inline char* LOG_FLUSH(char* buf);
```
