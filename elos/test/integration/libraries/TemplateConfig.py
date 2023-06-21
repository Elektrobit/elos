# SPDX-License-Identifier: MIT

from robot.libraries.BuiltIn import BuiltIn
from robot.api import logger
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
        self.config_path = BuiltIn().get_variable_value("${ELOS_DEFAULT_CONFIG_FILE}")
        self.root = _str_to_bool(BuiltIn().get_variable_value("${USER_IS_ROOT}"))
        password = BuiltIn().get_variable_value("${PASSWORD}")
        self.password = "" if password is None else password

    def default_config(self):
        """Get the default configuration from the Target."""
        ssh = BuiltIn().get_library_instance('SSHLibrary')

        ssh.get_file(self.config_path, self.LOCAL_TMP_DEFAULT, scp="ALL")
        with open(self.LOCAL_TMP_DEFAULT) as f:
            conf = f.read()

        config = json.loads(conf)

        return config

    def default_config_core(self):
        """Get the core of the default configuration from the target.
        Config without { "root": { "elsod": { } } } around it."""
        return self.default_config()["root"]["elos"]

    def make_config_full(self, config_core):
        """Add { "root": { "elosd": { } } } back around the config."""
        return {"root": {"elos": config_core}}

    def set_config_from_template(self, *param, **parameter):
        """Change the specified values in the default configuration,
        and set that as new config."""
        par = {}
        for p in param:
            if type(p) == dict:
                par.update(p)

        values = self.default_config_core() | par | parameter
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
