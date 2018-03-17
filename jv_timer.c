#include <jv_timer.h>

volatile jv_msec_t jv_current_msec;

jv_timer_t *jv_timer_create() {
  jv_timer_t *timer;
  jv_rbtree_t *rbtree;

  if ((timer = malloc(sizeof(jv_timer_t))) == NULL) {
    printf("jv_timer_create() failed\n");
    return NULL;
  }

  if ((rbtree = jv_rbtree_create()) == NULL) {
    printf("jv_timer_create() failed\n");
    return NULL;
  }

  timer->rbtree = rbtree;

  timer->alloc_count = 0;
  timer->active_count = 0;
  timer->idle_count = 0;

  return timer;
}

jv_timer_task_t *jv_timer_add(jv_timer_t *timer, jv_msec_t delay, jv_msec_t period, jv_timer_schedule *schedule, void *data) {
  jv_timer_task_t *task;
  jv_rbtree_data_t rbtree_data;

  if (delay == 0 && period == 0) {
    printf("delay millisecond and period millisecond are zero, add timer failed\n");
    return NULL;
  }

  if ((task = malloc(sizeof(jv_timer_task_t))) == NULL) {
    printf("jv_timer_add() failed\n");
    return NULL;
  }

  task->schedule = schedule;
  task->data = data;

  task->period = period;
  task->trigger = jv_current_msec + delay + period;

  rbtree_data.key = task->trigger;
  rbtree_data.value = task;

  if (jv_rbtree_insert(timer->rbtree, &rbtree_data) == JV_ERROR) {
    free(task);
    return NULL;
  }

  timer->alloc_count++;
  timer->active_count++;

  return task;
}

void jv_timer_del(jv_timer_t *timer, jv_timer_task_t *task) {
  jv_rbtree_node_t *node;

  node = jv_rbtree_get(timer->rbtree, task->trigger);

  if (node != NULL) {
    jv_rbtree_remove(timer->rbtree, node);
    free(task);
  }
}

void jv_timer_run(jv_timer_t *timer) {
  jv_rbtree_t *rbtree;
  jv_rbtree_node_t *node;
  jv_msec_t trigger;

  rbtree = timer->rbtree;

  /* printf("1 rbtree->root:%lu, rbtree->sentinel:%lu\n", (unsigned long)rbtree->root, (unsigned long)rbtree->sentinel); */

  if (rbtree->root == rbtree->sentinel) {
    printf("no task in timer\n");
    return;
  }

  /* while (1) { */
  node = jv_rbtree_min(rbtree->root, rbtree->sentinel);

  /*
  if (node->left == rbtree->sentinel) {
      jv_log_debug(timer->pool->log, JV_LOG_DEBUG, "no timing schedule task");
      return;
  } */

  trigger = node->data->key;

  /* printf("jv_current_msec :%d, trigger: %d\n", jv_current_msec, trigger); */

  if (jv_current_msec >= trigger) { /* trigger timer */
    jv_timer_task_t *task;

    task = (jv_timer_task_t *) node->data->value;
    task->schedule(task->data);

    jv_rbtree_remove(timer->rbtree, node);

    if (task->period > 0) { /* task is a periodic schedule, must be reschedule */
      jv_rbtree_data_t rbtree_data;

      task->trigger = jv_current_msec + task->period;

      /* printf("trigger:%d\n", task->trigger); */

      rbtree_data.key = task->trigger;
      rbtree_data.value = task;

      if (jv_rbtree_insert(timer->rbtree, &rbtree_data) == JV_ERROR) {
        free(task);
      }
    } else { /* task is a one-time schedule */
      printf("task is a one-time schedule.\n");
      free(task);
    }
    /*   } else {
     return;
     }*/
  }
}

jv_msec_t jv_timer_mstimeout(jv_timer_t *timer) {
  jv_rbtree_t *rbtree;
  jv_rbtree_node_t *node;
  jv_timer_task_t *task;
  jv_int_t ms;

  rbtree = timer->rbtree;

  /* printf("2 rbtree->root:%lu, rbtree->sentinel:%lu\n", (unsigned long)rbtree->root, (unsigned long)rbtree->sentinel); */

  if (rbtree->root == rbtree->sentinel) { /* 无定时器 */
    printf("have no timer schedule\n");
    return JV_INFTIM;
  }

  node = jv_rbtree_min(rbtree->root, rbtree->sentinel);

  task = (jv_timer_task_t *) node->data->value;

  ms = task->trigger - jv_current_msec;

  return ms <= 0 ? 0 /* 无定时任务 */ : ms;
}

jv_int_t jv_timer_reset(jv_timer_t *timer, jv_timer_task_t *task) {
  return JV_OK;
}

void jv_timer_cleanup(jv_timer_t *timer) {
}

void jv_timer_destroy(jv_timer_t *timer) {
  free(timer->rbtree);
  free(timer);
}
