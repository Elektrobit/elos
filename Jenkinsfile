import jenkins.*
import jenkins.model.*
import hudson.*
import hudson.model.*

def ASMCOV_URI

def withDockerNetwork(Closure inner) {
  try {
    networkId = UUID.randomUUID().toString()
    sh "docker network create ${networkId}"
    inner.call(networkId)
  } finally {
    sh "docker network rm ${networkId}"
  }
}
def stages() {
    def stages = ["elos", "build release", "build debug", "utest", "smoke test", "lint sources", "Benchmarks", "documentation", "integration test"]
    if (env.BRANCH_NAME =~ 'story.*' || env.BRANCH_NAME =~ 'bug.*' || env.BRANCH_NAME =~ 'task.*' || env.BRANCH_NAME == 'integration' || env.BRANCH_NAME == 'master' ) {
       stages.push("baseos-lab")
    }
    return stages
}

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
      println("stages:" + stages())
    }
}

node {
  SOURCES_URI = ''
  script {
    def creds = com.cloudbees.plugins.credentials.CredentialsProvider.lookupCredentials(
      com.cloudbees.plugins.credentials.impl.UsernamePasswordCredentialsImpl.class,
      Jenkins.instance,
      null,
      null
    );
    def baseos_gitlab_uri = creds.findResult { it.id == 'baseos_gitlab_uri' ? it : null }
    if(baseos_gitlab_uri) {
      println(baseos_gitlab_uri.id + ": " +baseos_gitlab_uri.username + ": " + baseos_gitlab_uri.password)
      SOURCES_URI=baseos_gitlab_uri.password
      println("Sources URI is " + SOURCES_URI)
    }
  }
}

properties([gitLabConnection('GitLab')])

