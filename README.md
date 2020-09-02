# xv6-scheduler

### Priority based Weighted Round Robin Scheduling in xv6:
The  current  scheduler  in  xv6  is  an  unweighted  round  robin  scheduler. Modified the scheduler to take into account user-defined process priorities and implement a weighted scheduler. Used the priorities as weights to implement a weighted round robin scheduler. 

### Objectives:
- a higher numerical priority should translate into more CPU time for the process, so that higher priority processes finish faster than lower priority ones  
- lower priority processes should not be excessively starved, and should get some CPU time even in the presence of higher priority processes.
