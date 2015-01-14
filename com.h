#ifndef COM_H
#define COM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


void error(const char*);
int client(char ip[], int portno);
int server(int portno);

#endif
