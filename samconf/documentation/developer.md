# samconf

This library has the following main tasks:

* Manage program configurations through a centralized configuration API.
* Merge arbitrary sources for configuration like Json and/or Xml, program
  parameters, environment and compile/buildin configurations.
* Transparently verify signed configurations

## Verification of a configuration source

The content of a configuration source shall be verified by a signature method
to verify its integrity and origin.

### requirements and constraints

The verification process shall cover the following requirements and constraints:

* The verification shall be based on the content of a file
* The signature can be provided by the file or separately (still to discuss)
* The signature is always applied to the entire content of a file, no
  mixed usage of signed and unsigned content in the same configuration source
* The implementation shall allow mixed usage of signed and unsigned content for
  different configuration sources
* The signature process itself shall not be implemented by samconf, it is
  planned to delegate this via an external crypto API to some sort of dedicated
  external crypto component
* The concept shall be extensible to support encrypted configuration sources in future

### basic concept

The content of a file, the configuration source, is read and validated together
with a signature through a external signature verification process.


#### The signature

For the signature the following requirements shall be met:

* To be future-proof the signature shall contain information about the
  signature mechanism to use.
* The signature shall be in a format that can be easily attached to a file
  without causing problems about used characters

To make a signature self explanatory it shall use a URI like syntax as follows:

```
signature_method://signature_payload
sha1://f0f8f92add392aff18dedad014732fc1dfb4427e
md5://9ce83ff3647269cf3b341824e243a25f
x509://RHVtbXkgY2VydCA6KQo=
http://external.verify.myconfig.somwhere/signature/c29tZSBhcmJpdHJhcnkgc2lnbmF0dXJlIGluZm9ybWF0aW9uCg==
unix+tcp:///run/cryptoservice.sock/signature/c29tZSBhcmJpdHJhcnkgc2lnbmF0dXJlIGluZm9ybWF0aW9uCg==
file:///dev/special_crypto_device/c29tZSBhcmJpdHJhcnkgc2lnbmF0dXJlIGluZm9ybWF0aW9uCg==
```

The signature has to be stored somewhere with a reference to the corresponding
file. The following options are considered :

* A) append / prepend to a file
```
signature_method://signature_payload
{
 "option": {...},
...
}
signature_method://signature_payload
```
* B) encode in the file name
```
/etc/myservice.d/feature_c2hhMTovL2YwZjhmOTJhZGQzOTJhZmYxOGRlZGFkMDE0NzMyZmMxZGZiNDQyN2UK.conf
```
* C) in a separate file
```
/etc/myservice.d/feature.conf /etc/myservice.d/feature.sig
```
* D) in a signature DB obtained from a file or a local service
```
/etc/myservice.d/feature.conf
cat /etc/signature.db
  /etc/myservice.d/feature.conf sha1://f0f8f92add392aff18dedad014732fc1dfb4427e

curl http://localhost:44444/etc/myservice.d/feature.conf
200 OK
...
/etc/myservice.d/feature.conf sha1://f0f8f92add392aff18dedad014732fc1dfb4427e
```
* E) in a extended file attribute
 * two usable namespaces `trusted` and `user`
 * either read and write by everyone (`user`) or only root user (`trusted`)
```
# only by root modifyable 'trusted' namespace
sudo setfattr -n trusted.signature -v "sha1://f0f8f92add392aff18dedad014732fc1dfb4427e" /etc/myservice.d/feature.conf
# only by root readable 'trusted' namespace
sudo getfattr -n trusted.signature /etc/myservice.d/feature.conf
# file: /etc/myservice.d/feature.conf
trusted.signature="sha1://f0f8f92add392aff18dedad014732fc1dfb4427e"

setfattr -n user.signature -v "sha1://f0f8f92add392aff18dedad014732fc1dfb4427e" /etc/myservice.d/feature.conf
getfattr -n user.signature /etc/myservice.d/feature.conf
# file: /etc/myservice.d/feature.conf
user.signature="sha1://f0f8f92add392aff18dedad014732fc1dfb4427e"
```

| Method | pro            | con |
|--------|----------------|-----|
| A      | simple         | can confuse tools which are not aware of this additional content, i.e. xml,json parser |
| B      | simple         | names will become extremely large, which is unusual or unexpected and can cause trouble |
| C      | simple, signature size and format not limited  | noisy in the file system; need to sync the contents of two files |
| D      | central independent place, flexible and extensible, control of signature is separated | additional service API necessary, not self contained |
| E      | not visible, access can be restricted to root by using 'trusted' namespace, it reads like it is made for this | filesystem must support xattr and signature size must be < 4k depending on the filesystem even smaller, needs extra caution on coping file '-a'|

To be future prove the process of obtaining the signature string for a
configuration source shall be a implementation detail of the configuration
backend. A backend can use a common component to lookup the signature string
but is not enforced to do so.



