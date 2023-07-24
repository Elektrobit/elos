// SPDX-License-Identifier: MIT

#include <getopt.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* clang-format off */
#include <openssl/ssl.h>
#include <auth-client.h>
#include <libesmtp.h>
/* clang-format on */

#include "elos/event/event.h"
#include "elos/event/event_vector.h"
#include "elos/libelos/libelos.h"

#define ELOS_NOTIFICATION_SUBJECT "Elos Event Notification"
#define ELOS_NOTIFICATION_MESSAGE "Received %s event from elos."

const char *elosProgramName;

static bool elosVerboseFlag;

typedef struct elosNotificationConfig {
    char *host;
    char *sender;
    char *username;
    char *password;
    char *subject;
    char *message;
    size_t numRecipients;
    char **recipients;
    size_t numFilters;
    char **filters;
} elosNotificationConfig_t;

static volatile bool elosSigintSeen = false;
static void elosSigintHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        elosSigintSeen = true;
    }
}

static int elosAuthenticate(auth_client_request_t request, char **result, int fields, void *arg) {
    elosNotificationConfig_t *config = arg;

    for (int i = 0; i < fields; i++) {
        if (request[i].flags & AUTH_USER) {
            result[i] = config->username;
        } else if (request[i].flags & AUTH_PASS) {
            result[i] = config->password;
        } else {
            return 0;
        }
    }

    return 1;
}

static void elosEventCb(UNUSED smtp_session_t session, int event, void *arg, ...) {
    va_list argumentList;
    int *status;

    va_start(argumentList, arg);
    switch (event) {
        case SMTP_EV_STARTTLS_OK:
            printf("TLS connection started\n");
            break;
        case SMTP_EV_NO_CLIENT_CERTIFICATE: {
            status = va_arg(argumentList, int *);
            *status = 1;
            printf("Accept connection without client certificate\n");
            break;
        }
        case SMTP_EV_WRONG_PEER_CERTIFICATE: {
            status = va_arg(argumentList, int *);
            *status = 1;
            printf("Accept connection with wrong peer certificate\n");
            break;
        }
        case SMTP_EV_NO_PEER_CERTIFICATE: {
            status = va_arg(argumentList, int *);
            *status = 1;
            printf("Accept connection with connection with  missing peer certificate\n");
            break;
        }
        case SMTP_EV_WEAK_CIPHER: {
            int cypherBits = va_arg(argumentList, long);
            status = va_arg(argumentList, int *);
            *status = 1;
            printf("Accepted weak cipher with %d bits\n", cypherBits);
            break;
        }
        default:
            printf("Ignore event %d: not implemented\n", event);
    }
    va_end(argumentList);
}

static void elosMonitorCb(const char *buf, int len, int writing, UNUSED void *arg) {
    if (writing == SMTP_CB_HEADERS) {
        printf("H: %.*s", len, buf);
        return;
    }

    printf("%c: %.*s", writing ? 'C' : 'S', len, buf);

    if (buf[len - 1] != '\n') puts("\n");
}

static void elosRecipientStatus(smtp_recipient_t recipient, const char *mailbox, UNUSED void *arg) {
    const smtp_status_t *status;

    status = smtp_recipient_status(recipient);
    if (status->text[strlen(status->text) - 2] == '\r') status->text[strlen(status->text) - 2] = 0;

    printf("Mail notification sent to %s: %d %s\n", mailbox, status->code, status->text);
}

