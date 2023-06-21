# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check empty authorized process filter

Resource          ../../elosd-keywords.resource
Resource          ../../keywords.resource

Library           ../../libraries/TemplateConfig.py

Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***
${BLACKLIST_FILTER}                .event.messageCode 2010 EQ
@{EMPTY_PROCESS_FILTERS}           ${EMPTY}    ${SPACE * 2}

*** Test Cases ***
01_Test_Empty_Filter
    [Documentation]    Empty process filter renders a process unauthorized
    [Teardown]         Reset Elosd Config

    Given An Empty Authorized Process Filter Is Set
    When Client Tries To Publish A Blacklisted Event
    Then A Security Event Is Published

02_Test_Empty_Filter
    [Documentation]    Normal events are published though process filter is empty
    [Teardown]         Reset Elosd Config

    Given An Empty Authorized Process Filter Is Set
    When Client Tries To Publish A Normal Event
    Then Event Is Published


*** Keywords ***
An Empty Authorized Process Filter Is Set
    [Documentation]    Set an empty authorized process filter in config

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template    EventBlacklist=${BLACKLIST_FILTER}    authorizedProcesses=${EMPTY_PROCESS_FILTERS}
    Start Elosd
    Wait Till Elosd Is Started


Client Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${rc}    Execute And Log Based On User Permissions    elosc -p '{"messageCode": 2010}'    ${RETURN_RC}
    Executable Returns An Error    ${rc}


Client Tries To Publish A Normal Event
    [Documentation]    An elos client tries to publish a normal event and Succeeds

    ${rc}    Execute And Log Based On User Permissions    elosc -p '{"messageCode": 150}'    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Client unable to publish normal event


A Security Event Is Published
    [Documentation]    Attempt to publish a blacklisted event will lead to a security event
    ...                to be published if client is unauthorized.

    ${stdout}    ${rc}   Execute And Log    elosc -f ".event.messageCode 8007 EQ" | grep 2010 | tail -1    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Blacklisted event not filtered out by blacklist filter


Event Is Published
    [Documentation]    Event not blacklisted will be published.

    ${stdout}    ${rc}   Execute And Log    elosc -f ".event.messageCode 150 EQ" | grep 150 | tail -1    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Contain    ${stdout}    150
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter


Reset Elosd Config
    [Documentation]     reset elosd config to default during test teardown.

    Stop Elosd
    Wait For Elosd To Stop
    Cleanup Template Config
    Start Elosd
    Wait Till Elosd Is Started
