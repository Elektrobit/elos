# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check trigger authorized process filter multiple times.

Resource          ../../elosd-keywords.resource
Resource          ../../keywords.resource

Library           ../../libraries/TemplateConfig.py

Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***
${BLACKLIST_FILTER}                .event.messageCode 2010 EQ
@{AUTHORIZED_PROCESS_FILTER}       .process.gid 0 EQ
${CLIENTS}                         2

*** Test Cases ***
01_Test_Trigger_Filter_Multiple_Times
    [Documentation]    Blacklisted events can be published by multiple authorized processes
    [Teardown]         Reset Elosd Config

    Given A Simple Authorized Process Filter Is Set
    When Multiple Clients Try To Publish A Blacklisted Event
    Then Blacklisted Event Is Published

*** Keywords ***
A Simple Authorized Process Filter Is Set
    [Documentation]    Set a simple authorized process filter in config

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template    EventBlacklist=${BLACKLIST_FILTER}    authorizedProcesses=${AUTHORIZED_PROCESS_FILTER}
    Start Elosd
    Wait Till Elosd Is Started


Multiple Clients Try To Publish A Blacklisted Event
    [Documentation]    run multiple clients to publish blacklisted filters

    ${PUBLISH_TIME}    Get Elos Event Publish Time Threshold
 
    Set Test Variable    ${PUBLISH_TIME}

    FOR    ${i}    IN RANGE    0     ${CLIENTS}
        Run Keyword     Client Tries To Publish A Blacklisted Event
    END


Client Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${rc}    Execute And Log Based On User Permissions    elosc -p '{"messageCode": 2010}'    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Blacklisted event cannot be published by authorized process.


Blacklisted Event Is Published
    [Documentation]    Blacklisted event will be published from authorized clients

    ${stdout}    ${rc}   Execute And Log    elosc -f ".event.messageCode 2010 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND" | grep "source"    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Contain X Times    ${stdout}    2010     ${CLIENTS}
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter


Reset Elosd Config
    [Documentation]     reset elosd config to default during test teardown.

    Stop Elosd
    Wait For Elosd To Stop
    Cleanup Template Config
    Start Elosd
    Wait Till Elosd Is Started
