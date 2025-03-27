*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if client with invalid filter
...                 is not able to retrieve published messages via unix
...                 sockets.

Resource            resources/keywords.resource
Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py
Library             libraries/TemplateConfig.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Initialize Variables
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${CONNECTION_URI}    unix:///tmp/elosd/elosd.socket
@{MESSAGES}         {"messageCode": 1004,"payload":"testEventFiltering"}
...                 {"messageCode": 1040,"payload":"testEventFiltering"}
...                 {"messageCode": 1400,"payload":"testEventFiltering"}
${SEARCH_STRING}    event find failed
@{PUBLISH_LOG}      @{EMPTY}


*** Test Cases ***
Finding Matching Events With Invalid Filter Fails
    [Documentation]    Finding matching events with an invalid filter will
    ...                fail.
    [Template]    Invalid Filter Test

    ${SPACE}
    dsp9imjrewwnufxp98jrewuuuuuuwrevgf
    .event.Source.appName 'hugo' STRCMP
    .event.source.appName 'hugo' EQ
    .event.source.appName 'hugo' LT
    .event.Source.appName hugo STRCMP
    .event.definitely.not.existing.field 'hugo' STRCMP


*** Keywords ***
Initialize Variables
     [Documentation]    Initialize variables of the test suite.

     ${socket_path}=
     ...    Get Option '$.root.elos.ClientInputs.Plugins.unixClient.Config.path' From Target Config
     Set Suite Variable    ${CONNECTION_URI}    unix://${socket_path}

Invalid Filter Test
    [Documentation]    test template to check if finding matching events with
    ...                given invalid filters fail.
    [Arguments]    ${filter}

    When An Event Is Published
    Then Finding Events Matching '${filter}' Fails

An Event Is Published
    [Documentation]    Publish Created Messages

    FOR    ${message}    IN    @{MESSAGES}
        ${publish_output}=    Publish '${message}' Via '${CONNECTION_URI}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}

Finding Events Matching '${filter}' Fails
    [Documentation]    Finding events matching invalid 'filter' argument fails

    ${output}    ${error}    ${rc}=    Find Events Matching '${filter}' Via '${CONNECTION_URI}'

    Should Be Equal As Integers    ${rc}    1
    Should Contain    ${error}    ${SEARCH_STRING}
