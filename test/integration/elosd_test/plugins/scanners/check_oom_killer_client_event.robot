*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if an event is not published
...                 when a client publishes an oom killer invoked event.

Library             String
Library             SSHLibrary
Library             ../../../libraries/ElosKeywords.py
Resource            ../../../elosd-keywords.resource
Resource            ../../../keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                     AND    Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${OOM_EVENT_FILTER}     .event.messageCode 5020 EQ
${OOM_LOG_EVENT}
...                     {
...                     "date": [ptime,0],
...                     "source": {
...                     "appName": "",
...                     "fileName": "testapp",
...                     },
...                     "severity": 3,
...                     "classification": 1,
...                     "messageCode": 1111,
...                     "payload": "3,270,497530363,-;Out of memory: Killed process 115 (tail)"
...                     }


*** Test Cases ***
01_Test_OOM_Killer_Client_Event
    [Documentation]    OOM killer invoked event is published only when
    ...    a kernel log event with oom killer invoked payload
    ...    is published, similar events not from kernel do not
    ...    trigger oom killer invoked event.

    Given Elosd Is Running
    When An Client Publishes OOM Log Event
    Then An OOM Killer Event Is Not Published


*** Keywords ***
An Client Publishes OOM Log Event
    [Documentation]    An client publishes an oom killer invoked log event

    ${OOM_LOG_EVENT}=    Set Event Publish Time    ${OOM_LOG_EVENT}

    Publish Event    ${OOM_LOG_EVENT}

An OOM Killer Event Is Not Published
    [Documentation]    OOM killer invoked event is not published.

    Latest Events Matching ${OOM_EVENT_FILTER} Not Found
