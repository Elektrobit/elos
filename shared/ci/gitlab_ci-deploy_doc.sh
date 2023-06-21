#!/bin/sh

BRANCH=$1

if [ $BRANCH != "integration" ]; then
    echo "skip deploymend of landing page for $BRANCH"
    exit
fi

URL_ENCODED_BRANCH=$(echo "${BRANCH}" | sed -e 's,/,%2F,g' -e 's,#,%23,g')

ELOS_URL="https://jenkins.emlix.com/job/eb-baseos-elos/job/${URL_ENCODED_BRANCH}/lastSuccessfulBuild/artifact/elos/doc/build/index.html"

SAFU_URL="https://jenkins.emlix.com/job/eb-baseos-safu/job/${URL_ENCODED_BRANCH}/lastSuccessfulBuild/artifact/safu/doc/build/index.html"

SAMCONF_URL="https://jenkins.emlix.com/job/eb-baseos-libconfig/job/${URL_ENCODED_BRANCH}/lastSuccessfulBuild/artifact/samconf/doc/build/index.html"

if [ "${BRANCH}" == "integration" ]; then
    LANDING_PAGE="public/index.html"
else
    LANDING_PAGE="public/${URL_ENCODED_BRANCH}/index.html"
fi

mkdir -p ${LANDING_PAGE%/*}
cp pages/index.html ${LANDING_PAGE}

sed -i "s,{{ELOS_URL}},${ELOS_URL}," ${LANDING_PAGE}
sed -i "s,{{SAFU_URL}},${SAFU_URL}," ${LANDING_PAGE}
sed -i "s,{{SAMCONF_URL}},${SAMCONF_URL}," ${LANDING_PAGE}
