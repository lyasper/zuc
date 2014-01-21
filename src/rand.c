#include "rand.h"

#include "util.h"

#include <time.h>
#include <assert.h>

int randbuffer(char* buf, int len)
{
    int randomData = open("/dev/urandom", O_RDONLY);
    int rlen = 0;
    rlen = read(randomData, buf, len);
    close(randomData);
    return rlen>0;
}

char allchar[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// remember to free ret later
char* allocrandomstring(int strlen)
{
    assert(strlen > 0);
    char* ret = (char*)malloc(strlen + 1);
    if(ret == NULL)
      {
	return NULL;
      }
    int i = 0;
    i = randbuffer(ret, strlen);
    for(i = 0; i < strlen; i++)
    {
        ret[i] = allchar[(128+ret[i])%52];
    }
    ret[strlen] = 0;
    return ret;
}

int randomint()
{
    int ret;
    int r = randbuffer((char*)&ret, 4);
    if(r)
      return ret;
    else
      return 0x13579bd;
}
