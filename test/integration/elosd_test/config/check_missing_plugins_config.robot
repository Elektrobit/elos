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

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Test Cases ***
Test Elosd Configuration with missing Plugins Lists
    [Documentation]    Set new config has no "Plugins" list
    ...                for "ClientInputs" or "EventLogging"

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..Plugins
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    INFO

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    ClientManager loaded 0 client plugins
    Check Elosd Log For    StorageManager loaded 0 storage plugins
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test Elosd Configuration With Empty Plugins Lists
    [Documentation]    Set new config has an empty "Plugins" list
    ...                for "ClientInputs" or "EventLogging"

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..Plugins.*
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    INFO

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    ClientManager loaded 0 client plugins
    Check Elosd Log For    StorageManager loaded 0 storage plugins
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd


Test Elosd With Missing ClientInputs Configuration
    [Documentation]    Set new config has no ClientInputs

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..ClientInputs
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    DEBUG

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    DEBUG: No ClientInputs configured
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test Elosd With Missing StorageManager Configuration
    [Documentation]    Set new config has no StorageManager

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..EventLogging
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    DEBUG

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    DEBUG: No EventLogging configured
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test Elosd Configuration With Empty EventLogging Configuration
    [Documentation]    Set new config with empty EventLogging part

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..EventLogging.*
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    INFO

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    StorageManager loaded 0 storage plugins
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Check Elosd Log For    StorageManager loaded 0 storage plugins
    ...    AND    Start Elosd


Test Elosd Configuration With Empty ClientInputs Configuration
    [Documentation]    Set new config with empty ClientInputs part

    ${Config}=    Default Config Core
    ${Config}=    Delete Object From Json    ${Config}    $..ClientInputs.*
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    INFO

    Ensure Elosd Is Running With New Config    &{Config}

    Ensure Elosd Is Stopped
    Check Elosd Log For    ClientManager loaded 0 client plugins
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd


Test Elosd With Wrong StorageManager Configuration
    [Documentation]    Set new config with wrong type for EventLogging

    ${Config}=    Default Config Core
    ${Config}=    Update Value To Json      ${Config}    $..EventLogging    4
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    ERROR

    Fails to Start Elosd With New Config     &{Config}

    Check Elosd Log For    ERROR: elosStorageManagerInitialize had errors during execution
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

Test Elosd With Wrong ClientManager Configuration
    [Documentation]    Set new config with wrong type for StorageManager

    ${Config}=    Default Config Core
    ${Config}=    Update Value To Json      ${Config}    $..ClientInputs    4
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    ERROR

    Fails To Start Elosd With New Config     &{Config}

    Check Elosd Log For    ERROR: elosClientManagerInitialize had errors during execution
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd

  
Test Elosd Configuration With Wrong Plugins Lists Type
    [Documentation]    Set new config has wrong "Plugins" list type
    ...                for "ClientInputs" or "EventLogging"

    ${Config}=    Default Config Core
    ${Config}=    Update Value To Json      ${Config}    $..Plugins        4
    ${Config}=    Update Value To Json      ${Config}    $..LogLevel    ERROR

    Fails to Start Elosd With New Config     &{Config}

    Check Elosd Log For    configuration is not in a valid format
    [Teardown]    Run Keywords    Cleanup Template Config
    ...    AND    Start Elosd


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
