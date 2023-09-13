Instructions to run:

1. Build (If Necessary)

gcc -o server server.c
gcc -o client client.c

2. On one terminal run:

./server

3. On other terminals run (by default connects to 127.0.0.1:8080 which server is automatically set to):

./client -i <server_ip> -p <server_port>

4. To modify can increase number of clients from macro in server.c