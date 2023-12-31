# Evaluation of the gitlab package registry, with regards to storing and using the elos subproject debian packages.

## Evaluation of the debian package registry
The creation of the debian package registry in gitlab, using the official documentation
worked as specified by the documentation.
But uploading an debian package did not.

### Uploading using dput
Evaluating the debian dput command was not possible, since the command requires
a debian packages .change file, which is not created when building the .deb file
using cpack

### Uploading using curl

The documentation specifies the following command to upload a .deb file to the
registry when no .change file is available:
``` shell
curl --request PUT --user "user:token" --get --data "distribution=registry" --data "component=main" "https://gitlabintern.emlix.com/api/v4/projects/62/packages/debian" --upload-file packages/cmocka_extensions_0.42.3_amd64.deb
```
This command failed with '{"error":"The provided content-type '' is not supported."}'

Explicitly stating a content type, as in:
``` shell
curl --request PUT --user "user:token" --get --data "distribution=registry" --data "component=main" "https://gitlabintern.emlix.com/api/v4/projects/62/packages/debian" --upload-file packages/cmocka_extensions_0.42.3_amd64.deb --header "CONTENT-TYPE: binary"
```
results in the error message 'An error has occurred and reported in the system's low-level error handler.'
All current attempts to debug this error further were not successful

## Generic repository
No creation of a repository is necessary, and uploading a debian file using 
``` shell
curl --header "PRIVATE-TOKEN: token" --upload-file $(pwd)/cmocka_extensions_0.42.3_amd64.deb "https://gitlabintern.emlix.com/api/v4/projects/62/packages/generic/corbos_tools/0.42.3/cmocka_extensions_0.42.3_amd64.deb"
```
works.

### Creating a debian repository inside the generic repository
It is possible to upload a debian Repositories Package file to the generic repository.
However, it seemed not possible to the access the registry as a debian repository using the command
``` shell
add-apt-repository "[trusted=yes] https://gitlabintern.emlix.com/api/v4/projects/62/packages/generic/corbos_tools/0.42.3"
```
Trying to add the repository without the 'trusted=yes' does work, but then accessing
the repository using ```apt update``` resulted in an error since the repository is not
a signed official one.

### Downloading debian files
Downloading a debian file from the generic repository works using the command
``` shell
curl --header "PRIVATE-TOKEN: token" "https://gitlabintern.emlix.com/api/v4/projects/62/packages/generic/corbos_tools/0.42.3/cmocka_extensions_0.42.3_amd64.deb" --output cmocka_extensions.deb
```

This file can the be installed using dpkg
