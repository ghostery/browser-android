/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cstdio>
#include <unistd.h>
#include "Zip.h"
#include "mozilla/RefPtr.h"

<<<<<<< HEAD
extern "C" void report_mapping() {}
extern "C" void delete_mapping() {}
||||||| merged common ancestors
extern "C" void report_mapping() { }
extern "C" void delete_mapping() { }
=======
#include "gtest/gtest.h"

Logging Logging::Singleton;
>>>>>>> upstream-releases

/**
 * test.zip is a basic test zip file with a central directory. It contains
 * four entries, in the following order:
 * "foo", "bar", "baz", "qux".
 * The entries are going to be read out of order.
 */
<<<<<<< HEAD
const char *test_entries[] = {"baz", "foo", "bar", "qux"};
||||||| merged common ancestors
const char *test_entries[] = {
  "baz", "foo", "bar", "qux"
};
=======
extern const unsigned char TEST_ZIP[];
extern const unsigned int TEST_ZIP_SIZE;
const char* test_entries[] = {"baz", "foo", "bar", "qux"};
>>>>>>> upstream-releases

/**
 * no_central_dir.zip is a hand crafted test zip with no central directory
 * entries. The Zip reader is expected to be able to traverse these entries
 * if requested in order, without reading a central directory
 * - First entry is a file "a", STOREd.
 * - Second entry is a file "b", STOREd, using a data descriptor. CRC is
 *   unknown, but compressed and uncompressed sizes are known in the local
 *   file header.
 * - Third entry is a file "c", DEFLATEd, using a data descriptor. CRC,
 *   compressed and uncompressed sizes are known in the local file header.
 *   This is the kind of entry that can be found in a zip that went through
 *   zipalign if it had a data descriptor originally.
 * - Fourth entry is a file "d", STOREd.
 */
<<<<<<< HEAD
const char *no_central_dir_entries[] = {"a", "b", "c", "d"};
||||||| merged common ancestors
const char *no_central_dir_entries[] = {
  "a", "b", "c", "d"
};
=======
extern const unsigned char NO_CENTRAL_DIR_ZIP[];
extern const unsigned int NO_CENTRAL_DIR_ZIP_SIZE;
const char* no_central_dir_entries[] = {"a", "b", "c", "d"};
>>>>>>> upstream-releases

<<<<<<< HEAD
int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(
        stderr,
        "TEST-FAIL | TestZip | Expecting the directory containing test Zips\n");
    return 1;
  }
  chdir(argv[1]);
||||||| merged common ancestors
int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "TEST-FAIL | TestZip | Expecting the directory containing test Zips\n");
    return 1;
  }
  chdir(argv[1]);
=======
TEST(Zip, TestZip)
{
>>>>>>> upstream-releases
  Zip::Stream s;
<<<<<<< HEAD
  RefPtr<Zip> z = ZipCollection::GetZip("test.zip");
  for (size_t i = 0; i < sizeof(test_entries) / sizeof(*test_entries); i++) {
    if (!z->GetStream(test_entries[i], &s)) {
      fprintf(stderr,
              "TEST-UNEXPECTED-FAIL | TestZip | test.zip: Couldn't get entry "
              "\"%s\"\n",
              test_entries[i]);
      return 1;
    }
||||||| merged common ancestors
  RefPtr<Zip> z = ZipCollection::GetZip("test.zip");
  for (size_t i = 0; i < sizeof(test_entries) / sizeof(*test_entries); i++) {
    if (!z->GetStream(test_entries[i], &s)) {
      fprintf(stderr, "TEST-UNEXPECTED-FAIL | TestZip | test.zip: Couldn't get entry \"%s\"\n", test_entries[i]);
      return 1;
    }
=======
  RefPtr<Zip> z = Zip::Create((void*)TEST_ZIP, TEST_ZIP_SIZE);
  for (auto& entry : test_entries) {
    ASSERT_TRUE(z->GetStream(entry, &s))
    << "Could not get entry \"" << entry << "\"";
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
  z = ZipCollection::GetZip("no_central_dir.zip");
  for (size_t i = 0;
       i < sizeof(no_central_dir_entries) / sizeof(*no_central_dir_entries);
       i++) {
    if (!z->GetStream(no_central_dir_entries[i], &s)) {
      fprintf(stderr,
              "TEST-UNEXPECTED-FAIL | TestZip | no_central_dir.zip: Couldn't "
              "get entry \"%s\"\n",
              no_central_dir_entries[i]);
      return 1;
    }
||||||| merged common ancestors
  z = ZipCollection::GetZip("no_central_dir.zip");
  for (size_t i = 0; i < sizeof(no_central_dir_entries)
                         / sizeof(*no_central_dir_entries); i++) {
    if (!z->GetStream(no_central_dir_entries[i], &s)) {
      fprintf(stderr, "TEST-UNEXPECTED-FAIL | TestZip | no_central_dir.zip: Couldn't get entry \"%s\"\n", no_central_dir_entries[i]);
      return 1;
    }
=======
TEST(Zip, NoCentralDir)
{
  Zip::Stream s;
  RefPtr<Zip> z =
      Zip::Create((void*)NO_CENTRAL_DIR_ZIP, NO_CENTRAL_DIR_ZIP_SIZE);
  for (auto& entry : no_central_dir_entries) {
    ASSERT_TRUE(z->GetStream(entry, &s))
    << "Could not get entry \"" << entry << "\"";
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  fprintf(
      stderr,
      "TEST-PASS | TestZip | no_central_dir.zip could be accessed in order\n");

  return 0;
||||||| merged common ancestors
  fprintf(stderr, "TEST-PASS | TestZip | no_central_dir.zip could be accessed in order\n");

  return 0;
=======
>>>>>>> upstream-releases
}
