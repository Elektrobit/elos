*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       Collection of high level keywords shared by the test cases

Library             String
Library             Collections
Library             SSHLibrary


*** Variables ***
${EXIT_SUCCESS}         0
${TARGET}               ${TARGET_IP}
${SSH_PORT}             22
${USERNAME}             ${TARGET_USER}
${PASSWORD}             ${TARGET_PASSWORD}
${RETRY_TIME}           2m
${RETRY_INTERVAL}       5s
${ERROR_MSG0}           System start-up is not finished
${TIMEOUT}              20s
${SYSTEM_MANAGER}       ${TARGET_SYS_MANAGER}
${USER_IS_ROOT}         ${TARGET_USER_IS_ROOT}

# Variables for Execute And Log
${RETURN_RC}            rc
${RETURN_STDOUT}        stdout
${RETURN_STDERR}        stderr


*** Keywords ***
Connect To Target And Log In
    [Documentation]    Opens a new SSH connection to the target system.
    ...
    ...    It uses the following global variables for establishing the target connection:
    ...
    ...    | \${TARGET}    | The host name or IP address of the target system |
    ...    | \${USERNAME} | The remote user name, e.g. ``root`` |
    ...    | \${PASSWORD} | The remote user's password |
    ...
    ...    | \${LINUX_VERSION} | Used for storing the used Linux version. |

    Open Connection    ${TARGET}    timeout=${TIMEOUT}    port=${SSH_PORT}
    Wait Until Keyword Succeeds    ${RETRY_TIME}    ${TIMEOUT}
    ...    Login    ${USERNAME}    ${PASSWORD}

Target IP Address Is Valid
    [Documentation]    Checks that TARGET_IP variable is set
    ...
    ...    and contains a valid IPv4/6 adress.
    Variable Should Exist    ${TARGET}
    ${valid}=    Is Valid IP    ${TARGET}
    Should Be True    ${valid}    msg=Invalid target IP address

Execute And Log
    [Documentation]    Runs Execute Command on remote, logs stdout, stderr and return code and
    ...    returns the specified options to the caller
    [Arguments]    ${command}    @{options}

    ${stdout}    ${stderr}    ${rc}=    Execute Command
    ...    ${command}
    ...    return_stdout=true
    ...    return_stderr=true
    ...    return_rc=true

    Log    ${stderr}
    Log    ${stdout}
    Log    ${rc}

    @{return_list}=    Create List

    FOR    ${option}    IN    @{options}
        IF    "${option}"=="${RETURN_STDOUT}"
            Append To List    ${return_list}    ${stdout}
        END
        IF    "${option}"=="${RETURN_STDERR}"
            Append To List    ${return_list}    ${stderr}
        END
        IF    "${option}"=="${RETURN_RC}"
            Append To List    ${return_list}    ${rc}
        END
    END

    ${return_list_length}=    Get Length    ${return_list}
    IF    ${return_list_length}==1    RETURN    ${return_list}[0]
    IF    ${return_list_length}>1    RETURN    @{return_list}

Execute And Log Based On User Permissions
    [Documentation]    Runs Execute Command on remote based on user permission, logs stdout, stderr
    ...    and return code and returns the specified options to the caller
    [Arguments]    ${command}    @{options}

    IF    '${USER_IS_ROOT}' == 'True'
        ${stdout}    ${stderr}    ${rc}=    Execute Command
        ...    ${command}
        ...    return_stdout=true
        ...    return_stderr=true
        ...    return_rc=true
    ELSE
        ${stdout}    ${stderr}    ${rc}=    Execute Command
        ...    ${command}
        ...    sudo=True
        ...    sudo_password=${PASSWORD}
        ...    return_stdout=true
        ...    return_stderr=true
        ...    return_rc=true
    END

    Log    ${stderr}
    Log    ${stdout}
    Log    ${rc}

    @{return_list}=    Create List

    FOR    ${option}    IN    @{options}
        IF    "${option}"=="${RETURN_STDOUT}"
            Append To List    ${return_list}    ${stdout}
        END
        IF    "${option}"=="${RETURN_STDERR}"
            Append To List    ${return_list}    ${stderr}
        END
        IF    "${option}"=="${RETURN_RC}"
            Append To List    ${return_list}    ${rc}
        END
    END

    ${return_list_length}=    Get Length    ${return_list}
    IF    ${return_list_length}==1    RETURN    ${return_list}[0]
    IF    ${return_list_length}>1    RETURN    @{return_list}

Executable Returns No Errors
    [Documentation]    Executed keyword does not return an error
    [Arguments]    ${rc}    ${msg}=None
    Should Be Equal As Integers    ${rc}    ${EXIT_SUCCESS}    ${msg}

Executable Returns An Error
    [Documentation]    Executed keyword returns an error
    [Arguments]    ${rc}
    Should Not Be Equal As Integers    ${rc}    ${EXIT_SUCCESS}
