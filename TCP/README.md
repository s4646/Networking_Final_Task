# How to use
### TCP Client-Server
**You will need _2 terminals_ for execution.**
1. Open a new terminal and type:
```
make all
./server
```
2. Open a new terminal and type:
```
./client HOSTNAME
```
   For instance:
```
pop@pop-os:~/Documents/Networking_Final_Task/TCP$ ./client pop-os
Client is alive and establishing socket connection.
Client has received 1 from socket.
...
...
```
***Output should appear in both terminals terminal where sink program is executed.***
### Web Client
Open a new terminal and type:
```
make web_client
./web_client http://<PROTOCOL>:<PORT>/<PATH>
```
This can work without specifying a port, default port is 80.  
For instance:
```
./web_client http://httpforever.com
```
***Output should appear in terminal***