### how to protect against loading self signed or not intended signed configs?

The config verification process must ensure that no signed configs can be
introduced which for example are copied from a different system in debug mode.

* The signature shall somehow include or bound to the specific hardware, the
  machine aka. hardware-id is a promising candidate for this
```
signature = sign(Config + hardwareID)
```
* The process shall ensure that options from a signed source are not
  overwritten by one from an unsigned source, to prevent enabling debug mode or
  similar by just adding self signed config files. To prevent this each option
  shall have a flag `isSigned`, this flag will be set by the configuration
  backend if the option is read from a signed and successfully verified source.
  An option with a signed flag has always precedence and will overwrite any
  other option. An option with a sign flag can not be overwritten.


## Internal Representation

### General Structure

The configuration is held in a tree structure. Each tree node is of type
`samconfConfig_t`. A node can hold a value of a certain type defined by its type
attribute or in case of type _object_ and type _array_ it can hold children.
For a detailed type description have a look at the class diagram.

![configuration class diagram](images/config_class.png "Configuration class")

This structure allows a hierarchical ordering of configuration options. The following diagram shows an example configuration.

![Example configuration tree](images/config_example.png "Example configuration")

Each node can be addressed by a _xpath_ like notation. The option _SyslogPath_ of the
_Syslog-Scanner_ is for example ``/elos/scanner/syslog/SyslogPath``.

### Combined from different Sources

The tree structure allows easy extension of a basic configuration or merging of an
arbitrary number of configurations. Thus a configuration tree can consist of nodes from different sources. Since
the goal of this library is the comfortable **reading** access to different configuration formats, there is no reference to the original source in a node of a tree. This also eases the merging of two trees.

### From Configuration Source to Configuration

The _samconf_ library shall support different types of sources for configuration
options and provided a unified interface to access them. The following diagram
shows the life cycle of a configuration source.

![configuration source life cycle](images/config_source_lifecycle.png "configuration source life cycle")

* A configuration is created by defining a location as file path, the type is determined form the file.
* A configuration is loaded by a dedicated backend implementation.
* A configuration backend can tell if a source pointed to by given path is valid
* A configuration can *never* change after initialization
* A configuration can *not* change the underling source of information.

The data format for a configuration source type is limited by the following requirements:

* a single options must be transferable into a key value based structure
* provide hierarchical structure information for each key value pair to construct a path

This means it must be possible to implement a dedicated loader that is capable
of transforming the plain data a configuration source is pointing to into a
samconfConfig_t tree.

The process of loading a configuration from and building a configuration tree
from an arbitrary configuration source shall be done by the ConfigurationFactory component.

#### The Configuration Factory

The _Configuration Factory_ shall provide an interface to abstract the details of creating a `samconfConfig_t` configuration tree, therefore it shall fullfill the following requirements:

* Easy extensibility for new source types (Open/Close principle)
* Validation of sources
* Statically linked backends
* Compile time switches for backends (-DSAMCONF_ENABLE_CONFIG_BACKEND_<name>)

The process to create a configuration source is as follows:

![create a Configuration Tree](images/config_factory.png "create a Configuration Tree")

As the _Configuration Factory_ shall provide the developer with the mechanisms to load, merge and release a configuration tree from various sources, the public API does only define the following three functions.

```c
samconfConfigStatusE_t samconfLoad(const char *location, bool enforceSignature, samconfConfig_t **conf);
samconfConfigStatusE_t samconfMerge(samconfConfig_t **new, samconfConfig_t *conf1, samconfConfig_t *conf2);
samconfConfigStatusE_t samconfClear(samconfConfig_t *conf);
```

To provide implementations for different configuration sources the interface `samconfConfigBackend_t` shall be implemented as follows:

```c
#define samconfConfigBackendForEach(source, key, value) \
  for (...)

typedef struct samconfConfigBackendOps {
  // List of function pointers
  samconfConfigStatusE_t (*supports)(const char *location, bool *isSupported);
  samconfConfigStatusE_t (*open)(const char *location, samconfConfigBackend_t *backend);
  samconfConfigStatusE_t (*load)(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config);
  samconfConfigStatusE_t (*close)(samconfConfigBackend_t *backend);
} samconfConfigBackendOps_t;

typedef struct samconfConfigBackend {
  void *originalHandle; // eg. File descriptor
  void *backendHandle; // internal json object
  samconfConfigBackendOps_t *ops; // Config operations
} samconfConfigBackend_t;
```

An implementation for a specific configuration source shall implement the above functions, which can then be used by the _Configuration Factory_ to parse a configuration.

#### Types of Configuration Sources

The following sources for configuration options shall be provided:

* Parameter list
* Environment variables
* JSON file
* Compiled in configuration

##### Parameter Source

