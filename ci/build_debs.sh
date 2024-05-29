#!/bin/bash -eu

export DEBIAN_FRONTEND="noninteractive"
export DEBEMAIL="noreply@test.com"

CODENAME="${1:-jammy}"

echo "Building .deb packages for series ${CODENAME}"
echo "---"
echo "NOTICE: make sure to clean up before running this! Debhelper will complain otherwise"

# Install dependencies
apt-get update
apt-get install -y git debhelper devscripts equivs software-properties-common fakeroot
add-apt-repository -y ppa:elos-team/ppa

# Use the native packaging folder for building the .debs
rm -rf debian
cp -rv debian.native debian

# Add a new changelog entry
git config --global --add safe.directory "$(pwd)"
dch --newversion="$(git describe --tags | cut -f2- -d- | tr '-' '.')~test" \
        --distribution "${CODENAME}" "Test build"

# Install dependencies
yes | mk-build-deps -ir || true

# Ignore Lintian errors from the native packaging
mkdir -p debian/source
echo "malformed-debian-changelog-version" >> debian/source/lintian-overrides
echo "root-in-contact" >> debian/source/lintian-overrides
echo "debian-watch-file-in-native-package" >> debian/source/lintian-overrides
echo "odd-historical-debian-changelog-version" >> debian/source/lintian-overrides

# Build the unsigned .debs
fakeroot debuild --no-tgz-check -uc -us

# Need to move the files into the project directory
mv ../*.deb ./
