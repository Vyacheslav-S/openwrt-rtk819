#ifndef _TIMER_H__
#define _TIMER_H__

typedef void (*timercb_t)(void);

void timer_init(void);
void timer_add(timercb_t timercb, int timeout);

#endif
