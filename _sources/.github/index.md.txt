
# CI with GitHub Actions

## Intentions in using GitHub Actions

We want to test all PRs before mergeing by:
- [x] build test
- [ ] code linting
- [ ] license check
- [ ] unit-tests
- [ ] creating a test coverage report
- [ ] smoke tests
- [ ] integration tests
- [ ] running benchmarks
- [ ] automatic upload of new releases

## How to develop GitHub Actions

Find a detailed documentation [here](https://docs.github.com/de/actions)

### Adding Badges with CI status

Paste a link in the form of  `![badge](https://github.com/Elektrobit/elos/actions/workflows/build-and-test.yml/badge.svg)` where you want to integrate the badge for the `build-and-test` workflow.

Find an explanation [here](https://docs.github.com/de/actions/monitoring-and-troubleshooting-workflows/adding-a-workflow-status-badge)

### Running GitHub Actions locally

To run the GitHub Actions locally [act](https://github.com/nektos/act) can be used.

The way the Actions are written right now they need an ubuntu image with passwordless sudo.
With that image build under the tag `ubuntu-sudo` the command can be run:

```bash
act -P ubuntu-latest=ubuntu-sudo --pull=false --artifact-server-path=/tmp/artifacts
```
which leaves the build artefacts under `/tmp/artifacts`


### known issues

- job artefacts lose their permissions so executables are no longer set as executable when downloaded in a different job
