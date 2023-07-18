#!/bin/bash -eu

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

MD_DOCUMENTAION_DIR="${BASE_DIR}/documentation"
ELOS_SOURCE_SOURCE_DIR=${BASE_DIR}/src

SPHINX_SOURCE_DIR=${BASE_DIR}/doc/source
SPHINX_BUILD_DIR=${BASE_DIR}/doc/build
SPHINX_GENERATED_SOURCE_DIR=${BASE_DIR}/doc/source/generated

. ${SPHINX_VENV-${BASE_DIR}/.venv/}/bin/activate

rm -rf ${SPHINX_GENERATED_SOURCE_DIR}
mkdir -p ${SPHINX_GENERATED_SOURCE_DIR}/ADRs ${SPHINX_GENERATED_SOURCE_DIR}/developer

function createApiDocu() {
    sphinx-c-apidoc --force \
        -o ${SPHINX_GENERATED_SOURCE_DIR}/api/libelos \
        --tocfile index \
        ${ELOS_SOURCE_SOURCE_DIR}/libelos/public/elos

    echo -e "
Elos API
==========================

.. toctree::
  :maxdepth: 1
  :caption: Contents:

  libelos <libelos/index>
" > ${SPHINX_GENERATED_SOURCE_DIR}/api/index.rst
}

function createDeveloperApiDocu() {
    local API_INDEX_TABLE=""
    local MODULE_PATHS=$(find elos/src/ -name "public" -or -name "interface" -type d -exec dirname {} \;)

    for MODULE_PATH in $MODULE_PATHS; do
        local MODULE=$(basename ${MODULE_PATH})
        echo "build doc for ${MODULE} in ${MODULE_PATH}"
        case "${MODULE}" in
          libelos)
            # use already generated for the user manual and avoid duplicate
            # "Duplicate C declaration" warnings
            API_INDEX_TABLE="${API_INDEX_TABLE}  ${MODULE} <../../api/libelos/index>\n"
            ;;
          src | clients | demo | components | plugins | scanners | storagebackends)
            echo "SKIP ${MODULE}"
            ;;
          *)
            if [ -d ${MODULE_PATH}/public ]; then
                sphinx-c-apidoc --force \
                    -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/api/ \
                    --tocfile index \
                    ${MODULE_PATH}/public
            fi
            if [ -d ${MODULE_PATH}/interface ]; then
                sphinx-c-apidoc --force \
                    -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/api/ \
                    --tocfile index \
                    ${MODULE_PATH}/interface
            fi
            if [ -d ${MODULE_PATH}/private ]; then
                sphinx-c-apidoc --force \
                    -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/api/ \
                    --tocfile index \
                    ${MODULE_PATH}/private
            fi
            ;;
        esac
    done

    # remove generated but unsued files
    rm -r ${SPHINX_GENERATED_SOURCE_DIR}/developer/api/elos/elos.rst

    for DOC in $(find /base/elos/doc/source/generated/developer/api/ -name "*.rst" -and -not -name "index.rst"); do
        CHAPTER_DOC_PATH="${DOC##"${SPHINX_GENERATED_SOURCE_DIR}/developer/api/"}"
        echo "----> ${CHAPTER_DOC_PATH}"
        API_INDEX_TABLE="${API_INDEX_TABLE}  ${CHAPTER_DOC_PATH}\n"
    done

    # override generated index.rst
    echo -e "
Elos API
==========================

.. toctree::
  :maxdepth: 1
  :caption: Contents:

${API_INDEX_TABLE}
" > ${SPHINX_GENERATED_SOURCE_DIR}/developer/api/index.rst
}

function createUserDocu() {
    mkdir -p ${SPHINX_GENERATED_SOURCE_DIR}/images
    cp ${MD_DOCUMENTAION_DIR}/images/elos_layout.png ${SPHINX_GENERATED_SOURCE_DIR}/images

    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/UserManual.rst ${MD_DOCUMENTAION_DIR}/userManual.md
}

function createDeveloperDocu() {
    cp -r ${MD_DOCUMENTAION_DIR}/images ${SPHINX_GENERATED_SOURCE_DIR}/developer/
    mkdir -p ${SPHINX_GENERATED_SOURCE_DIR}/images
    cp ${MD_DOCUMENTAION_DIR}/images/overview_event_logging.png ${SPHINX_GENERATED_SOURCE_DIR}/images
    cp ${MD_DOCUMENTAION_DIR}/images/eventprocessor_components.png ${SPHINX_GENERATED_SOURCE_DIR}/images

    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/DeveloperManual.rst ${MD_DOCUMENTAION_DIR}/developer.md
    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/eventprocessor.rst ${MD_DOCUMENTAION_DIR}/eventprocessor/eventprocessor.md
    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/rpnfilter.rst ${MD_DOCUMENTAION_DIR}/rpnfilter/rpnfilter.md
    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/VerificationStrategy.rst ${MD_DOCUMENTAION_DIR}/verification_strategy.md
    pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/developer/documentation.rst ${MD_DOCUMENTAION_DIR}/documentation.md

    echo -e "
Developer documentation
==========================

.. toctree::
  :maxdepth: 1
  :caption: Contents:

  DeveloperManual
  eventprocessor
  rpnfilter
  VerificationStrategy
  documentation
  Elos API <api/index>
" > ${SPHINX_GENERATED_SOURCE_DIR}/developer/index.rst
}

function createADRs() {

    mkdir -p ${SPHINX_GENERATED_SOURCE_DIR}/images
    cp ${MD_DOCUMENTAION_DIR}/images/adr_distributed_event_log_storage.png ${SPHINX_GENERATED_SOURCE_DIR}/images

    ADR_INDEX_TABLE=""
    ADRs=$(find ${MD_DOCUMENTAION_DIR}/Architecture_Design_Records/ -type f -name "*.md")
    for adrFile in ${ADRs}; do
        adr=$(basename ${adrFile} .md)
        pandoc --from gfm --to rst -o ${SPHINX_GENERATED_SOURCE_DIR}/ADRs/${adr}.rst ${adrFile}
        ADR_INDEX_TABLE="${ADR_INDEX_TABLE}   ${adr}\n"
    done

    echo -e "
Architecture Design Records
===========================

.. toctree::
   :maxdepth: 1
   :caption: Contents:

${ADR_INDEX_TABLE}
" > ${SPHINX_GENERATED_SOURCE_DIR}/ADRs/adrs.rst
}

createUserDocu
createApiDocu
createDeveloperDocu
createDeveloperApiDocu
createADRs

export PATH="${PATH}:${BASE_DIR}/build/Debug/dist/usr/local/bin"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-"./"}:${BASE_DIR}/build/Debug/dist/usr/local/lib"
sphinx-build -b html ${SPHINX_SOURCE_DIR} ${SPHINX_BUILD_DIR}
