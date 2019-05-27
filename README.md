# Ddetector

---

### Overview
- Runtim monitoring tool using runtim interpositioning to detects & predicts **resource deadlocks**
- Costruct two dynamic libraries to override Pthread
  - online deadlock detector
  - offline deadlock predictor

---

### Background
##### About Runtim Interpositioning
- if **LD_PRELOAD** env is set, when you load and execute program, the linker will search **LD_PRELOAD** before any other libraryies.

##### About Resource Deadlock
- TODO

---

### Components

##### Online deadlock detector
- alert a user to the occurrence of a cyclic deadlock at a target program execution
- Implement [Page 8 of "Deadlock Detection](https://github.com/hongshin/OperatingSystem/blob/master/notes/deadlock.pdf)
  - Construct DDetector as a dynamic lib `ddetector.so` (ddetector.c) that overrides certain Pthread API functions
- Online analysis: DDetector prints out an alert to **stderr** in runtime when target program execution falls into a cyclic deadlock
  - must be side-effect-free (i.e., do not change target program behaviors)

##### Offline deadlock predictor
- extract a runtime trace of a target program execution
- run an analysis on the runtime trace to check potential deadlocks
- Construct a cyclic deadlock predictor DPredictor
- Implement [Page 17 of "Goodlock algorithm](https://github.com/hongshin/OperatingSystem/blob/master/notes/deadlock.pdf) described in "Deadlock Detection"
  - dynamic library dmonitor.so (`dmonitor.c`)
  - offline trace analyzer dpredictor (`dpredictor.c`)
- Workflow
  - `dmonitor` is dynamically linked with the target program
  - `dmonitor` extracts the runtime trace of a target program execution for deadlock prediction
    - **generate demonitor.trace**
  - after the target program execution, **dpredictor** reads **dmonitor.trace** and detects potential cyclic deadlocks
- Deadlock report must contain following
  - number of threads associated with the potential deadlock
  - Source code locations of the lock operations associated with the potential deadllocks (**addr2line**)
---

### Requirements

- write example programs to show the two checkers detects deadlocks
- submit build scripts for the techniques and your example programs as source code
