*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to verify elos runtime directory configuration
...                 works as expected.

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Resource            resources/config.resource
Library             libraries/ElosKeywords.py
Library             JSONLibrary

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Test Cases ***
Set RunDir in elosd config
    [Documentation]    Check if RunDir can be changed through elosd.json

    ${Config}=    Default Config Core
    ${Config}=    Add Object To Json      ${Config}    $.RunDir    /tmp/elosd_itest
    ${Config}=    Add Object To Json      ${Config}
    ...           $.Scanner.Plugins.KmsgScanner.Config.KmsgStateFile
    ...           /tmp/elosd_itest/kmsg.state

    Given Ensure Elosd Is Running With New Config    &{Config}
    Then Directory '/tmp/elosd_itest' Should Exist On Target
    And File '/tmp/elosd_itest/kmsg.state' Should Exist On Target

    [Teardown]    Reset Elosd Config

Use default RunDir
    [Documentation]    Check if default RunDir works, if not configured.

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $.RunDir
    ${Config}=    Delete Object From Json    ${Config}
    ...           $.Scanner.Plugins.KmsgScanner.Config.KmsgStateFile

    Given Ensure Elosd Is Running With New Config    &{Config}
    Then Directory '/run/elosd/' Should Exist On Target
    And File '/run/elosd/kmsg.state' Should Exist On Target

    [Teardown]    Reset Elosd Config

Set RunDir to an invalid value
    [Documentation]    Verify behavior if RunDir is set to an invalid value.

    ${Config}=    Default Config Core
    ${Config}=    Add Object To Json      ${Config}    $.RunDir    invalid/path
    ${Config}=    Add Object To Json      ${Config}
    ...           $.Scanner.Plugins.KmsgScanner.Config.KmsgStateFile
    ...           /tmp/kmsg.state

    Given Fails To Start Elosd With New Config    &{Config}
    Then Check Elosd Log For    ERROR: Failed to setup run directory

    [Teardown]    Reset Elosd Config


*** Keywords ***
Fails To Start Elosd With New Config
    [Documentation]     Set a new Config and restart Elosd
    [Arguments]    &{Config}

    Ensure Elosd Is Stopped
    Set New Config Core    &{Config}
    Start Elosd
    Wait For Elosd To Stop

Ensure Elosd Is Running With New Config
    [Documentation]    Set a new Config and restart Elosd
    [Arguments]    &{Config}

    Ensure Elosd Is Stopped
    Set New Config Core    &{Config}
    Ensure Elosd Is Started
