importScripts("common_readableStreams.js");

function info(message) {
  postMessage({ type: "info", message });
}

function ok(a, message) {
  postMessage({ type: "test", test: !!a, message });
}

function is(a, b, message) {
  ok(a === b, message);
}

onmessage = function(e) {
<<<<<<< HEAD
  self[e.data](SAME_COMPARTMENT).then(ok => {
    postMessage({type: 'done'});
  }, exc => {
    dump(exc);
    dump(exc.stack);
    postMessage({type: 'error', message: exc.toString()});
  });
}
||||||| merged common ancestors
  self[e.data](SAME_COMPARTMENT);
}
=======
  self[e.data](SAME_COMPARTMENT).then(
    ok => {
      postMessage({ type: "done" });
    },
    exc => {
      dump(exc);
      dump(exc.stack);
      postMessage({ type: "error", message: exc.toString() });
    }
  );
};
>>>>>>> upstream-releases
