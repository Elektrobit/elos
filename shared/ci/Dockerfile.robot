FROM ubuntu:jammy

ARG USER=ci
ARG UID=1000
ARG GID=1000

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
 && apt-get install -y sudo locales build-essential binutils-dev pkg-config \
 git wget curl netcat net-tools libssl-dev openssl python3-pip iputils-ping \
 && rm -rf /var/lib/apt/lists/*

RUN locale-gen en_US.UTF-8 \
 && locale-gen de_DE.UTF-8 \
 && groupadd -g $GID -o ci \
 && useradd -m -u $UID -g $GID -o -s /bin/bash $USER \
 && echo "$USER ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER $USER
ENV LC_ALL=en_US.UTF-8
ENV DOCKERBUILD=1
ENV PATH=/home/$USER/.local/bin:$PATH
WORKDIR /build
RUN pip install robotframework
RUN pip install --upgrade robotframework-sshlibrary
RUN pip install --upgrade robotframework-datadriver
RUN pip install --upgrade robotframework-jsonlibrary

CMD /bin/bash