static safuResultE_t elosSendNotification(elosNotificationConfig_t *config, elosEvent_t *event) {
    safuResultE_t res = SAFU_RESULT_OK;
    smtp_session_t smtpSession = NULL;
    smtp_message_t smtpMessage = NULL;
    auth_context_t authCtx = NULL;
    const smtp_status_t *status;

    auth_client_init();

    smtpSession = smtp_create_session();
    if (smtpSession == NULL) {
        fprintf(stderr, "Creating SMTP session failed.\n");
        goto error;
    }

    if (config->username != NULL && config->password != NULL) {
        authCtx = auth_create_context();

        if (auth_set_mechanism_flags(authCtx, AUTH_PLUGIN_PLAIN, 0) == 0) {
            fprintf(stderr, "Changing authentication mechanism to plain failed.\n");
            goto error;
        }

        if (auth_set_interact_cb(authCtx, elosAuthenticate, (void *)config) == 0) {
            fprintf(stderr, "Setting authentication callback failed.\n");
            goto error;
        }

        if (smtp_auth_set_context(smtpSession, authCtx) == 0) {
            fprintf(stderr, "Registering authentication context failed.");
            goto error;
        }
    }

    smtpMessage = smtp_add_message(smtpSession);
    if (smtpMessage == NULL) {
        fprintf(stderr, "Creating SMTP message handle failed.\n");
        goto error;
    }

    if (elosVerboseFlag) {
        if (smtp_set_monitorcb(smtpSession, elosMonitorCb, stdout, 1) == 0) {
            fprintf(stderr, "Registering SMTP monitor callback failed.\n");
            goto error;
        }

        if (smtp_set_eventcb(smtpSession, elosEventCb, NULL) == 0) {
            fprintf(stderr, "Registering SMTP event callback failed.\n");
            goto error;
        }
    }

    /* Set reverse path (sender) */
    if (smtp_set_reverse_path(smtpMessage, config->sender) == 0) {
        fprintf(stderr, "Setting reverse path (sender) failed.\n");
        goto error;
    }

    /* Set recipient - This will also set the To header */
    for (size_t i = 0; i < config->numRecipients; i++) {
        if (smtp_add_recipient(smtpMessage, config->recipients[i]) == NULL) {
            fprintf(stderr, "Setting message recipient failed.\n");
            goto error;
        }
    }

    /* Specify the smtp server and port */
    if (smtp_set_server(smtpSession, config->host) == 0) {
        fprintf(stderr, "Setting smtp server failed.\n");
        goto error;
    }

    /* Set message subject */
    if (smtp_set_header(smtpMessage, "Subject", config->subject) == 0) {
        fprintf(stderr, "Setting subject header failed.\n");
        goto error;
    }

    if (smtp_set_header_option(smtpMessage, "Subject", Hdr_OVERRIDE, 1) == 0) {
        fprintf(stderr, "Setting subject header override failed.\n");
        goto error;
    }

    char msgText[2048];
    snprintf(msgText, 2048, config->message, event->source.appName);

    char msgBuf[4096];
    snprintf(msgBuf, 4096,
             "MIME-Version: 1.0\r\n"
             "Content-Type: text/plain; charset=\"US-ASCII\"\r\n"
             "Content-Transfer-Encoding: 8bit\r\n"
             "Subject: %s\r\n"
             "\r\n"
             "%s"
             "\r\n",
             config->subject, msgText);

    smtp_set_message_str(smtpMessage, msgBuf);

    if (smtp_start_session(smtpSession) == 0) {
        char errbuf[128];
        fprintf(stderr, "SMTP server problem: %s\n", smtp_strerror(smtp_errno(), errbuf, sizeof(errbuf)));
    } else {
        status = smtp_message_transfer_status(smtpMessage);
        printf("%d %s", status->code, (status->text != NULL) ? status->text : "\n");

        if (elosVerboseFlag) {
            /* Show a message for each sent mail. */
            smtp_enumerate_recipients(smtpMessage, elosRecipientStatus, NULL);
        }
    }

    goto cleanup;

error:
    res = SAFU_RESULT_FAILED;

cleanup:
    /* Free resources */
    if (authCtx != NULL) {
        auth_destroy_context(authCtx);
        authCtx = NULL;
    }

    if (smtpSession != NULL) {
        smtp_destroy_session(smtpSession);
        smtpSession = NULL;
    }

    if (config->username != NULL && config->password != NULL) {
        auth_client_exit();
    }

    return res;
}

static void elosPrintHelp(FILE *stream, int exitCode) {
    fprintf(stream, "Usage: %s [OPTION]...\n", elosProgramName);
    fprintf(stream,
            "  -h, --help              Display this usage information.\n"
            "  -v, --verbose           Print verbose messages.\n"
            "  -H, --host              Specifies the sending server in form <host>[:<port>] (default port 587).\n"
            "  -s, --sender            Sending mail address (FROM header).\n"
            "  -r, --recipient         Adds a new recipient e-mail address.\n"
            "  -f, --filter            Specifies an elos event filter to be added.\n"
            "  -a, --authentication    Specifies data needed for authentication with smtp server\n"
            "                          in form of <username>:<password> (optional).\n\n"
            "The options for filters (-f) and recipients (-r) can be used more than once\n"
            "in order to add more than one filter for events to be monitored\n"
            "or recipient to get informed of occurring system events.\n"
            "However at least one filter and recipient has to be specified.\n\n"
            "Example:\n"
            "  %s -H mailer.example.com:25 \\\n"
            "     -s noreply-elos@example.com \\\n"
            "     -f \".event.source.appName 'login' STRCMP\" \\\n"
            "     -r recipient1@mailer.com \\\n"
            "     -r recipient2@example.com\n",
            elosProgramName);
    exit(exitCode);
}

