*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if client with invalid filter
...                 is not able to retrieve published messages

Resource            resources/keywords.resource
Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
@{MESSAGES}         {"messageCode": 1004,"payload":"testEventFiltering"}
...                 {"messageCode": 1040,"payload":"testEventFiltering"}
...                 {"messageCode": 1400,"payload":"testEventFiltering"}
${SEARCH_STRING}    event find failed
@{PUBLISH_LOG}      @{EMPTY}


*** Test Cases ***
Published Events Are Not Retrieved By Client With Invalid Filter
    [Documentation]    Published message can not be retrieved using an invalid
    ...    filter string. Client with invalid filter does not
    ...    influence elosd.
    [Template]    Invalid Filter Test

    ${SPACE}
    dsp9imjrewwnufxp98jrewuuuuuuwrevgf
    .event.Source.appName 'hugo' STRCMP
    .event.source.appName 'hugo' EQ
    .event.source.appName 'hugo' LT
    .event.Source.appName hugo STRCMP
    .event.definitely.not.existing.field 'hugo' STRCMP


*** Keywords ***
Invalid Filter Test
    [Documentation]    test template to check different types of invalid filters.
    [Arguments]    ${filter}

    When An Event Is Published
    Then Client Fails To Retrieve Event With    ${filter}

An Event Is Published
    [Documentation]    Publish Created Messages

    FOR    ${message}    IN    @{MESSAGES}
        ${publish_output}=    Publish '${message}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}
    Sleep    1s

Client Fails To Retrieve Event With
    [Documentation]    Client with invalid filter string does not retrieve published messages
    [Arguments]    ${filter}

    ${output}    ${error}    ${rc}=    Find Events Matching '${filter}'

    Should Be Equal As Integers    ${rc}    1
    Should Contain    ${error}    ${SEARCH_STRING}
