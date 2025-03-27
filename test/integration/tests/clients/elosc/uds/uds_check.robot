*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if existing elosd unix socket is
...                 accessible by all users.

Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py
Library             libraries/TemplateConfig.py
Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Initialize Variables
...                 AND             Ensure Elosd Is Started

Suite Teardown      Close All Connections


*** Variables ***
${UNIX_SOCKET}            /tmp/elosd/elosd.socket
${PERMISSIONS}            ${EMPTY}
${REQUIRED_PERMISSION}    777


*** Test Cases ***
Unix Socket Accessible By All Users
     [Documentation]     default elosd unix domain socket is accessible by all users.

     Given Elosd Is Running
     When '${UNIX_SOCKET}' Permissions Are Read
     Then Socket Is Accessible By All Users


*** Keywords ***
Initialize Variables
     [Documentation]    Initialize variables of the test suite.

     ${socket_path}=
     ...    Get Option '$.root.elos.ClientInputs.Plugins.unixClient.Config.path' From Target Config
     Set Suite Variable    ${UNIX_SOCKET}    ${socket_path}

'${socket}' Permissions Are Read
     [Documentation]    Read permissions of given socket.

     ${permissions}=    Read '${socket}' Permissions

     Set Suite Variable    ${PERMISSIONS}    ${permissions}

Socket Is Accessible By All Users
     [Documentation]   check if socket permissions is 777

     Should Be Equal As Strings    ${PERMISSIONS}    ${REQUIRED_PERMISSION}
