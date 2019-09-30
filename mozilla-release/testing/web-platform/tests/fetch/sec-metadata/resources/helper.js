<<<<<<< HEAD
function parse_metadata(value) {
  let result = {};
  value.split(',').forEach(item => {
    let parsed = item.trim().split('=');
    result[parsed[0]] = parsed[1];
  });
  return result;
}

||||||| merged common ancestors
function parse_metadata(value) {
  let result = {};
  value.split(',').forEach(item => {
    let parsed = item.trim().split('=');
    parsed[1] = parsed[1].trim().replace(/^"|"$/g, '');
    result[parsed[0]] = parsed[1];
  });
  return result;
}

=======
>>>>>>> upstream-releases
function assert_header_equals(value, expected) {
  if (typeof(value) === "string"){
    assert_not_equals(value, "No header has been recorded");
    value = JSON.parse(value);
  }
  assert_equals(value.dest, expected.dest, "dest");
  assert_equals(value.mode, expected.mode, "mode");
  assert_equals(value.site, expected.site, "site");
  if (expected.hasOwnProperty("user"))
    assert_equals(value.user, expected.user, "user");
}
