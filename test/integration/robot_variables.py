# SPDX-License-Identifier: MIT
TARGET_IP = "172.17.0.2"
TARGET_USER = "target"
TARGET_PASSWORD = "target123"
TARGET_SYS_MANAGER = "service"
TARGET_USER_IS_ROOT = "False"
ELOS_START_COMMAND = "service elosd start"
ELOS_STOP_COMMAND = "service elosd stop"
ELOS_STATUS_COMMAND = "service elosd status"
ELOSD_LOG = "/tmp/elosd.log"

ELOS_DEFAULT_CONFIG_FILE = "/etc/elos/elosd.json"

ELOS_CONFIG_PATH = "/usr/lib/test/baseos-elos/integration/"

def set_config_file(file):
    return ELOS_CONFIG_PATH + file

def replace_default_with(config):
    return "mount --bind " + config + " " + ELOS_DEFAULT_CONFIG_FILE

ELOS_VALID_CONFIG_FILE = set_config_file("elos_valid_config.json")
ELOS_INVALID_CONFIG_FILE = set_config_file("elos_invalid_config.json")
ELOS_CONFIG_MISSING_FILTER = set_config_file("plugin_filter/elos_missing_plugin_filter.json")
ELOS_CONFIG_EMPTY_FILTER = set_config_file("plugin_filter/elos_empty_plugin_filter.json")
ELOS_CONFIG_STRING_FILTER = set_config_file("plugin_filter/elos_string_plugin_filter.json")
ELOS_CONFIG_INT_FILTER = set_config_file("plugin_filter/elos_int_plugin_filter.json")

ELOS_SET_VALID_CONFIG_CMD = replace_default_with(ELOS_VALID_CONFIG_FILE)
ELOS_SET_INVALID_CONFIG_CMD = replace_default_with(ELOS_INVALID_CONFIG_FILE)
ELOS_SET_CONFIG_MISSING_FILTER_CMD = replace_default_with(ELOS_CONFIG_MISSING_FILTER)
ELOS_SET_CONFIG_EMPTY_FILTER_CMD = replace_default_with(ELOS_CONFIG_EMPTY_FILTER)
ELOS_SET_CONFIG_STRING_FILTER_CMD = replace_default_with(ELOS_CONFIG_STRING_FILTER)
ELOS_SET_CONFIG_INT_FILTER_CMD = replace_default_with(ELOS_CONFIG_INT_FILTER)
