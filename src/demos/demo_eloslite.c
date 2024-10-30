// SPDX-License-Identifier: MIT

#include <elos/libelos_lite.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
    const char *host = "127.0.0.1";
    uint16_t port = 54321;
    switch (argc) {
        case 3:
            port = strtol(argv[2], NULL, 10);
            __attribute__((fallthrough));
        case 2:
            host = argv[1];
    }
    printf("Libelos lite version: %s\n", elosliteGetLibraryVersion());

    printf("connecting to %s:%d\n", host, port);
    elosliteSession_t session = {0};
    if (elosliteConnectTcpip(host, port, &session)) {
        printf("connected!\n");
    } else {
        printf("connection failed!\n");
    }

    elosliteEvent_t event = {
        .date = {42,0},
        .source = {
            .appName = "demo_eloslite",
            .fileName = "/usr/local/bin/demo_eloslite",
            .pid = 208,
        },
        .severity = 1,
        .hardwareid = "817d6b97-75f8-4faf-ba3c-583ae1123558",
        .classification = 6,
        .messageCode = 8000,
        .payload = "...PAYLOAD...",
    };

    if (eloslitePublish(&session, &event)) {
        printf("published!\n");
    } else {
        printf("publishing failed!\n");
    }

    if (elosliteDisconnect(&session)) {
        printf("disconnected!\n");
    } else {
        printf("disconnecting failed!\n");
    }
    return 0;
}
