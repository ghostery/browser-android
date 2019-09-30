/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

function run_test() {
  let testFlag = Services.telemetry.getHistogramById("TELEMETRY_TEST_FLAG");
<<<<<<< HEAD
  deepEqual(testFlag.snapshot().values, {0: 1, 1: 0}, "Original value is correct");
||||||| merged common ancestors
  equal(JSON.stringify(testFlag.snapshot().counts), "[1,0,0]", "Original value is correct");
=======
  deepEqual(
    testFlag.snapshot().values,
    { 0: 1, 1: 0 },
    "Original value is correct"
  );
>>>>>>> upstream-releases
  testFlag.add(1);
<<<<<<< HEAD
  deepEqual(testFlag.snapshot().values, {0: 0, 1: 1, 2: 0}, "Value is correct after ping");
||||||| merged common ancestors
  equal(JSON.stringify(testFlag.snapshot().counts), "[0,1,0]", "Value is correct after ping.");
=======
  deepEqual(
    testFlag.snapshot().values,
    { 0: 0, 1: 1, 2: 0 },
    "Value is correct after ping"
  );
>>>>>>> upstream-releases
  testFlag.clear();
<<<<<<< HEAD
  deepEqual(testFlag.snapshot().values, {0: 1, 1: 0}, "Value is correct after calling clear()");
||||||| merged common ancestors
  equal(JSON.stringify(testFlag.snapshot().counts), "[1,0,0]", "Value is correct after calling clear()");
=======
  deepEqual(
    testFlag.snapshot().values,
    { 0: 1, 1: 0 },
    "Value is correct after calling clear()"
  );
>>>>>>> upstream-releases
  testFlag.add(1);
<<<<<<< HEAD
  deepEqual(testFlag.snapshot().values, {0: 0, 1: 1, 2: 0}, "Value is correct after ping");
||||||| merged common ancestors
  equal(JSON.stringify(testFlag.snapshot().counts), "[0,1,0]", "Value is correct after ping.");
=======
  deepEqual(
    testFlag.snapshot().values,
    { 0: 0, 1: 1, 2: 0 },
    "Value is correct after ping"
  );
>>>>>>> upstream-releases
}
