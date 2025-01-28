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
${FILTER}                       .event.source.appName \'publish_poll\' STRCMP
${MESSAGE_COUNT}                10
${CLIENT_COUNT}                 2
@{EVENTS}                       @{EMPTY}
@{PUBLISH_LOG}                  @{EMPTY}


*** Test Cases ***
Client Logs Published Message
    [Documentation]    If published message matches a filter subscribed to by client
    ...    then it is logged by the client

    Given Elos Client Subscription Started
    When A Subscribed Event Is Published
    Then Client Receives Event Subscribed To


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

Elos Client Subscription Started
    [Documentation]    Start given number of elos clients
    FOR    ${count}    IN RANGE    ${CLIENT_COUNT}
        ${output}=    Subscribe To '${FILTER}'
        Log    Started client ${count}
    END

A Subscribed Event Is Published
    [Documentation]    Publish Created Messages matching a RPN-filter
    Run Keyword    Create Messages
    FOR    ${event}    IN    @{EVENTS}
        ${publish_output}=    Publish '${event}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}
    Sleep    1s
