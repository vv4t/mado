#include <stdio.h>
#include <sys/time.h>

long int time_ms_since_epoch() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  return ms;
}