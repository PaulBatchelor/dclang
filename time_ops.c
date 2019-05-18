#include <sys/time.h>

struct timespec tval, stm, dummy;

static void clockfunc()
{  
    // gettimeofday(&tval, NULL)
    clock_gettime(CLOCK_MONOTONIC, &tval);
    unsigned long long now = (tval.tv_sec * 1000000000L) + tval.tv_nsec;
    now = now / 1000L;
    push((MYINT)now);
}

static void sleepfunc() {
    MYINT sleeptime_ns = pop();
    MYINT sleeptime_sec = pop();
    stm.tv_sec = sleeptime_sec;
    stm.tv_nsec = sleeptime_ns;
    nanosleep(&stm, &dummy);
}
