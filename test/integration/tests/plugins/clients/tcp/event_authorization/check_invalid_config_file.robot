*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check invalid config file

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/TemplateConfig.py
Library             libraries/ElosKeywords.py

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Variables ***
${CONFIG}       Raw Config
...             {
...             "root": {
...             "elos": {
...             "UseEnv": true,
...             "Port": 54321,
...             "Interface": "0.0.0.0",
...             "LogFilter": "",
...             "EventBlacklist": ".event.messageCode 2010 EQ",
...             "authorizedProcesses":,
...             "EventLogging": {
...             "PluginSearchPath": "/usr/lib/x86_64-linux-gnu/elos/backend",
...             "Plugins": {
...             "Dummy": {
...             "File": "backend_dummy.so",
...             "Run": "always"
...             },
...             "SecondDummy": {
...             "File": "backend_dummy.so",
...             "Run": "always"
...             },
...             "SkippedDummy": {
...             "File": "backend_dummy.so",
...             "Run": "never"
...             }
...             }
...             },
...             "Scanner": {
...             "SyslogScanner": {
...             "Syslog": "/dev/log",
...             "MappingRules": {
...             "MessageCodes": {
...             "4000": ".event.source.appName 'ssh' STRCMP",
...             "2000": ".event.source.appName 'crinit' STRCMP",
...             "1000": ".event.source.appName 'login' STRCMP"
...             }
...             }
...             }
...             }
...             }
...             }
...             }
...             }


*** Test Cases ***
01_Test_Invalid_Config_File    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Elosd Does Not Start With Invalid Config File

    Given A Config File Is Invalid
    When Start Elosd
    Then Elosd Is Not Started
    [Teardown]    Reset Elosd Config


*** Keywords ***
A Config File Is Invalid
    [Documentation]    config does not have authorized process filter set

    Ensure Elosd Is Stopped
    Set Config From String    ${CONFIG}

Elosd Is Not Started
    [Documentation]    Elosd Is Not Started, timeout error is expected

    ${error}=    Run Keyword And Expect Error    *    Wait Till Elosd Is Started
    Log    ${error}
