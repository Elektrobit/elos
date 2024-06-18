*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if an event is published
...                 when ever an oom killer is invoked.

Library             String
Library             SSHLibrary
Library             ../../../libraries/ElosKeywords.py
Resource            ../../../elosd-keywords.resource
Resource            ../../../keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                     AND    Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${CLIENT_OOM_MESSAGE}
...                         {
...                         "date": [PTIME,0],
...                         "source": {
...                         "appName": "",
...                         "fileName": "testapp",
...                         },
...                         "severity": 3,
...                         "classification": 1,
...                         "messageCode": 1111,
...                         "payload": "3,270,497530363,-;Out of memory: Killed process 115 (tail)"
...                         }


*** Test Cases ***
01_Test_OOM_Killer_Client_Event
    [Documentation]    When a client publishes oom killer invoked
    ...    then event is not published

    Given Elosd Is Running
    When Client Publishes OOM Killer Event
    Then An OOM Event Is Not Published


*** Keywords ***
Client Publishes OOM Killer Event
    [Documentation]    an client publishes a oom killer invoked event

    ${PUBLISH_TIME}=    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    ${CLIENT_OOM_MESSAGE}=    Replace String    ${CLIENT_OOM_MESSAGE}    PTIME    ${PUBLISH_TIME}

    Publish Event    ${CLIENT_OOM_MESSAGE}

An OOM Event Is Not Published
    [Documentation]    OOM killer invoked event is not published.

    @{matched_events}=    Find Events Matching
    ...    .event.messageCode 5020 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND

    ${event_count}=    Get Length    ${matched_events}

    Should Be Equal As Integers    ${event_count}    0
