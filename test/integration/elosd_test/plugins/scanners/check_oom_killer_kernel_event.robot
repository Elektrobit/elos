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
${OOM_EVENT_FILTER}         .event.messageCode 5020 EQ
${KERNEL_OOM_MESSAGE}
...                         {
...                         "date":[ptime,0],
...                         "source":{
...                         "fileName":"\/dev\/kmsg"
...                         },
...                         "severity":3,
...                         "classification":1,
...                         "messageCode":1111,
...                         "payload":"3,270,497530363,-;Out of memory: Killed process 115 (tail)"
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
    [Documentation]    An oom killer    can be invoked by running
    ...    tail /dev/zero, but this is unreliable &
    ...    unsafe, so a similar kernel log event is
    ...    simulated via an elosc client

    ${KERNEL_OOM_MESSAGE}=    Set Event Publish Time    ${KERNEL_OOM_MESSAGE}

    Publish Event    ${KERNEL_OOM_MESSAGE}

An OOM Event Is Published
    [Documentation]    OOM killer invoked event is published.

    Latest Events Matching ${OOM_EVENT_FILTER} Found
