*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check elos behaviour when elos configuration
...                 is changed.

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Resource            resources/config.resource
Library             JSONLibrary
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Test Cases ***
Test New Valid Elosd Configuration
    [Documentation]    Set new valid config for elosd

    ${Config}=    Valid Configuration
    Restart Elosd With Config From Template    &{Config}
    Elosd Is Running
    [Teardown]    Run Keywords    Stop Elosd
    ...    AND    Wait For Elosd To Stop
    ...    AND    Cleanup Template Config
    ...    AND    Start Elosd

Test New Invalid Elosd Configuration
    [Documentation]    Set new invalid config for elosd

    ${Config}=    Invalid Configuration
    Stop Elosd
    Wait For Elosd To Stop
    Set Config From String    ${Config}
    Start Elosd
    Elosd Is Stopped
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test New Elosd Configuration Missing Plugin Filter
    [Documentation]    Set new config that is missing
    ...    the plugin filter for the json backend
    ...    of elosd

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..JsonBackend.Filter

    Restart Elosd With Config From Template    &{Config}
    Stop Elosd And Check Log For    WARNING: No filter rules for JsonBackend
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test New Elosd Configuration Empty Plugin Filter
    [Documentation]    Set new config that has an empty list
    ...    as plugin filter for the json backend
    ...    of elosd

    ${Config}=    Default Config Core
    ${Config}=    Update Value To Json    ${Config}    $..JsonBackend.Filter    []
    Set Test Variable    ${LOGLEVEL}    DEBUG

    Restart Elosd With Config From Template    &{Config}    LogLevel=${LOGLEVEL}
    Stop Elosd And Check Log For    Found 0 filter for JsonBackend
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test New Elosd Configuration Integer As Plugin Filter
    [Documentation]    Set new config that has an integer instead
    ...    of a list as plugin filter for the
    ...    json backend of elosd

    ${Config}=    Default Config Core
    ${Config}=    Update Value To Json    ${Config}    $..JsonBackend.Filter    ${2}
    Set Test Variable    ${LOGLEVEL}    DEBUG

    Restart Elosd With Config From Template    &{Config}    LogLevel=${LOGLEVEL}
    Stop Elosd And Check Log For    Found 0 filter for JsonBackend
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test New Elosd Configuration Filter String Instead Of Filter List
    [Documentation]    Set new config that has a filter string instead
    ...    of a list of filter strings as plugin filter for the
    ...    json backend of elosd

    ${Config}=    Default Config Core
    ${Config}=    Update Value To Json    ${Config}    $..JsonBackend.Filter    1 1 EQ
    Set Test Variable    ${LOGLEVEL}    DEBUG

    Restart Elosd With Config From Template    &{Config}    LogLevel=${LOGLEVEL}
    Stop Elosd And Check Log For    Found 0 filter for JsonBackend
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test New Elosd Configuration With Different Interface
    [Documentation]    Set new config that sets the Interface and Port
    ...    to 127.0.0.1:6666

    Set Test Variable    ${INTERFACE}    127.0.0.1
    Set Test Variable    ${PORT}    ${6666}
    Set Test Variable    ${LOGLEVEL}    DEBUG

    ${Config}=    Default Config Core
    ${Config}=    Update Value To Json
    ...           ${Config}    $..LocalTcpClient.Config.Interface    ${INTERFACE}
    ${Config}=    Update Value To Json    ${Config}    $..LocalTcpClient.Config.Port    ${PORT}
    ${Config}=    Update Value To Json    ${Config}    $..LogLevel    ${LOGLEVEL}

    Restart Elosd With Config From Template
    ...    &{Config}
    Stop Elosd And Check Log For    listen on: ${INTERFACE}:${PORT}
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd


*** Keywords ***
Stop Elosd And Check Log For
    [Documentation]    Stop elosd and check its log for the given string
    [Arguments]    ${part}    ${ignore_case}=False

    Stop Elosd
    Wait For Elosd To Stop
    Check Elosd Log For    ${part}    ${ignore_case}

Restart Elosd With Config From Template
    [Documentation]    Set a new Config with Template and restart Elos
    [Arguments]    &{Config}

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template    &{Config}
    Start Elosd
    Elosd Is Running
