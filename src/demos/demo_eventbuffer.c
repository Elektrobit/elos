// SPDX-License-Identifier: MIT

#include <float.h>
#include <getopt.h>
#include <safu/common.h>
#include <safu/vector.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "elos/event/event.h"
#include "elos/eventbuffer/eventbuffer.h"

// Based on timersub from time.h
#define timespecSub(a, b, result)                        \
    do {                                                 \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;    \
        (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec; \
        if ((result)->tv_nsec < 0) {                     \
            --(result)->tv_sec;                          \
            (result)->tv_nsec += 1000000;                \
        }                                                \
    } while (0)

// Used due to CLOCK_REALTIME and CLOCK_MONOTONIC sometimes generating negative timeDiffs with many threads
#define _CLOCK_ID CLOCK_THREAD_CPUTIME_ID

#define _DEFAULT_PUBLISHER_COUNT 4
#define _DEFAULT_CONSUMER_COUNT  4
#define _DEFAULT_MESSAGE_COUNT   1000
#define _DEFAULT_EVENT_LIMIT     16
#define _DEFAULT_HELPMODE        false

typedef void *_workerFunc_t(void *);

enum {
    OPTION_PUBLISHER = 0,
    OPTION_CONSUMER,
    OPTION_MESSAGECOUNT,
    OPTION_EVENTLIMIT,
    OPTION_HELPMODE,
};

typedef struct _demoTime {
    double min;
    double max;
    double avg;
    double sum;
} _demoTime_t;

typedef struct _demoUnit {
    size_t count;
    atomic_size_t processed;
} _demoUnit_t;

typedef struct _demoState {
    elosEventBuffer_t eventBuffer;
    _demoUnit_t publisher;
    _demoUnit_t consumer;
    atomic_bool unitsReady;
    atomic_size_t messagesLost;
    size_t messageCount;
    size_t eventLimit;
    bool helpMode;
} _demoState_t;

typedef struct _demoWorker {
    pthread_t thread;
    size_t idx;
    double *messageTime;
    _demoState_t *state;
} _demoWorker_t;

static int _deleteEventPtr(void const *element, UNUSED void const *data) {
    elosEventDelete(*(elosEvent_t **)element);
    return 0;
}

static void *_publisherWorkerThread(void *data) {
    _demoWorker_t *worker = (_demoWorker_t *)data;
    _demoState_t *state = worker->state;
    bool unitsReady = false;

    while (unitsReady == false) {
        unitsReady = atomic_load(&state->unitsReady);
    }

    for (size_t i = 0; i < state->messageCount; i += 1) {
        size_t const priority = (i % ELOS_EVENTBUFFER_PRIORITY_COUNT);
        struct timespec timeStart = {0};
        safuResultE_t result;
        int retVal;

        elosEvent_t event = {
            .messageCode = i,
            .severity = ELOS_SEVERITY_INFO,
            .payload = "workerTestPayload",
        };

        if (priority == ELOS_EVENTBUFFER_PRIORITY_HIGH) {
            event.severity = ELOS_SEVERITY_FATAL;
        }

        retVal = clock_gettime(_CLOCK_ID, &timeStart);
        if (retVal < 0) {
            fprintf(stderr, "clock_gettime (start) failed\n");
        } else {
            result = elosEventBufferWrite(&state->eventBuffer, &event);
            if (result != SAFU_RESULT_OK) {
                fprintf(stderr, "elosEventBufferWrite failed\n");
            } else {
                struct timespec timeStop = {0};

                retVal = clock_gettime(_CLOCK_ID, &timeStop);
                if (retVal < 0) {
                    fprintf(stderr, "clock_gettime (stop) failed\n");
                } else {
                    struct timespec timeDiff = {0};

                    timespecSub(&timeStop, &timeStart, &timeDiff);
                    worker->messageTime[i] = ((double)timeDiff.tv_sec * 1e3) + ((double)timeDiff.tv_nsec / 1e6);

                    atomic_fetch_add(&state->publisher.processed, 1);
                }
            }
        }
    }

    return NULL;
}

static void *_consumerWorkerThread(void *data) {
    _demoWorker_t *worker = (_demoWorker_t *)data;
    _demoState_t *state = worker->state;
    bool unitsReady = false;

    while (unitsReady == false) {
        unitsReady = atomic_load(&state->unitsReady);
    }

    for (size_t i = 0; i < state->messageCount; i += 1) {
        size_t const priority = (i % ELOS_EVENTBUFFER_PRIORITY_COUNT);
        struct timespec timeStart = {0};
        size_t elementsWritten = 0;
        elosEventPtrVector_t eventPtrVector = {0};
        safuResultE_t result;
        int retVal;

        retVal = clock_gettime(_CLOCK_ID, &timeStart);
        if (retVal < 0) {
            fprintf(stderr, "clock_gettime (start) failed\n");
        } else {
            result = elosEventBufferRead(&state->eventBuffer, priority, &eventPtrVector, &elementsWritten);
            if (result != SAFU_RESULT_OK) {
                fprintf(stderr, "elosEventBufferWrite failed\n");
            } else {
                struct timespec timeStop = {0};

                retVal = clock_gettime(_CLOCK_ID, &timeStop);
                if (retVal < 0) {
                    fprintf(stderr, "clock_gettime (stop) failed\n");
                } else {
                    ssize_t const elementsLost = elementsWritten - state->eventLimit;
                    struct timespec timeDiff = {0};

                    timespecSub(&timeStop, &timeStart, &timeDiff);
                    worker->messageTime[i] = ((double)timeDiff.tv_sec * 1e3) + ((double)timeDiff.tv_nsec / 1e6);

                    atomic_fetch_add(&state->consumer.processed, elementsWritten);
                    if (elementsLost > 0) {
                        atomic_fetch_add(&state->messagesLost, elementsLost);
                    }
                }

                safuVecIterate(&eventPtrVector, _deleteEventPtr, NULL);
                safuVecFree(&eventPtrVector);
            }
        }
    }

    return NULL;
}

static safuResultE_t _workerStart(_demoState_t *state, _demoWorker_t *workerArray, size_t workerCount,
                                  _workerFunc_t *workerFunc) {
    safuResultE_t result = SAFU_RESULT_OK;

    for (size_t i = 0; i < workerCount; i += 1) {
        _demoWorker_t *worker = &workerArray[i];
        int retVal;

        worker->idx = i;
        worker->state = state;
        worker->messageTime = calloc(state->messageCount, sizeof(double));

        if (worker->messageTime == NULL) {
            fprintf(stderr, "ERROR: worker memory allocation failed\n");
            result = SAFU_RESULT_FAILED;
        } else {
            retVal = pthread_create(&worker->thread, NULL, workerFunc, worker);
            if (retVal < 0) {
                fprintf(stderr, "ERROR: worker thread creation failed\n");
                result = SAFU_RESULT_FAILED;
                break;
            }
        }
    }

    return result;
}

static safuResultE_t _workerStop(_demoState_t *state, _demoWorker_t *workerArray, size_t workerCount,
                                 _demoTime_t *times) {
    safuResultE_t result = SAFU_RESULT_OK;

    for (size_t i = 0; i < workerCount; i += 1) {
        _demoWorker_t *worker = &workerArray[i];

        pthread_join(worker->thread, NULL);

        for (size_t o = 0; o < state->messageCount; o += 1) {
            double value = worker->messageTime[o];

            if (times->min > value) {
                times->min = value;
            } else if (times->max < value) {
                times->max = value;
            }

            times->sum += value;
        }

        free(worker->messageTime);
        worker->messageTime = NULL;
    }

    return result;
}

safuResultE_t _demoRun(_demoState_t *state) {
    safuResultE_t result = SAFU_RESULT_OK;
    size_t const publisherCount = state->publisher.count;
    size_t const consumerCount = state->consumer.count;
    size_t const messageCount = state->messageCount;

    _demoWorker_t publisher[publisherCount];
    _demoWorker_t consumer[consumerCount];
    _demoTime_t publisherTimes = {.min = DBL_MAX, .max = DBL_MIN, .avg = DBL_MIN, .sum = DBL_MIN};
    _demoTime_t consumerTimes = {.min = DBL_MAX, .max = DBL_MIN, .avg = DBL_MIN, .sum = DBL_MIN};

    result = _workerStart(state, publisher, publisherCount, _publisherWorkerThread);
    if (result == SAFU_RESULT_OK) {
        result = _workerStart(state, consumer, consumerCount, _consumerWorkerThread);
        if (result == SAFU_RESULT_OK) {
            atomic_store(&state->unitsReady, true);

            _workerStop(state, publisher, publisherCount, &publisherTimes);
            _workerStop(state, consumer, consumerCount, &consumerTimes);

            publisherTimes.avg = publisherTimes.sum / (double)publisherCount / (double)messageCount;
            consumerTimes.avg = consumerTimes.sum / (double)consumerCount / (double)messageCount;

            puts("");
            printf("publisher: %lu\n", state->publisher.count);
            printf("consumer: %lu\n", state->consumer.count);
            printf("messageCount: %lu\n", state->messageCount);
            puts("");
            printf("messagesPublished: %lu\n", state->publisher.processed);
            printf("messagesConsumed: %lu\n", state->consumer.processed);
            printf("messagesLostByLimit: %lu\n", atomic_load(&state->messagesLost));
            puts("");
            printf("publishTimes.min: %.6lfmsec\n", publisherTimes.min);
            printf("publishTimes.avg: %.6lfmsec\n", publisherTimes.avg);
            printf("publishTimes.max: %.6lfmsec\n", publisherTimes.max);
            printf("publishTimes.sum: %.6lfmsec\n", publisherTimes.sum);
            puts("");
            printf("consumerTimes.min: %.6lfmsec\n", consumerTimes.min);
            printf("consumerTimes.avg: %.6lfmsec\n", consumerTimes.avg);
            printf("consumerTimes.max: %.6lfmsec\n", consumerTimes.max);
            printf("consumerTimes.sum: %.6lfmsec\n", consumerTimes.sum);
            puts("");
        }
    }
    return result;
}

static safuResultE_t _parseCommandLine(int argc, char *argv[], _demoState_t *state) {
    safuResultE_t result = SAFU_RESULT_OK;
    bool runParser = true;

    static struct option options[] = {
        [OPTION_PUBLISHER] = {"publisher", required_argument, 0, 0},
        [OPTION_CONSUMER] = {"consumer", required_argument, 0, 0},
        [OPTION_MESSAGECOUNT] = {"messageCount", required_argument, 0, 0},
        [OPTION_EVENTLIMIT] = {"eventLimit", required_argument, 0, 0},
        [OPTION_HELPMODE] = {"help", no_argument, 0, 0},
        {0, 0, 0, 0},
    };

    while (runParser == true) {
        int optionsIdx;
        char option;

        option = getopt_long(argc, argv, ":h", options, &optionsIdx);
        switch (option) {
            case 'h':
                state->helpMode = true;
                break;
            case -1:
                runParser = false;
                break;
            case 0:
                switch (optionsIdx) {
                    case OPTION_PUBLISHER:
                        state->publisher.count = strtoul(optarg, NULL, 10);
                        break;
                    case OPTION_CONSUMER:
                        state->consumer.count = strtoul(optarg, NULL, 10);
                        break;
                    case OPTION_MESSAGECOUNT:
                        state->messageCount = strtoul(optarg, NULL, 10);
                        break;
                    case OPTION_EVENTLIMIT:
                        state->eventLimit = strtoul(optarg, NULL, 10);
                        break;
                    case OPTION_HELPMODE:
                        state->helpMode = true;
                        break;
                    default:
                        fprintf(stderr, "ERROR: getopt returned unknown option index %d\n", optionsIdx);
                        result = SAFU_RESULT_FAILED;
                }
                break;
            default:
                fprintf(stderr, "ERROR: getopt returned unknown character code 0%o\n", option);
                result = SAFU_RESULT_FAILED;
                break;
        }
    }

    return result;
}

static void _printHelp(UNUSED int argc, char *argv[]) {
    printf(
        "usage: %s [-h|--help]"
        " [--publisher NUMBER] [--consumer NUMBER] [--messageCount NUMBER] [--eventLimit NUMBER]\n"
        "\n"
        "EventBuffer demonstration with time measurements for reading and writing.\n"
        "Uses a single EventBuffer that is written/read by multiple publishers/consumers.\n"
        "\n"
        "options:\n"
        "  -h, --help               Shows this help message and exits\n"
        "  --publisher NUMBER       Number of publishers used (with each publisher running in its own thread)\n"
        "  --consumer NUMBER        Number of consumers used (with each consumer running in its own thread)\n"
        "  --messageCount NUMBER    Number of messages to be sent by each publisher\n"
        "  --eventLimit NUMBER      Number of Events that can be stored before old unread Events are dropped\n"
        "\n",
        argv[0]);
}

int main(int argc, char *argv[]) {
    safuResultE_t result;
    _demoState_t state = {
        .eventBuffer = {0},
        .unitsReady = false,
        .messagesLost = 0,
        .publisher = {.count = _DEFAULT_PUBLISHER_COUNT, .processed = 0},
        .consumer = {.count = _DEFAULT_CONSUMER_COUNT, .processed = 0},
        .messageCount = _DEFAULT_MESSAGE_COUNT,
        .eventLimit = _DEFAULT_EVENT_LIMIT,
        .helpMode = _DEFAULT_HELPMODE,
    };

    result = _parseCommandLine(argc, argv, &state);
    if (result == SAFU_RESULT_OK) {
        if (state.helpMode == true) {
            _printHelp(argc, argv);
        } else {
            elosEventBufferParam_t eventBufferParam = {.limitEventCount = state.eventLimit};

            result = elosEventBufferInitialize(&state.eventBuffer, &eventBufferParam);
            if (result == SAFU_RESULT_OK) {
                result = _demoRun(&state);
            }

            elosEventBufferDeleteMembers(&state.eventBuffer);
        }
    }

    return (result == SAFU_RESULT_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}
