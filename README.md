# Daemonized TCP server

Just a simple TCP server that was daemonized with the Systemd standards. 

Modern services for Linux should be implemented as Systemd-style daemons. For developing a new-style daemon, none of the initialization steps recommended for SysV daemons need to be implemented. 

The server was built with a Linux TCP socket with a multi-thread approach. Each client has his thread and they can send a message to the server so that the server will send the message back.

## How to install?

In order to compile and install the files, you must run:

```
mkdir build
cd build
cmake ../
make
sudo make install
```

## How to start?

The next step is to start your daemon:

```
sudo systemctl daemon-reload
sudo systemctl start TCPserver
sudo systemctl status TCPserver
```

To remove the daemon, you must run:

```
sudo systemctl stop TCPserver
```

To see the syslog:

```
tail -f /var/log/syslog
```

To run the client, you must run:

```
gcc client.c -o client
./client 127.0.0.1 3000 
```
