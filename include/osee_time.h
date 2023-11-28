#ifndef OSEE_TIME_H
#define OSEE_TIME_H

#include <time.h>

//秒
#define s2timespec(s, ts) {                                   \
    (ts)->tv_sec = (s);                                    \
    (ts)->tv_nsec = 0;                           \
}
# define s2timeval(s, tv) {                                   \
    (tv)->tv_sec = s;                                    \
    (tv)->tv_usec = 0;                           \
}

//毫秒
#define ms2timespec(ms, ts) {                                   \
    (ts)->tv_sec = (ms) / 1000;                                    \
    (ts)->tv_nsec = (ms) % 1000 * 1000000;                           \
}
# define ms2timeval(ms, tv) {                                   \
    (tv)->tv_sec = (ms) / 1000;                                    \
    (tv)->tv_usec = (ms) % 1000 * 1000;                           \
}

//微秒
#define us2timespec(us, ts) {                                   \
    (ts)->tv_sec = (us) / 1000000;                                    \
    (ts)->tv_nsec = (us) % 1000000 * 1000;                           \
}
# define us2timeval(us, tv) {                                   \
    (tv)->tv_sec = (us) / 1000000;                                    \
    (tv)->tv_usec = (us) % 1000000;                           \
}

# define timespec_isset(tvp)	((tvp)->tv_sec || (tvp)->tv_nsec)
# define timespec_clear(tvp)	((tvp)->tv_sec = (tvp)->tv_nsec = 0)
# define timespec_cmp(a, b, CMP) 						      \
  (((a)->tv_sec == (b)->tv_sec) ? 					      \
   ((a)->tv_nsec CMP (b)->tv_nsec) : 					      \
   ((a)->tv_sec CMP (b)->tv_sec))
# define timespec_add(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;			      \
    (result)->tv_nsec = (a)->tv_nsec + (b)->tv_nsec;			      \
    if ((result)->tv_nsec >= 1000000000)					      \
      {									      \
    ++(result)->tv_sec;						      \
    (result)->tv_nsec -= 1000000000;					      \
      }									      \
  } while (0)
# define timespec_sub(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec;			      \
    if ((result)->tv_nsec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_nsec += 1000000000;					      \
    }									      \
  } while (0)

#endif // OSEE_TIME_H
