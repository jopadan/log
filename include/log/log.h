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
#ifndef log_tty
#define log_tty stderr
#endif
#define log_len_min 2048
#ifndef log_len
#define log_len log_len_min
#endif

bool  log_timestamp = true;
bool  log_colored = true;
FILE* log_file = NULL;
const enum log_col log_level_color[8]  = { LOG_COLOR_GREEN, LOG_COLOR_YELLOW, LOG_COLOR_BLUE, LOG_COLOR_RED, LOG_COLOR_RED, LOG_COLOR_RED, LOG_COLOR_MAGENTA };
const enum log_col log_timestamp_color = LOG_COLOR_BLUE;
const char* log_level_string[8] = { "NFO", "WRN", "TRC", "ERR", "DBG", "FTL", "SYS" };

/* color functions */
extern inline char* log_color(uint8_t attr, uint8_t fg, uint8_t bg, const char* msg)
{
	static char* dst = NULL;

	if(asprintf(&dst,"\x1b[%hhu;3%hhu;4%hhum%s%s", attr, fg, bg, msg, "\x1b[0m") == -1)
		dst = NULL;
	return dst;
}

extern inline char* log_color_rgba(rgba8888 fg, rgba8888 bg, const char* msg)
{
	static char* dst = NULL;
	uint8_t attr = 0;
	switch(((fg[3] + bg[3]) / 2) / 64)
	{
		case 0 : attr = 8; break;
		case 1 : attr = 2; break;
		case 3 : attr = 1; break;
		case 2 : attr = 0; break;
		default: attr = 0; break;
	};
	if(asprintf(&dst,"\x1b[%hhu;38:2:%hhu:%hhu:%hhu;48:2:%hhu:%hhu:%hhum%s%s ", attr, fg[0], fg[1], fg[2], bg[0], bg[1], bg[2], msg, "\x1b[0m") == -1)
		dst = NULL;
	return dst;
}

/* log level prefix generation */
extern inline const char* log_level(enum log_lvl lvl)
{
	static char* dst = NULL;
	if(asprintf(&dst, "[%s] ", log_colored ? log_color(0, log_level_color[lvl], LOG_COLOR_DEFAULT, log_level_string[lvl]) : log_level_string[lvl]) == -1)
		dst = NULL;
	return dst;
}

/* log filename string determination using arguments */
extern inline char* log_getopt_ith(int argc, char** argv, int i)
{
	static char src[FILENAME_MAX] = { '\0' };
	return argc == 1 ? strcat(strcpy(src, argv[0]), ".log") : strcpy(src, argv[i]);
}

/* close file */
extern inline void log_close()
{
	if(log_file != NULL)
	{
		fclose(log_file);
		log_file = NULL;
	}
}

/* output log and empty queue */
extern inline char* log_flush(char* buf)
{
	if(buf[0] != '\0')
	{
		if(log_file != NULL)
			fputs(buf, log_file);
		if(log_tty != NULL)
			fputs(buf, log_tty);
		buf[0] = '\0';
		buf[1] = '\0';
	}
	return buf;
}

/* queue log non/prefixed entries  */
extern inline char* log_queue(const char* pre, const char* msg)
{
	static char buf[MAX(log_len, log_len_min)] = {'\0'};

	if(msg != NULL)
	{
		if(pre != NULL)
			strcat(buf, pre);

		if(log_timestamp)
		{
			time_t t = time(NULL);
			struct tm* tm = localtime(&t);
			char tme[32];
			strftime(tme, sizeof(tme), "%X0", tm);
			tme[strlen(tme) - 1] = ' ';
			const char* color = log_colored ? log_color(0, log_timestamp_color, 9, tme) : tme;
			strcat(buf, color);
		}

		if(strlen(buf) + strlen(msg) + 2 >= MAX(log_len, log_len_min))
			log_flush(buf);

		strcat(buf, msg);
		strcat(buf, "\n\0");
	}
	return buf;
}

