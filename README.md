# Reverse Shell 🏴‍☠️
In order to gain code execution on a remote machine, we can open a shell and start running shell commands on the attacked machine. 
There are 2 types of methods that we can use:

### I) Bind shell
 In bind shell, the client is the attacker and the attacked machine is the server. By connecting to the remote server and gaining a shell
 we can perform remote code execution.
 
### II) Reverse shell
In reverse shell the the client is attacked machine, and the server is the attacker. The attacked mechine connects to the server hosted by
an attacker, and redirects the output into it. That way we achieve remote code execution. 

![reverse shell](https://static.wixstatic.com/media/f20f53_eb087b9dfa4746c3aabf12c64b04a835~mv2.png/v1/fit/w_578,h_360,al_c,q_80/file.png)
