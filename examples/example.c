/* set log length and sink */
#define LOG_LEN 4096
#define LOG_TTY stdout
#include <log/log.h>

int main(int argc, char** argv)
{
	/* use default/optional LOG_FILE and close it at exit */
	LOG_FILE = fopen(LOG_GETOPT_ITH(argc, argv, 1), "w");
	atexit(LOG_CLOSE);

	/* disable timestamping */
	LOG_TIMESTAMP = false;

	/* queue test log messages */
	for(size_t i = 0; i < 7; i++)
		LOG_QUEUE(LOG_LEVEL(i), NULL, "example.c", "main(int argc, char** argv)", 17, "Hello OwlFroggers World!");
		
	LOG_QUEUE(NULL, NULL, "example.c", "main(int argc, char** argv)", 19, "Hello OwlFroggers World!");

	/* set tty sink from stderr default to stdout */
	#undef LOG_TTY
	#define LOG_TTY stderr

	/* enable timestamping */
	LOG_TIMESTAMP = true;

	/* queue test log messages */
	for(size_t i = 0; i < 7; i++)
		LOG_QUEUE(LOG_LEVEL(i), "%X0", "example.c", "main(int argc, char** argv)", 30, "Hello OwlFroggers World!");

	/* queue last test log message and flush sinks */
	LOG_FLUSH(LOG_QUEUE(NULL, "%X0", "example.c", "main(int argc, char** argv)", 33, "Hello OwlFroggers World!"));

	exit(EXIT_SUCCESS);
}
