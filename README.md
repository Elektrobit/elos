# elos

<img src="elos/doc/source/_static/elos_blue.svg" width=20% height=20% align="right">

elos is a tool to collect, store and publish various system events (i.e. syslogs, core dumps, measurements obtained from proc- and sys-fs, …) while providing easy access to the collected data.


elos and its libraries:
* [elos](elos/documentation/documentation.md)
* [safu](safu/documentation/documentation.md)
* [samconf](samconf/documentation/developer.md)

and for testing:

* [cmocka_mocks](cmocka_mocks/documentation/documentation.md)
* [cmocka_extensions](cmocka_extensions/documentation/documentation.md)




## Quick start

### Try elos

```bash
./elos/ci/docker-target-run.sh
...
ci@2153762153:/$ elosc -f "1 1 EQ"
...
ci@2153762153:/$ elosc -s "1 1 EQ"
```

On a second shell attach to the running container

```bash
docker exec -ti <elos_container_name> bash
ci@2153762153:/$ elosc -p '{"payload": "Hello elos"}'
```

For detailed build instructions have a look into `./elos/ci/docker-target-run.sh`
and/or consult the documentation https://elektrobit.github.io/elos .


### Development

For a quick start to get hands on the code and start coding use :
```bash
./elos/ci/docker-run.sh
```
You are now in a ready to use development environment. To build elos and all
dependencies run:
```bash
ci@62209400:/base$ ./elos/ci/build.sh
```
To start elosd and play with your changes:
```bash
ci@62209400:/base$ ./elos/test/smoketest/smoketest_interactive.sh
[elos-interactive]$ elosd&
...
[elos-interactive]$ elosc -f "1 1 EQ"
...
[elos-interactive]$ elosc -s "1 1 EQ"&
...
[elos-interactive]$ elosc -p '{"payload": "Hello elos"}'
...
[elos-interactive]$ elosc # be creative
```
Be aware this is a smoketest environment so it is very verbose.


# License

MIT License

Copyright (c) [2023] [emlix GmbH, Elektrobit Automotive GmbH]

The full text of the license can be found in the [LICENSE](LICENSE) file in the repository root directory.

## Powered by EB

<img src="elos/doc/source/_static/eb-logo.png" width=70 height=70 align="left">
elos is powered by elektrobit automotive gmbh.
Elektrobit is an automotive software company and developer of embedded software products for ECU, AUTOSAR, automated driving, connected vehicles and UX.
elos isan  integrated part of EB corbos Linux – built on Ubuntu is an open-source operating system for high-performance computing, leveraging the rich functionality of Linux while meeting security and industry regulations.


## Maintainers

* Wolfgang Gehrhardt wolfgang.gehrhardt@emlix.com [@gehwolf](https://github.com/gehwolf)
* Thomas Brinker thomas.brinker@emlix.com [@ThomasBrinker](https://github.com/ThomasBrinker)

## Artwork

The elos logo is the Vombatus ursinus, also known as the bare-nosed wombat,
designed from the handwriting word elos. Originator is Anja Lehwess-Litzmann
(emlix GmbH). Year 2023. It is licensed under Creative Commons No Derivatives
(CC-nd). It shall be used in black on white or HKS43 color.
