++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             servers/pm/const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
17100 /* Constants used by the Process Manager. */
17101
17102 #define NO_MEM ((phys_clicks) 0) /* returned by alloc_mem() with mem is up */
17103
17104 #define NR_PIDS 30000 /* process ids range from 0 to NR_PIDS-1.
17105 * (magic constant: some old applications use
17106 * a ’short’ instead of pid_t.)
17107 */
17108
17109 #define PM_PID 0 /* PM’s process id number */
17110 #define INIT_PID 1 /* INIT’s process id number */
17111