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

#ifndef log_tty
#define log_tty stderr
#endif
#ifndef log_len
#define log_len 2048
#endif

bool  log_timestamp = true;
FILE* log_file = NULL;

extern inline char* log_getopt_ith(int argc, char** argv, int i)
{
	static char src[FILENAME_MAX] = { '\0' };
	return argc == 1 ? strcat(strcpy(src, argv[0]), ".log") : strcpy(src, argv[i]);
}

extern inline void log_close()
{
	if(log_file != NULL)
	{
		fclose(log_file);
		log_file = NULL;
	}
}

extern inline char* log_flush(char* buf)
{
	if(buf[0] != '\0')
	{
		if(log_file != NULL)
			fputs(buf, log_file);
		if(log_tty != NULL)
			fputs(buf, log_tty);
		buf[0] = '\0';
	}
	return buf;
}

extern inline char* log_queue(const char pre[3], const char* msg)
{
	static char hdr[8] = {'\0','\0','\0',':',' ','\0','\0','\0'};
	static char buf[log_len] = {'\0'};

	if(msg != NULL)
	{
		if(pre != NULL)
		{
			strncpy(hdr, pre, 3);
			strcat(buf, hdr);
		}

		if(log_timestamp)
		{
			time_t t = time(NULL);
			char* tme = ctime(&t);
			tme[strlen(tme) - 1] = ' ';
			strcat(buf, tme);
		}

		if(strlen(buf) + strlen(msg) + 2 >= log_len)
			log_flush(buf);

		strcat(buf, msg);
		strcat(buf, "\n");
	}
	return buf;
}

