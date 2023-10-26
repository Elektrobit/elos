# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check authorized process filter set multiple times

Resource          ../../elosd-keywords.resource
Resource          ../../keywords.resource

Library           ../../libraries/TemplateConfig.py

Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***
${BLACKLIST_FILTER}                 .event.messageCode 2010 EQ
@{FILTER_1}                         .process.exec '${ELOSC_PATH}' STRCMP
@{FILTER_2}                         .process.uid 0 EQ
@{FILTER_3}                         .process.gid 0 EQ
@{AUTHORIZED_PROCESS_FILTERS}       ${FILTER_1}    ${FILTER_2}    ${FILTER_3} 

*** Test Cases ***
01_Test_Multiple_Filters
    [Documentation]    Any authorized process can publish a blacklisted event

    FOR    ${filter}    IN    @{AUTHORIZED_PROCESS_FILTERS}
        Given A Process Filter Is Set    ${filter}
        When Client Tries To Publish A Blacklisted Event
        Then Blacklisted Event Is Published
    END

02_Test_Multiple_Filters
    [Documentation]    Any authorized process can publish a normal event

    FOR    ${filter}    IN    @{AUTHORIZED_PROCESS_FILTERS}
        Given A Process Filter Is Set    ${filter}
        When Client Tries To Publish A Normal Event
        Then Event Is Published
    END

*** Keywords ***
A Process Filter Is Set
    [Arguments]        ${process_filter}
    [Documentation]    Set given authorized process filter in config

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template    EventBlacklist=${BLACKLIST_FILTER}    authorizedProcesses=${process_filter}
    Start Elosd
    Wait Till Elosd Is Started


Client Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${rc}    Execute And Log Based On User Permissions    elosc -p '{"messageCode": 2010}'    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Authorized client unable to publish blacklisted event


Blacklisted Event Is Published
    [Documentation]    Blacklisted event will be published from authorized clients
    [Teardown]         Reset Elosd Config

    ${stdout}    ${rc}   Execute And Log    elosc -f ".event.messageCode 2010 EQ" | grep 2010 | tail -1    ${RETURN_STDOUT}    ${RETURN_RC}
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter


Client Tries To Publish A Normal Event
    [Documentation]    An elos client tries to publish a normal event and Succeeds

    ${rc}    Execute And Log    elosc -p '{"messageCode": 150}'    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Client unable to publish normal event


Event Is Published
    [Documentation]    Event not blacklisted will be published.
    [Teardown]         Reset Elosd Config

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
