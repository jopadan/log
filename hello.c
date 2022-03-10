#include "log/log.h"

log_t* logger = NULL;

void cleanup(void)
{
	log_halt(logger);
}

int main(int argc, char** argv)
{
	if((logger = log_create()) != NULL)
	{
		atexit(cleanup);

		log_queue(logger, LOG_SYSTEM, "Hello World!");
		log_flush(logger);

		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}
