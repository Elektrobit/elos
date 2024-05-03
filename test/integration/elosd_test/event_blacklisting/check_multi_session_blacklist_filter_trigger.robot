*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check blacklist filter triggered mutiple times over
...                 different sessions work.

Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource
Library             ../../libraries/TemplateConfig.py
Library             ../../libraries/ElosKeywords.py

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Variables ***
${BLACKLIST_FILTER}     .event.messageCode 2010 EQ
@{PROCESS_FILTER}       1 0 EQ    # all processes are unauthorized
${SESSIONS}             2
${CLIENTS}              2
${SESSION_ID}           1


*** Test Cases ***
01_Test_Trigger_Blacklist_Filter_Multiple_Sessions    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Blacklist filter is triggered more than once
    ...    in separate sessions.
    [Setup]    A Simple Blacklist Filter Is Set

    FOR    ${session}    IN RANGE    0    ${SESSIONS}
        Given New Session Is Started
        When Multiple Unauthorized Processes Try To Publish A Blacklisted Event
        Then A Security Event Is Published
    END
    [Teardown]    Reset Elosd Config


*** Keywords ***
A Simple Blacklist Filter Is Set
    [Documentation]    Set a simple blacklist filter in config

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template
    ...    EventBlacklist=${BLACKLIST_FILTER}
    ...    authorizedProcesses=${PROCESS_FILTER}
    Start Elosd
    Wait Till Elosd Is Started

New Session Is Started
    [Documentation]    Start a new elos session

    Run Keyword    Restart Elosd
    ${SESSION_ID}=    Evaluate    ${SESSION_ID} + 1
    Set Test Variable    ${SESSION_ID}

Multiple Unauthorized Processes Try To Publish A Blacklisted Event
    [Documentation]    run multiple client to publish blacklisted filters

    ${PUBLISH_TIME}    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    FOR    ${i}    IN RANGE    0    ${CLIENTS}
        Run Keyword    Unauthorized Process Tries To Publish A Blacklisted Event
    END

Unauthorized Process Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${SESSION_TOKEN}=    Set Variable    01_Test_Trigger_Blacklist_Filter_Multiple_Sessions${SESSION_ID}
    Set Test Variable    ${SESSION_TOKEN}
    ${rc}    Execute And Log    elosc -p '{"messageCode": 2010, "payload":"${SESSION_TOKEN}"}'    ${RETURN_RC}
    Executable Returns An Error    ${rc}

A Security Event Is Published
    [Documentation]    Attempt to publish a blacklisted event will lead to a security event
    ...    to be published if client is unauthorized.

    ${stdout}    ${rc}    Execute And Log
    ...    elosc -f ".event.messageCode 8007 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND"
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}
    Should Contain X Times    ${stdout}    ${SESSION_TOKEN}    ${CLIENTS}
    Executable Returns No Errors    ${rc}    Blacklisted event not filtered out by blacklist filter

Reset Elosd Config
    [Documentation]    reset elosd config to default during test teardown.

    Stop Elosd
    Wait For Elosd To Stop
    Cleanup Template Config
    Start Elosd
    Wait Till Elosd Is Started
