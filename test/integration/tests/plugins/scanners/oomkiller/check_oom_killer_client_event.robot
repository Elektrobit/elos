*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if an event is not published
...                 when a client publishes an oom killer invoked event.

Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py
Library             libraries/ElosEventKeywords.py
Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                     AND    Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${OOM_EVENT_FILTER}     .event.messageCode 5020 EQ
${FILENAME}             testapp
${SEVERITY}             3
${CLASSIFICATION}       1
${MESSAGE_CODE}         1111
${PAYLOAD}              3,270,497530363,-;Out of memory: Killed process 115 (tail)


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

    ${OOM_EVENT}=    Get Default Event
    ${OOM_EVENT}=    Set '${FILENAME}' As '${OOM_EVENT}' File Name
    ${OOM_EVENT}=    Set '${SEVERITY}' As '${OOM_EVENT}' Severity
    ${OOM_EVENT}=    Set '${CLASSIFICATION}' As '${OOM_EVENT}' Classification
    ${OOM_EVENT}=    Set '${MESSAGE_CODE}' As '${OOM_EVENT}' Message Code
    ${OOM_EVENT}=    Set '${PAYLOAD}' As '${OOM_EVENT}' Payload

    ${OOM_EVENT_JSON}=   Get '${OOM_EVENT}' As Json String

    Publish '${OOM_EVENT_JSON}'

An OOM Killer Event Is Not Published
    [Documentation]    OOM killer invoked event is not published.

    Latest Events Matching '${OOM_EVENT_FILTER}' Not Found
