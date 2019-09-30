/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
 * Test to make sure matches against the url, title, tags are first made on word
 * boundaries, instead of in the middle of words, and later are extended to the
 * whole words. For this test it is critical to check sorting of the matches.
 *
 * Make sure we don't try matching one after a CamelCase because the upper-case
 * isn't really a word boundary. (bug 429498)
 */

var katakana = ["\u30a8", "\u30c9"]; // E, Do
var ideograph = ["\u4efb", "\u5929", "\u5802"]; // Nin Ten Do

add_task(async function test_escape() {
  Services.prefs.setBoolPref("browser.urlbar.autoFill.searchEngines", false);
  Services.prefs.setBoolPref("browser.urlbar.autoFill", false);

  await PlacesTestUtils.addVisits([
    { uri: "http://matchme/", title: "title1" },
    { uri: "http://dontmatchme/", title: "title1" },
    { uri: "http://title/1", title: "matchme2" },
    { uri: "http://title/2", title: "dontmatchme3" },
    { uri: "http://tag/1", title: "title1" },
    { uri: "http://tag/2", title: "title1" },
    { uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" },
    { uri: "http://katakana/", title: katakana.join("") },
    { uri: "http://ideograph/", title: ideograph.join("") },
    { uri: "http://camel/pleaseMatchMe/", title: "title1" },
  ]);
<<<<<<< HEAD
  await addBookmark( { uri: "http://tag/1", title: "title1", tags: [ "matchme2" ] } );
  await addBookmark( { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ] } );
||||||| merged common ancestors
  await addBookmark( { uri: uri5, title: "title1", tags: [ "matchme2" ] } );
  await addBookmark( { uri: uri6, title: "title1", tags: [ "dontmatchme3" ] } );

  // match only on word boundaries
  Services.prefs.setIntPref("browser.urlbar.matchBehavior", 2);
=======
  await addBookmark({
    uri: "http://tag/1",
    title: "title1",
    tags: ["matchme2"],
  });
  await addBookmark({
    uri: "http://tag/2",
    title: "title1",
    tags: ["dontmatchme3"],
  });
>>>>>>> upstream-releases

  info("Match 'match' at the beginning or after / or on a CamelCase");
  await check_autocomplete({
    search: "match",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://tag/1", title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://matchme/", title: "title1" },
      { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://dontmatchme/", title: "title1" },
    ],
||||||| merged common ancestors
    matches: [ { uri: uri1, title: "title1" },
               { uri: uri3, title: "matchme2" },
               { uri: uri5, title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
               { uri: uri10, title: "title1" } ],
=======
    checkSorting: true,
    matches: [
      {
        uri: "http://tag/1",
        title: "title1",
        tags: ["matchme2"],
        style: ["bookmark-tag"],
      },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://matchme/", title: "title1" },
      {
        uri: "http://tag/2",
        title: "title1",
        tags: ["dontmatchme3"],
        style: ["bookmark-tag"],
      },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://dontmatchme/", title: "title1" },
    ],
>>>>>>> upstream-releases
  });

  info("Match 'dont' at the beginning or after /");
  await check_autocomplete({
    search: "dont",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://dontmatchme/", title: "title1" },
    ],
||||||| merged common ancestors
    matches: [ { uri: uri2, title: "title1" },
               { uri: uri4, title: "dontmatchme3" },
               { uri: uri6, title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] } ],
=======
    checkSorting: true,
    matches: [
      {
        uri: "http://tag/2",
        title: "title1",
        tags: ["dontmatchme3"],
        style: ["bookmark-tag"],
      },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://dontmatchme/", title: "title1" },
    ],
>>>>>>> upstream-releases
  });

  info("Match 'match' at the beginning or after / or on a CamelCase");
  await check_autocomplete({
    search: "2",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
      { uri: "http://tag/1", title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
    ],
||||||| merged common ancestors
    matches: [ { uri: uri3, title: "matchme2" },
               { uri: uri4, title: "dontmatchme3" },
               { uri: uri5, title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
               { uri: uri6, title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] } ],
=======
    checkSorting: true,
    matches: [
      {
        uri: "http://tag/2",
        title: "title1",
        tags: ["dontmatchme3"],
        style: ["bookmark-tag"],
      },
      {
        uri: "http://tag/1",
        title: "title1",
        tags: ["matchme2"],
        style: ["bookmark-tag"],
      },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
    ],
>>>>>>> upstream-releases
  });

  info("Match 't' at the beginning or after /");
  await check_autocomplete({
    search: "t",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
      { uri: "http://tag/1", title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
      { uri: "http://katakana/", title: katakana.join("") },
      { uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" },
    ],
||||||| merged common ancestors
    matches: [ { uri: uri1, title: "title1" },
               { uri: uri2, title: "title1" },
               { uri: uri3, title: "matchme2" },
               { uri: uri4, title: "dontmatchme3" },
               { uri: uri5, title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
               { uri: uri6, title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
               { uri: uri10, title: "title1" } ],
=======
    checkSorting: true,
    matches: [
      {
        uri: "http://tag/2",
        title: "title1",
        tags: ["dontmatchme3"],
        style: ["bookmark-tag"],
      },
      {
        uri: "http://tag/1",
        title: "title1",
        tags: ["matchme2"],
        style: ["bookmark-tag"],
      },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
      { uri: "http://katakana/", title: katakana.join("") },
      { uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" },
    ],
>>>>>>> upstream-releases
  });

  info("Match 'word' after many consecutive word boundaries");
  await check_autocomplete({
    search: "word",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" },
    ],
||||||| merged common ancestors
    matches: [ { uri: uri7, title: "!@#$%^&*()_+{}|:<>?word" } ],
=======
    checkSorting: true,
    matches: [{ uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" }],
>>>>>>> upstream-releases
  });

  info("Match a word boundary '/' for everything");
  await check_autocomplete({
    search: "/",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
      { uri: "http://tag/1", title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://ideograph/", title: ideograph.join("") },
      { uri: "http://katakana/", title: katakana.join("") },
      { uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
    ],
  });

  info("Match word boundaries '()_+' that are among word boundaries");
||||||| merged common ancestors
    matches: [ { uri: uri1, title: "title1" },
               { uri: uri2, title: "title1" },
               { uri: uri3, title: "matchme2" },
               { uri: uri4, title: "dontmatchme3" },
               { uri: uri5, title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
               { uri: uri6, title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
               { uri: uri7, title: "!@#$%^&*()_+{}|:<>?word" },
               { uri: uri8, title: katakana.join("") },
               { uri: uri9, title: ideograph.join("") },
               { uri: uri10, title: "title1" } ],
  });

  info("Match word boundaries '()_+' that are among word boundaries");
=======
    checkSorting: true,
    matches: [
      {
        uri: "http://tag/2",
        title: "title1",
        tags: ["dontmatchme3"],
        style: ["bookmark-tag"],
      },
      {
        uri: "http://tag/1",
        title: "title1",
        tags: ["matchme2"],
        style: ["bookmark-tag"],
      },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://ideograph/", title: ideograph.join("") },
      { uri: "http://katakana/", title: katakana.join("") },
      { uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
    ],
  });

  info("Match word boundaries '()_' that are among word boundaries");
>>>>>>> upstream-releases
  await check_autocomplete({
<<<<<<< HEAD
    search: "()_+",
    checkSorting: true,
    matches: [
      { uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" },
    ],
||||||| merged common ancestors
    search: "()_+",
    matches: [ { uri: uri7, title: "!@#$%^&*()_+{}|:<>?word" } ],
=======
    search: "()_",
    checkSorting: true,
    matches: [{ uri: "http://crazytitle/", title: "!@#$%^&*()_+{}|:<>?word" }],
>>>>>>> upstream-releases
  });

  info("Katakana characters form a string, so match the beginning");
  await check_autocomplete({
    search: katakana[0],
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://katakana/", title: katakana.join("") },
    ],
||||||| merged common ancestors
    matches: [ { uri: uri8, title: katakana.join("") } ],
=======
    checkSorting: true,
    matches: [{ uri: "http://katakana/", title: katakana.join("") }],
>>>>>>> upstream-releases
  });

<<<<<<< HEAD
/*
  info("Middle of a katakana word shouldn't be matched");
  await check_autocomplete({
||||||| merged common ancestors
/*
  do_print("Middle of a katakana word shouldn't be matched");
  yield check_autocomplete({
=======
  /*
  info("Middle of a katakana word shouldn't be matched");
  await check_autocomplete({
>>>>>>> upstream-releases
    search: katakana[1],
    matches: [ ],
  });
*/
<<<<<<< HEAD

 info("Ideographs are treated as words so 'nin' is one word");
||||||| merged common ancestors
 info("Ideographs are treated as words so 'nin' is one word");
=======

  info("Ideographs are treated as words so 'nin' is one word");
>>>>>>> upstream-releases
  await check_autocomplete({
    search: ideograph[0],
<<<<<<< HEAD
    checkSorting: true,
    matches: [ { uri: "http://ideograph/", title: ideograph.join("") } ],
||||||| merged common ancestors
    matches: [ { uri: uri9, title: ideograph.join("") } ],
=======
    checkSorting: true,
    matches: [{ uri: "http://ideograph/", title: ideograph.join("") }],
>>>>>>> upstream-releases
  });

  info("Ideographs are treated as words so 'ten' is another word");
  await check_autocomplete({
    search: ideograph[1],
<<<<<<< HEAD
    checkSorting: true,
    matches: [ { uri: "http://ideograph/", title: ideograph.join("") } ],
||||||| merged common ancestors
    matches: [ { uri: uri9, title: ideograph.join("") } ],
=======
    checkSorting: true,
    matches: [{ uri: "http://ideograph/", title: ideograph.join("") }],
>>>>>>> upstream-releases
  });

  info("Ideographs are treated as words so 'do' is yet another word");
  await check_autocomplete({
    search: ideograph[2],
<<<<<<< HEAD
    checkSorting: true,
    matches: [ { uri: "http://ideograph/", title: ideograph.join("") } ],
||||||| merged common ancestors
    matches: [ { uri: uri9, title: ideograph.join("") } ],
=======
    checkSorting: true,
    matches: [{ uri: "http://ideograph/", title: ideograph.join("") }],
>>>>>>> upstream-releases
  });

<<<<<<< HEAD
 info("Match in the middle. Should just be sorted by frecency.");
||||||| merged common ancestors
 info("Extra negative assert that we don't match in the middle");
=======
  info("Match in the middle. Should just be sorted by frecency.");
>>>>>>> upstream-releases
  await check_autocomplete({
    search: "ch",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
      { uri: "http://tag/1", title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
    ],
||||||| merged common ancestors
    matches: [ ],
=======
    checkSorting: true,
    matches: [
      {
        uri: "http://tag/2",
        title: "title1",
        tags: ["dontmatchme3"],
        style: ["bookmark-tag"],
      },
      {
        uri: "http://tag/1",
        title: "title1",
        tags: ["matchme2"],
        style: ["bookmark-tag"],
      },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
    ],
>>>>>>> upstream-releases
  });

<<<<<<< HEAD
 // Also this test should just be sorted by frecency.
 info("Don't match one character after a camel-case word boundary (bug 429498). Should just be sorted by frecency.");
||||||| merged common ancestors
 info("Don't match one character after a camel-case word boundary (bug 429498)");
=======
  // Also this test should just be sorted by frecency.
  info(
    "Don't match one character after a camel-case word boundary (bug 429498). Should just be sorted by frecency."
  );
>>>>>>> upstream-releases
  await check_autocomplete({
    search: "atch",
<<<<<<< HEAD
    checkSorting: true,
    matches: [
      { uri: "http://tag/2", title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
      { uri: "http://tag/1", title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
    ],
||||||| merged common ancestors
    matches: [ ],
  });

  // match against word boundaries and anywhere
  Services.prefs.setIntPref("browser.urlbar.matchBehavior", 1);

  await check_autocomplete({
    search: "tch",
    matches: [ { uri: uri1, title: "title1" },
               { uri: uri2, title: "title1" },
               { uri: uri3, title: "matchme2" },
               { uri: uri4, title: "dontmatchme3" },
               { uri: uri5, title: "title1", tags: [ "matchme2" ], style: [ "bookmark-tag" ] },
               { uri: uri6, title: "title1", tags: [ "dontmatchme3" ], style: [ "bookmark-tag" ] },
               { uri: uri10, title: "title1" } ],
=======
    checkSorting: true,
    matches: [
      {
        uri: "http://tag/2",
        title: "title1",
        tags: ["dontmatchme3"],
        style: ["bookmark-tag"],
      },
      {
        uri: "http://tag/1",
        title: "title1",
        tags: ["matchme2"],
        style: ["bookmark-tag"],
      },
      { uri: "http://camel/pleaseMatchMe/", title: "title1" },
      { uri: "http://title/2", title: "dontmatchme3" },
      { uri: "http://title/1", title: "matchme2" },
      { uri: "http://dontmatchme/", title: "title1" },
      { uri: "http://matchme/", title: "title1" },
    ],
>>>>>>> upstream-releases
  });

  await cleanup();
});
