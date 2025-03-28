*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check complex authorized process filter

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/TemplateConfig.py
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${AUTHORIZE_ROOT}               ${true}
${BLACKLIST_FILTER}             .event.messageCode 2010 EQ
@{ALLOW_ELOSC_AS_ROOT}          .process.gid 0 EQ .process.exec '${ELOSC_PATH}' STRCMP AND
@{ALLOW_ELOSC_AS_NON_ROOT}      .process.gid 0 NE .process.exec '${ELOSC_PATH}' STRCMP AND


*** Test Cases ***
01_Test_Complex_Filters    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Authorized processes can publish blacklisted event

    Given A Filter To Authorize Elosc As Root Is Set
    When Root Elosc Tries To Publish A Blacklisted Event
    Then Blacklisted Event Is Published
    [Teardown]    Reset Elosd Config

02_Test_Complex_Filters    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Authorized processes can publish normal event

    Given A Filter To Authorize Elosc As Root Is Set
    When Root Elosc Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config

03_Test_Complex_Filters    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Unauthorized processes can publish normal events

    Given A Filter To Authorize Elosc As Non Root Is Set
    When Root Elosc Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config

04_Test_Complex_Filters    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Unauthorized processes can not publish blacklisted events

    Given A Filter To Authorize Elosc As Non Root Is Set
    When Root Elosc Tries To Publish A Blacklisted Event
    Then A Security Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
A Filter To Authorize Elosc As Root Is Set
    [Documentation]    Set a    authorized process filter to allow elosc as root

    Stop Elosd
    Wait For Elosd To Stop

    ${CONFIG}    Default Config Core
    ${CONFIG}    Update Value To Json
    ...          ${CONFIG}    $..LocalTcpClient.Config.EventBlacklist    ${BLACKLIST_FILTER}
    ${CONFIG}    Update Value To Json
    ...          ${CONFIG}    $..LocalTcpClient.Config.authorizedProcesses
    ...          ${ALLOW_ELOSC_AS_ROOT}

    Set Config From Template    &{CONFIG}

    Start Elosd
    Wait Till Elosd Is Started

A Filter To Authorize Elosc As Non Root Is Set
    [Documentation]    Set authorized process filter to allow elosc as non root

    Set Test Variable    ${AUTHORIZE_ROOT}    ${false}
    Ensure Elosd Is Stopped
    ${CONFIG}    Default Config Core
    ${CONFIG}    Update Value To Json
    ...          ${CONFIG}    $..LocalTcpClient.Config.EventBlacklist    ${BLACKLIST_FILTER}
    ${CONFIG}    Update Value To Json
    ...          ${CONFIG}    $..LocalTcpClient.Config.authorizedProcesses
    ...          ${ALLOW_ELOSC_AS_NON_ROOT}

    Set Config From Template    &{CONFIG}
    Ensure Elosd Is Started

Root Elosc Tries To Publish A Blacklisted Event
    [Documentation]    An authorized elos client tries to publish a blacklisted event

    ${output}    ${error}     ${rc}    Publish '{"messageCode": 2010}'

    IF    ${AUTHORIZE_ROOT}
        Executable Returns No Errors
        ...    ${rc}
        ...    Authorized client unable to publish blacklisted event
    ELSE
        Executable Returns An Error    ${rc}
    END

Root Elosc Tries To Publish A Normal Event
    [Documentation]    An process tries to publish a normal event

    ${output}    ${error}    ${rc}    Publish '{"messageCode": 150}'
    Executable Returns No Errors    ${rc}    Client unable to publish normal event

Blacklisted Event Is Published
    [Documentation]    Blacklisted event will be published from authorized clients

    Latest Events Matching '.event.messageCode 2010 EQ' Found

Event Is Published
    [Documentation]    Event not blacklisted will be published from authorized clients.

    Latest Events Matching '.event.messageCode 150 EQ' Found

A Security Event Is Published
    [Documentation]    Security event will be published if an unauthorized client tries to publish
    ...    a blacklisted event.

    Latest Events Matching '.event.messageCode 8007 EQ' Found
