#ifndef _RAND_H_
#define _RAND_H_


#include <inttypes.h>

// need free
char* allocrandomstring(int strlen);
int randbuffer(char* buf, int len);
int randomint();

#endif