static safuResultE_t elosInitConfig(int argc, char *argv[], elosNotificationConfig_t *config) {
    safuResultE_t result = SAFU_RESULT_OK;
    int c;

    elosProgramName = argv[0];

    while (result == SAFU_RESULT_OK) {
        int optionIndex = 0;
        /* clang-format off */
        static struct option longOptions[] = {
            { "help",           0,                 0, 'h' },
            { "verbose",        0,                 0, 'v' },
            { "host",           required_argument, 0, 'H' },
            { "sender",         required_argument, 0, 's' },
            { "recipient",      required_argument, 0, 'r' },
            { "filter",         required_argument, 0, 'f' },
            { "authentication", required_argument, 0, 'a' },
            { 0,                0,                 0,  0  }
        };
        /* clang-format on */

        c = getopt_long(argc, argv, "hvH:s:r:f:a:", longOptions, &optionIndex);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'h':
                elosPrintHelp(stdout, 0);
                break;
            case 'v':
                elosVerboseFlag = true;
                break;
            case 'H':
                /* Set smtp server and service */
                config->host = strdup(optarg);
                break;
            case 's':
                /* Set smtp server and service */
                config->sender = strdup(optarg);
                break;
            case 'a': {
                /* Set smtp authentication */
                char *password = strchr(optarg, ':');
                if (password != NULL) {
                    config->username = strndup(optarg, optarg - password);
                    config->password = strdup(++password);
                } else {
                    fprintf(stderr, "ERROR: Reading smtp authentication data failed.");
                    result = SAFU_RESULT_FAILED;
                }
                break;
            }
            case 'r':
                config->numRecipients++;
                config->recipients = realloc(config->recipients, config->numRecipients * sizeof(*config->recipients));
                if (config->recipients == NULL) {
                    fprintf(stderr, "ERROR: Resizing of recipient list failed.");
                    result = SAFU_RESULT_FAILED;
                } else {
                    config->recipients[config->numRecipients - 1] = strdup(optarg);
                }
                break;
            case 'f':
                config->numFilters++;
                config->filters = realloc(config->filters, config->numFilters * sizeof(*config->filters));
                if (config->filters == NULL) {
                    fprintf(stderr, "ERROR: Resizing of recipient list failed.");
                    result = SAFU_RESULT_FAILED;
                } else {
                    config->filters[config->numFilters - 1] = strdup(optarg);
                }
                break;
            case -1: /* Done with options.  */
                break;
            case '?':
                elosPrintHelp(stderr, 1);
                return 1;
            default:
                fprintf(stderr, "ERROR: Unkown error during commandline parsing.\n");
                elosPrintHelp(stderr, 1);
        }
    }

    if (config->host == NULL) {
        fprintf(stderr, "ERROR: No SMTP server given to use as sending host.\n");
        elosPrintHelp(stderr, 1);
    }

    if (config->sender == NULL) {
        fprintf(stderr, "ERROR: No sending e-mail address specified.\n");
        elosPrintHelp(stderr, 1);
    }

    if (config->numFilters == 0 || config->numRecipients == 0) {
        fprintf(stderr, "ERROR: %s needs at least one filter and one recipient to be specified.\n", elosProgramName);
        elosPrintHelp(stderr, 1);
    }

    return result;
}

static void elosDeleteConfig(elosNotificationConfig_t *config) {
    for (size_t i = 0; i < config->numRecipients; i++) {
        free(config->recipients[i]);
    }

    free(config->recipients);
    config->recipients = NULL;
    config->numRecipients = 0;

    for (size_t i = 0; i < config->numFilters; i++) {
        free(config->filters[i]);
    }

    free(config->filters);
    config->filters = NULL;
    config->numFilters = 0;

    free(config->host);
    free(config->sender);
    free(config->username);
    free(config->password);
}

int main(int argc, char *argv[]) {
    safuResultE_t res = SAFU_RESULT_OK;

    elosEvent_t *event;
    elosSession_t *session = NULL;
    elosEventVector_t *eventVector = NULL;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;

    elosNotificationConfig_t config = {.subject = ELOS_NOTIFICATION_SUBJECT, .message = ELOS_NOTIFICATION_MESSAGE};

    signal(SIGINT, elosSigintHandler);
    signal(SIGTERM, elosSigintHandler);

    res = elosInitConfig(argc, argv, &config);
    if (res != SAFU_RESULT_OK) {
        fprintf(stderr, "ERROR: Initializing configuration failed!\n");
        return SAFU_RESULT_FAILED;
    }

    /* Connect to elos service */
    res = elosConnectTcpip("127.0.0.1", 54321, &session);
    if (res == SAFU_RESULT_OK) {
        /* Subscribe for ssh connection events */
        res = elosEventSubscribe(session, (const char **)config.filters, config.numFilters, &eventQueueId);
        if (res == SAFU_RESULT_OK) {
            printf("Polling for new events...\n");
            while (elosSigintSeen == false) {
                /* Read events from elos event queue */
                res = elosEventQueueRead(session, eventQueueId, &eventVector);

                if (res == SAFU_RESULT_OK && eventVector) {
                    event = safuVecGetLast(eventVector);
                    if (event != NULL) {
                        printf("Received login event, sending notification ...\n");
                        elosSendNotification(&config, event);
                    } else {
                        fprintf(stderr, "ERROR: Extracting event from queue failed!\n");
                        res = SAFU_RESULT_FAILED;
                    }

                    elosEventVectorDelete(eventVector);
                    eventVector = NULL;
                } else if (res != SAFU_RESULT_OK) {
                    fprintf(stderr, "ERROR: Reading from event queue failed!\n");
                }

                usleep(200000);
            }
        } else {
            fprintf(stderr, "ERROR: Subscription to event queue failed!\n");
        }

        printf("Disconnecting ...\n");
        res = elosDisconnect(session);
        if (res != SAFU_RESULT_OK) {
            fprintf(stderr, "ERROR: Disconnecting from elosd failed!\n");
        }
    } else {
        fprintf(stderr, "ERROR: Connection to elosd failed!\n");
    }

    elosDeleteConfig(&config);

    return res;
}
