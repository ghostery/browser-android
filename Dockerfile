FROM ubuntu:16.04
MAINTAINER Sharath Ganesh Pai <sharath@cliqz.com>
ENV DEBIAN_FRONTEND noninteractive

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y autoconf2.13 build-essential ccache curl git lib32z1 \
        libc6:i386 libncurses5:i386 libstdc++6:i386 mercurial openjdk-8-jdk \
        python-dev python-pip python-setuptools unzip uuid \
        wget xz-utils zip && \
    apt-get clean -y && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ARG UID
ARG GID
RUN getent group $GID || groupadd jenkins --gid $GID && \
useradd --create-home --shell /bin/bash jenkins --uid $UID --gid $GID

ENV SHELL /bin/bash
ENV MOZBUILD_STATE_PATH /home/jenkins/.mozbuild/
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64/

USER jenkins
ENV ANDROID_HOME /home/jenkins/.mozbuild/android-sdk-linux
ENV PATH "/home/jenkins/node-v8.9.3-linux-x64/bin:$PATH"

#Install Proguard
RUN mkdir -p /home/jenkins/.mozbuild/proguard/lib && mkdir -p /home/jenkins/tmp && \
    wget --output-document=/home/jenkins/tmp/proguard.tgz --quiet "https://repository.cliqz.com/dist/android/artifacts/proguard/proguard5.3.3.tar.gz" && \
    (cd /home/jenkins/tmp; tar xf proguard.tgz && cp proguard5.3.3/lib/proguard.jar /home/jenkins/.mozbuild/proguard/lib) && \
    rm -rf /home/jenkins/tmp/*

#Install Android SDK and the Required SDKs
RUN mkdir -p $ANDROID_HOME; \
    cd $ANDROID_HOME; \
    wget --output-document=sdktools.zip --quiet 'https://repository.cliqz.com/dist/android/artifacts/android-sdk/sdk-tools-linux-3859397.zip'; \
    unzip sdktools.zip; \
    rm -r sdktools.zip; \
    (while (true); do echo y; sleep 2; done) | \
      tools/bin/sdkmanager  "build-tools;25.0.3" "platforms;android-23" "platform-tools" "tools" "extras;google;m2repository" "extras;android;m2repository" "extras;google;google_play_services" "emulator";

# Install Node.JS
RUN cd /home/jenkins && \
    wget --output-document=nodejs.tgz --quiet \
      'https://nodejs.org/dist/v8.9.3/node-v8.9.3-linux-x64.tar.xz' && \
    tar xf nodejs.tgz && \
    rm -f nodejs.tgz

#Installation of 'yarn' for the Extension, 'appium' & 'wd' for Integration Tests
RUN npm install -g yarn
RUN npm install -g appium
RUN npm install -g wd