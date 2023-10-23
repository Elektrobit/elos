# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check if elosd is installed and running in Target

Library           SSHLibrary
Resource         ../../keywords.resource
Resource         ../../elosd-keywords.resource
Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Test Cases ***
Check If Elosd Is Installed
    [Documentation]    Test checks if elosd is installed
    ${output}=         Is Elosd Installed
    Should Be Equal    ${output}    ${ELOSD_PATH}    msg=Elosd is not installed

Check If Elosd Is Running
    [Documentation]    Test checks if elosd is running
    ${output}=         Elosd Is Running
    Should Be True     ${output}

*** Keywords ***
Is Elosd Installed
    ${value}=         Execute And Log    which elosd    ${RETURN_STDOUT}
    RETURN    ${value}
