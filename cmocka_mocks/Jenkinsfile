import jenkins.*
import jenkins.model.*
import hudson.*
import hudson.model.*

def ASMCOV_URI

node {
    ASMCOV_URI = ''

    script {
      def creds = com.cloudbees.plugins.credentials.CredentialsProvider.lookupCredentials(
        com.cloudbees.plugins.credentials.impl.UsernamePasswordCredentialsImpl.class,
        Jenkins.instance,
        null,
        null
      );
      def jenkins_asmcov_uri = creds.findResult { it.id == 'jenkins_asmcov_uri' ? it : null }
      if(jenkins_asmcov_uri) {
        println(jenkins_asmcov_uri.id + ": " +jenkins_asmcov_uri.username + ": " + jenkins_asmcov_uri.password)
        ASMCOV_URI=jenkins_asmcov_uri.password
      }
    }
}

properties([gitLabConnection('GitLab')])

pipeline {
  options {
    gitlabBuilds(builds: ["cmocka_mocks", "cmocka_mocks: build debug", "cmocka_mocks: build release", "cmocka_mocks: lint sources", "cmocka_mocks: documentation"])
    buildDiscarder(logRotator(numToKeepStr: env.BRANCH_NAME == "master"? "1000": env.BRANCH_NAME == "integration"?"1000":"3"))
  }

  agent {
    dockerfile {
        filename './cmocka_mocks/ci/Dockerfile'
        reuseNode true
        additionalBuildArgs "--build-arg USER=jenkins \
                        --build-arg UID=\$(id -u) --build-arg GID=\$(id -g) --build-arg ASMCOV_URI=${ASMCOV_URI}"
        args '--privileged --userns=keep-id'
        label "podman"
    }
  }

  stages {
    stage('debug') {
      steps{
        sh 'ls -lah'
        sh 'env'
        sh 'gcc --version'
        sh 'cmake --version'
        updateGitlabCommitStatus name: 'cmocka_mocks', state: 'running'
      }
    }

    stage('Build') {
      steps {
        parallel(
          debug: {
            gitlabCommitStatus("cmocka_mocks: build debug") {
              sh '''#!/bin/bash -xe
                ./cmocka_mocks/ci/build.sh --ci Debug
              '''
            }
          },
          release: {
            gitlabCommitStatus("cmocka_mocks: build release") {
              sh '''#!/bin/bash -xe
                ./cmocka_mocks/ci/build.sh --ci Release
              '''
            }
          }
        )
      }
    }

    stage('Lint sources') {
      steps{
        gitlabCommitStatus("cmocka_mocks: lint sources") {
          sh '''#!/bin/bash -xe
            ./cmocka_mocks/ci/code_lint.py --ci
            ./cmocka_mocks/ci/checklicense.sh
          '''
        }
      }
      post {
        always {
          archiveArtifacts artifacts: "cmocka_mocks/build/Release/result/lint_results/**", fingerprint: true
        }
      }
    }

    stage('Build documentation') {
      steps{
        gitlabCommitStatus("cmocka_mocks: documentation") {
          sh './cmocka_mocks/ci/build_doc.sh'
        }
      }
      post {
        success {
          archiveArtifacts artifacts: "cmocka_mocks/build/Debug/doc/**, documentation/monitor.md", fingerprint: true
        }
      }
    }
  }

  post {
    changed {
      script {
        def jobName = env.JOB_NAME.tokenize('/') as String[];
        def projectName = jobName[0];
        def title = '[' + projectName + '] '
        def message = '';

        if (currentBuild.currentResult == 'FAILURE') {
          title += 'Pipeline for ' + env.BRANCH_NAME + ' has failed';
          message = 'Hi, sorry but the pipeline is broken. See ' + env.BUILD_URL + ' for details.'
        }

        if(currentBuild.currentResult == 'SUCCESS') {
          title += 'Pipeline for ' + env.BRANCH_NAME + ' is now stable again';
          message = 'Hi, the pipeline is now stable again. See ' + env.BUILD_URL + ' for details.'
        }

        emailext subject: title,
          body: message,
          recipientProviders: [
              [$class: 'CulpritsRecipientProvider'],
              [$class: 'DevelopersRecipientProvider'],
              [$class: 'RequesterRecipientProvider']
          ],
          replyTo: '$DEFAULT_REPLYTO',
          to: '$DEFAULT_RECIPIENTS'
      }
    }
    success {
        updateGitlabCommitStatus name: 'cmocka_mocks', state: 'success'
    }
    failure {
        updateGitlabCommitStatus name: 'cmocka_mocks', state: 'failed'
    }
    always {
      withCredentials([usernamePassword(credentialsId: 'kpi_creds', passwordVariable: 'KPI_API_TOKEN', usernameVariable: 'KPI_API_URL')]) {
        sh './cmocka_mocks/ci/publish_kpis.sh'
      }
      cleanWs(cleanWhenNotBuilt: false,
          deleteDirs: true,
          disableDeferredWipeout: true,
          notFailBuild: true,
          patterns: [[pattern: '.gitignore', type: 'INCLUDE'],
	      [pattern: '.trace', type: 'INCLUDE']])
    }
  }
}
