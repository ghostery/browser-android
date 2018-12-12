#!/bin/bash
curl https://raw.githubusercontent.com/creationix/nvm/v0.33.8/install.sh | bash
. $NVM_DIR/nvm.sh
echo '''
export NODE_PATH="$NVM_DIR/versions/node/v$NODE_VERSION/lib/node_modules"
export PATH="$NVM_DIR/versions/node/v$NODE_VERSION/bin:$PATH"
''' >> ~/.bashrc
echo 'node - Done' >> status.log