#!/bin/bash -eu

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

MD_DOCUMENTAION_DIR="${BASE_DIR}/documentation"
SAFU_SOURCE_SOURCE_DIR=${BASE_DIR}/src

SPHINX_SOURCE_DIR=${BASE_DIR}/doc/source
SPHINX_BUILD_DIR=${BASE_DIR}/doc/build
SPHINX_GENERATED_SOURCE_DIR=${BASE_DIR}/doc/source/generated

. ${SPHINX_VENV-${BASE_DIR}/.venv/}/bin/activate

rm -rf ${SPHINX_GENERATED_SOURCE_DIR}
mkdir -p ${SPHINX_GENERATED_SOURCE_DIR}/ADRs ${SPHINX_GENERATED_SOURCE_DIR}/developer

function createApiDocu() {
    sphinx-c-apidoc --force \
	    -o ${SPHINX_GENERATED_SOURCE_DIR}/api/ \
	    --tocfile index \
	    ${SAFU_SOURCE_SOURCE_DIR}/safu/public
}

function createDeveloperDocu() {
    cp -r ${MD_DOCUMENTAION_DIR}/images ${SPHINX_GENERATED_SOURCE_DIR}/developer/
    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/DeveloperManual.rst ${MD_DOCUMENTAION_DIR}/developer.md
    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/documentation.rst ${MD_DOCUMENTAION_DIR}/documentation.md

    echo -e "
Developer documentation
==========================

.. toctree::
  :maxdepth: 1
  :caption: Contents:

  DeveloperManual
  documentation
" > ${SPHINX_GENERATED_SOURCE_DIR}/developer/index.rst
}

createApiDocu
createDeveloperDocu

sphinx-build -b html ${SPHINX_SOURCE_DIR} ${SPHINX_BUILD_DIR}
