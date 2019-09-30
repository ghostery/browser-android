<<<<<<< HEAD
/* globals state:true */
is(document.readyState, "interactive", "readyState should be interactive during defer.");
||||||| merged common ancestors
is(document.readyState, "interactive", "readyState should be interactive during defer.");
=======
/* globals state:true */
is(
  document.readyState,
  "interactive",
  "readyState should be interactive during defer."
);
>>>>>>> upstream-releases
is(state, "readyState interactive", "Bad state upon defer");
state = "defer";
