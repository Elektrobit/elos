*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check complex blacklist filter

Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource
Library             ../../libraries/TemplateConfig.py
Library             ../../libraries/ElosKeywords.py

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Variables ***
# robocop: disable=line-too-long
${BLACKLIST_FILTER}     .event.severity 3 EQ .event.classification 260 EQ AND .event.messageCode 2010 EQ AND
@{PROCESS_FILTER}       1 0 EQ    # all processes are unauthorized


*** Test Cases ***
01_Test_Complex_Blacklist_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    check complex elos blacklist filter filters blacklisted event.

    Given A Complex Blacklist Filter Is Set
    When Unauthorized Process Tries To Publish A Blacklisted Event
    Then A Security Event Is Published
    [Teardown]    Reset Elosd Config

02_Test__Complex_Blacklist_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    check complex elos blacklist filter does not filter normal events.

    Given A Complex Blacklist Filter Is Set
    When Unauthorized Process Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
A Complex Blacklist Filter Is Set
    [Documentation]    Set a complex blacklist filter in config

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template
    ...    EventBlacklist=${BLACKLIST_FILTER}
    ...    authorizedProcesses=${PROCESS_FILTER}
    Start Elosd
    Wait Till Elosd Is Started

Unauthorized Process Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${rc}    Execute And Log
    ...    elosc -p '{"severity": 3,"classification": 260,"messageCode": 2010}'
    ...    ${RETURN_RC}
    Executable Returns An Error    ${rc}

Unauthorized Process Tries To Publish A Normal Event
    [Documentation]    An elos client tries to publish a normal event and Succeeds

    ${rc}    Execute And Log
    ...    elosc -p '{"severity": 3,"classification": 260,"messageCode": 150}'
    ...    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Unauthorized Process unable to publish normal event

A Security Event Is Published
    [Documentation]    Attempt to publish a blacklisted event will lead to a security event
    ...    to be published if client is unauthorized.

    ${stdout}    ${rc}    Execute And Log
    ...    elosc -f ".event.messageCode 8007 EQ"
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Blacklisted event not filtered out by blacklist filter

Event Is Published
    [Documentation]    Event not blacklisted will be published.

    ${stdout}    ${rc}    Execute And Log
    ...    elosc -f ".event.severity 3 EQ .event.messageCode 150 EQ AND"
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}
    Should Contain    ${stdout}    150
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter

Reset Elosd Config
    [Documentation]    reset elosd config to default during test teardown.

    Stop Elosd
    Wait For Elosd To Stop
    Cleanup Template Config
    Start Elosd
    Wait Till Elosd Is Started
