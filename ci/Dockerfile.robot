FROM ubuntu:jammy

ARG USER=ci
ARG UID=1000
ARG GID=1000
ARG WORKDIR="/elos/build"

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
 && apt-get install -y sudo locales build-essential binutils-dev pkg-config \
 git wget curl netcat net-tools libssl-dev openssl python3-pip iputils-ping \
 && rm -rf /var/lib/apt/lists/*

RUN locale-gen en_US.UTF-8 \
 && locale-gen de_DE.UTF-8 \
 && groupadd -g $GID -o $USER \
 && useradd -m -u $UID -g $GID -o -s /bin/bash $USER \
 && echo "$USER ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN mkdir -p $WORKDIR && chown $USER:$USER $WORKDIR

USER $USER
ENV LC_ALL=en_US.UTF-8
ENV DOCKERBUILD=1
ENV PATH=/home/$USER/.local/bin:$PATH
WORKDIR $WORKDIR
RUN pip install robotframework
RUN pip install robotframework-sshlibrary>=3.8.0 --pre
RUN pip install --upgrade robotframework-datadriver
RUN pip install --upgrade robotframework-jsonlibrary

CMD /bin/bash
