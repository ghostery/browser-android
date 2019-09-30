#include "gtest/gtest.h"

#include "nsHttpHeaderArray.h"

<<<<<<< HEAD
TEST(TestHeaders, DuplicateHSTS) {
  // When the Strict-Transport-Security header is sent multiple times, its
  // effective value is the value of the first item. It is not merged as other
  // headers are.
  mozilla::net::nsHttpHeaderArray headers;
  nsresult rv =
      headers.SetHeaderFromNet(mozilla::net::nsHttp::Strict_Transport_Security,
                               NS_LITERAL_CSTRING("Strict_Transport_Security"),
                               NS_LITERAL_CSTRING("max-age=360"), true);
  ASSERT_EQ(rv, NS_OK);
||||||| merged common ancestors

TEST(TestHeaders, DuplicateHSTS) {
    // When the Strict-Transport-Security header is sent multiple times, its
    // effective value is the value of the first item. It is not merged as other
    // headers are.
    mozilla::net::nsHttpHeaderArray headers;
    nsresult rv = headers.SetHeaderFromNet(
        mozilla::net::nsHttp::Strict_Transport_Security,
        NS_LITERAL_CSTRING("Strict_Transport_Security"),
        NS_LITERAL_CSTRING("max-age=360"), true);
    ASSERT_EQ(rv, NS_OK);
=======
TEST(TestHeaders, DuplicateHSTS)
{
  // When the Strict-Transport-Security header is sent multiple times, its
  // effective value is the value of the first item. It is not merged as other
  // headers are.
  mozilla::net::nsHttpHeaderArray headers;
  nsresult rv =
      headers.SetHeaderFromNet(mozilla::net::nsHttp::Strict_Transport_Security,
                               NS_LITERAL_CSTRING("Strict_Transport_Security"),
                               NS_LITERAL_CSTRING("max-age=360"), true);
  ASSERT_EQ(rv, NS_OK);

  nsAutoCString h;
  rv = headers.GetHeader(mozilla::net::nsHttp::Strict_Transport_Security, h);
  ASSERT_EQ(rv, NS_OK);
  ASSERT_EQ(h.get(), "max-age=360");
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsAutoCString h;
  rv = headers.GetHeader(mozilla::net::nsHttp::Strict_Transport_Security, h);
  ASSERT_EQ(rv, NS_OK);
  ASSERT_EQ(h.get(), "max-age=360");
||||||| merged common ancestors
    nsAutoCString h;
    rv = headers.GetHeader(mozilla::net::nsHttp::Strict_Transport_Security, h);
    ASSERT_EQ(rv, NS_OK);
    ASSERT_EQ(h.get(), "max-age=360");
=======
  rv = headers.SetHeaderFromNet(mozilla::net::nsHttp::Strict_Transport_Security,
                                NS_LITERAL_CSTRING("Strict_Transport_Security"),
                                NS_LITERAL_CSTRING("max-age=720"), true);
  ASSERT_EQ(rv, NS_OK);
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = headers.SetHeaderFromNet(mozilla::net::nsHttp::Strict_Transport_Security,
                                NS_LITERAL_CSTRING("Strict_Transport_Security"),
                                NS_LITERAL_CSTRING("max-age=720"), true);
  ASSERT_EQ(rv, NS_OK);

  rv = headers.GetHeader(mozilla::net::nsHttp::Strict_Transport_Security, h);
  ASSERT_EQ(rv, NS_OK);
  ASSERT_EQ(h.get(), "max-age=360");
||||||| merged common ancestors
    rv = headers.SetHeaderFromNet(
        mozilla::net::nsHttp::Strict_Transport_Security,
        NS_LITERAL_CSTRING("Strict_Transport_Security"),
        NS_LITERAL_CSTRING("max-age=720"), true);
    ASSERT_EQ(rv, NS_OK);

    rv = headers.GetHeader(mozilla::net::nsHttp::Strict_Transport_Security, h);
    ASSERT_EQ(rv, NS_OK);
    ASSERT_EQ(h.get(), "max-age=360");
=======
  rv = headers.GetHeader(mozilla::net::nsHttp::Strict_Transport_Security, h);
  ASSERT_EQ(rv, NS_OK);
  ASSERT_EQ(h.get(), "max-age=360");
>>>>>>> upstream-releases
}
