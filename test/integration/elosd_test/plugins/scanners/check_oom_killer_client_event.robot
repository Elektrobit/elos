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
${TEST_PID}                 100
${CLIENT_OOM_MESSAGE}
...                         {
...                         "date": [0,0],
...                         "source": {
...                         "appName": "",
...                         "fileName": "testapp",
...                         "pid": 42
...                         },
...                         "severity": 3,
...                         "hardwareid": "HOSTNAME",
...                         "classification": 1,
...                         "messageCode": 1111,
...                         "payload": "Out of memory: Killed process 100 (testapp)"
...                         }


*** Test Cases ***
01_Test_OOM_Killer_Client_Event
    [Documentation]    When a client publishes oom killer invoked
    ...    then event is not published

    Given Elosd Is Running
    When Client Publishes OOM Killer Event
    Then An Event Is Not Published


*** Keywords ***
Client Publishes OOM Killer Event
    [Documentation]    an client publishes a oom killer invoked event

    ${output}    ${rc}=    Execute Command    elosc -p ${CLIENT_OOM_MESSAGE}    return_rc=True

    Log    ${output}
    Log    ${rc}

An Event Is Not Published
    [Documentation]    OOM killer invoked event is not published.

    ${search_output}    ${rc}=    Execute Command
    ...    elosc -f ".event.messageCode 5020 EQ .event.source.pid 100 EQ AND" | grep ${TEST_PID}
    ...    return_rc=True

    Log    ${search_output}
    Should Be Empty    ${search_output}
