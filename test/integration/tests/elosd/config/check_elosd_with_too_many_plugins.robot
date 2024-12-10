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
Library             libraries/TemplateConfig.py

Suite Setup         Run Keywords    Connect To Target And Log In
Suite Teardown      Close All Connections


*** Variables ***
${NUM_PLUGINS}      10    # shuld be more than PLUGINMANAGER_PLUGINVECTOR_INIT_SIZE


*** Test Cases ***
Test Elosd Configuration With A Lot Of Plugins
    [Documentation]    test to check that elosd stats corrctly
    ...    even when the number of plugins to load is high

    ${Config}=    Default Config Core
    ${Config}=    Add Many Client Dummies To Config    ${NUM_PLUGINS}    ${Config}
    ${Config}=    Add Many Scanner Dummies To Config    ${NUM_PLUGINS}    ${Config}
    ${Config}=    Add Many Backend Dummies To Config    ${NUM_PLUGINS}    ${Config}
    Ensure Elosd Is Running With New Config    &{Config}

    [Teardown]    Reset Elosd Config


*** Keywords ***
Add Scanner Dummy To Config
    [Documentation]    add a dummy scanner to the config
    [Arguments]    ${key}    ${config}

    ${ScannerDummy}=    Json
    ...    {
    ...    "File": "scanner_dummy.so",
    ...    "Run": "always"
    ...    }
    ${config}=    Add Object To Json    ${config}
    ...    $.Scanner.Plugins.${key}    ${ScannerDummy}
    RETURN    ${config}

Add Client Dummy To Config
    [Documentation]    add a dummy config to the config
    [Arguments]    ${key}    ${config}

    ${ClientDummy}=    Json
    ...    {
    ...    "File": "client_dummy.so",
    ...    "Run": "always",
    ...    "Config": {
    ...    "Port": 543210,
    ...    "Interface": "0.0.0.0"
    ...    }
    ...    }
    ${config}=    Add Object To Json    ${config}
    ...    $.ClientInputs.Plugins.${key}    ${ClientDummy}
    RETURN    ${config}

Add Backend Dummy To Config
    [Documentation]    add a dummy backend to the config
    [Arguments]    ${key}    ${config}

    ${BackendDummy}=    Json
    ...    {
    ...    "File": "backend_dummy.so",
    ...    "Run": "always",
    ...    "Filter": [
    ...    ".event.messageCode 1000 NE"
    ...    ]
    ...    }
    ${config}=    Add Object To Json    ${config}
    ...    $.EventLogging.Plugins.${key}    ${BackendDummy}
    RETURN    ${config}

Add Many Scanner Dummies To Config
    [Documentation]    add a number of dummy scanner to the config
    [Arguments]    ${number}    ${config}

    FOR    ${num}    IN RANGE    0    ${number}
        ${config}=    Add Scanner Dummy To Config    ScannerDummy${num}    ${config}
    END
    RETURN    ${config}

 Add Many Client Dummies To Config
    [Documentation]    add a number of dummy clients to the config
    [Arguments]    ${number}    ${config}

    FOR    ${num}    IN RANGE    0    ${number}
        ${config}=    Add Client Dummy To Config    ClientDummy${num}    ${config}
    END
    RETURN    ${config}

Add Many Backend Dummies To Config
    [Documentation]    add a number of dummy backends to the config
    [Arguments]    ${number}    ${config}

    FOR    ${num}    IN RANGE    0    ${number}
        ${config}=    Add Backend Dummy To Config    BackendDummy${num}    ${config}
    END
    RETURN    ${config}

Ensure Elosd Is Running With New Config
    [Documentation]    Set a new Config and restart Elosd
    [Arguments]    &{Config}

    Ensure Elosd Is Stopped
    Set New Config Core    &{Config}
    Ensure Elosd Is Started
