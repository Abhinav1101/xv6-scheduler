# xv6-scheduler

### Priority based Weighted Round Robin Scheduling in xv6:
The  current  scheduler  in  xv6  is  an  unweighted  round  robin  scheduler. Modified the scheduler to take into account user-defined process priorities and use it as weights to implement a weighted round robin scheduler. 

### Objectives:
- A higher numerical priority should translate into more CPU time for the process, so that higher priority processes finish faster than lower priority ones  
- Lower priority processes should not be excessively starved, and should get some CPU time even in the presence of higher priority processes.

### Instructions:  
You must replace these files in your xv6-public directory and then run the qemu simulator.  
You must add a makefile for your testcases.

### To be added:  
Testcases and expected outputs
