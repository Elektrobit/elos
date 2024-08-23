*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if publish and poll of messages between
...                 elos daemon and clients subscribed to it behave as intended.

Library             String
Library             SSHLibrary
Resource            resources/keywords.resource
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${CLIENT_LOG_FILE}              /tmp/client_$X.log
${FILTERSTRING}                 ".event.source.appName 'publish_poll' STRCMP"
${MESSAGE_COUNT}                10
${CLIENT_COUNT}                 2
@{MESSAGES}                     @{EMPTY}
@{SUBSCRIBED_CLIENTS_LOG}       @{EMPTY}
@{PUBLISH_LOG}                  @{EMPTY}
&{ATTRIBUTES}                   &{EMPTY}
${MESSAGE_TEMPLATE}
...                             {
...                             "date": [$DATE,0],
...                             "source": {
...                             "appName": "publish_poll",
...                             "fileName": "$ELOSC_FILE_NAME",
...                             "pid": 42
...                             },
...                             "severity": 1,
...                             "hardwareid": "$HOSTNAME",
...                             "classification": $CLASSIFICATION,
...                             "messageCode": $MESSAGE_CODE,
...                             "payload": "$PAYLOAD"
...                             }


*** Test Cases ***
Client Logs Published Message
    [Documentation]    If published message matches a filter subscribed to by client
    ...    then it is logged by the client

    Given Elos Clients Started And Listening
    When Message Published Matching A Subscribed Filter
    Then Message Logged By Listening Clients
    [Teardown]    Run Keyword    Remove Temporary Test Files


*** Keywords ***
Remove Temporary Test Files
    [Documentation]    Remove all test related temp files

    ${output}    ${rc}=    Execute Command    rm -rf /tmp/client_*.log    return_rc=True
    Should Be Equal As Integers    ${rc}    0

Set Message Attributes
    [Documentation]    Create a dictionary with specific message attributes
    [Arguments]    ${msg_code}

    ${date}=    Execute Command    date +"%s"
    Set To Dictionary    ${ATTRIBUTES}    date    ${date}
    Set To Dictionary    ${ATTRIBUTES}    appName    "publish_poll"
    ${filename}=    Execute Command    which elosc
    ${filename}=    Replace String Using Regexp    ${filename}    \/    \\\/
    Set To Dictionary    ${ATTRIBUTES}    filename    ${filename}
    Set To Dictionary    ${ATTRIBUTES}    pid    42
    Set To Dictionary    ${ATTRIBUTES}    severity    1
    ${hostname}=    Execute Command    hostname
    Set To Dictionary    ${ATTRIBUTES}    hardwareid    ${hostname}
    ${classification}=    Execute Command    printf %u 0x0000BEEFCAFFEE00
    Set To Dictionary    ${ATTRIBUTES}    classification    ${classification}
    ${message_code}=    Convert To String    ${msg_code}
    Set To Dictionary    ${ATTRIBUTES}    messageCode    ${message_code}
    ${payload}=    Set Variable    test message
    ${payload}=    Catenate    ${payload}    ${message_code}
    Set To Dictionary    ${ATTRIBUTES}    payload    ${payload}

    Log Dictionary    ${ATTRIBUTES}

Create Message
    [Documentation]    Create a customized message with the message templates
    [Arguments]    ${msg_code}

    Run Keyword    Set Message Attributes    ${msg_code}

    ${message}=    Replace String    ${MESSAGE_TEMPLATE}    $DATE    ${ATTRIBUTES}[date]
    ${message}=    Replace String    ${message}    $ELOSC_FILE_NAME    ${ATTRIBUTES}[filename]
    ${message}=    Replace String    ${message}    $HOSTNAME    ${ATTRIBUTES}[hardwareid]
    ${message}=    Replace String    ${message}    $CLASSIFICATION    ${ATTRIBUTES}[classification]
    ${message}=    Replace String    ${message}    $MESSAGE_CODE    ${ATTRIBUTES}[messageCode]
    ${message}=    Replace String    ${message}    $PAYLOAD    ${ATTRIBUTES}[payload]

    Log    ${message}
    RETURN    ${message}

Create Messages
    [Documentation]    Create multiple messages with message template
    FOR    ${i}    IN RANGE    1001    1001+${MESSAGE_COUNT}
        ${msg}=    Create Message    ${i}
        Append To List    ${MESSAGES}    ${msg}
    END
    Log List    ${MESSAGES}

Get File Name From Index
    [Documentation]    Retutn the filename with given Index
    [Arguments]    ${index}
    ${index_str}=    Convert To String    ${index}
    ${file}=    Replace String    ${CLIENT_LOG_FILE}    $X    ${index_str}

    RETURN    ${file}

Elos Clients Started And Listening
    [Documentation]    Start given number of elos clients
    FOR    ${count}    IN RANGE    ${CLIENT_COUNT}
        ${client_log}=    Get File Name From Index    ${count}
        ${output}=    Execute Command    elosc -s ${FILTERSTRING} > ${client_log} 2>&1 &
        Log    Started client ${count}
    END

Message Published Matching A Subscribed Filter
    [Documentation]    Publish Created Messages matching a RPN-filter
    Run Keyword    Create Messages
    FOR    ${message}    IN    @{MESSAGES}
        ${publish_output}=    Execute Command    elosc -p '${message}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}
    Sleep    1s

Generate Client Logs
    [Documentation]    Copy all client logs into a list
    Run Keyword    Terminate Running Client Instances

    FOR    ${count}    IN RANGE    ${CLIENT_COUNT}
        ${log_files}=    Get File Name From Index    ${count}
        ${message_received}=    Set Variable    0
        WHILE    ${message_received} == 0    limit=5 seconds
            ${client_output}=    Execute Command    cat ${log_files}
            ${ret}=    Get Lines Containing String    ${client_output}    "appName":"publish_poll"
            ${message_received}=    Get Length    ${ret}
        END
        Append To List    ${SUBSCRIBED_CLIENTS_LOG}    ${client_output}
    END
    Log List    ${SUBSCRIBED_CLIENTS_LOG}

Message Logged By Listening Clients
    [Documentation]    A Published message matching a subscribed
    ...    RPN-filter is logged by elos client
    Run Keyword    Generate Client Logs

    FOR    ${log}    IN    @{SUBSCRIBED_CLIENTS_LOG}
        FOR    ${key}    IN    @{ATTRIBUTES.keys()}
            Should Contain    ${log}    ${ATTRIBUTES["${key}"]}
        END
    END

Terminate Running Client Instances
    [Documentation]    Terminate all running client instance after
    ...    each test case run
    ${output}    ${rc}=    Execute Command    pkill elosc    return_rc=True
    Should Be Equal As Integers    ${rc}    0