A configuration option shall be passed to a program as parameter. The parsing
and the format of such parameter shall be follow the format accepted by
`getopt` libc function. The general processing rule shall be as follows:

* if a parameter is invalid or unknown return an error
* if parameter is valid insert it into a configuration tree

To integrate parameter as configuration options into a `samconfConfig_t` tree, it requires a mapping or strategy on how to place the parameter in the config tree.
There two possible solutions:

A) the parameter name must be structured like a full qualified name `--app.categoryA.itemA.subItem "the value of a configuration option"`
B) a set of parameter is mapped statically in the code to a full qualified name by the ParameterConfigLoader for example.
   ```
   $> app -p 42 --ItemB
   ```
   this will results in two options (key value pairs):
   ```
   root.categoryA.itemA = 42
   root.categoryB.itemB = true
   ```

In either case the following rules shall be applied to set the different types of values:

###### Boolean

* if a parameter is specified the corresponding option value shall be true otherwise false
* if a parameter is set explicitly to true or false the corresponding value shall be used for the corresponding option

```
-/--itemB  => /root/categoryB/itemB = true
-/--itemB=false  => root/categoryB/itemB = false
```

###### Integer

If a parameter provides an integer value it shall be converted to the configuration option type and used.

```
-/--itemA=42  => /root/categoryA/itemA = 42
-/--itemA 42  => /root/categoryA/itemA = 42
```

###### Real

If a parameter provides a real value it shall be converted to the configuration option type and used.

```
-/--itemA=42.3  => /root/categoryA/itemA = 42.3
-/--itemA 42.3  => /root/categoryA/itemA = 42.3
```

###### String

If a parameter provides a string value it shall be used.

```
-/--subItem="the value of a configuration option"  => /root/categoryA/itemA/subItem = "the value of a configuration option"
-/--subItem "the value of a configuration option"  => /root/categoryA/itemA/subItem = "the value of a configuration option"
```

##### Environment Source

To integrate environment configuration options into a `samconfConfig_t` tree, the issue to solve is where to insert the parameter into the configuration tree. There two possible solutions:

A) the environment variable name must be structured like a full qualified name `--app.categoryA.itemA.subItem "the value of a configuration option"`
B) a set of parameter is mapped statically in the code to a full qualified name by the parameter loader for example.

```
$> app -p 42 --ItemB
```

this will results in two options (key value pairs):

```
root.categoryA.itemA = 42
root.categoryB.itemB = true
```

In either case the following rules shall be applied to set the different types of values:

###### Boolean

If a environment is set to true or false the corresponding value shall be used
for the corresponding option. If the value of an environment variable can not
be mapped, it shall be ignored and can optionally be reported via a log
message. The variable name shall be uppercase and the environment loader shall be case sensitive.

```
ITEM_B="true"  => /root/categoryB/itemB = true
ITEM_B="false"  => /root/categoryB/itemB = false
```

###### Integer

If a parameter provides an integer value it shall be converted to the configuration option type and used.

```
ITEM_A=42  => /root/categoryA/itemA = 42
```

###### Real

If a parameter provides a real value it shall be converted to the configuration option type and used.

```
ITEM_A=42.3 => /root/categoryA/itemA = 42.3
```

###### String

If a parameter provides a string value it shall be used.

```
SUBITEM="string value" => /root/categoryA/itemA/subItem = "string value"
```

##### JSON File Source

The information from a JSON file is transformed in to a configuration by using the JSON object composition structure.

The following JSON object:

```json
{
  "root": {
    "categoryA" : {
      "itemA" : {
        "subItem" : {
          "value" : "the value of a configuration option"
        },
        "value" : 42
      },
    },
    "categoryB" : {
      "itemA" : false
    }
  }
}
```

will be transformed in:

![samconfConfig_t tree](images/config_from_json_example.png "samconfConfig_t tree")


##### Compiled in Source

It shall be possible to integrate a compiled in configuration tree. So it is
possible to provide compiled in defaults for configuration options. To be convenient a the confiuration values shall be privided in a static definable way like:

```c
struct samconfbuildInConfig {
  const char* key;
  const char* value;
}[] = {
  {
    .key = "/root/categoryA/itemA",
    .value = "42",
  },
};
```

## Public API

### Building a Configuration Tree

In order to generate a `samconfConfig_t` tree from a specific source, you will do something like:

```c
const char *location = "/path/to/conf.json";
samconfConfig_t *config;
samconfConfigStatusE_t status;

status = samconfLoad(location, true, &conf);
if (status != SAMCONF_CONFIG_OK)
    // Include error handling here
```

### Merging Configuration Trees

In order to combine different configuration sources like json files and commandline parameters, you can merge them:

```c
samconfConfig_t *config, *jsonConfig, *parameters;
samconfConfigStatusE_t status;

// Load json config and commandline parameters

status = samconfMerge(&config, jsonConfig, parameters);
if (status != SAMCONF_CONFIG_OK)
    // Include error handling here
```

