// SPDX-License-Identifier: MIT

#include <elos/libelosdlt/libelosdlt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int ret = -1;
    elosDltConnection_t dlt = {0};

    if (argc < 2) {
        printf("usage :\n %s </path/socket|/path/pipe>\n", argv[0]);
    } else {
        printf("try to connect\n");
#if 0
    dlt.socketPath = argv[1];
    if (elosDltConnectUnix(&dlt) < 0) {
      printf("Failed to connect to %s\n", dlt.socketPath);
    } else {
      const char *logMessage = "hugo hat husten";
      ret = elosDltSendControlMessage(&dlt, logMessage, strlen(logMessage));
      close(dlt.socketFd);
    }
#elif 0
        dlt.host = "::1";
        dlt.port = 3490;
        if (elosDltConnectTcp(&dlt) < 0) {
            printf("Failed to connect to %s : %d\n", dlt.host, dlt.port);
        } else {
            const char *logMessage = "hugo hat husten";
            ret = elosDltSendControlMessage(&dlt, logMessage, strlen(logMessage));
            close(dlt.socketFd);
        }
#else
        dlt.pipePath = argv[1];
        ret = elosDltConnectPipe(&dlt);
        if (ret < 0) {
            printf("Failed to connect to %s\n", dlt.pipePath);
        } else {
            ret = elosDltRegisterContext(&dlt);
            if (ret < 0) {
                printf("Failed to register app context\n");
            } else {
                ret = elosDltSendUserLog(&dlt, "hugo hat husten");
                if (ret < 0) {
                    printf("Failed to send log message\n");
                }

                ret = elosDltUnregisterContext(&dlt);
                if (ret < 0) {
                    printf("Failed to unregister app context\n");
                }
            }
        }

#endif
    }

    return ret != -1 ? EXIT_SUCCESS : EXIT_SUCCESS;
}
