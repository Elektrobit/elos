// SPDX-License-Identifier: MIT

#include <elos/libelos_lite.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
    const char *host = "localhost";
    uint16_t port = 54321;
    switch (argc) {
        case 3:
            port = strtol(argv[2], NULL, 10);
            __attribute__((fallthrough));
        case 2:
            host = argv[1];
    }
    printf("Libelos lite version: %s\n", elosGetLibraryVersion());

    printf("connecting to %s:%d\n", host, port);
    elosSession_t session = {0};
    if (elosConnectTcpip(host, port, &session)) {
        printf("connected!\n");
    } else {
        printf("connection failed!\n");
    }
    return 0;
}