### Clearing Configuration Trees

In order to clean up configuration trees, you can use `samconfClear`:

```c
samconfConfig_t *config;
samconfConfigStatusE_t status;

// Load the configuration and do your thing

status = samconfClear(config);
```

## Internal API

### Data Types

#### samconfConfigStatusE_t

This enum defines the result of an operation in the configuration library. All function shall use this to indicate a success or give a reason what happened.

```c
typedef enum samconfConfigStatusE {
  SAMCONF_CONFIG_ERROR = -1,
  SAMCONF_CONFIG_OK = 0,
  SAMCONF_CONFIG_EOF,
  SAMCONF_CONFIG_NOT_FOUND,
  SAMCONF_CONFIG_INVALID_TYPE,
  SAMCONF_CONFIG_PARSE_ERROR,
} samconfConfigStatusE_t;
```

#### samconfConfigValueTypeE_t

This enum defines the data type of a value that a node can have or ``SAMCONF_CONFIG_VALUE_NONE`` if the node is empty and just to maintain a structure or ordering of the configuration.

```c
typedef enum samconfConfigValueTypeE {
  SAMCONF_CONFIG_VALUE_NONE = 0,
  SAMCONF_CONFIG_VALUE_STRING,
  SAMCONF_CONFIG_VALUE_INT,
  SAMCONF_CONFIG_VALUE_REAL,
  SAMCONF_CONFIG_VALUE_ARRAY,
  SAMCONF_CONFIG_VALUE_OBJECT,
} samconfConfigValueTypeE_t;
```

#### samconfConfig_t

This structure is the central key component. It represents a node in the configuration tree. It has a name the ``key`` and can contain a value and/or references to further `samconfConfig_t` child nodes. It also maintains a reference to its parent `samconfConfig_t` node. In case the parent node reference is `NULL` it is the top level node of the configuration tree.

```c
typedef struct samconfConfig {
  samconfConfig_t *parent;
  char *key;
  samconfConfigValueTypeE_t type;
  union value {
    char *string;
    int32_t integer;
    bool boolean;
    double real;
  };
  samconfConfig_t **children;
  size_t childCount;
} samconfConfig_t;
```

#### samconfConfigBackendOps_t

This structure defines the internal interface used by the _Configuration Factory_ to parse the configuration source format into a `samconfConfig_t` configuration tree. Every backend has to implement these functions as static functions and provide an instance including the required function pointers to these internal functions.

```c
typedef struct samconfConfigBackendOps {
  // List of function pointers
  bool (*supports)(const char *location);
  samconfConfigStatusE_t (*open)(const char *location, samconfConfigBackend_t *backend);
  samconfConfigStatusE_t (*load)(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config);
  samconfConfigStatusE_t (*close)(samconfConfigBackend_t *backend);
} samconfConfigBackendOps_t;
```

#### samconfConfigBackend_t

This struct is used to maintain a state during parsing of configuration source format into a `samconfConfig_t` configuration tree by the _Configuration Factory_. Therefore it holds a reference to the configuration backend operations and different stateful variables.

```c
typedef struct samconfConfigBackend {
  void *originalHandle; // eg. File descriptor
  void *backendHandle; // internal json object
  samconfConfigBackendOps_t *ops; // Config operations
} samconfConfigBackend_t;
```

#### samconfConfigSourceTypeE_t

The samconfConfigSourceTypeE_t enum defines all types of supported sources for a configuration option (or node).

```c
  typedef enum samconfConfigSourceTypeE {
    SAMCONF_CONFIG_SOURCE_TYPE_NONE = 0,
    SAMCONF_CONFIG_SOURCE_TYPE_PARAMETER,
    SAMCONF_CONFIG_SOURCE_TYPE_ENVIRONMENT,
    SAMCONF_CONFIG_SOURCE_TYPE_FILE,
    SAMCONF_CONFIG_SOURCE_TYPE_BUILDIN,
  } samconfConfigSourceTypeE_t;
```

#### samconfConfigSource_t

The samconfConfigSource_t is used to describe the origin of a configuration option (or node). It only manages the specific type of a configuration source (eg. file, command line argument or environment variable) and a corresponding backend for the used data format.
The samconfConfigBackened_t will be initialized for each interaction with a new source configuration and will hold the original handle (eg. a file descriptor), an internal handle like the `struct json_object` of _json-c_ and the pointer to the configuration operations for this specific backend (eg. _json_, _yaml_, _ini_).
The configuration operations are stored in an samconfConfigBackendOps_t and will be initialized to a list of function pointers used to interact with the specific data format.

```c
typedef struct samconfConfigSource {
  samconfConfigSourceTypeE_t type;
  samconfConfigBackend_t *backend;
} samconfConfigSource_t;
```

