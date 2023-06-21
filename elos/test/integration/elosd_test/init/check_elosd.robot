# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check if elosd is installed and running in Target

Library           SSHLibrary
Resource         ../../keywords.resource
Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***
${ELOSD}    /usr/bin/elosd


*** Test Cases ***
Check If Elosd Is Installed
    [Documentation]    Test checks if elosd is installed
    ${output}=         Is Elosd Installed
    Should Be Equal    ${output}    ${ELOSD}    msg=Elosd is not installed

Check If Elosd Is Running
    [Documentation]    Test checks if elosd is running
    ${output}=         Is Elosd Running
    Should Not Be Empty    ${output}    msg=Elosd is not running

*** Keywords ***
Is Elosd Installed
    ${value}=         Execute And Log    which elosd    ${RETURN_STDOUT}
    RETURN    ${value}

Is Elosd Running
    ${value}=         Execute And Log    ps aux | grep 'elosd' | grep -v sudo       ${RETURN_STDOUT}
    RETURN    ${value}
