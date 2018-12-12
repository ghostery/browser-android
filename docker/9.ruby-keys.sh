#!/bin/bash
for key in 409B6B1796C275462A1703113804BB82D39DC0E3 \
           7D2BAF1CF37B13E2069D6956105BD0E739499BDB; do \
    for server in "hkp://keys.gnupg.net" \
                     "hkp://p80.pool.sks-keyservers.net:80" \
                     "pgp.mit.edu" \
                     "hkp://keyserver.ubuntu.com:80"; do \
        gpg2 --keyserver "${server}" --recv-keys "${key}" || echo "Trying new server..."; \
    done; \
done
echo 'ruby-keys - Done' >> status.log