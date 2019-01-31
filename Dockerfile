FROM spacifici/fennec:65.0
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
    openssh-client \
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

# Add extra dependencies to the maven cache
RUN mvn dependency:get \
    -Dmaven.repo.local=/sdk/android-gradle-dependencies/jcenter \
    -DgroupId=com.android.support.test.uiautomator \
    -DartifactId=uiautomator-v18 \
    -Dversion=2.1.3 -Dpackaging=aar \
    -DremoteRepositories=http://repo.spring.io/libs-milestone && \
    mvn dependency:get \
    -Dmaven.repo.local=/sdk/android-gradle-dependencies/jcenter \
    -DgroupId=com.github.PhilJay \
    -DartifactId=MPAndroidChart \
    -Dversion=v3.0.2 \
    -DremoteRepositories=https://jitpack.io && \
    mvn dependency:get \
    -Dmaven.repo.local=/sdk/android-gradle-dependencies/jcenter \
    -DgroupId=com.android.support.test \
    -DartifactId=testing-support-lib \
    -Dversion=0.1 \
    -Dpackaging=aar \
    -DremoteRepositories=https://maven.google.com

USER jenkins
SHELL ["/bin/bash", "-l", "-c"]

#Installation of 'appium' & 'wd' for Integration Tests
ENV NPM_CONFIG_PREFIX=~/.npm-global
RUN npm install --global appium@1.10.0 wd

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
