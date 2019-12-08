#include "utils.h"
#include "version.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define CONFIG_FILE "./cear.conf"
#define SERVER_PORT 4000
#define MAX_CLIENTS 30
#define MAX_REQUEST_SIZE 8192

int main(int argc, char const *argv[]) {
  signal(SIGPIPE, SIG_IGN);

  printf("starting cear %s\n", VERSION);

  printf("reading cear config file\n");
  FILE *cnfPtr;
  cnfPtr = fopen(CONFIG_FILE, "r");

  if (cnfPtr == NULL) {
    perror("cear.db is not defined");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  int n;
  char c;
  char *str;

  char *reply = "HTTP/1.1 200 OK\n"
                "Content-Type: text/html\n"
                "Content-Length: 8\n"
                "\n"
                "executed";

  fseek(cnfPtr, 0L, SEEK_END);
  long cnfSize = ftell(cnfPtr);
  rewind(cnfPtr);
  char *cnf;
  cnf = malloc(sizeof *cnf * cnfSize);
  memset(cnf, '\0', sizeof *cnf * cnfSize);

  while (c != EOF) {
    c = fgetc(cnfPtr);
    cnf[i] = c;

    if (c == EOF) {
      cnf[i] = '\0';
    }

    i++;
  }
  fclose(cnfPtr);

  i = 0;
  struct sockaddr_in serverAddress;
  int serverAddrLen = sizeof(serverAddress);
  int serverFd;

  if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("server socket");
    exit(EXIT_FAILURE);
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(SERVER_PORT);
  memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);

  if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) <
      0) {
    perror("server binding");
    exit(EXIT_FAILURE);
  }

  if (listen(serverFd, MAX_CLIENTS) < 0) {
    perror("server listening");
    exit(EXIT_FAILURE);
  }

  while (1) {
    int clientFd;

    printf("waiting for client connection\n");
    if ((clientFd = accept(serverFd, (struct sockaddr *)&serverAddress,
                           (socklen_t *)&serverAddrLen)) < 0) {
      perror("client connection accept");
      exit(EXIT_FAILURE);
    }

    printf("client-server connection established\n");

    char request[MAX_REQUEST_SIZE];
    while ((n = read(clientFd, request, MAX_REQUEST_SIZE)) > 0) {
      // printf("%s", request);
      if (n < MAX_REQUEST_SIZE) {
        break;
      }
    }

    char *body;
    body = strtok(request, "\n");

    while (body != NULL) {
      body = strtok(NULL, "\n");
      if (body[0] == 13) {
        body = strtok(NULL, "\n");
        break;
      }
    }

    char *command = concat(concat(body, " "), cnf);
    printf("executing command %s\n", command);
    system(command);

    free(command);

    send(clientFd, reply, strlen(reply), 0);
    close(clientFd);
  }

  printf("closing cear\n");

  return 0;
}
