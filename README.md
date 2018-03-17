# A modern, high performance, efficient task timer based rbtree using ANSI C

A modern, high performance, efficient task timer using ANSI C, based Red Black Tree (rbtree), you may be also see []https://github.com/jxva/jv-rbtree

## Features

- No dependence
- ~150 LOC
- Use efficient algorithms based rbtree

## Getting Started

### jv_timer_main.c

```c
#include <jv_timer.h>
#include <sys/time.h> /* gettimeofday */
#include <unistd.h>   /* usleep */

extern volatile jv_msec_t jv_current_msec;

static jv_msec_t jv_time_update(void) {

    time_t sec;
    jv_uint_t msec;
    struct timeval tv;

    (void)gettimeofday(&tv, NULL);

    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;

    jv_current_msec = (jv_msec_t)sec * 1000 + msec;

    return jv_current_msec;
}

void timer_test1(void *arg) {
  printf("timer_test1()\n");
}

void timer_test2(void *arg) {
  printf("timer_test2()\n");
}

void timer_test3(void *arg) {
  printf("timer_test3()\n");
  usleep(100);
}

void sig_int(int signo) {
  puts("Exit");
  exit(0);
}

int main(int argc, char *argv[]) {
  jv_timer_t *timer;

  signal(SIGINT, sig_int);

  timer = jv_timer_create();

  jv_time_update();

  if (!jv_timer_add(timer, 0, 1 * 1000L, timer_test1, NULL)) {
    printf("tmr_create(test1) failed\n");
    exit(1);
  }

  if (!jv_timer_add(timer, 2 * 1000L, 0, timer_test2, NULL)) {
    printf("tmr_create(test2) failed\n");
    exit(1);
  }

  if (!jv_timer_add(timer, 0, 3 * 1000L, timer_test3, NULL)) {
    printf("tmr_create(test3) failed\n");
    exit(1);
  }

  while (1) {
    printf("jv_timer_mstimeout:%lu\n", jv_timer_mstimeout(timer));
    jv_time_update();
    jv_timer_run(timer);
    usleep(1000 * 1000L);
  }

  jv_timer_destroy(timer);
  return 0;
}
```

## Build

    $ make

## Run

    $ ./jv_timer_main

## Api References

  `TODO`

## License

This project is under MIT License. See the [LICENSE](LICENSE) file for the full license text.

