#ifndef _JV_TIMER_H_INCLUDED_
#define _JV_TIMER_H_INCLUDED_

#include <jv_core.h>
#include <jv_rbtree.h>

typedef jv_uint_t jv_msec_t;

typedef struct jv_timer_s jv_timer_t;
typedef struct jv_timer_task_s jv_timer_task_t;
typedef struct timeval jv_timeval_t;

typedef void jv_timer_schedule(void *data);

struct jv_timer_s {
  jv_timer_task_t *first;
  jv_timer_task_t *last;

  jv_rbtree_t *rbtree;

  jv_uint_t alloc_count;
  jv_uint_t active_count;
  jv_uint_t idle_count;
};

struct jv_timer_task_s {
  jv_timer_schedule *schedule;
  void *data;

  jv_msec_t trigger; /* rbtree key: current_msec + delay + (period * n)  */
  jv_msec_t period;  /* periodic millisecond */
};

jv_timer_t *jv_timer_create();

/*
 *  delay=0与 period不能都为0;
 *  delay=0时，表示采用当前时间；
 *  period=0时，表示不重复执行；
 * */
jv_timer_task_t *jv_timer_add(jv_timer_t *timer, jv_msec_t delay, jv_msec_t period, jv_timer_schedule *schedule, void *data);

void jv_timer_del(jv_timer_t *timer, jv_timer_task_t *task);

void jv_timer_run(jv_timer_t *timer);

jv_msec_t jv_timer_mstimeout(jv_timer_t *timer);

jv_int_t jv_timer_reset(jv_timer_t *timer, jv_timer_task_t *task);

void jv_timer_cleanup(jv_timer_t *timer);

void jv_timer_destroy(jv_timer_t *timer);

#endif /* _JV_TIMER_H_INCLUDED_ */
