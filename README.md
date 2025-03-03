
# User Threads Project

## Overview

This project implements a user-level thread library supporting preemptive round-robin scheduling and lottery scheduling. The scheduler efficiently manages multiple user-level threads within a single kernel thread and demonstrates different scheduling policies.

## Features
- **Preemptive Round-Robin Scheduling**: Threads are scheduled in a cyclic order, each getting an equal time slice.

- **Lottery Scheduling**: Threads receive a configurable number of lottery tickets, and a random draw determines the next thread to run.

- **User-Level Thread Management**: Custom thread creation, execution, and context switching without relying on kernel threads.

- **Preemptive Scheduling with Timer Interrupts**: Uses SIGALRM for automatic thread switching.

- **Comparison of Scheduling Strategies**: Includes experiments reconstructing Waldspurger's Lottery Scheduling experiments.

## Project Structure

```
/workspace/user-threads
├── build                     # Directory for compiled binaries
├── include                # Directory for header files
│   └── schedulers.h
│   └── queue.h
│   └── uthreads.h
├── src                     # Directory for source files
│   ├── schedulers.c
│   └── queue.c
│   └── uthreads.c
├── test                    # Directory for test files
│   ├── test_1_single_thread.c
│   ├── test_2_thread_yield.c
│   ├── test_3_round_robin.c
│   ├── test_2_lottery.c
│   ├── test_2_waldspurger.c
│   └── test_3_preemptive.c
├── Makefile                # Makefile for building the project
├── README.md               # This README file
├── design.txt              # Design document
├── command.txt             # Project Run Command document
└── docker-compose.yml      # Docker Compose configuration
```

## Building the Project

To build the project, run the following command:

```sh
make
```

This will compile the source files and create the necessary binaries in the `bin` directory.

## Running Tests

To run the tests, execute the following command:

```sh
make test
```

This will compile the test files and run the test executables.

## Cleaning Up

To clean up the compiled files, run:

```sh
make clean
```

This will remove the `bin` directory and all compiled binaries.

## Additional Information

- The `include` directory contains the header files.
- The `src` directory contains the source files.
- The `test` directory contains the test files.
- The `Makefile` is used to automate the build process.
- The `design.txt` file contains the design documentation.
- The `docker-compose.yml` file is used for Docker Compose configuration.
