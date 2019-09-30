/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

// Tests that history initialization correctly handles a request to forcibly
// replace the current database.

add_task(async function() {
  await createCorruptDb("places.sqlite");

  let count = Services.telemetry
<<<<<<< HEAD
                      .getHistogramById("PLACES_DATABASE_CORRUPTION_HANDLING_STAGE")
                      .snapshot()
                      .values[3];
  Assert.equal(count, undefined, "There should be no telemetry");
||||||| merged common ancestors
                      .getHistogramById("PLACES_DATABASE_CORRUPTION_HANDLING_STAGE")
                      .snapshot()
                      .counts[3];
  Assert.equal(count, 0, "There should be no telemetry");
=======
    .getHistogramById("PLACES_DATABASE_CORRUPTION_HANDLING_STAGE")
    .snapshot().values[3];
  Assert.equal(count, undefined, "There should be no telemetry");
>>>>>>> upstream-releases

  Assert.equal(
    PlacesUtils.history.databaseStatus,
    PlacesUtils.history.DATABASE_STATUS_CORRUPT
  );

  count = Services.telemetry
<<<<<<< HEAD
                  .getHistogramById("PLACES_DATABASE_CORRUPTION_HANDLING_STAGE")
                  .snapshot()
                  .values[3];
||||||| merged common ancestors
                  .getHistogramById("PLACES_DATABASE_CORRUPTION_HANDLING_STAGE")
                  .snapshot()
                  .counts[3];
=======
    .getHistogramById("PLACES_DATABASE_CORRUPTION_HANDLING_STAGE")
    .snapshot().values[3];
>>>>>>> upstream-releases
  Assert.equal(count, 1, "Telemetry should have been added");
});
