*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check direct interaction with elosd

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${ELOSD_VERSION}            1
${ELOSD_PUBLISH_CODE}       2
${ELOSD_VALID_MESSAGE}      {"messageCode": 4,"payload":"testEventFiltering"}
${ELOSD_INVALID_MESSAGE}    {"messageCode": 4"payload":"testEventFiltering"}


*** Test Cases ***
Test Elosd With Valid Data
    [Documentation]    Send a valid message via a direct tcp connection
    ...    to target elosd and no error is received.

    Connect To Elosd
    Send Message To Elosd    ${ELOSD_VERSION}    ${ELOSD_PUBLISH_CODE}    ${ELOSD_VALID_MESSAGE}
    No Error Received
    Disconnect From Elosd

Test Elosd With Invalid Data
    [Documentation]    Send a invalid message via a direct tcp connection
    ...    to target elosd and an error is received as expected..

    Connect To Elosd
    Send Message To Elosd    ${ELOSD_VERSION}    ${ELOSD_PUBLISH_CODE}    ${ELOSD_INVALID_MESSAGE}
    Error Received
    Disconnect From Elosd


*** Keywords ***
No Error Received
    [Documentation]    Message received from elosd is not an error

    ${message}=    Receive Message From Elosd
    Should Contain    ${message}    "error":null

Error Received
    [Documentation]    Message received from elosd is an error

    ${error}=    Receive Message From Elosd
    Should Contain    ${error}    "error":"elosEventDeserialize failed"
