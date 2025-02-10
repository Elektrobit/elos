*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test to check that unix client plugin can be loaded successfully.
...

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Resource            resources/config.resource
Library             JSONLibrary
Library             libraries/ElosKeywords.py
Library             libraries/TemplateConfig.py

Suite Setup         Run Keywords    Connect To Target And Log In
Suite Teardown      Close All Connections


*** Test Cases ***
Test Elosd Configuration With unix plugin config
    [Documentation]    test to check that elosd stats correctly

    ${Config}=    Default Config Core
    ${Config}=    Add Unix Client To Config    ${Config}

    Ensure Elosd Is Running With New Config    &{Config}

    [Teardown]    Reset Elosd Config


*** Keywords ***
Add Unix Client To Config
    [Documentation]    add unix client to the config
    [Arguments]    ${config}

    ${ClientDummy}=    Json
    ...    {
    ...    "File": "client_unix.so",
    ...    "Run": "always"
    ...    }
    ${config}=    Add Object To Json    ${config}
    ...    $.ClientInputs.Plugins.unixClient    ${ClientDummy}
    RETURN    ${config}

Ensure Elosd Is Running With New Config
    [Documentation]    Set a new Config and restart Elosd
    [Arguments]    &{Config}

    Ensure Elosd Is Stopped
    Set New Config Core    &{Config}
    Ensure Elosd Is Started
