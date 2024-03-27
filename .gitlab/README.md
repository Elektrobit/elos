# GitLab CI for elos

The goal of this is to provide a gitlab-ci setup which can be used to build and
test the elos build on premise in self hosted gitlab instance.
The official open source repository uses the GitHub-CI.

## Aim of the Pipeline

- [x] build elos
- [x] run the unit tests
	- [x] integrate the [unit-test report](https://docs.gitlab.com/ee/ci/yaml/artifacts_reports.html#artifactsreportsjunit) into GitLab
- [x] run the smoke tests
- [x] run the integration tests
	- [ ] integrate the integration test report into GitLab like the unit tests
- [x] lint the source code
- [x] run benchmarks
- [ ] create a coverage report
	- [ ] integrate the [coverage report](https://docs.gitlab.com/ee/ci/yaml/artifacts_reports.html#artifactsreportscoverage_report) into GitLab
- [ ] publish the KPIs to a time series database
- [x] build the documentation
- [ ] run the [BaseOS Lab](https://gitlabintern.emlix.com/elektrobit/base-os/eb-baseos-lab) tests to test for other targets

## How to enable the pipeline for a repository

To enable this pipeline in elos under *Settings > CI/CD > General pipelines > CI/CD configuration file* enter:

```
./gitlab/gitlab-ci.yml
```

Or follow the GitLab [pipeline documentation](https://docs.gitlab.com/ee/ci/pipelines/settings.html#specify-a-custom-cicd-configuration-file) for all the options.

### Exported configuration variables

The gitlab-CI setup exports some variables which can be configured by the
gitlab instance ti should run on. For details how to set those variables see
[gitlab:define-a-cicd-variable-in-the-ui](https://docs.gitlab.com/ee/ci/variables/#define-a-cicd-variable-in-the-ui).

#### SOURCES_URI:

Set this to a base URI to checkout dependencies like safu and samconf:

```
SOURCES_URI = https://${GIT_USER_TOKEN}@mygitlabinstance.com/<group>/
```

The default is the public github repository group https://github.com/Elektrobit/elos.

#### ELOS_DEPENDENCY_CONFIG:

Set this to define configuration file for `install_deps` script. See
https://elektrobit.github.io/elos/ci/index.html for details about options to
tune each dependency checkout and build.

```
ELOS_DEPENDENCY_CONFIG = ci/dependencies_emlix.json
```

The default uses a configuration file which in turn uses `SOURCES_URI` to
checkout all dependencies.

## How to extend the pipeline

For a documentation on how to add/modify follow the [GitLab CI documentation](https://docs.gitlab.com/ee/ci/)

## known issues

- right now it is constrained to shell runner because docker in docker doesn't work for the moment
