*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if all published events are
...                 stored and specific events can be retrieved by elos client.

Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py
Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${FILTERSTRING}     .event.messageCode 1400 EQ
@{MESSAGES}         {"messageCode": 1004,"payload":"testEventFiltering"}
...                 {"messageCode": 1040,"payload":"testEventFiltering"}
...                 {"messageCode": 1400,"payload":"testEventFiltering"}
${SEARCH_STRING}    testEventFiltering
@{PUBLISH_LOG}      @{EMPTY}


*** Test Cases ***
Published Events Are Stored And Retrieved
    [Documentation]    Any published message can be retrieved using a given
    ...    filter string

    Given Elosd Is Running
    When An Event Is Published
    Then Client Retrieves It Successfully


*** Keywords ***
An Event Is Published
    [Documentation]    Publish Created Messages

    FOR    ${message}    IN    @{MESSAGES}
        ${publish_output}=    Publish '${message}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}
    Sleep    1s

Client Retrieves It Successfully
    [Documentation]    Latest events matching filter string are found by the client

    ${output}    ${error}    ${rc}=    Find Events Matching '${FILTERSTRING}'

    Should Be Equal As Integers    ${rc}    0
    Should Contain    ${output}    ${SEARCH_STRING}
