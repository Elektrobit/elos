#!/bin/bash -e

OUTDIR=/base/debianbuild

function print_help() {
  echo "$0 [-r|-p|-a|-h] <x.y.z>"
  echo -e "-r\tonly do the debian/main release"
  echo -e "-p\tonly do the pristine tar build and update"
  echo -e "-a\tdo all (default)"
  echo -e "-h\tprint help"
}

function setup_env() {
  GLOBMAIL=$(git config --global user.email || true)
  GLOBNAME=$(git config --global user.name || true)

  LOCMAIL=$(git config --local user.email || true)
  LOCNAME=$(git config --local user.name || true)

  TMPNAME="${GIT_AUTHOR_NAME:-${LOCNAME:-${GLOBNAME:-}}}"

  if [ -z "${TMPNAME}" ]; then
    echo "Invalid environment. Could not determine author name for git"
    exit 1
  fi

  TMPMAIL="${GIT_AUTHOR_EMAIL:-${LOCMAIL:-${GLOBMAIL:-}}}"

  if [ -z "${TMPMAIL}" ]; then
    echo "Invalid environment. Could not determine author e-mail for git"
    exit 1
  fi

  export GIT_AUTHOR_EMAIL="${TMPMAIL}"
  export GIT_COMMITTER_EMAIL="${TMPMAIL}"
  export EMAIL="${TMPMAIL}"
  export GIT_AUTHOR_NAME="${TMPNAME}"
  export GIT_COMMITTER_NAME="${TMPNAME}"
  export NAME="${TMPNAME}"

  export DEBNAME="${NAME}"
  export DEBMAIL="${EMAIL}"

  export DEBIAN_FRONTEND=noninteractive

  export ELOS_DEPENDENCY_CONFIG=./ci/dependencies_emlix.json
}

function create_and_publish_debian_main() {
  gbp import-ref -u "${NEW_VERSION}" --debian-branch "$(git branch --show-current)"

  cp debian/changelog debian.native/
  cp debian/gbp.conf debian.native/
  git rm -rf debian/
  cp -arv debian.native debian
  rm debian/README.md
  echo "3.0 (quilt)" > debian/source/format

  dch -D unstable -M "--newversion=${NEW_VERSION}-1" "New upstream tag ${NEW_VERSION}"
  git add debian/ && git commit -m "New upstream tag ${NEW_VERSION}"
  git checkout HEAD -- debian.native && git clean -fxd -- debian.native
}

function install_dependencies() {
  ./ci/install_deps.py

  DEP_LIST="cmocka_extensions cmocka_mocks safu samconf"
  for dependency in ${DEP_LIST}; do
    pushd "build/deps/src/${dependency}"
    git checkout debian/main
    gbp buildpackage --git-ignore-branch --git-compression=xz -uc -us
    case "${dependency}" in
      cmocka_mocks)
          sudo find ../ -type f -name "libmock-*_*.deb" -exec dpkg -i {} \;
          ;;
      safu)
          sudo find ../ -type f -name "libsafu*_*.deb" -exec dpkg -i {} \;
          sudo find ../ -type f -name "libmock-safu*_*.deb" -exec dpkg -i {} \;
          ;;
      samconf)
          sudo dpkg -i /base/build/deps/src/libsamconf*.deb /base/build/deps/src/libmock-samconf*.deb
          ;;
    esac
    sudo find ../ -type f -name "*$(echo "${dependency}"| tr '_' '-')*-dev_*.deb" -exec dpkg -i {} \;
    popd
  done

  rm -rf build/
}

function create_and_publish_pristine_tar() {
  install_dependencies
  gbp buildpackage --git-ignore-branch --git-compression=xz --git-ignore-new -uc -us --git-export-dir="${OUTDIR}" --git-ignore-branch
}

UPDATE_RELEASE=0
UPDATE_PRISTINE=0
UPDATE_ALL=1

while getopts "raph" opt; do
    case $opt in
        r) UPDATE_RELEASE=1;;
        p) UPDATE_PRISTINE=1;;
        a) UPDATE_ALL=1;;
        h) print_help; exit 0;;
        \?) echo "Invalid option: -$OPTARG"; print_help; exit 1;;
    esac
done
NEW_VERSION=${!OPTIND}

if [ -z "$NEW_VERSION" ]; then
    echo "Error: Version number is required."
    print_help
    exit 1
fi

if [ $UPDATE_RELEASE -eq 0 ] && [ $UPDATE_PRISTINE -eq 0 ]; then
    UPDATE_ALL=1
else
    UPDATE_ALL=0
fi

echo "Create release: ${NEW_VERSION}"

setup_env

if [ $UPDATE_ALL -eq 1 ] || [ $UPDATE_RELEASE -eq 1 ]; then
  create_and_publish_debian_main
fi

if [ $UPDATE_ALL -eq 1 ] || [ $UPDATE_PRISTINE -eq 1 ]; then
  create_and_publish_pristine_tar
fi
