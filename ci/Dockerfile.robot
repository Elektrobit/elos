FROM ubuntu:jammy

ARG USER=ci
ARG UID=1000
ARG GID=1000
ARG WORKDIR="/elos/build"
ARG UBUNTU_MIRROR="https://ftp.gwdg.de/pub/linux/debian/ubuntu/"

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i "s,http://archive.ubuntu.com/ubuntu/,$UBUNTU_MIRROR," /etc/apt/sources.list \
 && sed -i "s,http://security.ubuntu.com/ubuntu/,$UBUNTU_MIRROR," /etc/apt/sources.list \
 && cat /etc/apt/sources.list

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
RUN pip install robotframework>=6.1.1
RUN pip install robotframework-sshlibrary>=3.8.0 --pre
RUN pip install robotframework-datadriver>=1.8.1
RUN pip install robotframework-jsonlibrary>=0.5
RUN pip install robotframework-robocop>=4.1.1
RUN pip install robotframework-tidy>=4.5.0

CMD /bin/bash