To create a configuration the following set of functions is given:

```c
samconfConfigStatusE_t samconfConfigNew(samconfConfig_t **config)
samconfConfigStatusE_t samconfConfigInit(samconfConfig_t *config)
samconfConfigStatusE_t samconfConfigClone(samconfConfig_t **to, samconfConfig_t *from)
samconfConfigStatusE_t samconfConfigDeepCopy(samconfConfig_t *to, samconfConfig_t *from)
samconfConfigStatusE_t samconfConfigDelete(samconfConfig_t *config)
samconfConfigStatusE_t samconfConfigDeleteMembers(samconfConfig_t *config)

samconfConfigStatusE_t samconfConfigAddNodeAt(samconfConfig_t *root, char *path, samconfConfig_t *newChild)

samconfConfigStatusE_t samconfConfigLoaderLoad(samconfConfigSourceTypesE_t type, void* location, samconfConfigSource_t* configSource);

samconfConfigStatusE_t samconfConfigBackendNew(samconfConfigBackend_t **backend, samconfConfigBackendOps *ops)
samconfConfigStatusE_t samconfConfigBackendInit(samconfConfigBackend_t *backend, samconfConfigBackendOps *ops)
```

```c
bool samconf<Type>Supported(void* location);
samconfConfigStatusE_t samconf<Type>BackendLoad(void* location, bool isSigned, samconfConfigSource_t* configSource)
samconfConfigStatusE_t samconf<Type>BackendClose(samconfConfigBackend_t *backend)

samconfConfigStatusE_t samconfConfigBuilderBuild(samconfConfigSource_t*, samconfConfig_t* config)
samconfConfigStatusE_t samconfConfigBuilderMerge(vec<samconfConfig_t>* configs, samconfConfigSource_t** resultConfig)
samconfConfigStatusE_t samconfConfigBuilderRelease(vec<samconfConfig_t>* configs)

samconfConfigStatusE_t samconfJsonBackendOpen(const char * location, samconfConfigBackend_t* backend);
samconfConfigStatusE_t samconfJsonBackendLoad(samconfConfigBackend_t* backend, bool isSigned, samconfConfig_t **config);
```

#### samconfConfigNew

Shall allocate memory of size ``samconfConfig_t`` using ``safuAllocMem`` and assign its address to the given ``samconfConfig_t **`` config pointer.


```c
samconfConfigStatusE_t samconfConfigNew(
        samconfConfig_t **const config,
    );
```

Parameters:

* `[out] ` config -> the pointer to the pointer pointing to the newly allocated memory.

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> memory allocation fails

#### samconfConfigInit

Shall initialize all  member variables to 0 and member pointers to NULL for the given ``samconfConfig_t`` pointed to by config pointer.


```c
samconfConfigStatusE_t samconfConfigInit(
        samconfConfig_t *config
    );
```

Parameters:

* `[out] ` config -> the pointer to the `samconfConfig_t` to be initialized

Returns:
* `SAMCONF_CONFIG_OK` -> on successful `samconfConfig_t` initialization
* `SAMCONF_CONFIG_ERROR` -> if given pointer is NULL

#### samconfConfigDelete

Shall delete the given ``samconfConfig_t`` configuration, after its member values are deleted using ``samconfConfigDeleteMembers``. This includes all
subsequent generations of child configurations. It is important to ensure that the given configuration and its members are allocated dynamically.
This function uses the `free()` call internally, should a free() fail at any point (e.g. by trying to free an unallocated address) the result is
undefined and the program is expected to receive a SIGSEV and gets terminated on default if not treated otherwise.


```c
samconfConfigStatusE_t samconfConfigDelete(
        samconfConfig_t *config
    );
```

Parameters:

* `[in] ` config -> the pointer to the configuration that needs to be deleted.

Returns:
* `SAMCONF_CONFIG_OK` -> on successfull deletion of given configuration
* `SAMCONF_CONFIG_ERROR` -> deletion of configuration fails upon receiving invalid/null input value

#### samconfConfigDeleteMembers

Shall delete the given ``samconfConfig_t`` configuration's member values. This includes all subsequent generations of child configurations.
It is important to ensure that the given configuration's members values are allocated dynamically. This function uses the `free()` call internally,
should a free() fail at any point (e.g. by trying to free an unallocated address) the result is undefined and the program is expected to receive a
SIGSEV and gets terminated on default if not treated otherwise.


```c
samconfConfigStatusE_t samconfConfigDeleteMembers(
        samconfConfig_t *config
    );
```

Parameters:

* `[in] ` config -> the pointer to the configuration whose member values need to be deleted.

Returns:
* `SAMCONF_CONFIG_OK` -> on successful deletion member values of given configuration
* `SAMCONF_CONFIG_ERROR` -> deletion of member values of given configuration fails upon receiving invalid/null input value

