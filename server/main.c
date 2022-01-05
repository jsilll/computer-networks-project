#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <signal.h>

#include "parsing.h"
#include "connection.h"
#include "command_handling.h"

#define DEFAULT_PORT "58006"

#define max(x, y) (x > y ? x : y)

char PORT[MAX_INPUT_SIZE];
bool VERBOSE = false;

fd_set rset;
int listenfd, udpfd;

void signal_handler(int signal_num);
void loadInitArgs(int argc, char *argv[]);
void readCommand();

int main(int argc, char *argv[])
{
  signal(SIGTERM, signal_handler);

  strcpy(PORT, DEFAULT_PORT);
  loadInitArgs(argc, argv);

  printf("Centralized Messaging Server\n");
  printf("PORT %s VERBOSE %s\n", PORT, VERBOSE ? "On" : "Off");

  setupAddresses(PORT);
  listenfd = openSocket(SOCK_STREAM);
  if (listen(listenfd, 5) < 0)
  {
    fprintf(stderr, "Error on listen()\n");
    exit(EXIT_FAILURE);
  }
  udpfd = openSocket(SOCK_DGRAM);

  FD_ZERO(&rset);
  for (;;)
  {
    readCommand();
  }
}

/**
 * Signal Handler for terminating the server
 * @param signal_num
 */
void signal_handler(int signal_num)
{
  close(listenfd);
  close(udpfd);
  exit(signal_num);
}

/**
 * Parses the initial arguments for the program.
 * @param argc number of arguments in argv
 * @param argv array passed arguments
 */
void loadInitArgs(int argc, char *argv[])
{
  int opt;
  while ((opt = getopt(argc, argv, ":vp:")) != -1)
  {
    switch (opt)
    {
    case 'p':
      parsePortArg(optarg);
      strcpy(PORT, optarg);
      break;
    case 'v':
      VERBOSE = true;
      break;
    case ':':
      fprintf(stderr, "Missing argument for port (-p) option\n");
      exit(EXIT_FAILURE);
    case '?':
      fprintf(stderr, "Unknown option: -%c\n", optopt);
      exit(EXIT_FAILURE);
    default:
      fprintf(stderr, "Unknown error\n");
      exit(EXIT_FAILURE);
    }
  }

  if (optind < argc)
  {
    fprintf(stderr, "Unnecessary extra argument: %s\n", argv[optind]);
    exit(EXIT_FAILURE);
  }
}

/**
 * Reads commands from udp and tcp clients
 */
void readCommand()
{
  FD_SET(listenfd, &rset);
  FD_SET(udpfd, &rset);
  select(max(listenfd, udpfd) + 1, &rset, NULL, NULL, NULL);

  struct sockaddr_in cliaddr;
  if (FD_ISSET(listenfd, &rset))
  {
    socklen_t len = sizeof(cliaddr);
    int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);

    if (fork() == 0)
    {

      // struct timeval tmout;
      // memset((char *)&tmout, 0, sizeof(tmout));
      // tmout.tv_sec = 15;
      // if (setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tmout, sizeof(struct timeval)) < 0)
      // {
      //   fprintf(stderr, "setsockopt(SO_RCVTIMEO) failed.\n");
      //   exit(EXIT_FAILURE);
      // }

      // memset((char *)&tmout, 0, sizeof(tmout));
      // tmout.tv_sec = 15;
      // if (setsockopt(connfd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tmout, sizeof(struct timeval)) < 0)
      // {
      //   fprintf(stderr, "setsockopt(SO_RCVTIMEO) failed.\n");
      //   exit(EXIT_FAILURE);
      // }

      close(listenfd);
      close(udpfd);
      if (VERBOSE)
      {
        printf("[TCP] IP: %s PORT: %lu ", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
      }
      handleTCPCommand(connfd, VERBOSE);
      close(connfd);
      exit(EXIT_SUCCESS);
    }

    close(connfd);
  }

  if (FD_ISSET(udpfd, &rset))
  {
    handleCommandUDP(udpfd, cliaddr, VERBOSE);
  }
}