#ifndef INTERFACE_H_
#define INTERFACE_H_

/* Commands */

#define CMD_REGISTER "reg"
#define CMD_UNREGISTER "unregister"
#define CMD_UNREGISTER_SHORT "unr"
#define CMD_LOGIN "login"
#define CMD_LOGOUT "logout"
#define CMD_SHOW_UID "showuid"
#define CMD_SHOW_UID_SHORT "su"
#define CMD_EXIT "exit"
#define CMD_GROUPS "groups"
#define CMD_GROUPS_SHORT "gl"
#define CMD_SUBSCRIBE "subscribe"
#define CMD_SUBSCRIBE_SHORT "s"
#define CMD_UNSUBSCRIBE "unsubscribe"
#define CMD_UNSUBSCRIBE_SHORT "u"
#define CMD_MY_GROUPS "my_groups"
#define CMD_MY_GROUPS_SHORT "mgl"
#define CMD_SELECT "select"
#define CMD_SELECT_SHORT "sag"
#define CMD_SHOW_GID "showgid"
#define CMD_SHOW_GID_SHORT "sg"
#define CMD_ULIST "ulist"
#define CMD_ULIST_SHORT "ul"
#define CMD_POST "post"
#define CMD_RETRIEVE "retrieve"
#define CMD_RETRIEVE_SHORT "r"

/* Info Messages */

#define INFO_LOCAL_GID "[LOCAL] Currently selected group ID: %02d\n"
#define INFO_LOCAL_SELECTED_GID "[LOCAL] group ID %02d is now selected.\n"
#define INFO_LOCAL_UID "[LOCAL] Currently selected user ID: %05d\n"

#define INFO_EXITING "Exiting Centralized Messaging Client.\n"
#define INFO_LOGGED_IN "User logged in successfully.\n"
#define INFO_LOGGED_OUT "Logged out successfully.\n"
#define INFO_NEW_GROUP "New group created: %02d.\n"
#define INFO_NOT_IP "Address isn't a valid IP, checking if it's a valid address.\n"
#define INFO_POSTED "Posted in group %02d successfully, message ID is %04d.\n"
#define INFO_REGISTERED "User registered successfully.\n"
#define INFO_RETRIEVING "Retrieving %d message(s) from group %02d.\n"
#define INFO_SUBSCRIBED "Subscribed to group successfully.\n"
#define INFO_UNREGISTERED "User unregistered successfully.\n"
#define INFO_UNSUBSCRIBED "Unsubscribed from group %02d successfully.\n"

/* Error Messages */

#define ERR_LOCAL_FILE_NOT_FOUND "[LOCAL] File does not exist.\n"
#define ERR_LOCAL_GID_NOT_SELECTED "[LOCAL] Group ID not selected.\n"
#define ERR_LOCAL_INVALID_FNAME "[LOCAL] Invalid filename.\n"
#define ERR_LOCAL_INVALID_GID "[LOCAL] Invalid group ID.\n"
#define ERR_LOCAL_INVALID_GNAME "[LOCAL] Invalid group name.\n"
#define ERR_LOCAL_INVALID_MID "[LOCAL] Invalid message ID.\n"
#define ERR_LOCAL_INVALID_PASSWD "[LOCAL] Invalid password.\n"
#define ERR_LOCAL_INVALID_POST_CMD "[LOCAL] Invalid post command.\n"
#define ERR_LOCAL_INVALID_TXT_MSG "[LOCAL] Message exceeds 240 characters.\n"
#define ERR_LOCAL_INVALID_UID "[LOCAL] Invalid user ID.\n"
#define ERR_LOCAL_UNKNOWN_CMD "[LOCAL] Unknown command.\n"
#define ERR_LOCAL_USER_NEEDS_TO_LOGIN "[LOCAL] User needs to be logged in.\n"
#define ERR_LOCAL_USER_NEEDS_TO_LOGOUT "[LOCAL] User needs to logout first.\n"

#define ERR_CONNECT "connect(ADDR_TCP) failed.\n"
#define ERR_GETADDRINFO_TCP "Error on getaddrinfo (tcp): %s.\n"
#define ERR_GETADDRINFO_UDP "Error on getaddrinfo (udp): %s.\n"
#define ERR_INVALID_PORT "Invalid value for port argument.\n"
#define ERR_RCV_SERVER_RESPONSE "Error receiving server's response.\n"
#define ERR_RECVFROM "recvfrom(RESPONSE_BUFFER) failed.\n"
#define ERR_SEND_CMD_BUF "Couldn't send command_buffer.\n"
#define ERR_SENDTO "sendto(COMMAND_BUFFER) failed.\n"
#define ERR_SETSOCKOPT "setsockopt(SO_RCVTIMEO) failed.\n"
#define ERR_SOCK_TCP "Error creating TCP socket.\n"
#define ERR_SOCK_UDP "Error creating UDP socket.\n"

#define ERR_GROUP_DOESNT_EXIST "Group %02d doesn't exist.\n"
#define ERR_INVALID_GID "Invalid group ID.\n"
#define ERR_INVALID_GNAME "Invalid group name.\n"
#define ERR_INVALID_MID "Invalid message ID.\n"
#define ERR_INVALID_PASSWD "Invalid password.\n"
#define ERR_INVALID_UID "Invalid user ID.\n"
#define ERR_INVALID_UID_OR_PASSWD "Invalid user ID and/or password.\n"
#define ERR_INVALID_UID_OR_PASSWD_OR_DOESNT_EXIST "Invalid user ID and/or password, or user doesn't exist.\n"
#define ERR_MAX_GROUPS "Maximum number of groups reached.\n"
#define ERR_POST_IN_GROUP "Couldn't post in group %02d\n"
#define ERR_RETRIEVE_EOF "There's no available messages to display starting from message ID %04d\n"
#define ERR_RETRIEVE_NOK "Couldn't retrieve message starting from message ID %04d from group %02d.\n"
#define ERR_SEND_FILE "Couldn't send file: %s.\n"
#define ERR_SEND_FILE_FINISH "Couldn't send '\\n' after file: %s.\n"
#define ERR_SUBSCRIBE "Couldn't subscribe to group %02d.\n"
#define ERR_USER_REGISTERED "User already registered.\n"

#endif // INTERFACE_H_