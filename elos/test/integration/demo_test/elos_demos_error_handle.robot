# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check if the elos demo applications handle error
...               as expected when elosd is not running in target

Resource          ../elosd-keywords.resource
Resource          ../keywords.resource

Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***
${ELOSD}    /usr/bin/elosd
@{ELOS_DEMOS}    elosc -s ".event.Source.appName test STRCMP"
...              demo_libelos_v2
...              tinyElosc -p "{}"
...              syslog_example

*** Test Cases ***
Expected Error Handling By Demo Apps
    [Documentation]    Demo Applications handle errors as expected
    [Setup]            Stop Elosd
    [Teardown]         Start Elosd

    FOR    ${app}    IN    @{ELOS_DEMOS}
        Given Elosd Is Stopped
        When Demo App Is Started     ${app}
        Then App Handles Error
    END

*** Keywords ***
Elosd Is Stopped
    [Documentation]    Check if elosd is really stopped

    ${output}=     Execute Command    pgrep elosd
    Should Be Empty    ${output}

Demo App Is Started
    [Arguments]        ${app}
    [Documentation]    Run the given elos demo application

    ${output}    ${error}    ${rc}=         Execute Command    ${app}    return_stderr=True    return_rc=True

    Log    ${app}
    Log    ${output}
    Log    ${error}
    Log    ${rc}

    Set Test Variable    ${APP}           ${app}
    Set Test Variable    ${ERROR}         ${error}
    Set Test Variable    ${ERROR_CODE}    ${rc}

App Handles Error
    [Documentation]    App returns error on connection error

    Should Be Equal As Integers    ${ERROR_CODE}    1
    Should Contain Any    ${ERROR}    ERROR: connect to    ERR: connect to port
