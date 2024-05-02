*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite elosd service is stoppable and startable

Resource            ../elosd-keywords.resource
Resource            ../keywords.resource

Library             ../libraries/ElosKeywords.py 

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Test Cases ***
Test Elosd Service Stops Elosd Instance
    [Documentation]    test to check elosd service stops elosd

    Given Elosd Instance Is Running
    When Elosd Service Stops Instance
    Then Elosd Completely Stops

Test Elosd Service Starts Elosd Instance
    [Documentation]    test to check elosd service starts elosd

    Given Elosd Instance Is Not Running
    When Elosd Service Starts Instance
    Then Elosd Instance Is Started


*** Keywords ***
Elosd Instance Is Running
    [Documentation]    Check if a elosd instance is running

    ${status_running}=    Elosd Is Running
    Should Be True    ${status_running}

    ${pid}    ${rc}=    Execute And Log    pgrep elosd    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Not Be Empty    ${pid}

Elosd Service Stops Instance
    [Documentation]    elosd service stops an elosd instance

    Stop Elosd
    Wait For Elosd To Stop

Elosd Completely Stops
    [Documentation]    elosd stops, no running elosd instance found

    ${status_stopped}=    Elosd Is Stopped
    Should Be True    ${status_stopped}

    ${pid}    ${rc}=    Execute And Log    pgrep elosd    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Be Empty    ${pid}

Elosd Instance Is Not Running
    [Documentation]    check elosd is stopped
    Run Keyword    Elosd Completely Stops

Elosd Service Starts Instance
    [Documentation]    elosd service starts an elosd instance

    Start Elosd
    Wait Till Elosd Is Started

Elosd Instance Is Started
    [Documentation]    Check if a elosd instance is running
    Run Keyword    Elosd Is Running
