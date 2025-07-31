# SPDX-License-Identifier: MIT

from robot.libraries.BuiltIn import BuiltIn
from robot.api import logger
from robot.api.deco import keyword
from jsonpath_ng import parse
import json


def _str_to_bool(x):
    if x:
        return x.lower() == "true"
    else:
        return False


class TemplateConfig(object):
    LOCAL_TMP_CONF = "/tmp/elosd-config.json"
    LOCAL_TMP_DEFAULT = "/tmp/elosd-default.json"

    def json(self, *parts):
        """Build a JSON object from the provided parts.

        Example:
        ${json}=    json
        ...         {
        ...             "UseEnv": true,
        ...             "Port": 54322,
        ...             "Interface": "0.0.0.0",
        ...             "EventBlacklist": ".event.messageCode 2000 EQ",
        ...         }
        """
        return json.loads(' '.join(parts))

    def raw_config(self, *parts):
        """Join the provided strings with newlines to be used as raw config."""
        return '\n'.join(parts)

    def __init__(self):
        self.tmp_config_path = "/tmp/elosd-config.json"
        self.config_path = BuiltIn().get_variable_value(
            "${ELOS_DEFAULT_CONFIG_FILE}")
        self.root = _str_to_bool(
            BuiltIn().get_variable_value("${USER_IS_ROOT}"))
        password = BuiltIn().get_variable_value("${PASSWORD}")
        self.password = "" if password is None else password
        self.config = None

    def default_config(self):
        """Get the default configuration from the Target."""
        return self.target_config()

    @keyword("Get Target Config")
    def target_config(self):
        """Get the current configuration from the Target."""
        ssh = BuiltIn().get_library_instance('SSHLibrary')

        ssh.get_file(self.config_path, self.LOCAL_TMP_DEFAULT, scp="ALL")
        with open(self.LOCAL_TMP_DEFAULT) as f:
            conf = f.read()

        config = json.loads(conf)

        return config

    def default_config_core(self):
        """Get the core of the default configuration from the target.
        Config without { "root": { "elosd": { } } } around it."""
        return self.default_config()["root"]["elos"]

    def make_config_full(self, config_core):
        """Add { "root": { "elosd": { } } } back around the config."""
        return {"root": {"elos": config_core}}

    def set_config_from_template(self, *param, **parameter):
        """Change the specified values in the default configuration,
        and set that as new config."""
        par = {}
        for p in param:
            if type(p) is dict:
                par.update(p)

        values = self.default_config_core() | par | parameter
        config = json.dumps(self.make_config_full(values), indent=4)
        self.set_config_from_string(config)

    def set_new_config_core(self, **values):
        """Give a new configuration to use for elosd."""
        config = json.dumps(self.make_config_full(values), indent=4)
        self.set_config_from_string(config)

    def set_config_from_string(self, config):
        """Change the configuration to one provided as a string."""
        ssh = BuiltIn().get_library_instance('SSHLibrary')

        logger.info(f"using config: {config}")
        with open(self.LOCAL_TMP_CONF, "w") as f:
            f.write(config)

        ssh.put_file(self.LOCAL_TMP_CONF, self.tmp_config_path, scp="ALL")

        res = ssh.execute_command(
            f"mount --bind {self.tmp_config_path} {self.config_path}",
            sudo=not self.root, sudo_password=self.password,
            return_rc=True, return_stderr=True)

        if res[-1] != 0:
            raise Exception("setting config failed "
                            + json.dumps(res, indent=4))

    def reset_config_to_default(self):
        """Reset to default configuration."""
        ssh = BuiltIn().get_library_instance('SSHLibrary')
        res = ssh.execute_command(f"mount | grep {self.config_path}",
                                  return_rc=True)
        self.config = None
        if res[-1] == 0:
            res = ssh.execute_command(f"umount {self.config_path}",
                                      sudo=not self.root,
                                      sudo_password=self.password,
                                      return_rc=True,
                                      return_stderr=True)

            if res[-1] != 0:
                raise Exception("resetting config failed "
                                + json.dumps(res, indent=4))

    def cleanup_template_config(self):
        """Reset to default configuration and remove temporary config file."""
        ssh = BuiltIn().get_library_instance('SSHLibrary')
        self.reset_config_to_default()
        res = ssh.execute_command(f"rm -f {self.tmp_config_path}",
                                  return_rc=True, return_stderr=True)

        if res[-1] != 0:
            raise Exception("removing temporary config failed "
                            + json.dumps(res, indent=4))

    @keyword("Get Option '${option_path}' From Target Config")
    def get_option_from_target(self, option_path):
        """
        Fetch the value of a given option by JSON path syntax from the target
        config. Returns `None` if no value found and only the first match.
        """

        jpath = parse(option_path)
        result = jpath.find(self.target_config())
        logger.info(f"##### {result}")
        if len(result) > 0:
            match = result[0]
            logger.info(f"value for {option_path} : {match.value}")
            value = match.value
        else:
            logger.info(f"No value for {option_path} found")
            value = None

        return value

    @keyword("Subconfig under '${option_path}' gets set to")
    def set_config_part(self, option_path, *param, **parameter):
        par = {}
        for p in param:
            if type(p) is dict:
                par.update(p)

        values = self.default_config_core() | par | parameter

        jsonpath_exp = parse(option_path)
        if self.config is None:
            self.config = self.target_config()

        self.config = jsonpath_exp.update(self.config, values)

    @keyword("Config option '${option_path}' gets set to '${option_value}'")
    def set_config_value(self, option_path, option_value):
        jsonpath_exp = parse(option_path)
        if self.config is None:
            self.config = self.default_config_core()

        self.config = jsonpath_exp.update_or_create(self.config, option_value)

    @keyword("Config option '${option_path}' gets set to ${option_value}")
    def set_config_value_number(self, option_path, option_value):
        """Updates or adds a value in the elosd configuration

        the updated config is not send to the target to be used directly `Set assembled config` is needet for that

        When setting new values the json path needs to be an absolute path

        Examples:
        | *Config option '$..SQLBackend.Run' gets set to "never"` |
        | `Config option '$.EventLogging.Plugins.JsonBackend.Config.Bool' gets set to true` |
        | `Config option '$.EventLogging.Plugins.JsonBackend.Config.Boolean' gets set to false` |
        | `Config option '$.EventLogging.Plugins.JsonBackend.Config.Null' gets set to null` |
        | `Config option '$.EventLogging.Plugins.JsonBackend.Config.array' gets set to [ "foo", 1, 0.5, false, { "bar": true, "batz": 12 }, 4]` |
        | `Config option '$.root.elos.EventLogging.Plugins.JsonBackend.Config.object' gets set to { "a":"foo", "b":1, "c": 0.5, "d": false, "nop": { "bar": true, "batz": 12 }, "5": 4 }` |
        """
        if option_value == "true":
            value = True
        elif option_value == "false":
            value = False
        elif option_value == "null":
            value = None
        elif option_value.startswith('"') and option_value.endswith('"'):
            value = option_value[1:-1]
        elif (option_value.startswith("[") and option_value.endswith("]")) or (option_value.startswith("{") and option_value.endswith("}")):
            value = self.json(option_value)
        elif "." in option_value:
            value = float(option_value)
        else:
            value = int(option_value)

        self.set_config_value(option_path, value)


    def set_assembled_config(self):
        """Sets the updated elosd configuration to be the active config on the target
        """
        if self.config is None:
            raise Exception("No config was assembled before")

        self.set_new_config_core(**self.config)