pipeline {
  options {
    gitlabBuilds(builds: stages() )
    buildDiscarder(logRotator(numToKeepStr: env.BRANCH_NAME == "master"? "1000": env.BRANCH_NAME == "integration"?"1000":"3"))
  }
  agent none
  stages {
    stage("Elos Run") {
      parallel {
        stage('trigger baseos-lab') {
          when { anyOf {
            equals expected: true, actual: stages().findAll{"baseos-lab".toLowerCase().contains(it.toLowerCase())}.any{true}
          }}
        steps {
          updateGitlabCommitStatus name: 'baseos-lab', state: 'skipped'
            gitlabCommitStatus("baseos-lab") {
              script {
                echo "triggering eb-baseos-elos-baseos-lab Pipeline with elos Branch '${BRANCH_NAME}'"
                build(job: 'eb-baseos-elos-baseos-lab',
                      wait: true,
                      parameters: [
                          string(name: 'ELOS_DEV_BRANCH', value: "${BRANCH_NAME}"),
                      ]
                )
              }
            }
          }
        }
        stage("Build and test") {
            agent {
              dockerfile {
                  filename './ci/Dockerfile'
                  reuseNode true
                  additionalBuildArgs "--build-arg USER=jenkins \
                                --build-arg UID=\$(id -u) --build-arg GID=\$(id -g) --build-arg ASMCOV_URI=${ASMCOV_URI}"
                  args "--privileged --userns=keep-id -e SOURCES_URI=${SOURCES_URI}"
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
                  updateGitlabCommitStatus name: 'elos', state: 'running'
                }
              }

              stage('Build release') {
                steps{
                  gitlabCommitStatus("build release") {
                    sh '''#!/bin/bash -xe
                      ./ci/build.sh --ci Release
                    '''
                  }
                }
              }

              stage('Build debug') {
                steps{
                  gitlabCommitStatus("build debug") {
                    sh '''#!/bin/bash -xe
                     ./ci/build.sh --ci Debug
                    '''
                  }
                }
              }

              stage('Run UnitTest') {
                steps {
                  gitlabCommitStatus("utest") {
                    sh '''#!/bin/bash -xe
                      ./ci/run_utest.sh
                      ./ci/run_utest.sh Release
                    '''
                  }
                }
                post {
                  always {
                    archiveArtifacts artifacts: "build/Debug/cmake/result/unit_test/,build/Release/result/unit_test/", fingerprint: true
                      script {
                      step (
                        [$class: 'JUnitResultArchiver', testResults: 'build/*/result/unit_test/junit.xml']
                      )
                    }

                  }
                }
              }

              stage('Run Smoke Test') {
                steps {
                  gitlabCommitStatus("smoke test") {
                    sh '''#!/bin/bash -xe
                      export SMOKETEST_TMP_DIR="$(realpath ./build/tmp)"
                      ./ci/run_smoketests.sh Debug
                      ./ci/run_smoketests.sh Release
                    '''
                  }
                }
                post {
                  always {
                    archiveArtifacts artifacts: "build/Debug/result/smoketest_results/,build/Release/result/smoketest_results/", fingerprint: true
                  }
                }
              }

              stage('Lint sources') {
                steps{
                  gitlabCommitStatus("lint sources") {
                    sh '''#!/bin/bash -xe
                      . ./ci/ignored_sources.sh
                      ./ci/code_lint.py --ci
                      ./ci/checklicense.sh
                      ./ci/run_integration_tests_linter.sh Release
                    '''
                  }
                }
                post {
                  failure {
                    archiveArtifacts artifacts: "build/Release/result/integration/robot_lint/*.log", fingerprint: true
                  }
                  always {
                    archiveArtifacts artifacts: "build/Release/cmake/lint_results/**", fingerprint: true
                  }
                }
              }

              stage('Build documentation') {
                steps{
                  gitlabCommitStatus("documentation") {
                    sh './ci/build_doc.sh'
                  }
                }
                post {
                  success {
                    archiveArtifacts artifacts: "build/Debug/doc/**", fingerprint: true
                  }
                }
              }

              stage('Run Benchmarks') {
                steps {
                  gitlabCommitStatus("Benchmarks") {
                    sh './ci/run_benchmarks.sh Release'
                  }
                }
                post {
                  success {
                    archiveArtifacts artifacts: "build/Release/result/benchmark_results/**/*.csv", fingerprint: true
                    archiveArtifacts artifacts: "build/Release/result/benchmark_results/**/*.png", fingerprint: true
                  }
                  failure {
                    archiveArtifacts artifacts: "build/Release/result/benchmark_results/**", fingerprint: true
                  }
                }
              }

              stage('Create coverage report') {
                steps {
                  gitlabCommitStatus("coverage") {
                    sh '''#!/bin/bash -xe
                      ./ci/create_coverage.sh
                    '''
                  }
                }
                post {
                  always {
                    archiveArtifacts artifacts: "build/Release/result/coverage_results/**", fingerprint: true
                  }
                }
              }
            }
            post {
              success {
                  updateGitlabCommitStatus name: 'elos', state: 'success'
              }
              failure {
                  updateGitlabCommitStatus name: 'elos', state: 'failed'
              }
              always {
                  withCredentials([usernamePassword(credentialsId: 'kpi_creds', passwordVariable: 'KPI_API_TOKEN', usernameVariable: 'KPI_API_URL')]) {
                    sh './ci/publish_kpis.sh'
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
        stage ("Run integration test") {
          agent {
            label "docker"
          }
          environment {
            DOCKER_BUILDKIT = 0
	    BUILD_ARG = "--build-arg USER=jenkins"
            SOURCES_URI = "${SOURCES_URI}"
          }
          steps {
            gitlabCommitStatus("integration test") {
              sh '''#!/bin/bash -xe
                ./ci/run_integration_tests.sh Release
              '''
            }
          }
          post {
            always {
              archiveArtifacts artifacts: "build/Release/result/**", fingerprint: true
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
        updateGitlabCommitStatus name: 'elos', state: 'success'
    }
    failure {
        updateGitlabCommitStatus name: 'elos', state: 'failed'
    }
  }
}
