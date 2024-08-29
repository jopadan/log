#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/param.h>

enum log_lvl
{
	LOG_NFO,
	LOG_WRN,
	LOG_TRC,
	LOG_ERR,
	LOG_DBG,
	LOG_FTL,
	LOG_SYS,
};

enum log_col
{
	LOG_COLOR_BLACK   = 0,
	LOG_COLOR_RED     = 1,
	LOG_COLOR_GREEN   = 2,
	LOG_COLOR_YELLOW  = 3,
	LOG_COLOR_BLUE    = 4,
	LOG_COLOR_MAGENTA = 5,
	LOG_COLOR_CYAN    = 6,
	LOG_COLOR_WHITE   = 7,
	LOG_COLOR_DEFAULT = 9,
};

typedef uint8_t __attribute__((vector_size(sizeof(uint8_t) * 4))) vec4ub;
typedef vec4ub rgba8888;


/* default config settings */
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
static char* LOG_TMP                   = NULL;

extern inline void LOG_CLEAN()
{
	if(LOG_TMP != NULL)
	{
		free(LOG_TMP);
		LOG_TMP = NULL;
	}
}

/* close file */
extern inline void LOG_CLOSE()
{
	LOG_CLEAN();
	if(LOG_FILE != NULL)
	{
		fclose(LOG_FILE);
		LOG_FILE = NULL;
	}
}

/* color functions */
extern inline char* LOG_COLOR(uint8_t attr, uint8_t fg, uint8_t bg, const char* msg)
{
	if(asprintf(&LOG_TMP,"\x1b[%hhu;3%hhu;4%hhum%s%s", attr, fg, bg, msg, "\x1b[0m") == -1)
		LOG_CLEAN();
	return LOG_TMP;
}

extern inline char* LOG_COLOR_RGBA(rgba8888 fg, rgba8888 bg, const char* msg)
{
	uint8_t attr = 0;
	switch(((fg[3] + bg[3]) / 2) / 64)
	{
		case 0 : attr = 8; break;
		case 1 : attr = 2; break;
		case 3 : attr = 1; break;
		case 2 : attr = 0; break;
		default: attr = 0; break;
	};
	if(asprintf(&LOG_TMP,"\x1b[%hhu;38:2:%hhu:%hhu:%hhu;48:2:%hhu:%hhu:%hhum%s%s ", attr, fg[0], fg[1], fg[2], bg[0], bg[1], bg[2], msg, "\x1b[0m") == -1)
		LOG_CLEAN();
	return LOG_TMP;
}

/* log level prefix generation */
extern inline const char* LOG_LEVEL(enum log_lvl lvl)
{
	if(asprintf(&LOG_TMP, "[%s] ", LOG_COLORED ? LOG_COLOR(0, LOG_LEVEL_COLOR[lvl], LOG_COLOR_DEFAULT, LOG_LEVEL_STRING[lvl]) : LOG_LEVEL_STRING[lvl]) == -1)
		LOG_CLEAN();
	return LOG_TMP;
}

/* log filename string determination using arguments */
extern inline char* LOG_GETOPT_ITH(int argc, char** argv, int i)
{
	static char src[FILENAME_MAX] = { '\0' };
	return argc == 1 ? strcat(strcpy(src, argv[0]), ".log") : strcpy(src, argv[i]);
}

/* output log and empty queue */
extern inline char* LOG_FLUSH(char* buf)
{
	if(buf[0] != '\0')
	{
		if(LOG_FILE != NULL)
			fputs(buf, LOG_FILE);
		if(LOG_TTY != NULL)
			fputs(buf, LOG_TTY);
		buf[0] = '\0';
		buf[1] = '\0';
	}
	return buf;
}

/* queue log non/prefixed entries  */
extern inline char* LOG_QUEUE(const char* pre, const char* time_format, const char* filename, const char* funcname, const ssize_t lineno, const char* msg)
{
        static char buf[MAX(LOG_LEN, LOG_LEN_MIN)] = {'\0'};

        if(msg != NULL)
        {
                if(pre != NULL)
                        strcat(buf, pre);

                if(LOG_TIMESTAMP && time_format != NULL)
                {
                        time_t t = time(NULL);
                        struct tm* tm = localtime(&t);
                        char tme[32];
                        strftime(tme, sizeof(tme), time_format, tm);
                        tme[strlen(tme) - 1] = ' ';
                        const char* color = LOG_COLORED ? LOG_COLOR(0, LOG_TIMESTAMP_COLOR, 9, tme) : tme;
                        strcat(buf, color);
                }


                if(LOG_FILENAME && filename != NULL)
                {
                        if(asprintf(&LOG_TMP, "%s", LOG_COLORED ? LOG_COLOR(0, LOG_FILENAME_COLOR, LOG_COLOR_DEFAULT, filename) : filename) == -1)
                                LOG_CLEAN();

                        if((strlen(buf) + strlen(LOG_TMP) + 2) >= MAX(LOG_LEN, LOG_LEN_MIN))
                                LOG_FLUSH(buf);

                        strcat(buf, LOG_TMP);
                }

                if(LOG_LINENO)
                {
                        char line[32] = { '\0' };
                        snprintf(line, 32, "%zu", lineno);

                        if(LOG_COLORED)
                        {
                                if(snprintf(line, 32, ":%s", LOG_COLOR(0, LOG_LINENO_COLOR, LOG_COLOR_DEFAULT, line)) == -1)
                                        LOG_CLEAN();
                        }
                        if((strlen(buf) + strlen(line) + 2) >= MAX(LOG_LEN, LOG_LEN_MIN))
                                LOG_FLUSH(buf);

                        strcat(buf, line);
                }
                strcat(buf, " ");

                if(LOG_FUNCNAME && funcname != NULL)
                {
                        if(asprintf(&LOG_TMP, "%s ", LOG_COLORED ? LOG_COLOR(0, LOG_FUNCNAME_COLOR, LOG_COLOR_DEFAULT, funcname) : funcname) == -1)
				LOG_CLEAN();
		}

		if(LOG_FUNCNAME && funcname != NULL)
		{
			if(asprintf(&LOG_TMP, "%s ", LOG_COLORED ? LOG_COLOR(0, LOG_FUNCNAME_COLOR, LOG_COLOR_DEFAULT, funcname) : funcname) == -1)
				LOG_CLEAN();

			if((strlen(buf) + strlen(LOG_TMP) + 2) >= MAX(LOG_LEN, LOG_LEN_MIN))
				LOG_FLUSH(buf);

			strcat(buf, LOG_TMP);
		}

		if(strlen(buf) + strlen(msg) + 2 >= MAX(LOG_LEN, LOG_LEN_MIN))
			LOG_FLUSH(buf);

		strcat(buf, msg);
		strcat(buf, "\n\0");
	}
	return buf;
}