Note:
* The function returns an `SAMCONF_CONFIG_ERROR` when given configuration to be deleted is NULL. In cases like deleting already deleted configuration
or trying to delete a configuration in stack, the functions is expected to be terminated by a SIGSEG.

#### samconfConfigLoaderLoad

Shall lookup an appropriate samconfConfigBackend_t implementation for
samconfConfigSourceTypesE_t to forward the `load` call to. If no backend can be
determined or the backend returns an error, an appropriate error value shall be
returned. On success the structure pointed to by the samconfConfigSource_t
pointer is populated and SAMCONF_CONFIG_OK shall be returned.

```c
samconfConfigStatusE_t samconfConfigLoaderLoad(
        samconfConfigSourceTypesE_t type,
        void* location,
        samconfConfigSource_t* configSource
    );
```

Parameters:

* `[in] `  type         -> samconfConfigSourceTypesE_t value specifying the source type.
* `[in] `  location     -> a pointer to a structure specified by the source type backend.
* `[out] ` configSource -> a pointer to a samconfConfigSource_t object that shall be initialized.

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> failed to setup the configuration source (not accessible, failed to verify, no suitable backend)

#### samconfConfigBackendNew

Shall allocate memory of size ``samconfConfigBackend_t`` using ``safuAllocMem``, initialize it by calling ``samconfConfigBackendInit`` and assign the backend address to the given ``samconfConfigBackend_t **`` configuration backend pointer.

```c
samconfConfigStatusE_t samconfConfigBackendNew(
        samconfConfigBackend_t** backend,
        const samconfConfigBackendOps_t* ops
    );
```

Parameters:

* `[out] ` backend -> the pointer to the pointer pointing to the newly allocated memory.
* `[in] ` ops -> the pointer to backend operations struct.

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> memory allocation fails

#### samconfConfigBackendInit

Shall initialize the `samconfConfigBackend_t*` with the source format specific file operations.

```c
samconfConfigStatusE_t samconfCongifBackendInit(
        samconfConfigBackend_t* backend,
        const samconfConfigBackendOps_t* ops
    );
```

Parameters:

* `[out] ` backend -> a pointer to a samconfConfigBackend_t object that shall be initialized.
* `[in] ` ops -> the pointer to backend operations struct.

Returns:
* `SAMCONF_CONFIG_OK` -> on success

#### samconfConfigBackendDelete

Shall free the memory of the given ``samconfConfigBackend_t`` pointer.

```c
samconfConfigStatusE_t samconfConfigBackendDelete(
        samconfConfigBackend_t* backend
    );
```

Parameters:

* `[in] ` backend -> the pointer to the configuration backend.

Returns:
* `SAMCONF_CONFIG_OK` -> on success

#### samconfJsonBackendOpen

Shall locate and open a JSON file and store the necessary handles to access the content in backend->backendHandle

```c
samconfConfigStatusE_t samconfJsonBackendOpen(
        const char * location,
        samconfConfigBackend_t* backend
    );
```

Parameters:
* `[in]` location -> Path of the json config file.
* `[in]` backend -> a pointer to a samconfConfigSource_t object, which contains the samconfConfigBackend_t object containing the pointer to the that shall be initialized.

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> failed to setup the configuration source (not accessible, failed to verify)

#### samconfJsonBackendSupports

Shall check if the content of the given file contains a Json document and is processable by this backend.

```c
samconfConfigStatusE_t samconfJsonBackendSupports(
        const char *location,
        bool *isSupported
    );
```
Parameters:
* `[in]` location -> Path of the file whose extension is checked.
* `[out]` isSupported -> Pointer flag indicating support for given file extension.

Returns:
* `SAMCONF_CONFIG_OK` -> if given file extension is supported.
* `SAMCONF_CONFIG_ERROR` -> if error occurs.

#### samconfJsonBackendClose

Shall free all Memory related to a given samconfConfigBackend_t struct.

```c
samconfConfigStatusE_t samconfJsonBackendLoad(
        samconfConfigBackend_t *backend
    );
```

Parameters:

* `[in] ` configSource -> a pointer to a samconfConfigBackend_t struct which shall be closed.

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> failed to free the json object completly

#### samconfJsonBackendLoad

Shall parse json_object data from given backend into a samconfConfig_t *.

```c
samconfConfigStatusE_t samconfJsonBackendLoad(
        samconfConfigBackend_t *backend,
        bool isSigned,
        samconfConfig_t **config
    );
```

Parameters:

* `[in] ` backend -> a pointer to a samconfConfigBackend_t struct which contains the to be parsed json_object *.
* `[in] ` isSigned -> whether or not the given configuration file is signed.
* `[out] ` config -> a pointer pointer to the root of the to-be-built config tree.

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> failed to build the complete config tree.

