*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if publish and poll of messages between
...                 elos daemon and clients subscribed to it behave as intended.

Library             String
Library             SSHLibrary
Resource            resources/keywords.resource
Library             libraries/ElosKeywords.py
Library             libraries/ElosEventKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${CONNECTION_URI}               unix:///tmp/elosd/elosd.socket
${FILTER}                       .event.source.appName \'publish_poll\' STRCMP
${MESSAGE_COUNT}                10
${CLIENT_COUNT}                 2
@{EVENTS}                       @{EMPTY}
@{PUBLISH_LOG}                  @{EMPTY}
@{SUBSCRIPTIONS}                @{EMPTY}


*** Test Cases ***
Client Logs Published Message
    [Documentation]    If published message matches a filter subscribed to by client
    ...    then it is logged by the client

    Given '${CLIENT_COUNT}' Elos Clients Subscribe To '${FILTER}'
    When An Event Matching The Filter Is Published
    Then All Clients Receive The Events Subscribed To


*** Keywords ***
Create Messages
    [Documentation]    Create multiple messages with message template
    FOR    ${i}    IN RANGE    1001    1001+${MESSAGE_COUNT}
        ${date}=     Get Current Time From Target
        ${event}=    Get Default Event
        ${event}=    Set '${date}' As '${event}' Date
        ${event}=    Set '${i}' As '${event}' Message Code
        ${event}=    Set 'publish_poll' As '${event}' App Name
        ${event_json}=    Get '${event}' As Json String
        Append To List    ${EVENTS}    ${event_json}
    END
    Log List    ${EVENTS}

'${N}' Elos Clients Subscribe To '${filter}'
    [Documentation]    Start 'N' number of elos clients which subscribe to
    ...                events matching given filter
    FOR    ${i}    IN RANGE    ${N}
        ${subscription}=    Subscribe To '${filter}' Via '${CONNECTION_URI}'
        Append To List    ${SUBSCRIPTIONS}    ${subscription}
        Log    Started client ${i}
    END

An Event Matching The Filter Is Published
    [Documentation]    Publish Created Messages matching a RPN-filter
    Run Keyword    Create Messages
    FOR    ${event}    IN    @{EVENTS}
        ${publish_output}=    Publish '${event}' Via '${CONNECTION_URI}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}

All Clients Receive The Events Subscribed To
    [Documentation]    All clients find all the events they have subscribed to
    FOR    ${subscription}    IN    @{SUBSCRIPTIONS}
        Client With '${subscription}' Receives Event
    END
