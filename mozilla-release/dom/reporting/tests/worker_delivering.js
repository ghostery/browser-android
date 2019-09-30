<<<<<<< HEAD
fetch("delivering.sjs?task=header&worker=true")
.then(r => r.text())
.then(text => {
  postMessage("All good!");
});
||||||| merged common ancestors
=======
fetch("delivering.sjs?task=header&worker=true")
  .then(r => r.text())
  .then(text => {
    postMessage("All good!");
  });
>>>>>>> upstream-releases