#### samconfConfigBuilderBuild

Shall create a configuration tree based on all available configuration options
from samconfConfigSource_t and create a configuration tree and store the root
node in the pointer pointed to by config.

```c
samconfConfigStatusE_t samconfConfigBuilderBuild(
        samconfConfigSource_t* configSource,
        samconfConfig_t** config
    );
```

Parameters:

* `[in] `  configSource -> a pointer to the configurations source to use to build a configuration tree.
* `[out] ` config       -> a pointer to a samconfConfig_t pointer to return the new configuration tree.

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> failed to build the configuration tree

#### samconfConfigBuilderRelease

Shall free and release all allocated resources related to the provided configuration tree. The implementation shall try to free resources even if one or more child elements could not be freed.

```c
samconfConfigStatusE_t samconfConfigBuilderRelease(
        samconfConfig_t* configs
    );
```

Parameters:

* `[in] `  config -> the pointe to a configuration root node

Returns:
* `SAMCONF_CONFIG_OK` -> on success
* `SAMCONF_CONFIG_ERROR` -> failed to free or release at least one child node or configSource

### query a configuration tree

```c
* samconfConfigStatusE_t samconfConfigGet(samconfConfig_t *root, const char *path, samconfConfig_t **result)
* samconfConfigStatusE_t samconfConfigGetString(samconfConfig_t *root, const char *path, const char **result)
* samconfConfigStatusE_t samconfConfigGetInt32(samconfConfig_t *root, const char *path, int32_t *result)
* samconfConfigStatusE_t samconfConfigGetBool(samconfConfig_t *root, const char *path, bool *result)
```
#### samconfConfigAdd

Shall add the given ``samconfConfig_t`` child node to the given parent node.

```c
samconfConfigStatusE_t samconfConfigAdd(
        samconfConfig_t *parent,
        samconfConfig_t *child
    );
```

Parameters:
* `[in]` parent    -> node to which given child is added
* `[in]` child     -> child node to be added to  parent

Returns:
* `SAMCONF_CONFIG_OK`        -> when given child node is added successfully to the parent node.
* `SAMCONF_CONFIG_ERROR`     -> when child node could not be added to parent node.

#### samconfConfigGet

Make a lookup for a given path relative to the given ``samconfConfig_t`` root node.

A path looks like ``/elos/scanner/syslog/SyslogPath``, where ``SyslogPath`` is the target node and all other are parents.


```c
samconfConfigStatusE_t samconfConfigGet(
        const samconfConfig_t *root,
        const char *path,
        const samconfConfig_t **result
    );
```

Parameters:

* `[in] `  root      -> root node where to start the lookup
* `[in] `  path      -> the search path pointing to the searched samconfConfig_t node relative to the root node
* `[out] ` result    -> the pointer to the searched node or NULL if not found

Returns:
* `SAMCONF_CONFIG_OK`        -> on success
* `SAMCONF_CONFIG_NOT_FOUND` -> no node can be found following the given path starting at the given root node

#### samconfConfigGetString

Make a lookup for a given path relative to the given ``samconfConfig_t`` root node and if found return the value as string.

A path looks like ``/elos/scanner/syslog/SyslogPath``, where ``SyslogPath`` is the target node and all other are parents.


```c
samconfConfigStatusE_t samconfConfigGetString(
        const samconfConfig_t *root,
        const char *path,
        const char **result
    );
```

Parameters:

* `[in] `  root      -> root node where to start the lookup
* `[in] `  path      -> the search path pointing to the searched samconfConfig_t node relative to the root node
* `[out] ` result    -> the pointer to the string value of the samconfConfig_t node

Returns:
* `SAMCONF_CONFIG_OK`           -> on success
* `SAMCONF_CONFIG_NOT_FOUND`    -> no node can be found following the given path starting at the given root node
* `SAMCONF_CONFIG_INVALID_TYPE` -> the node exists but contains no value of the requested type

#### samconfConfigSetString

Shall copy the ``const char*`` stringValue to the given ``samconfConfig_t`` config node and set config node's  type to ``SAMCONF_CONFIG_VALUE_STRING``.


```c
samconfConfigStatusE_t samconfConfigSetString(
        samconfConfig_t *config,
        const char *stringValue
)
```

Parameters:
* `[in]` config       -> the ``samconfConfig_t`` node whose type should set to string and value is set to given string value.
* `[in]` stringValue  -> the string which is copied to ``config->value``

Returns:
* `SAMCONF_CONFIG_OK`    -> on success
* `SAMCONF_CONFIG_ERROR` -> when invalid input is given or the string could not be copied

#### samconfConfigGetBool

Make a lookup for a given path relative to the given ``samconfConfig_t`` root node. If boolean value is found assign it to result, if not then result is not assigned any value.

