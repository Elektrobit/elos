SOURCES_URI=${SOURCES_URI:-https://github.com/Elektrobit/}

DEFAULT_BRANCH="integration"
if [ "$SOURCES_URI" = "https://github.com/Elektrobit/" ]; then
    DEFAULT_BRANCH="main"
fi

CMOCKA_EXTENSIONS_REPO_NAME=${CMOCKA_EXTENSIONS_REPO_NAME:-cmocka_extensions.git}
CMOCKA_EXTENSIONS_REPO_PATH=${CMOCKA_EXTENSIONS_REPO_PATH:-${SOURCES_URI}/${CMOCKA_EXTENSIONS_REPO_NAME}}
CMOCKA_EXTENSIONS_REPO_REF=${CMOCKA_EXTENSIONS_REPO_REF:-$DEFAULT_BRANCH}

CMOCKA_MOCKS_REPO_NAME=${CMOCKA_MOCKS_REPO_NAME:-cmocka_mocks.git}
CMOCKA_MOCKS_REPO_PATH=${CMOCKA_MOCKS_REPO_PATH:-${SOURCES_URI}/${CMOCKA_MOCKS_REPO_NAME}}
CMOCKA_MOCKS_REPO_REF=${CMOCKA_MOCKS_REPO_REF:-$DEFAULT_BRANCH}

SAFU_REPO_NAME=${SAFU_REPO_NAME:-safu.git}
SAFU_REPO_PATH=${SAFU_REPO_PATH:-${SOURCES_URI}/${SAFU_REPO_NAME}}
SAFU_REPO_REF=${SAFU_REPO_REF:-$DEFAULT_BRANCH}

SAMCONF_REPO_NAME=${SAMCONF_REPO_NAME:-samconf.git}
SAMCONF_REPO_PATH=${SAMCONF_REPO_PATH:-${SOURCES_URI}/${SAMCONF_REPO_NAME}}
SAMCONF_REPO_REF=${SAMCONF_REPO_REF:-$DEFAULT_BRANCH}

CMAKE_PARAM="${CMAKE_PARAM} -D CMOCKA_EXTENSIONS_URI=${CMOCKA_EXTENSIONS_REPO_PATH} \
                            -D CMOCKA_EXTENSIONS_REF=${CMOCKA_EXTENSIONS_REPO_REF}"
CMAKE_PARAM="${CMAKE_PARAM} -D CMOCKA_MOCKS_URI=${CMOCKA_MOCKS_REPO_PATH} \
                            -D CMOCKA_MOCKS_REF=${CMOCKA_MOCKS_REPO_REF}"
CMAKE_PARAM="${CMAKE_PARAM} -D SAFU_URI=${SAFU_REPO_PATH} \
                            -D SAFU_REF=${SAFU_REPO_REF}"
CMAKE_PARAM="${CMAKE_PARAM} -D SAMCONF_URI=${SAMCONF_REPO_PATH} \
                            -D SAMCONF_REF=${SAMCONF_REPO_REF}"