*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check elos behaviour when elos configuration
...                 is changed.

Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource
Resource            ../../config.resource
Library             ../../libraries/ElosKeywords.py
Library             JSONLibrary

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Test Cases ***
Test Elosd Configuration with missing Plugins Lists
    [Documentation]    Set new config has no "Plugins" list
    ...                for "ClientInputs" or "EventLogging"

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..Plugins

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    WARNING: ClientConnections configuration is missing a 'Plugins' list
    Check Elosd Log For    WARNING: StorageBackends configuration is missing a 'Plugins' list 
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test Elosd With Missing ClientInputs Configuration
    [Documentation]    Set new config has no ClientInputs

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..ClientInputs

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    ERROR: Loading clientManager config failed
    Check Elosd Log For    WARNING: elosClientManagerInitialize had errors during execution
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test Elosd With Missing StorageManager Configuration
    [Documentation]    Set new config has no StorageManager

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..EventLogging

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    ERROR: Loading storageManager config failed
    Check Elosd Log For    WARNING: elosStorageManagerInitialize had errors during execution
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd


*** Keywords ***
Ensure Elosd Is Running With New Config
    [Documentation]    Set a new Config and restart Elosd
    [Arguments]    &{Config}

    Ensure Elosd Is Stopped
    Set New Config Core    &{Config}
    Ensure Elosd Is Started
