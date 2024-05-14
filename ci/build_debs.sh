#!/bin/bash -eu

export DEBIAN_FRONTEND="noninteractive"
export DEBEMAIL="noreply@test.com"

CODENAME="${1:-jammy}"

# Install dependencies
apt-get update
apt-get install -y git debhelper devscripts equivs software-properties-common fakeroot
add-apt-repository -y ppa:elos-team/ppa

# Get the packaging scripts from the debian branch
git fetch origin
git checkout origin/debian/main -- debian

# Use native packaging so we don't need to muck about with tarballs
echo "3.0 (native)" > debian/source/format

# Add a new changelog entry
dch --local="+gitlab-ci" --distribution "${CODENAME}" "Gitlab CI/CD test build for branch ${CI_COMMIT_BRANCH}"

# Install dependencies
yes | mk-build-deps -ir || true

# Make sure to remove any temporary files that may have been created
git clean -fxd

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
