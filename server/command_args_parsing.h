#ifndef CENTRALIZED_MESSAGING_PARSING_H
#define CENTRALIZED_MESSAGING_PARSING_H

#include <regex.h>
#include <stdlib.h>
#include <string.h>

#define T_SIZE 240

/**
 * @brief Parses a string representing a group ID.
 * 2-digit number (01 – 99)
 *
 * @param gid string that represents a group ID
 * @return -1 on errors, 0 otherwise 
 */
int parseGID(char *gid) // TODO aceita o "00" em casos que comandos que nao aceitam 00
{
  long gid_parsed = strtol(gid, NULL, 10);
  if (!strcmp(gid, "00") || (strlen(gid) == 2 && gid_parsed != 0))
    return 0;
  return -1;
}

/**
 * @brief Parses a string representing a user ID.
 * 5-digit IST student number
 *
 * @param uid string that represents a user ID
 * @return -1 on errors, 0 otherwise 
 */
int parseUID(char *uid)
{
  if (strlen(uid) != 5)
    return -1;
  int uid_parsed = strtol(uid, NULL, 10);
  if (!uid_parsed && strcmp(uid, "00000") != 0)
    return -1;
  return uid_parsed;
}

/**
 * @brief Parses a string representing a password.
 * 8 alphanumerical characters, restricted to
 * letters and numbers
 *
 * @param password string that represents a password
 * @return -1 on errors, 0 otherwise 
 */
int parsePassword(char *password)
{
  regex_t re; // TODO make this const
  if (regcomp(&re, "^[a-zA-Z0-9]{8}$", REG_EXTENDED | REG_NOSUB) != 0 || regexec(&re, password, 0, NULL, 0) != 0)
  {
    regfree(&re);
    return -1;
  }
  regfree(&re);
  return 0;
}

/**
 * @brief Parses a string representing a group name.
 * limited to a total of 24 alphanumerical characters
 * (plus ‘-‘, and ‘_’)
 *
 * @param gname string that represents a group name
 * @return -1 on errors, 0 otherwise 
 */
int parseGName(char *gname)
{
  regex_t re; // TODO make this const
  if (regcomp(&re, "^[a-zA-Z0-9_-]{1,24}$", REG_EXTENDED | REG_NOSUB) != 0 || regexec(&re, gname, 0, NULL, 0) != 0)
  {
    regfree(&re);
    return -1;
  }
  regfree(&re);
  return 0;
}

/**
 * @brief Parses a string representing a message ID.
 * 4-digit number
 *
 * @param mid string that represents a message ID
 * @return -1 on errors, 0 otherwise 
 */
int parseMID(char *mid)
{
  if (strlen(mid) != 4 || strtol(mid, NULL, 10) <= 0)
    return -1;
  return 0;
}

/**
 * @brief Parses Tsize argument
 * limited to a maximum of 3 characters
 * must be in [1, 240]
 * 
 * @param tsize string representing the text message size
 * @return -1 on errors, 0 otherwise  
 */
int parseTSize(char *tsize)
{
  int size = atoi(tsize);
  if (strlen(tsize) > 3 || size > 240 || size <= 0)
  {
    return -1;
  }
  return 0;
}

/**
 * @brief Parses the file size
 * 
 * @param size string representing the file size
 * @return -1 on errors, 0 otherwise 
 */
int parseFileSize(char *size)
{
  if (strlen(size) > 10)
    return -1;
  return 0;
}

/**
 * @brief Parses a string representing a filename.
 * limited to a maximum of 24 alphanumerical characters
 * (and ‘-‘, ‘_’ and ‘.’), including the separating dot and
 * the 3-letter extension
 *
 * @param fname string that represents a filename
 * @return -1 on errors, 0 otherwise 
 */
int parseFName(char *fname)
{
  regex_t re; // TODO make this const
  if (strlen(fname) > 24)
    return -1;
  if (regcomp(&re, "^[a-zA-Z0-9_.-]+[.]{1}[a-zA-Z0-9]{3}$", REG_EXTENDED | REG_NOSUB) != 0 || regexec(&re, fname, 0, NULL, 0) != 0)
  {
    regfree(&re);
    return -1;
  }
  regfree(&re);
  return 0;
}

#endif //CENTRALIZED_MESSAGING_PARSING_H