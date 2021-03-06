#ifndef COMMANDS_H_
#define COMMANDS_H_

/* Server Addresses (UDP and TCP) */

int setupServerAddresses(char *ip, char *port);

/* Local commands */

void showUID();
void freeServerAddresses();
void selectGroup(int gid);
void showGID();

/* Client-Server Commands */

void registerUser(int uid, char *pass);
void unregisterUser(int uid, char *pass);
void login(int uid, char *pass);
void logout();
void groups();
void subscribe(int gid, char *gname);
void unsubscribe(int gid);
void myGroups();
void ulist();
void post(char *message, char *fname);
void retrieve(int mid);

#endif //COMMANDS_H_
