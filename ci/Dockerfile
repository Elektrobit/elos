FROM ubuntu:jammy

ARG USER=ci
ARG UID=1000
ARG GID=1000
ARG ASMCOV_URI="not available"
ARG CAPSTONE_VERSION=4.0.2
ARG UBUNTU_MIRROR="https://ftp.gwdg.de/pub/linux/debian/ubuntu/"
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get reinstall ca-certificates -y
RUN sed -i "s,http://archive.ubuntu.com/ubuntu/,$UBUNTU_MIRROR," /etc/apt/sources.list \
 && sed -i "s,http://security.ubuntu.com/ubuntu/,$UBUNTU_MIRROR," /etc/apt/sources.list \
 && cat /etc/apt/sources.list

RUN apt-get update \
 && apt-get install -y \
    binutils-dev \
    build-essential \
    clang-format \
    clang-tidy \
    cmake \
    curl \
    debhelper \
    expat \
    gdb \
    git \
    git-buildpackage \
    gnuplot \
    jq \
    libcmocka-dev \
    libcsv-dev \
	libcurl4-openssl-dev \
    libesmtp-dev \
    libiberty-dev \
    libjson-c-dev \
    liblog4c-dev \
    libmnl-dev \
    libsqlite3-0 \
    libsqlite3-dev \
    libssl-dev \
    linux-tools-generic \
    locales \
    moreutils \
    net-tools \
    netcat \
    ninja-build \
    openssl \
    pandoc \
    pandoc-plantuml-filter \
    pkg-config \
    python-is-python3 \
    python3-pip \
    python3-venv \
    sqlite3 \
    sudo \
    wget \
    zlib1g-dev \
 && rm -rf /var/lib/apt/lists/* \
 && rm /usr/bin/perf \
 && find /usr/lib/linux-tools -name "perf" -executable -exec ln -s {} /usr/bin/perf \;

RUN locale-gen en_US.UTF-8 \
 && locale-gen de_DE.UTF-8 \
 && groupadd -g $GID -o $USER \
 && useradd -m -u $UID -g $GID -o -s /bin/bash $USER \
 && echo "$USER ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN git clone --depth 1 --branch $CAPSTONE_VERSION https://github.com/aquynh/capstone.git
RUN cd capstone/ \
 && ./make.sh \
 &&  sudo ./make.sh install

RUN git clone https://github.com/mongodb/mongo-c-driver.git \
 && cd mongo-c-driver/build \
 && cmake .. -DCMAKE_INSTALL_PREFIX=/usr \
 && make && sudo make install

RUN if [ "${ASMCOV_URI}" != "not available" ]; then git clone --depth 1 ${ASMCOV_URI} -b 'integration' \
 && cmake -B build asmcov \
 && make -C build \
 && make -C build install; \
 fi

USER $USER
ENV LC_ALL=en_US.UTF-8
ENV DOCKERBUILD=1

RUN python -m venv /home/$USER/.venv \
  && . /home/$USER/.venv/bin/activate \
  && python -m pip install \
     beautifulsoup4==4.12.2  \
     clang==16.0.1.1 \
     myst-parser==2.0.0 \
     sphinx==7.2.6 \
     sphinx-c-autodoc==1.3.0 \
     sphinx-copybutton==0.5.2 \
     sphinx-favicon==1.0.1 \
     sphinxcontrib-plantuml==0.27 \
     sphinxcontrib-programoutput==0.17

# install sphinx-c-autodoc dependedncies
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc \
  && echo "\n\
# for llvm 16 packages\n\
deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main\n\
deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main\n\
" | sudo tee -a /etc/apt/sources.list \
  && sudo apt-get update \
  && sudo apt-get install -y libclang1-16 \
  && sudo ln -s /usr/lib/x86_64-linux-gnu/libclang-16.so.1 /usr/lib/libclang-16.so

ENV SPHINX_VENV="/home/$USER/.venv/"
ENV LOG4C_RCPATH="/base/src/demos/elos_log4c_demo"
ENV PATH=/home/$USER/.local/bin:$PATH

WORKDIR /home/$USER
RUN pip install robotframework-robocop==5.5.0
RUN pip install robotframework-tidy==4.14.0

WORKDIR /base

CMD /bin/bash

