*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check complex authorized process filter

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/TemplateConfig.py
Library             libraries/ElosKeywords.py
Library             libraries/OsLibrary.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Test Cases ***
Test JSON Backend Roates The Log File When Reaching The Size Limit
    [Documentation]    See that the json backend rotates the log file
    ...                when enough events are written to reach the
    ...                file size limit

    ${config_template}      Set Variable        /tmp/elosd_size_itests_%count%.log
    Given Config Option '$..JsonBackend.Config.StoragePath' Gets Set To "${config_template}"
    And Config Option '$..EventLogging.Plugins.JsonBackend.Config.MaxSize' Gets Set To 500
    And Config Option '$..EventLogging.Plugins.SQLBackend.Run' Gets Set To 'never'
    And Elosd Is Running With New Config
    When More Events Are Stored Than Fit Into One Log File
    Then Logs '${config_template}' Are All Smaller Than The '500' Limit

    [Teardown]    Reset Elosd Config

Test JSON Backend Roates The Log File When The Current File Is Deleted
    [Documentation]    See that the json backend continues writing
    ...                into the next file when the current one is deleted

    ${config_template}      Set Variable        /tmp/elosd_delete_itest_%count%.log
    Given Config Option '$..JsonBackend.Config.StoragePath' Gets Set To "${config_template}"
    And Config Option '$..EventLogging.Plugins.SQLBackend.Run' Gets Set To 'never'
    And Elosd Is Running With New Config
    When The Current Json Log File '${config_template}' Gets Deleted
    And Another Event Gets Stored
    Then The Json Backend Continues With The Next '${config_template}' Log File
    And The Last Json Log Should Not Exist

    [Teardown]    Reset Elosd Config

Test JSON Backend Continues With The Last Log File When Restarted
    [Documentation]    See that the json backend continues writing
    ...                into last log file when elosd is restarted

    ${config_template}      Set Variable        /tmp/elosd_restart_itest_%count%.log
    Given Config Option '$..JsonBackend.Config.StoragePath' Gets Set To "${config_template}"
    And Config Option '$..EventLogging.Plugins.JsonBackend.Config.MaxSize' Gets Set To 500
    And Config Option '$..EventLogging.Plugins.SQLBackend.Run' Gets Set To 'never'
    And Elosd Is Running With New Config
    And A Few Log Files Are Filled Already
    And Some Of The Older '${config_template}' Log Files Are Deleted
    When Elosd Is Restarted
    Then The Json Backend Continues Logging To The Last Log File

    [Teardown]    Reset Elosd Config


*** Keywords ***
Elosd is restarted
    restart elosd
    ensure elosd is started

Elosd Is Restarted
    [Documentation]     ensure elosd is restarted
    Restart Elosd
    Ensure Elosd Is Started

Elosd Is Running With New Config
    [Documentation]     assemble the new config
    ...                 and make sure elos is running with it
    Set Assembled Config
    Restart Elosd
    Ensure Elosd Is Started

More Events Are Stored Than Fit Into One Log File
    [Documentation]     Publish some events
    FOR    ${i}    IN RANGE    0    100
        Publish '{ "payload": "event number ${i}" }'
    END

A Few Log Files Are Filled Already
    [Documentation]     Publish a bunch of events
    More Events Are Stored Than Fit Into One Log File
    More Events Are Stored Than Fit Into One Log File
    More Events Are Stored Than Fit Into One Log File

Another Event Gets Stored
    [Documentation]     Publish another event
    Publish '{ "payload": "another event" }'

The Last Json Log Should Not Exist
    [Documentation]     Check that the file saved as the last log file
    ...                 doesn't exist
    File Should Not Exist       ${NEWEST_LOG_FILE}
