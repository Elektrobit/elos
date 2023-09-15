# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check simple authorized process filter

Resource          ../../elosd-keywords.resource
Resource          ../../keywords.resource

Library           ../../libraries/TemplateConfig.py

Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***
${PUBLISH_TIME}                                  0
${AUTHORIZE_ROOT}                                ${true}
${BLACKLIST_FILTER}                              .event.messageCode 2010 EQ
@{FILTER_TO_ALLOW_ROOT_PROCESSES}                .process.gid 0 EQ
@{FILTER_TO_ALLOW_NON_ROOT_PROCESSES}            .process.gid 0 NE

*** Test Cases ***
01_Test_Simple_Filter
    [Documentation]    Authorized processes can publish blacklisted event.
    ...                All root processes are authorized.
    [Teardown]         Reset Elosd Config

    Given A Filter To Authorize Root Processes Is Set
    When Root Process Tries To Publish A Blacklisted Event
    Then Blacklisted Event Is Published

02_Test_Simple_Filter
    [Documentation]    Authorized processes can publish normal event
    ...                All root processes are authorized.
    [Teardown]         Reset Elosd Config

    Given A Filter To Authorize Root Processes Is Set
    When Root Process Tries To Publish A Normal Event
    Then Event Is Published

03_Test_Simple_Filter
    [Documentation]    Unauthorized processes can publish normal events
    ...                All root processes are unauthorized.
    [Teardown]         Reset Elosd Config

    Given A Filter To Authorize Non Root Processes Is Set
    When Root Process Tries To Publish A Normal Event
    Then Event Is Published

04_Test_Simple_Filter
    [Documentation]    Unauthorized processes can not publish blacklisted events
    ...                All root processes are unauthorized.
    [Teardown]         Reset Elosd Config

    Given A Filter To Authorize Non Root Processes Is Set
    When Root Process Tries To Publish A Blacklisted Event
    Then A Security Event Is Published


*** Keywords ***
A Filter To Authorize Root Processes Is Set
    [Documentation]    Set a simple authorized process filter to authorize all root processes

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template    EventBlacklist=${BLACKLIST_FILTER}    authorizedProcesses=${FILTER_TO_ALLOW_ROOT_PROCESSES}
    Start Elosd
    Wait Till Elosd Is Started


A Filter To Authorize Non Root Processes Is Set 
    [Documentation]    Set a simple authorized process filter to authorize all  non root processes

    Set Test Variable     ${AUTHORIZE_ROOT}    ${false}
    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template    EventBlacklist=${BLACKLIST_FILTER}    authorizedProcesses=${FILTER_TO_ALLOW_NON_ROOT_PROCESSES}
    Start Elosd
    Wait Till Elosd Is Started


Root Process Tries To Publish A Blacklisted Event
    [Documentation]    A root process tries to publish a blacklisted event

    ${PUBLISH_TIME}    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    ${rc}    Execute And Log Based On User Permissions    elosc -p '{"messageCode": 2010}'    ${RETURN_RC}
     
    Run Keyword If  ${AUTHORIZE_ROOT}    Executable Returns No Errors    ${rc}    Authorized client unable to publish blacklisted event
    ...     ELSE   Executable Returns An Error    ${rc} 
 

Root Process Tries To Publish A Normal Event
    [Documentation]    A root process tries to publish a normal event

    ${PUBLISH_TIME}    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    ${rc}    Execute And Log Based On User Permissions    elosc -p '{"messageCode": 150}'    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Client unable to publish normal event   


Blacklisted Event Is Published
    [Documentation]    Blacklisted event will be published from authorized clients

    ${stdout}    ${rc}   Execute And Log    elosc -f ".event.messageCode 2010 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND"   ${RETURN_STDOUT}    ${RETURN_RC}
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter


Event Is Published
    [Documentation]    Event not blacklisted will be published from authorized clients.

    ${stdout}    ${rc}   Execute And Log    elosc -f ".event.messageCode 150 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND"    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Contain    ${stdout}    150
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter


A Security Event Is Published
    [Documentation]    Security event will be published if an unauthorized client tries to publish
    ...                a blacklisted event.

    ${stdout}    ${rc}   Execute And Log    elosc -f ".event.messageCode 8007 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND"    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Blacklisted event not filtered out by blacklist filter


Reset Elosd Config
    [Documentation]     reset elosd config to default during test teardown.

    Stop Elosd
    Wait For Elosd To Stop
    Cleanup Template Config
    Start Elosd
    Wait Till Elosd Is Started
