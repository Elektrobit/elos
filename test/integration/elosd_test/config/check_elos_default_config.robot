*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if elos default configuration is correct

Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Variables ***
${ELOSD_PORT}           54321
${ELOSD_INTERFACE}      "0.0.0.0"


*** Test Cases ***
Test Elosd Port
    [Documentation]    test to check elosd port

    Check Elosd Is Listening In Port    ${ELOSD_PORT}

Test Elosd Interface
    [Documentation]    test to check elosd interface

    Check If Elosd Network Interface Is    ${ELOSD_INTERFACE}


*** Keywords ***
Check Elosd Is Listening In Port
    [Documentation]    Check if elosd is connected to given port
    [Arguments]    ${port}

    ${hexport}=    Convert To Hex    ${port}

    Log    ${hexport}

    ${output}=    Execute And Log Based On User Permissions
    ...    sh -c 'cat /proc/$(pgrep elosd)/net/tcp | grep ${hexport}'
    ...    ${RETURN_STDOUT}

    Should Not Be Empty    ${output}

Check If Elosd Network Interface Is
    [Documentation]    Check if elosd has given network interface
    [Arguments]    ${interface}

    ${output}=    Execute And Log Based On User Permissions
    ...    sh -c 'netstat -tl | grep ${ELOSD_PORT} | grep ${interface}'
    ...    ${RETURN_STDOUT}

    Should Not Be Empty    ${output}
