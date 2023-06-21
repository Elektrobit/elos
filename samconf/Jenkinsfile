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
    gitlabBuilds(builds: ["samconf", "samconf: build debug", "samconf: build release", "samconf: utest debug", "samconf: utest release", "samconf: lint sources", "samconf: documentation", "samconf: coverage"])
    buildDiscarder(logRotator(numToKeepStr: env.BRANCH_NAME == "master"? "1000": env.BRANCH_NAME == "integration"?"1000":"3"))
  }

  agent {
    dockerfile {
        filename './samconf/ci/Dockerfile'
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
        updateGitlabCommitStatus name: 'samconf', state: 'running'
      }
    }

    stage('Build') {
      steps {
        parallel(
          debug: {
            gitlabCommitStatus("samconf: build debug") {
              sh '''#!/bin/bash -xe
                ./samconf/ci/build.sh --ci Debug
              '''
            }
          },
          release: {
            gitlabCommitStatus("samconf: build release") {
              sh '''#!/bin/bash -xe
                ./samconf/ci/build.sh --ci Release
              '''
            }
          }
        )
      }
    }

    stage('Run UnitTest') {
      steps {
        parallel(
          debug: {
            gitlabCommitStatus("samconf: utest debug") {
              sh './samconf/ci/run_utest.sh'
            }
          },
          release: {
            gitlabCommitStatus("samconf: utest release") {
              sh './samconf/ci/run_utest.sh Release'
            }
          }
        )
      }
      post {
        failure {
          archiveArtifacts artifacts: "samconf/build/Debug/Testing/Temporary/,samconf/build/Release/Testing/Temporary/", fingerprint: true
        }
      }
    }

    stage('Run Smoke Test') {
      steps {
        gitlabCommitStatus("samconf: smoke test") {
          sh '''#!/bin/bash -xe
            ./samconf/ci/run_smoketests.sh
            ./samconf/ci/run_smoketests.sh Release
          '''
        }
      }
      post {
        always {
          archiveArtifacts artifacts: "samconf/build/Debug/result/smoketest_results/,samconf/build/Release/result/smoketest_results/", fingerprint: true
        }
      }
    }

    stage('Lint sources') {
      steps{
        gitlabCommitStatus("samconf: lint sources") {
          sh '''#!/bin/bash -xe
            ./samconf/ci/code_lint.py --ci
            ./samconf/ci/checklicense.sh
          '''
        }
      }
      post {
        always {
          archiveArtifacts artifacts: "samconf/build/Release/cmake/lint_results/**", fingerprint: true
        }
      }
    }

    stage('Build documentation') {
      steps{
        gitlabCommitStatus("samconf: documentation") {
          sh './samconf/ci/build_doc.sh'
        }
      }
      post {
        success {
          archiveArtifacts artifacts: "samconf/doc/build/**", fingerprint: true
        }
      }
    }

    stage('Create coverage report') {
      steps {
        gitlabCommitStatus("samconf: coverage") {
          sh '''#!/bin/bash -xe
            ./samconf/ci/create_coverage.sh
          '''
        }
      }
      post {
        always {
          archiveArtifacts artifacts: "samconf/build/Release/result/coverage_results/**", fingerprint: true
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
        updateGitlabCommitStatus name: 'samconf', state: 'success'
    }
    failure {
        updateGitlabCommitStatus name: 'samconf', state: 'failed'
    }
    always {
      withCredentials([usernamePassword(credentialsId: 'kpi_creds', passwordVariable: 'KPI_API_TOKEN', usernameVariable: 'KPI_API_URL')]) {
        sh './samconf/ci/publish_kpis.sh'
      }
      cleanWs(deleteDirs: true, patterns: [
          [pattern: '*', type: 'INCLUDE'],
          [pattern: 'samconf', type: 'EXCLUDE'],
          [pattern: 'safu', type: 'EXCLUDE'],
          [pattern: 'elos', type: 'EXCLUDE'],
          [pattern: '*/build*', type: 'INCLUDE'],
        ])
    }
  }
}