A path looks like ``/elos/scanner/syslog/SyslogPath``, where ``SyslogPath`` is the target node and all other are parents.


```c
samconfConfigStatusE_t samconfConfigGetBool(
        const samconfConfig_t *root,
        const char *path,
        bool *result
    );
```

Parameters:

* `[in] `  root      -> root node where to start the lookup
* `[in] `  path      -> the search path pointing to the searched samconfConfig_t node relative to the root node
* `[out] ` result    -> the pointer where to store the boolean value of the samconfConfig_t node

Returns:
* `SAMCONF_CONFIG_OK`           -> on success
* `SAMCONF_CONFIG_NOT_FOUND`    -> no node can be found following the given path starting at the given root node
* `SAMCONF_CONFIG_INVALID_TYPE` -> the node exists but contains no value of the requested type

#### samconfConfigSetBool

Shall set the ``bool`` value to the given ``samconfConfig_t`` config node and set config node's  type to ``SAMCONF_CONFIG_VALUE_BOOLEAN``.


```c
samconfConfigStatusE_t samconfConfigSetBool(
        samconfConfig_t *config,
        bool value
)
```

Parameters:
* `[in]` config    -> the ``samconfConfig_t`` node whose type should set to boolean and value is set to given bool value.
* `[in]` value     -> the boolean value to set to ``config->value``

Returns:
* `SAMCONF_CONFIG_OK`    -> on success
* `SAMCONF_CONFIG_ERROR` -> when invalid input is given

#### samconfConfigGetInt32

Make a lookup for a given path relative to the given ``samconfConfig_t`` root node. If Int32 value is found assign it to result.If no value is found or when error is encountered, then result is not assigned any value.

A path looks like ``/elos/scanner/syslog/SyslogPath``, where ``SyslogPath`` is the target node and all other are parents.


```c
samconfConfigStatusE_t samconfConfigGetInt32(
        const samconfConfig_t *root,
        const char *path,
        int32_t *result
    );
```

Parameters:

* `[in] `  root      -> root node where to start the lookup
* `[in] `  path      -> the search path pointing to the searched samconfConfig_t node relative to the root node
* `[out] ` result    -> the pointer where to store the int32 value of the samconfConfig_t node

Returns:
* `SAMCONF_CONFIG_OK`           -> on success
* `SAMCONF_CONFIG_NOT_FOUND`    -> no node can be found following the given path starting at the given root node
* `SAMCONF_CONFIG_INVALID_TYPE` -> the node exists but contains no value of the requested type

#### samconfConfigSetInt

Shall set the ``int64_t`` intValue to the given ``samconfConfig_t`` config node and set config node's  type to ``SAMCONF_CONFIG_VALUE_INT``.


```c
samconfConfigStatusE_t samconfConfigSetInt(
        samconfConfig_t *config,
        int64_t intValue
)
```

Parameters:
* `[in]` config    -> the ``samconfConfig_t`` node whose type should set to integer and value is set to given interger value.
* `[in]` intValue  -> the integer value to set to ``config->value``

Returns:
* `SAMCONF_CONFIG_OK`    -> on success
* `SAMCONF_CONFIG_ERROR` -> when invalid input is given

#### samconfConfigGetReal

Make a lookup for a given path relative to the given ``samconfConfig_t`` root node. If Real value is found assign it to result.If no value is found or when error is encountered, then result is not assigned any value.

A path looks like ``/elos/scanner/syslog/SyslogPath``, where ``SyslogPath`` is the target node and all other are parents.


```c
samconfConfigStatusE_t samconfConfigGetReal(
        const samconfConfig_t *root,
        const char *path,
        double *result
    );
```

Parameters:

* `[in] `  root      -> root node where to start the lookup
* `[in] `  path      -> the search path pointing to the searched samconfConfig_t node relative to the root node
* `[out] ` result    -> the pointer where to store the real value of the samconfConfig_t node

Returns:
* `SAMCONF_CONFIG_OK`           -> on success
* `SAMCONF_CONFIG_NOT_FOUND`    -> no node can be found following the given path starting at the given root node
* `SAMCONF_CONFIG_INVALID_TYPE` -> the node exists but contains no value of the requested type

#### samconfConfigSetReal

Shall set the ``double`` value to the given ``samconfConfig_t`` config node and set config node's  type to ``SAMCONF_CONFIG_VALUE_REAL``.


```c
samconfConfigStatusE_t samconfConfigSetReal(
        samconfConfig_t *config,
        double value
)
```

Parameters:
* `[in]` config    -> the ``samconfConfig_t`` node whose type should set to real and value is set to given double value.
* `[in]` value     -> the double value to set to ``config->value``

Returns:
* `SAMCONF_CONFIG_OK`    -> on success
* `SAMCONF_CONFIG_ERROR` -> when invalid input is given
