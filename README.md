# log

Minimalist single-header logging system library in C

## Usage

### Build
Run `cmake --install-prefix=/usr . && make install` to build and install

### Code

Investigate the `examples/example.c` example and `log.h`

```c
/* set log length and sink */
#define log_len 512
#define log_tty stdout
#include <log/log.h>

int main(int argc, char** argv)
{
        /* use default/optional log_file and close it at exit */
        log_file = fopen(log_getopt_ith(argc, argv, 1), "w");
        atexit(log_close);

        /* disable timestamping */
        log_timestamp = false;

        /* queue test log messages */
        for(size_t i = 0; i < 7; i++)
                log_queue(log_level(i), "prefixed output!");

        log_queue(NULL, "none prefixed output!");

        /* set tty sink from stderr default to stdout */
        #undef log_tty
        #define log_tty stderr

        /* enable timestamping */
        log_timestamp = true;

        /* queue test log messages */
        for(size_t i = 0; i < 7; i++)
                log_queue(log_level(i), "timestamped prefixed output!");

        /* queue last test log message and flush sinks */
        log_flush(log_queue(NULL, "none prefixed timestamped output!"));

        exit(EXIT_SUCCESS);
}
```
