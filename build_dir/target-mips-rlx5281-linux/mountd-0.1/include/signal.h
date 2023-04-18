#ifndef _SIGNAL_H__
#define _SIGNAL_H__

void signal_init(void (*_crtlc_cb)(void));

int signal_usr1(void);
int signal_usr2(void);

#endif
