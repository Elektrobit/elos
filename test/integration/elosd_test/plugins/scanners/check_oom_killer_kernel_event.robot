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
${KERNEL_OOM_MESSAGE}
...                         {
...                         "date": [PTIME,0],
...                         "source": {
...                         "fileName": "\/dev\/kmsg"
...                         },
...                         "severity": 3,
...                         "classification": 1,
...                         "messageCode": 1111,
...                         "payload": "3,270,497530363,-;Out of memory: Killed process 115 (tail)"
...                         }


*** Test Cases ***
01_Test_OOM_Killer_Kernel_Event
    [Documentation]    When oom killer is invoked an event is
    ...    published.

    Given Elosd Is Running
    When An OOM Killer Is Invoked
    Then An OOM Event Is Published


*** Keywords ***
An OOM Killer Is Invoked
    [Documentation]    Instead of OOM killer invocation a similar event is
    ...    is published through a client.

    ${PUBLISH_TIME}=    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    ${KERNEL_OOM_MESSAGE}=    Replace String    ${KERNEL_OOM_MESSAGE}    PTIME    ${PUBLISH_TIME}

    Publish Event    ${KERNEL_OOM_MESSAGE}

An OOM Event Is Published
    [Documentation]    OOM killer invoked event is published.

    @{matched_events}=    Find Events Matching
    ...    .event.messageCode 5020 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND

    ${event_count}=    Get Length    ${matched_events}

    Should Not Be Equal As Integers    ${event_count}    0
