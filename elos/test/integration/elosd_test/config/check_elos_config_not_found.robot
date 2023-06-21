# SPDX-License-Identifier: MIT
*** Settings ***
Documentation     A test suite to check elos behaviour when  config file is not
...               in the given path

Resource          ../../elosd-keywords.resource
Resource          ../../keywords.resource

Suite Setup       Connect To Target And Log In
Suite Teardown    Close All Connections

*** Variables ***
${INVALID_CONFIG_DIR}    /tmp/elos_empty

*** Test Cases ***
Test Elos Configuration not set
    [Documentation]    When config file not found in given path then
    ...                elosd terminates throwing error as expected
    [Teardown]         Run Keywords     Reset Config File
    ...                AND    Start Elosd

    Stop Elosd
    Remove Config File
    Start Elosd
    Elosd Does Not Start

*** Keywords ***
Remove Config File
    [Documentation]    Point config directory to an empty directory,
    ...                so that elosd does have a config file and fails to start.

    ${rc}=      Execute And Log Based On User Permissions
    ...         sh -c 'mkdir -p ${INVALID_CONFIG_DIR} && mount --bind ${INVALID_CONFIG_DIR} /etc/elos'
    ...         ${RETURN_RC}
    Executable Returns No Errors    ${rc}

Elosd Does Not Start
    [Documentation]    Elosd does not start due to invalid path to
    ...                config file

    ${output}=    Execute And Log    pgrep elosd    ${RETURN_STDOUT}
    Should Be Empty     ${output}

Reset Config File
    [Documentation]    Reset Config file to default path and reset root file system

    ${rc}=      Execute And Log Based On User Permissions
    ...         sh -c 'rm -rf ${INVALID_CONFIG_DIR} && umount /etc/elos/'
    ...         ${RETURN_RC}
    Executable Returns No Errors    ${rc}
