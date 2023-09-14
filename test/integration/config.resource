# SPDX-License-Identifier: MIT
*** Settings ***
Documentation    Collection Configurations to be used in tests.

Library          libraries/TemplateConfig.py
Library          JSONLibrary

*** Variables ***

*** Keywords ***
Valid Configuration
    [Documentation]    A Valid Configuration for elosd

    ${config}=     JSON
    ...     {
    ...         "UseEnv": true,
    ...         "Port": 54322,
    ...         "Interface": "0.0.0.0",
    ...         "EventBlacklist": ".event.messageCode 2000 EQ",
    ...         "authorizedProcesses": [
    ...     	    ".process.uid 0 EQ .process.gid 0 EQ .process.exec '/usr/bin/elosc' STRCMP AND",
    ...     	    ".process.gid 200 EQ .process.exec '/usr/bin/elosc' STRCMP AND",
    ...     	    ".process.pid 1 EQ"
    ...         ],
    ...         "EventLogging": {
    ...             "PluginSearchPath": "/usr/lib/x86_64-linux-gnu/elos/backend",
    ...             "Plugins": {
    ...                 "Dummy": {
    ...                     "File": "backend_dummy.so",
    ...                     "Run": "always"
    ...                 },
    ...                 "SecondDummy": {
    ...                     "File": "backend_dummy.so",
    ...                     "Run": "always"
    ...                 },
    ...                 "SkippedDummy": {
    ...                     "File": "backend_dummy.so",
    ...                     "Run": "never"
    ...                 }
    ...             }
    ...         },
    ...         "Scanner": {
    ...             "SyslogScanner": {
    ...                 "Syslog": "/dev/log",
    ...                 "MappingRules": {
    ...                     "MessageCodes": {
    ...                             "4000": ".event.source.appName 'ssh' STRCMP",
    ...                             "2000": ".event.source.appName 'crinit' STRCMP",
    ...                             "1000": ".event.source.appName 'login' STRCMP"
    ...                     }
    ...                 }
    ...             }
    ...         }
    ...     }
    ${default_config}=        Default Config Core
    ${plugin_search_path}=    Get Value From Json     ${default_config}    $..EventLogging.PluginSearchPath
    ${valid_config}=          Update Value To Json    ${config}
    ...                       $..EventLogging.PluginSearchPath       ${plugin_search_path}[0]
    Return From Keyword       ${valid_config}

Invalid Configuration
    [Documentation]    An Invalid Configuration for elosd
    
    ${_}      Set Variable    ${SPACE*4}
    ${invalid_config}   Raw Config
    ...     {
    ...     ${_}"start": {
    ...     ${_}${_}"elos": {
    ...     ${_}${_}${_}"UseEnv": true,
    ...     ${_}${_}${_}"Port": ,
    ...     ${_}${_}${_}"Interface": "0.0.0.0",
    ...     ${_}${_}${_}"EventLogging": {
    ...     ${_}${_}${_}${_}"PluginSearchPath": "/usr/lib/elos/backend",
    ...     ${_}${_}${_}${_}"Plugins": {
    ...     ${_}${_}${_}${_}${_}"Dummy": {
    ...     ${_}${_}${_}${_}${_}${_}"File": "backend_dummy.so",
    ...     ${_}${_}${_}${_}${_}${_}"Run": "always"
    ...     ${_}${_}${_}${_}${_}},
    ...     ${_}${_}${_}${_}${_}"SecondDummy": {
    ...     ${_}${_}${_}${_}${_}${_}"File": "backend_dummy.so",
    ...     ${_}${_}${_}${_}${_}${_}"Run": "always"
    ...     ${_}${_}${_}${_}${_}},
    ...     ${_}${_}${_}${_}${_}"SkippedDummy": {
    ...     ${_}${_}${_}${_}${_}${_}"File": "backend_dummy.so",
    ...     ${_}${_}${_}${_}${_}${_}"Run": "never"
    ...     ${_}${_}${_}${_}${_}}
    ...     ${_}${_}${_}${_}}
    ...     ${_}${_}${_}},
    ...     ${_}${_}${_}"Scanner": {
    ...     ${_}${_}${_}${_}"SyslogScanner": {
    ...     ${_}${_}${_}${_}${_}"Syslog": "/dev/log",
    ...     ${_}${_}${_}${_}${_}"MappingRules": {
    ...     ${_}${_}${_}${_}${_}${_}"MessageCodes": {
    ...     ${_}${_}${_}${_}${_}${_}${_}${_}"4000": ".event.source.appName 'ssh' STRCMP",
    ...     ${_}${_}${_}${_}${_}${_}${_}${_}"2000": ".event.source.appName 'crinit' STRCMP",
    ...     ${_}${_}${_}${_}${_}${_}${_}${_}"1000": ".event.source.appName 'login' STRCMP"
    ...     ${_}${_}${_}${_}${_}${_}${_}}
    ...     ${_}${_}${_}${_}${_}${_}}
    ...     ${_}${_}${_}${_}${_}}
    ...     ${_}${_}${_}${_}}
    ...     ${_}${_}${_}}
    ...     ${_}${_}}
    ...     ${_}}
    ...     }
    Return From Keyword     ${invalid_config}
