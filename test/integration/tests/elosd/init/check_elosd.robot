*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if elosd is installed and running in Target

Library             SSHLibrary
Resource            resources/keywords.resource
Resource            resources/elosd-keywords.resource

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Test Cases ***
Check If Elosd Is Installed
    [Documentation]    Test checks if elosd is installed
    ${output}=    Is Elosd Installed
    Should Be Equal    ${output}    ${ELOSD_PATH}    msg=Elosd is not installed


*** Keywords ***
Is Elosd Installed
    [Documentation]    check if elosd is installed
    ${value}=    Execute And Log    which elosd    ${RETURN_STDOUT}
    RETURN    ${value}
