#!/bin/bash -eu

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

function printHelp() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo -e "\t -c\t\tclean output directory and generated files before building"
    echo -e "\t -h\t\tprint this help and exit"
}

PARAM=""
OPTION_CLEAN=0
for element in $@; do
    case $element in
        --clean|-c)
            OPTION_CLEAN=1 ;;
        --help|-h)
            printHelp
			exit 0 ;;
        -*)
            echo "error: unknown option: ${element}"
            printHelp
            exit 1 ;;
        *)  PARAM="$PARAM $element" ;;
    esac
done

set -- $PARAM

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
DIST_DIR="$BUILD_DIR/dist"

MD_DOCUMENTAION_DIR="${BASE_DIR}/documentation"
ELOS_SOURCE_SOURCE_DIR=${BASE_DIR}/src

SPHINX_SOURCE_DIR=${BASE_DIR}
SPHINX_BUILD_DIR=${BUILD_DIR}/doc
SPHINX_GENERATED_SOURCE_DIR=${SPHINX_BUILD_DIR}/source_generated
SPHINX_HTML_OUTPUT_DIR=${SPHINX_BUILD_DIR}/html

. ${SPHINX_VENV-${BASE_DIR}/.venv/}/bin/activate


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

function createModuleApiDoc() {
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

}

function createDeveloperApiDocu() {
    local API_INDEX_TABLE=""
    local MODULE_PATHS=$(find ${ELOS_SOURCE_SOURCE_DIR} -name "public" -or -name "interface" -type d -exec dirname {} \;)

    API_JOB_IDS=""
    for MODULE_PATH in $MODULE_PATHS; do
        createModuleApiDoc "${MODULE_PATH}" &
        API_JOB_IDS="${API_JOB_IDS} $!"
    done

    wait ${API_JOB_IDS}

    # remove generated but unsued files
    rm -r ${SPHINX_GENERATED_SOURCE_DIR}/developer/api/elos/elos.rst

    for DOC in $(find ${SPHINX_GENERATED_SOURCE_DIR}/developer/api/ -name "*.rst" -and -not -name "index.rst"); do
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

if [ ${OPTION_CLEAN} -eq 1 ]; then
    echo "Delete ${SPHINX_GENERATED_SOURCE_DIR} ${SPHINX_BUILD_DIR}"
    rm -rf ${SPHINX_GENERATED_SOURCE_DIR} ${SPHINX_BUILD_DIR}
fi

mkdir -p ${SPHINX_BUILD_DIR} ${SPHINX_GENERATED_SOURCE_DIR}/ADRs ${SPHINX_GENERATED_SOURCE_DIR}/developer

createApiDocu
createDeveloperApiDocu

export PATH="${PATH}:${DIST_DIR}/usr/local/bin"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-"./"}:${DIST_DIR}/usr/local/lib"
sphinx-build -b html ${SPHINX_SOURCE_DIR} ${SPHINX_HTML_OUTPUT_DIR} 2> ${SPHINX_BUILD_DIR}/html_doc_error.log
if [ $? -ne 0 ]; then
    echo "Build failed, for details see ${SPHINX_BUILD_DIR}/html_doc_error.log"
    exit 1
fi

WARNINGS=$(grep -e ": WARNING:" -e ": ERROR:" ${SPHINX_BUILD_DIR}/html_doc_error.log | wc -l)
if [ ${WARNINGS} -ne 0 ]; then
    echo ""
    echo "Build warnings ${WARNINGS}"
    echo ""
    cat ${SPHINX_BUILD_DIR}/html_doc_error.log
    exit 1
fi
