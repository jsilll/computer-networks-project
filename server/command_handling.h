#ifndef RC_PROJECT_SERVER_COMMAND_HANDLING_H_
#define RC_PROJECT_SERVER_COMMAND_HANDLING_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <errno.h>
#include <dirent.h>
#include "command_args_parsing.h"
#include "state/operations.h"

/**
 * @brief 
 * 
 * @param udpfd 
 * @param cliaddr 
 * @param verbose 
 */
void handleCommandUDP(int udpfd, struct sockaddr_in cliaddr, bool verbose)
{
  socklen_t len = sizeof(cliaddr);
  char command_buffer[33], response_buffer[3274];

  bzero(command_buffer, sizeof(command_buffer));
  if (recvfrom(udpfd, command_buffer, sizeof(command_buffer), 0, (struct sockaddr *)&cliaddr, &len) == -1 && verbose)
  {
    printf("?\n");
    fprintf(stderr, "[ERROR] recvfrom(udpfd)\n");
    return;
  }

  if (verbose)
  {
    printf("CMD: %s", command_buffer);
  }

  char op[5], uid[7], arg2[10], gname[26], arg4[2];
  int numTokens = sscanf(command_buffer, "%4s %6s %9s %25s %1s", op, uid, arg2, gname, arg4);
  if (numTokens > 4)
  {
    strcpy(response_buffer, "ERR\n");
  }
  else if (!strcmp(op, "REG"))
  {
    if (parseUID(uid) == -1 || parsePassword(arg2) == -1)
    {
      strcpy(response_buffer, "RRG NOK\n");
    }
    else if (registerUser(uid, arg2) == -1)
    {
      strcpy(response_buffer, "RRG DUP\n");
    }
    else
    {
      strcpy(response_buffer, "RRG OK\n");
    }
  }
  else if (!strcmp(op, "UNR"))
  {
    if (parseUID(uid) == -1 || parsePassword(arg2) == -1 || unregisterUser(uid, arg2) == -1)
    {
      strcpy(response_buffer, "RUN NOK\n");
    }
    else
    {
      strcpy(response_buffer, "RUN OK\n");
    }
  }
  else if (!strcmp(op, "LOG"))
  {
    if (parseUID(uid) == -1 || parsePassword(arg2) == -1 || login(uid, arg2) == -1)
    {
      strcpy(response_buffer, "RLO NOK\n");
    }
    else
    {
      strcpy(response_buffer, "RLO OK\n");
    }
  }
  else if (!strcmp(op, "OUT"))
  {
    if (parseUID(uid) == -1 || parsePassword(arg2) == -1 || logout(uid, arg2) == -1)
    {
      strcpy(response_buffer, "ROU NOK\n");
    }
    else
    {
      strcpy(response_buffer, "ROU OK\n");
    }
  }
  else if (!strcmp(op, "GLS"))
  {
    groups(response_buffer);
  }
  else if (!strcmp(op, "GSR"))
  {
    if (parseUID(uid) == -1)
    {
      strcpy(response_buffer, "RGS E_USR\n");
    }
    else if (parseGID(arg2) == -1)
    {
      strcpy(response_buffer, "RGS E_GRP\n");
    }
    else if (parseGName(gname) == -1)
    {
      strcpy(response_buffer, "RGS E_GNAME\n");
    }
    else
    {
      int res;
      switch (res = subscribe(uid, arg2, gname))
      {
      case 0:
        strcpy(response_buffer, "RGS OK\n");
        break;

      case -1:
        strcpy(response_buffer, "RGS NOK\n");
        break;

      case -2:
        strcpy(response_buffer, "RGS FULL\n");
        break;

      default:
        sprintf(response_buffer, "RGS NEW %02d\n", res);
        break;
      }
    }
  }
  else if (!strcmp(op, "GUR"))
  {
    if (parseUID(uid) == -1)
    {
      strcpy(response_buffer, "RGU E_USR\n");
    }
    else if (parseGID(arg2) == -1)
    {
      strcpy(response_buffer, "RGU E_GRP\n");
    }
    else if (unsubscribe(uid, arg2) == -1)
    {
      strcpy(response_buffer, "RGU NOK\n");
    }
    else
    {
      strcpy(response_buffer, "RGU OK\n");
    }
  }
  else if (!strcmp(op, "GLM"))
  {
    if (parseUID(uid) == -1 || myGroups(uid, response_buffer) == -1)
    {
      strcpy(response_buffer, "RGM E_USR\n");
    }
  }
  else
  {
    strcpy(response_buffer, "ERR\n");
  }

  sendto(udpfd, response_buffer, strlen(response_buffer), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
}

/**
 * @brief 
 * 
 * @param connfd 
 * @param verbose 
 */
void handleTCPCommand(int connfd, bool verbose)
{
  char op[5];
  bzero(op, sizeof(op));
  read(connfd, op, 4);
  if (op[3] == ' ') // ?
  {
    op[3] = '\0';
  }

  char command_buffer[36], buffer[1024];
  bzero(buffer, sizeof(buffer));
  if (!strcmp(op, "ULS"))
  {
    bzero(command_buffer, sizeof(command_buffer));
    read(connfd, command_buffer, 3);

    char gid[4];
    sscanf(command_buffer, "%3[^\n]", gid);

    if (verbose)
    {
      printf("CMD: %s %s\n", op, gid);
    }

    if (parseGID(gid) == -1)
    {
      strcpy(buffer, "RUL NOK\n");
    }
    else
    {
      DIR *dr;
      if ((dr = ulist(gid)) == NULL)
      {
        strcpy(buffer, "RUL NOK\n");
      }
      else
      {
        strcpy(buffer, "RUL OK");
        write(connfd, buffer, strlen(buffer));

        getGName(gid, buffer);
        write(connfd, buffer, strlen(buffer));

        struct dirent *de;
        bzero(buffer, strlen(buffer));
        while ((de = readdir(dr)) != NULL)
        {
          ulsAux(de, buffer);

          if (strlen(buffer) > 512)
          {
            write(connfd, buffer, strlen(buffer));
            bzero(buffer, strlen(buffer));
          }
        }
        write(connfd, buffer, strlen(buffer));
        write(connfd, "\n", 1);

        close(connfd);
        return;
      }
    }
  }
  else if (!strcmp(op, "PST"))
  {
    char uid[7], gid[4], tsize[5], text[240];

    bzero(command_buffer, sizeof(command_buffer));
    read(connfd, command_buffer, 9);

    sscanf(command_buffer, "%6s %3s", uid, gid);

    bzero(command_buffer, sizeof(command_buffer));
    read(connfd, command_buffer, 4);

    sscanf(command_buffer, "%4s", tsize);
    bzero(text, sizeof(text));
    strcpy(text, &command_buffer[strlen(tsize) + 1]);

    if (parseUID(uid) == -1 || (parseGID(gid) == -1) || parseTSize(tsize) == -1 || atoi(tsize) < strlen(text))
    {
      strcpy(buffer, "RPT NOK\n");
    }
    else
    {
      read(connfd, &text[strlen(text)], atoi(tsize) - strlen(text));

      char c, mid[5];
      read(connfd, &c, 1);
      if (c == '\n')
      {
        if (verbose)
        {
          printf("CMD: %s %s %s %s\n", op, uid, gid, tsize);
        }

        if (post(uid, gid, text, NULL, mid) == NULL)
        {
          strcpy(buffer, "RPT NOK\n");
        }
        else
        {
          sprintf(buffer, "RPT %s\n", mid);
        }
      }
      else if (c == ' ')
      {
        bzero(command_buffer, sizeof(command_buffer));
        int n = read(connfd, command_buffer, 36);

        char fname[26], fsize[12];
        sscanf(command_buffer, "%25s %11s", fname, fsize);

        if (verbose)
        {
          printf("CMD: %s %s %s %s %s %s\n", op, uid, gid, tsize, fname, fsize);
        }

        if (parseFileSize(fsize) == -1 || parseFName(fname) == -1)
        {
          strcpy(buffer, "RPT NOK\n");
        }
        else
        {
          FILE *FPtr;
          if ((FPtr = post(uid, gid, text, fname, mid)) == NULL)
          {
            strcpy(buffer, "RPT NOK\n");
          }
          else
          {
            int fbytes = atoi(fsize), fbytes_read = n - (strlen(fname) + strlen(fsize) + 2);

            if (fbytes_read > 0)
            {
              WriteToFile(FPtr, &command_buffer[strlen(fname) + strlen(fsize) + 2], fbytes_read);
            }

            bzero(buffer, sizeof(buffer));
            while ((fbytes_read < fbytes) && (n = read(connfd, buffer, (((sizeof(buffer)) < (fbytes - fbytes_read)) ? (sizeof(buffer)) : (fbytes - fbytes_read)))) > 0)
            {
              WriteToFile(FPtr, buffer, n);

              fbytes_read += n;
              bzero(buffer, sizeof(buffer));
            }
            fclose(FPtr);

            sprintf(buffer, "RPT %s\n", mid);
          }
        }
      }
      else
      {
        strcpy(buffer, "RPT NOK\n");
      }
    }
  }
  else if (!strcmp(op, "RTV"))
  {
    bzero(command_buffer, sizeof(command_buffer));
    read(connfd, command_buffer, 14);

    char uid[7], gid[4], mid[6];
    sscanf(command_buffer, "%6s %3s %5s", uid, gid, mid);

    if (verbose)
    {
      printf("CMD: %s %s %s %s\n", op, uid, gid, mid);
    }

    int n_msg = 0;
    if (parseUID(uid) == -1 || parseGID(gid) == -1 || parseMID(mid) == -1 || (n_msg = retrieve(uid, gid, mid)) == -1)
    {
      strcpy(buffer, "RRT NOK\n");
    }
    else if (n_msg == 0)
    {
      strcpy(buffer, "RRT EOF\n");
    }
    else
    {
      sprintf(buffer, "RRT OK %d", n_msg);
      write(connfd, buffer, strlen(buffer));

      int base_msg = atoi(mid);
      for (int i = 0; i < n_msg; i++)
      {
        bzero(buffer, sizeof(buffer));
        FILE *FPtr = retrieveAux(gid, base_msg + i, buffer);
        write(connfd, buffer, strlen(buffer));

        if (FPtr != NULL)
        {
          int bytes_read;
          bzero(buffer, sizeof(buffer));
          while ((bytes_read = ReadFile(FPtr, buffer, 1024)) > 0)
          {
            if (write(connfd, buffer, bytes_read) == -1)
            {
              close(connfd);
              fprintf(stderr, "Couldn't send command_buffer. Error sending command_buffer.\n");
            }
            bzero(buffer, sizeof(buffer));
          }
          fclose(FPtr);
        }
      }

      write(connfd, "\n", 1);
      close(connfd);
      return;
    }
  }
  else
  {
    strcpy(buffer, "ERR\n");
  }

  write(connfd, buffer, strlen(buffer));
  close(connfd);
}

#endif //RC_PROJECT_SERVER_COMMAND_HANDLING_H_