# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check elos behaviour when elos configuration
...               is changed.

Resource          ../../elosd-keywords.resource
Resource          ../../keywords.resource
Resource          ../../config.resource
Library           JSONLibrary

Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***

*** Test Cases ***
Test New Valid Elosd Configuration
    [Documentation]    Set new valid config for elosd
    [Teardown]         Run Keywords     Stop Elosd
    ...                AND    Wait For Elosd To Stop
    ...                AND    Cleanup Template Config
    ...                AND    Start Elosd

    ${Config}=   Valid Configuration
    Restart Elosd With Config From Template         &{Config}
    Elosd Is Running

Test New Invalid Elosd Configuration
    [Documentation]    Set new invalid config for elosd
    [Teardown]         Run Keywords     Cleanup Template Config
    ...                AND    Start Elosd

    ${Config}=   Invalid Configuration
    Stop Elosd
    Wait For Elosd To Stop
    Set Config From String      ${Config}
    Start Elosd
    Elosd Is Stopped

Test New Elosd Configuration Missing Plugin Filter
    [Documentation]    Set new config that is missing
    ...                the plugin filter for the json backend
    ...                of elosd
    [Teardown]         Run Keywords     Cleanup Template Config
    ...                AND    Start Elosd

    ${Config}=      Default Config Core
    ${Config}=      Delete Object From Json      ${Config}   $..JsonBackend.Filter

    Restart Elosd With Config From Template         &{Config}
    Stop Elosd And Check Log For      WARNING: No filter rules for JsonBackend

Test New Elosd Configuration Empty Plugin Filter
    [Documentation]    Set new config that has an empty list
    ...                as plugin filter for the json backend
    ...                of elosd
    [Teardown]         Run Keywords     Cleanup Template Config
    ...                AND    Start Elosd

    ${Config}=      Default Config Core
    ${Config}=      Update Value To Json      ${Config}   $..JsonBackend.Filter       []
    Set Test Variable           ${LogLevel}     DEBUG

    Restart Elosd With Config From Template         &{Config}   LogLevel=${LogLevel}
    Stop Elosd And Check Log For      Found 0 filter for JsonBackend

Test New Elosd Configuration Integer As Plugin Filter
    [Documentation]    Set new config that has an integer instead
    ...                of a list as plugin filter for the
    ...                json backend of elosd
    [Teardown]         Run Keywords     Cleanup Template Config
    ...                AND    Start Elosd

    ${Config}=      Default Config Core
    ${Config}=      Update Value To Json      ${Config}   $..JsonBackend.Filter       ${2}
    Set Test Variable           ${LogLevel}     DEBUG

    Restart Elosd With Config From Template         &{Config}   LogLevel=${LogLevel}
    Stop Elosd And Check Log For      Found 0 filter for JsonBackend

Test New Elosd Configuration Filter String Instead Of Filter List
    [Documentation]    Set new config that has a filter string instead
    ...                of a list of filter strings as plugin filter for the
    ...                json backend of elosd
    [Teardown]         Run Keywords     Cleanup Template Config
    ...                AND    Start Elosd

    ${Config}=      Default Config Core
    ${Config}=      Update Value To Json      ${Config}   $..JsonBackend.Filter       1 1 EQ
    Set Test Variable           ${LogLevel}     DEBUG

    Restart Elosd With Config From Template         &{Config}   LogLevel=${LogLevel}
    Stop Elosd And Check Log For      Found 0 filter for JsonBackend

Test New Elosd Configuration With Different Interface
    [Documentation]    Set new config that sets the Interface and Port
    ...                to 127.0.0.1:6666
    [Teardown]         Run Keywords     Cleanup Template Config
    ...                AND    Start Elosd

    Set Test Variable           ${Interface}    127.0.0.1
    Set Test Variable           ${Port}         ${6666}
    Set Test Variable           ${LogLevel}     DEBUG

    Restart Elosd With Config From Template    Interface=${Interface}   Port=${Port}   LogLevel=${LogLevel}
    Stop Elosd And Check Log For        listen on: ${Interface}:${Port}

*** Keywords ***
Stop Elosd And Check Log For
    [Documentation]     Stop elosd and check its log for the given string
    [Arguments]         ${part}     ${ignore_case}=False

    Stop Elosd
    Wait For Elosd To Stop
    Check Elosd Log For         ${part}     ${ignore_case}

Restart Elosd With Config From Template
    [Documentation]     Set a new Config with Template and restart Elos
    [Arguments]         &{Config}

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template    &{Config}
    Start Elosd
    Elosd Is Running
