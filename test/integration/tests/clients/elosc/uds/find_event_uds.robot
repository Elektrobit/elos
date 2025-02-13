*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if elosc is able to find events matching
...                 given filter via unix sockets, when they are published.

Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py
Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${CONNECTION_URI}    unix:///tmp/elosd/elosd.socket
${FILTERSTRING}     .event.messageCode 1400 EQ
@{MESSAGES}         {"messageCode": 1004,"payload":"testEventFiltering"}
...                 {"messageCode": 1040,"payload":"testEventFiltering"}
...                 {"messageCode": 1400,"payload":"testEventFiltering"}
${SEARCH_STRING}    testEventFiltering
@{PUBLISH_LOG}      @{EMPTY}


*** Test Cases ***
Client Finds Events Matching Given Filter
    [Documentation]    Client finds events matching given filter once the
    ...                events are published.

    Given Elosd Is Running
    When An Event Is Published
    Then Client Finds Matching Events


*** Keywords ***
An Event Is Published
    [Documentation]    Publish Created Messages

    FOR    ${message}    IN    @{MESSAGES}
        ${publish_output}=    Publish '${message}' Via '${CONNECTION_URI}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}
    Sleep    1s

Client Finds Matching Events
    [Documentation]    Events matching filter string are found by the client

    ${output}    ${error}    ${rc}=
    ...          Find Events Matching '${FILTERSTRING}' Via '${CONNECTION_URI}'

    Should Be Equal As Integers    ${rc}    0
    Should Contain    ${output}    ${SEARCH_STRING}
