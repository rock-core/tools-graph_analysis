FROM ubuntu:16.04

MAINTAINER 2maz "https://github.com/2maz"

RUN apt update
RUN apt upgrade -y
RUN apt install -y ruby ruby-dev wget tzdata locales g++ autotools-dev make cmake sudo git
RUN echo "Europe/Berlin" > /etc/timezone; dpkg-reconfigure -f noninteractive tzdata
RUN export LANGUAGE=de_DE.UTF-8; export LANG=de_DE.UTF-8; export LC_ALL=de_DE.UTF-8; locale-gen de_DE.UTF-8; DEBIAN_FRONTEND=noninteractive dpkg-reconfigure locales

RUN useradd -ms /bin/bash docker
RUN echo "docker ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

USER docker
WORKDIR /home/docker

ENV LANG de_DE.UTF-8
ENV LANG de_DE:de
ENV LC_ALL de_DE.UTF-8
ENV SHELL /bin/bash

RUN git config --global user.email "rock-users@dfki.de"
RUN git config --global user.name "Rock CI"

RUN wget https://raw.githubusercontent.com/rock-core/autoproj/master/bin/autoproj_bootstrap

RUN mkdir -p /home/docker/rock_test
WORKDIR /home/docker/rock_test
# Use the existing seed configuration
COPY --chown=docker test/ci/autoproj-config.yml seed-config.yml
ENV AUTOPROJ_BOOTSTRAP_IGNORE_NONEMPTY_DIR 1
RUN ruby /home/docker/autoproj_bootstrap git https://github.com/rock-core/buildconf.git branch=master --seed-config=seed-config.yml
RUN sed -i 's#rock-core/rock-package_set#2maz/rock-package_set#g' autoproj/manifest
RUN sed -i 's#rock\.core#tools/graph_analysis#g' autoproj/manifest
# Activate testing
RUN /bin/bash -c "source env.sh; autoproj test enable tools/graph_analysis"
## Update
RUN /bin/bash -c "source env.sh; autoproj update; autoproj osdeps"
RUN /bin/bash -c "source env.sh; amake"
