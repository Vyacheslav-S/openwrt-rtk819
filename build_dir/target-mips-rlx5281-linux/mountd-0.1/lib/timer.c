#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/log.h"
#include "include/list.h"
#include "include/timer.h"
#include "include/ucix.h"

/* when using this file, alarm() is used */

struct list_head timers;

struct timer {
	struct list_head list;
	int count;
	int timeout;
	timercb_t timercb;
};

void timer_add(timercb_t timercb, int timeout)
{
	struct timer *timer;
	timer = malloc(sizeof(struct timer));
	if(!timer)
	{
		log_printf("unable to get timer buffer\n");
		return;
	}
	timer->count = 0;
	timer->timeout = timeout;
	timer->timercb = timercb;
	INIT_LIST_HEAD(&timer->list);
	list_add(&timer->list, &timers);
}

void timer_proc(int signo)
{
	struct list_head *p;
	list_for_each(p, &timers)
	{
		struct timer *q = container_of(p, struct timer, list);
		q->count++;
		if(!(q->count%q->timeout))
		{
			q->timercb();
		}
	}
	alarm(1);
}

void timer_init(void)
{
	struct sigaction s;
	INIT_LIST_HEAD(&timers);
	s.sa_handler = timer_proc;
	s.sa_flags = 0;
	sigaction(SIGALRM, &s, NULL);
	alarm(1);
}
