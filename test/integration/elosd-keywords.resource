*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       Collection of high level keywords for direct interaction with elosd

Resource            keywords.resource
Library             libraries/ElosdLibrary.py
Library             libraries/TemplateConfig.py


*** Variables ***
${ELOSD_PATH}      ${ELOS_INSTALL_PREFIX}/elosd
${ELOSC_PATH}      ${ELOS_INSTALL_PREFIX}/elosc
${ELOSD_PORT}      54321
${ELOS_SERVICE_STOP_RC}    -1


*** Keywords ***
Connect To Elosd
    [Documentation]    connect to instance of elosd

    ${rc}    Elosd Connect    ${TARGET}    ${ELOSD_PORT}
    Should Be Equal As Integers    ${rc}    0

Disconnect From Elosd
    [Documentation]    disconnect from an instance of elosd

    ${rc}    Elosd Disconnect
    Should Be Equal As Integers    ${rc}    0

Send Message To Elosd
    [Documentation]    Send given message with version and message code to elosd Target
    [Arguments]    ${version}    ${messagecode}    ${message}

    Log    ${version}
    Log    ${messagecode}
    Log    ${message}

    ${output}    Elosd Send    ${version}    ${messagecode}    ${message}
    Log    ${output}

Receive Message From Elosd
    [Documentation]    Recieve message from    elosd

    ${output}    Elosd Receive
    Log    ${output}
    ${received_message}    Convert To String    ${output}
    RETURN    ${received_message}

Elosd Is Running
    [Documentation]    Elosd is still running

    ${running}    Set Variable    Running

    ${status}    ${rc}    Execute And Log Based On User Permissions    ${ELOS_STATUS_COMMAND}
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}

    ${is_running}    Run Keyword And Return Status
    ...    Should Contain    ${status}    ${running}
    ...    ignore_case=True

    RETURN    ${is_running}

Elosd Is Stopped
    [Documentation]    check if elosd is stopped

    ${stopped}    Set Variable    Stopped

    ${status}    ${rc}    Execute And Log Based On User Permissions    ${ELOS_STATUS_COMMAND}
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}

    ${is_stopped}    Run Keyword And Return Status
    ...    Should Contain    ${status}    ${stopped}
    ...    ignore_case=True

    RETURN    ${is_stopped}

Check Elosd Log For
    [Documentation]    Check the elosd log for specific part
    [Arguments]    ${part}    ${ignore_case}=False

    ${elosd_log}    Execute And Log    cat ${ELOSD_LOG}    ${RETURN_STDOUT}
    Should Contain    ${elosd_log}    ${part}    ignore_case=${ignore_case}
    # IF    ${ignore_case}
    #    ${rc}=    Execute And Log    grep -i "${part}" ${ELOSD_LOG}    ${RETURN_RC}
    # ELSE
    #    ${rc}=    Execute And Log    grep "${part}" ${ELOSD_LOG}    ${RETURN_RC}
    # END
    # Should Be Equal As Integers    ${rc}    0

Get Elos Event Publish Time Threshold
    [Documentation]    Return the threshold event publish time, to check for
    ...    events published after this time.

    ${event_publish_time_threshold}    ${rc}    Execute And Log
    ...    date +%s
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    event publish time threshold can not be set
    Log    ${event_publish_time_threshold}

    RETURN    ${event_publish_time_threshold}
