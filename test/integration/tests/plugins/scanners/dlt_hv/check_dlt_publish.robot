*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to verfiy dlt-scanner behaviour.

Library             SSHLibrary
Library             JSONLibrary
Library             libraries/ElosKeywords.py
Library             libraries/TemplateConfig.py
Resource            resources/config.resource
Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND    Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${DLT_PATH}          /dev/shm/test_dlt_shmem
${DLT_OFFSET}        0x1000
${DLT_SIZE}          1448
${DLT_EVENT_NUMBER}         5


*** Test Cases ***
New Entries In DLT Ring Buffer
    [Documentation]    Check if the dlt scanner finds and stores new dlt events

    Given Elosd Is Started With Dlt Scanner
    And Some Dlt Events Are Queued
    Then Those Events Can Be Fetched

    [Teardown]  Reset Elosd Config


*** Keywords ***
Those Events Can Be Fetched
    [Documentation]    Check if the correct number of dlt events can be fetched

    '${DLT_EVENT_NUMBER}' Latest Events Matching '.e.source.appName "DltHv" STRCMP' Found

Ensure Elosd Is Running With New Config
    [Documentation]    Set a new Config and restart Elosd
    [Arguments]    &{config}

    Ensure Elosd Is Stopped
    Set New Config Core    &{config}
    Ensure Elosd Is Started

Create Dlt Test Buffer
    [Documentation]    Create a dlt ring buffer to use for testing the dlt scanner

    Execute And Log Based On User Permissions
    ...     mng_dlt_buffer -c 10 -f "${DLT_PATH}" -o "${DLT_OFFSET}" -s "${DLT_SIZE}" -e
    ...     ${RETURN_STDOUT}

Some Dlt Events Are Queued
    [Documentation]    Add some dlt events into the shared ring buffer

    FOR    ${idx}    IN RANGE    ${DLT_EVENT_NUMBER}
        Execute And Log Based On User Permissions
        ...     mng_dlt_buffer -t -f "${DLT_PATH}" -o "${DLT_OFFSET}"
        ...     ${RETURN_STDOUT}
    END

Enable Dlt Scanner In Elosd Config
    [Documentation]    Add a configuration for the dlt scanner plugin and restart elosd

    ${config}=    Default Config Core
    ${DltScannerConfig}=    Json
    ...    {
    ...    "File": "scanner_dlt_hv.so",
    ...    "Run": "always",
    ...    "Config": {
    ...    "DeviceFile": "${DLT_PATH}",
    ...    "OffsetAddress": "${DLT_OFFSET}",
    ...    "BufferSize": "${DLT_SIZE}"
    ...    }
    ...    }
    ${config}=    Add Object To Json    ${config}
    ...    $.Scanner.Plugins.DltHv    ${DltScannerConfig}
    Ensure Elosd Is Running With New Config    &{config}

Elosd Is Started With Dlt Scanner
    [Documentation]    Create a shared dlt ring buffer
    ...                and enable the dlt scanner in the elosd config

    Create Dlt Test Buffer
    Enable Dlt Scanner In Elosd Config
