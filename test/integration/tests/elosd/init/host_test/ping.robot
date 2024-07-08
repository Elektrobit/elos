*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       Simple ping test to check that target is reachable.

Library             libraries/NetworkLibrary.py
Library             libraries/OsLibrary.py
Resource            resources/keywords.resource


*** Variables ***
${PING_TIMEOUT}     1


*** Test Cases ***
Ping Target
    [Documentation]    Ping a given target.

    Given Ping Command Exists
    And Target IP Address Is Valid
    Then Target Responds To A Ping


*** Keywords ***
Ping Command Exists
    [Documentation]    Ping a given target.

    ${exists}    Program Exists    ping

Target Responds To A Ping
    [Documentation]    Ping a given target.

    ${rc}    ${output}    Ping    ${TARGET_IP}    ${PING_TIMEOUT}
    Should Be Equal As Integers    ${rc}    ${0}    msg=Target did not respond to ping:\n${output}
