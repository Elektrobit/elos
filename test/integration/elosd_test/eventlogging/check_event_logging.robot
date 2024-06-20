*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check direct interaction with elosd

Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource

Library             ../../libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${ELOSD_VERSION}            1
${ELOSD_PUBLISH_CODE}       2
${ELOSD_QUERY_CODE}         4
${ELOSD_PUBLISH_MESSAGE}    {"messageCode": 4,"payload":"message from tcp published"}
${ELOSD_QUERY}              {"filter":".event.messageCode 4 EQ"}


*** Test Cases ***
Test Publish And Retrieve Data
    [Documentation]    Send a valid message via a direct tcp connection
    ...    to target elosd and it shall publish it.

    Connect To Elosd
    Send Message To Elosd    ${ELOSD_VERSION}    ${ELOSD_PUBLISH_CODE}    ${ELOSD_PUBLISH_MESSAGE}
    No Error Received
    Retrieve Published Message
    Disconnect From Elosd

Test Elosd Data Persistence
    [Documentation]    A published message in elosd in remains
    ...    logged upon elosd restart.

    Connect To Elosd
    Send Message To Elosd    ${ELOSD_VERSION}    ${ELOSD_PUBLISH_CODE}    ${ELOSD_PUBLISH_MESSAGE}
    No Error Received
    Retrieve Published Message
    Disconnect From Elosd
    Restart Elosd
    Connect To Elosd
    Retrieve Published Message
    Disconnect From Elosd


*** Keywords ***
No Error Received
    [Documentation]    Message received from elosd is not an error

    ${message}=    Receive Message From Elosd
    Should Contain    ${message}    "error":null

Retrieve Published Message
    [Documentation]    Use given query to retrieve published event from    elosd Target

    Send Message To Elosd    ${ELOSD_VERSION}    ${ELOSD_QUERY_CODE}    ${ELOSD_QUERY}
    ${response}=    Receive Message From Elosd
    Log    ${response}
    Should Contain    ${response}    message from tcp published
