<<<<<<< HEAD
function localStorageFlush(cb)
{
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    SimpleTest.executeSoon(function () {
      cb();
    });
    return;
  }

||||||| merged common ancestors
function localStorageFlush(cb)
{
=======
function localStorageFlush(cb) {
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    SimpleTest.executeSoon(function() {
      cb();
    });
    return;
  }

>>>>>>> upstream-releases
  var ob = {
    observe: function(sub, top, dat) {
      os().removeObserver(ob, "domstorage-test-flushed");
      cb();
    },
  };
  os().addObserver(ob, "domstorage-test-flushed");
  notify("domstorage-test-flush-force");
}

<<<<<<< HEAD
function localStorageReload(callback)
{
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    localStorage.close();
    let qms = SpecialPowers.Services.qms;
    let principal = SpecialPowers.wrap(document).nodePrincipal;
    let request = qms.resetStoragesForPrincipal(principal, "default", "ls");
    request.callback = SpecialPowers.wrapCallback(function() {
      localStorage.open();
      callback();
    });
    return;
  }

||||||| merged common ancestors
function localStorageReload()
{
=======
function localStorageReload(callback) {
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    localStorage.close();
    let qms = SpecialPowers.Services.qms;
    let principal = SpecialPowers.wrap(document).nodePrincipal;
    let request = qms.resetStoragesForPrincipal(principal, "default", "ls");
    request.callback = SpecialPowers.wrapCallback(function() {
      localStorage.open();
      callback();
    });
    return;
  }

>>>>>>> upstream-releases
  notify("domstorage-test-reload");
<<<<<<< HEAD
  SimpleTest.executeSoon(function () {
    callback();
  });
||||||| merged common ancestors
=======
  SimpleTest.executeSoon(function() {
    callback();
  });
>>>>>>> upstream-releases
}

<<<<<<< HEAD
function localStorageFlushAndReload(callback)
{
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    localStorage.close();
    let qms = SpecialPowers.Services.qms;
    let principal = SpecialPowers.wrap(document).nodePrincipal;
    let request = qms.resetStoragesForPrincipal(principal, "default", "ls");
    request.callback = SpecialPowers.wrapCallback(function() {
      localStorage.open();
      callback();
    });
    return;
  }

||||||| merged common ancestors
function localStorageFlushAndReload(cb)
{
=======
function localStorageFlushAndReload(callback) {
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    localStorage.close();
    let qms = SpecialPowers.Services.qms;
    let principal = SpecialPowers.wrap(document).nodePrincipal;
    let request = qms.resetStoragesForPrincipal(principal, "default", "ls");
    request.callback = SpecialPowers.wrapCallback(function() {
      localStorage.open();
      callback();
    });
    return;
  }

>>>>>>> upstream-releases
  localStorageFlush(function() {
    localStorageReload(callback);
  });
}

<<<<<<< HEAD
function localStorageClearAll(callback)
{
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    let qms = SpecialPowers.Services.qms;
    let ssm = SpecialPowers.Services.scriptSecurityManager;

    qms.getUsage(SpecialPowers.wrapCallback(function(request) {
      if (request.resultCode != SpecialPowers.Cr.NS_OK) {
        callback();
        return;
      }

      let clearRequestCount = 0;
      for (let item of request.result) {
        let principal = ssm.createCodebasePrincipalFromOrigin(item.origin);
        let clearRequest =
          qms.clearStoragesForPrincipal(principal, "default", "ls");
        clearRequestCount++;
        clearRequest.callback = SpecialPowers.wrapCallback(function() {
          if (--clearRequestCount == 0) {
            callback();
          }
        });
      }
    }));
    return;
  }

||||||| merged common ancestors
function localStorageClearAll()
{
=======
function localStorageClearAll(callback) {
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    let qms = SpecialPowers.Services.qms;
    let ssm = SpecialPowers.Services.scriptSecurityManager;

    qms.getUsage(
      SpecialPowers.wrapCallback(function(request) {
        if (request.resultCode != SpecialPowers.Cr.NS_OK) {
          callback();
          return;
        }

        let clearRequestCount = 0;
        for (let item of request.result) {
          let principal = ssm.createCodebasePrincipalFromOrigin(item.origin);
          let clearRequest = qms.clearStoragesForPrincipal(
            principal,
            "default",
            "ls"
          );
          clearRequestCount++;
          clearRequest.callback = SpecialPowers.wrapCallback(function() {
            if (--clearRequestCount == 0) {
              callback();
            }
          });
        }
      })
    );
    return;
  }

>>>>>>> upstream-releases
  os().notifyObservers(null, "cookie-changed", "cleared");
<<<<<<< HEAD
  SimpleTest.executeSoon(function () {
    callback();
  });
||||||| merged common ancestors
=======
  SimpleTest.executeSoon(function() {
    callback();
  });
>>>>>>> upstream-releases
}

<<<<<<< HEAD
function localStorageClearDomain(domain, callback)
{
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    let qms = SpecialPowers.Services.qms;
    let principal = SpecialPowers.wrap(document).nodePrincipal;
    let request = qms.clearStoragesForPrincipal(principal, "default", "ls");
    let cb = SpecialPowers.wrapCallback(callback);
    request.callback = cb;
    return;
  }

  os().notifyObservers(null, "browser:purge-domain-data", domain);
  SimpleTest.executeSoon(function () {
    callback();
  });
||||||| merged common ancestors
function localStorageClearDomain(domain)
{
  os().notifyObservers(null, "browser:purge-domain-data", domain);
=======
function localStorageClearDomain(domain, callback) {
  if (SpecialPowers.Services.lsm.nextGenLocalStorageEnabled) {
    let qms = SpecialPowers.Services.qms;
    let principal = SpecialPowers.wrap(document).nodePrincipal;
    let request = qms.clearStoragesForPrincipal(principal, "default", "ls");
    let cb = SpecialPowers.wrapCallback(callback);
    request.callback = cb;
    return;
  }

  os().notifyObservers(null, "extension:purge-localStorage", domain);
  SimpleTest.executeSoon(function() {
    callback();
  });
>>>>>>> upstream-releases
}

function os() {
  return SpecialPowers.Services.obs;
}

function notify(top) {
  os().notifyObservers(null, top);
}

/**
 * Enable testing observer notifications in DOMStorageObserver.cpp.
 */
<<<<<<< HEAD
function localStorageEnableTestingMode(cb)
{
  SpecialPowers.pushPrefEnv({ set: [["dom.storage.testing", true],
                                    ["dom.quotaManager.testing", true]] },
                            cb);
||||||| merged common ancestors
function localStorageEnableTestingMode(cb)
{
  SpecialPowers.pushPrefEnv({ "set": [["dom.storage.testing", true]] }, cb);
=======
function localStorageEnableTestingMode(cb) {
  SpecialPowers.pushPrefEnv(
    {
      set: [["dom.storage.testing", true], ["dom.quotaManager.testing", true]],
    },
    cb
  );
>>>>>>> upstream-releases
}
