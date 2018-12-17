FROM spacifici/fennec:64.0.1
MAINTAINER Stefano Pacifici <stefano@cliqz.com>
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get install -y \
    autoconf \
    automake \
    bison \
    curl \
    gawk \
    gnupg2 \
    libffi-dev \
    libgdbm-dev \
    libgmp-dev \
    libncurses5-dev \
    libreadline-dev \
    libsqlite3-dev \
    libssl-dev \
    libtool \
    libyaml-dev \
    pkg-config \
    python-pip \
    python-virtualenv \
    sqlite3 \
    zlib1g-dev && \
    apt-get clean -y && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ARG UID
ARG GID
ENV SHELL=/bin/bash
ENV PATH=/sdk/android-sdk/platform-tools:/sdk/android-sdk/platform-tools/bin:/sdk/android-sdk/tools:/sdk/android-sdk/tools/bin:${PATH}
RUN getent group $GID || groupadd jenkins --gid $GID && \
    useradd --create-home --shell /bin/bash jenkins --uid $UID --gid $GID

USER jenkins
SHELL ["/bin/bash", "-l", "-c"]

#Installation of 'appium' & 'wd' for Integration Tests
RUN npm install --global appium wd

#Install Ruby and Fastlane
RUN for key in 409B6B1796C275462A1703113804BB82D39DC0E3 \
               7D2BAF1CF37B13E2069D6956105BD0E739499BDB; do \
        for server in "hkp://keys.gnupg.net" \
                         "hkp://p80.pool.sks-keyservers.net:80" \
                         "pgp.mit.edu" \
                         "hkp://keyserver.ubuntu.com:80"; do \
            gpg2 --keyserver "${server}" --recv-keys "${key}" || echo "Trying new server..."; \
        done; \
    done
RUN curl -sSL https://get.rvm.io | bash -s stable --ruby=2.4.3 --autolibs=read-fail && \
    source /home/jenkins/.rvm/scripts/rvm \
    rvm reload && \
    gem install fastlane --version 2.111.0

#Install AWS CLI
RUN pip install awscli --upgrade --user