# elos Debian Packaging Scripts

## Overview

This directory contains Debian packaging scripts for building the current source
tree as .deb packages. This is primarily for CI pipelines to automatically
verify if the packages are buildable or if anything needs to be changed. This is
not intended to be used directly for building .deb packages for new releases.

## Building

The .deb packages are constructed in the standard Debian way using `debhelper`.
The easiest way to build the packages is to just call the `ci/build_debs.sh`
script from the top-level directory from inside a Docker/Podman container, which
will automatically install all of the necessary dependencies, set the package
version, and build the packages using `debhelper`.

It's possible to do this all in one line to verify if the packages are buildable
on various Debian-based distributions and series. E.g., to build it on Ubuntu
Jammy:

```bash
podman run --rm -v $(pwd):/build -w /build ubuntu:jammy ci/build_debs.sh
```

Please note that there are some dependencies which are not (yet) in the Debian
or Ubuntu archives. These have been built and published for Ubuntu in the
[elos PPA](https://launchpad.net/~elos-team/+archive/ubuntu/ppa), which is
automatically added by the `ci/build_debs.sh` script. This currently restricts
builds using the script to Ubuntu systems, as PPAs are not available on other
distributions.

## Extending

The Debian packaging scripts leverage the existing CMake build system used by
elos to build everything, meaning that very little, if anything, generally needs
to be changed inside the packaging scripts when a code change is made to elos.
The major exception to this is when new libraries or files need to be installed.

In most cases, new files and/or folders can be added to the `.install` file
corresponding to the package name that the new files and/or folders should
belong to.

New packages must be added to the `control` file, and their respective files
should be added to a new `.install` file matching the name of the new package.

## Packaging Script Maintainer

* Isaac True isaac.true@emlix.com [@IsaacJT](https://github.com/IsaacJT)
