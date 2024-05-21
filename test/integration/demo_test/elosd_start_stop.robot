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

    Given Ensure Elosd Is Started
    When Elosd Service Stops Instance
    Then Wait For Elosd To Stop

Test Elosd Service Starts Elosd Instance
    [Documentation]    test to check elosd service starts elosd

    Given Ensure Elosd Is Stopped
    When Elosd Service Starts Instance
    Then Elosd Instance Is Started


*** Keywords ***
Elosd Service Stops Instance
    [Documentation]    elosd service stops an elosd instance
    Stop Elosd

Elosd Instance Is Not Running
    [Documentation]    check elosd is stopped
    Wait For Elosd To Stop

Elosd Service Starts Instance
    [Documentation]    elosd service starts an elosd instance
    Start Elosd

Elosd Instance Is Started
    [Documentation]    Check if a elosd instance is running
    Wait Till Elosd Is Started
