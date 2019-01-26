# rcs
Remote Client Shell

Process (rcs) starts a conection. If any server responses, the client duplicate STDIN, STDOUT and STERR and execve() a new process specified at 'rcs' parameters (maybe '/bin/bash'). Connection request is able to be retried (parameter) and sleep (parameter) at each connect() fails, waiting the server goes listenning.
