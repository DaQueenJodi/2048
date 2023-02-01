#ifndef GLOBALS_H
#define GLOBALS_H
#include <pthread.h>
extern pthread_mutex_t board_mutex;
extern pthread_mutex_t quit_mutex;
#include <stdbool.h>
extern bool quit;
#endif // GLOBALS_H
