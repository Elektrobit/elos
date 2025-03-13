*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to verfiy kmsg-scanner behaviour.

Library             SSHLibrary
Library             libraries/ElosKeywords.py
Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND    Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Test Cases ***
New message in '/dev/kmsg'
    [Documentation]    Check if a new message in /dev/kmsg gets published

    Given Ensure Elosd Is Started
    And 'KmsgScanner' Plugin Is Loaded
    And Subscribe To '.e.source.fileName \'/dev/kmsg\' STRCMP'
    When A Kernel Message Appears
    Then Client Receives Event Subscribed To

    [Teardown]    Unsubscribe All


*** Keywords ***
A Kernel Message Appears
    [Documentation]    Triggers a new message in /dev/kmsg

    Write 'test kmsg' To '/dev/kmsg'
