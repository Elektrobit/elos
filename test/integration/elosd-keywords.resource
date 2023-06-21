# SPDX-License-Identifier: MIT
*** Settings ***
Documentation    Collection of high level keywords for direct interaction with elosd

Resource         keywords.resource

Library          libraries/ElosdLibrary.py
Library          libraries/TemplateConfig.py

*** Variables ***
${ELOSD_PORT}      54321
${ELOS_SERVICE_STOP_RC}    -1

*** Keywords ***
Connect To Elosd
    [Documentation]    connect to instance of elosd

    ${rc}=    Elosd Connect    ${TARGET}    ${ELOSD_PORT}
    Should Be Equal As Integers    ${rc}    0

Disconnect From Elosd
    [Documentation]    disconnect from an instance of elosd

    ${rc}=    Elosd Disconnect
    Should Be Equal As Integers    ${rc}    0

Stop Elosd
    [Documentation]    Stop running instance of elosd

    ${rc}    Execute And Log Based On User Permissions    ${ELOS_STOP_COMMAND}    ${RETURN_RC}
    Sleep    1s

Start Elosd
    [Documentation]    Start an instance of elosd.

    ${rc}    Execute And Log Based On User Permissions    ${ELOS_START_COMMAND}    ${RETURN_RC}
    Executable Returns No Errors   ${rc}    Unable to start an instance of elosd
    Sleep    1s


Send Message To Elosd
    [Arguments]    ${version}    ${messagecode}    ${message}
    [Documentation]    Send given message with version and message code to elosd Target

    Log    ${version}
    Log    ${messagecode}
    Log    ${message}

    ${output}=    Elosd Send    ${version}    ${messagecode}     ${message}
    Log    ${output}

Receive Message From Elosd
    ${output}=     Elosd Receive
    Log    ${output}
    ${received_message}=    Convert To String     ${output}
    RETURN    ${received_message}

Elosd Is Running
    [Documentation]    Elosd is still running

    ${is_running}=    Set Variable    ${false}
    ${running}=    Set Variable    Running

    ${status}    ${rc}    Execute And Log Based On User Permissions    ${ELOS_STATUS_COMMAND}    ${RETURN_STDOUT}    ${RETURN_RC}
    ${is_running}=    Run Keyword And Return Status    Should Contain    ${status}    ${running}    ignore_case=True

    RETURN    ${is_running}

Elosd Is Stopped
    [Documentation]     check if elosd is stopped

    ${is_stopped}=    Set Variable    ${false}
    ${stopped}=    Set Variable    Stopped

    ${status}    ${rc}    Execute And Log Based On User Permissions    ${ELOS_STATUS_COMMAND}    ${RETURN_STDOUT}    ${RETURN_RC}

    ${is_stopped}=    Run Keyword And Return Status    Should Contain    ${status}    ${stopped}    ignore_case=True

    RETURN    ${is_stopped}

Wait For Elosd To Stop
    [Documentation]    Wait for a running elosd instance to stop.
    ...                The wait has a time out of 30 seconds

    ${stopped}=     Elosd Is Stopped

    WHILE   '${stopped}' == '${false}'    limit=30 seconds
        ${stopped}=     Elosd Is Stopped
        Sleep    0.2s
    END

    Log    ${stopped}

Wait Till Elosd Is Started
    [Documentation]    Wait till elosd is started and running.
    ...                The wait has a time out of 30 seconds

    ${started}=     Elosd Is Running

    WHILE   '${started}' == '${false}'    limit=30 seconds
        ${started}=     Elosd Is Running
        Sleep    0.2s
    END

    Log    ${started}

Restart Elosd
    [Documentation]    stop and start an instance of elosd.
    Stop Elosd
    Wait For Elosd To Stop
    Start Elosd
    Wait Till Elosd Is Started

Check Elosd Log For
    [Documentation]    Check the elosd log for specific part
    [Arguments]        ${part}     ${ignore_case}=False

    ${elosd_log}=      Execute And Log    cat ${ELOSD_LOG}     ${RETURN_STDOUT}
    Should Contain    ${elosd_log}    ${part}    ignore_case=${ignore_case}
    #IF    ${ignore_case}
    #    ${rc}=   Execute And Log    grep -i "${part}" ${ELOSD_LOG}      ${RETURN_RC}
    #ELSE
    #    ${rc}=   Execute And Log    grep "${part}" ${ELOSD_LOG}         ${RETURN_RC}
    #END
    #Should Be Equal As Integers    ${rc}    0
