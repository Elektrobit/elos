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


*** Test Cases ***
01_Test_OOM_Killer_Kernel_Event
    [Documentation]    When oom killer is invoked an event is
    ...    published.

    Given Elosd Is Running
    When An OOM Killer Is Invoked
    Then An Event Is Published


*** Keywords ***
An OOM Killer Is Invoked
    [Documentation]    OOM killer is invoked.

    ${output}    ${rc}=    Execute Command    tail /dev/zero    return_rc=True

    Log    ${output}
    Log    ${rc}

An Event Is Published
    [Documentation]    OOM killer invoked event is published.

    ${search_output}    ${rc}=    Execute Command
    ...    elosc -f ".event.messageCode 5020 EQ"
    ...    return_rc=True

    Log    ${search_output}
    Should Not Be Empty    ${search_output}
