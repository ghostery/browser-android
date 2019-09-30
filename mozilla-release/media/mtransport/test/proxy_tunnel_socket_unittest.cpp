/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Original authors: ekr@rtfm.com; ryan@tokbox.com

<<<<<<< HEAD
#include <vector>
#include <numeric>

#include "mozilla/dom/PBrowserOrId.h"
||||||| merged common ancestors
#include <iostream>

extern "C" {
#include "nr_api.h"
#include "nr_socket.h"
#include "nr_proxy_tunnel.h"
#include "transport_addr.h"
#include "stun.h"
}

#include "dummysocket.h"
=======
#include <vector>
#include <numeric>
>>>>>>> upstream-releases

<<<<<<< HEAD
#include "nr_socket_proxy.h"
#include "nr_socket_proxy_config.h"
#include "WebrtcProxyChannelWrapper.h"
||||||| merged common ancestors
#include "nr_socket_prsock.h"
#include "nriceresolverfake.h"
=======
#include "mozilla/net/NeckoChannelParams.h"
#include "nr_socket_proxy.h"
#include "nr_socket_proxy_config.h"
#include "WebrtcProxyChannelWrapper.h"
>>>>>>> upstream-releases

#define GTEST_HAS_RTTI 0
#include "gtest/gtest.h"
#include "gtest_utils.h"

using namespace mozilla;

<<<<<<< HEAD
// update TestReadMultipleSizes if you change this
const std::string kHelloMessage = "HELLO IS IT ME YOU'RE LOOKING FOR?";
||||||| merged common ancestors
const std::string kRemoteAddr = "192.0.2.133";
const uint16_t kRemotePort = 3333;

const std::string kProxyHost = "example.com";
const std::string kProxyAddr = "192.0.2.134";
const uint16_t kProxyPort = 9999;

const std::string kHelloMessage = "HELLO";
const std::string kGarbageMessage = "xxxxxxxxxx";
=======
// update TestReadMultipleSizes if you change this
const std::string kHelloMessage = "HELLO IS IT ME YOU'RE LOOKING FOR?";

class NrSocketProxyTest : public MtransportTest {
 public:
  NrSocketProxyTest()
      : mSProxy(nullptr),
        nr_socket_(nullptr),
        mEmptyArray(0),
        mReadChunkSize(0),
        mReadChunkSizeIncrement(1),
        mReadAllowance(-1),
        mConnected(false) {}

  void SetUp() override {
    nsCString alpn = NS_LITERAL_CSTRING("webrtc");
    std::shared_ptr<NrSocketProxyConfig> config;
    config.reset(new NrSocketProxyConfig(0, alpn, net::LoadInfoArgs()));
    // config is never used but must be non-null
    mSProxy = new NrSocketProxy(config);
    int r = nr_socket_create_int((void*)mSProxy.get(), mSProxy->vtbl(),
                                 &nr_socket_);
    ASSERT_EQ(0, r);
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef mozilla::dom::PBrowserOrId PBrowserOrId;

class NrSocketProxyTest : public MtransportTest {
 public:
  NrSocketProxyTest()
    : mSProxy(nullptr)
    , nr_socket_(nullptr)
    , mEmptyArray(0)
    , mReadChunkSize(0)
    , mReadChunkSizeIncrement(1)
    , mReadAllowance(-1)
    , mConnected(false) {}
||||||| merged common ancestors
std::string connect_message(const std::string &host, uint16_t port, const std::string &alpn, const std::string &tail) {
  std::stringstream ss;
  ss << "CONNECT " << host << ":" << port << " HTTP/1.0\r\n";
  if (!alpn.empty()) {
    ss << "ALPN: " << alpn << "\r\n";
  }
  ss << "\r\n" << tail;
  return ss.str();
}

std::string connect_response(int code, const std::string &tail = "") {
  std::stringstream ss;
  ss << "HTTP/1.0 " << code << "\r\n\r\n" << tail;
  return ss.str();
}

class DummyResolver {
 public:
  DummyResolver() {
    vtbl_ = new nr_resolver_vtbl;
    vtbl_->destroy = &DummyResolver::destroy;
    vtbl_->resolve = &DummyResolver::resolve;
    vtbl_->cancel = &DummyResolver::cancel;
    nr_resolver_create_int((void *)this, vtbl_, &resolver_);
  }
=======
    // fake calling AsyncOpen() due to IPC calls. must be non-null
    mSProxy->AssignChannel_DoNotUse(new WebrtcProxyChannelWrapper(nullptr));
  }

  void TearDown() override { mSProxy->close(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetUp() override {
    nsCString alpn = NS_LITERAL_CSTRING("webrtc");
    std::shared_ptr<NrSocketProxyConfig> config;
    config.reset(new NrSocketProxyConfig(PBrowserOrId(), alpn));
    // config is never used but must be non-null
    mSProxy = new NrSocketProxy(config);
    int r = nr_socket_create_int((void*)mSProxy.get(),
                                 mSProxy->vtbl(),
                                 &nr_socket_);
    ASSERT_EQ(0, r);
||||||| merged common ancestors
  ~DummyResolver() {
    nr_resolver_destroy(&resolver_);
    delete vtbl_;
  }

  static int destroy(void **objp) {
    return 0;
  }

  static int resolve(void *obj,
                     nr_resolver_resource *resource,
                     int (*cb)(void *cb_arg, nr_transport_addr *addr),
                     void *cb_arg,
                     void **handle) {
    nr_transport_addr addr;

    nr_str_port_to_transport_addr(
        (char *)kProxyAddr.c_str(), kProxyPort, IPPROTO_TCP, &addr);

    cb(cb_arg, &addr);
    return 0;
  }
=======
  static void readable_cb(NR_SOCKET s, int how, void* cb_arg) {
    NrSocketProxyTest* test = (NrSocketProxyTest*)cb_arg;
    size_t capacity = std::min(test->mReadChunkSize, test->mReadAllowance);
    nsTArray<uint8_t> array(capacity);
    size_t read;

    nr_socket_read(test->nr_socket_, (char*)array.Elements(), array.Capacity(),
                   &read, 0);

    ASSERT_TRUE(read <= array.Capacity());
    ASSERT_TRUE(test->mReadAllowance >= read);

    array.SetLength(read);
    test->mData.AppendElements(array);
    test->mReadAllowance -= read;

    // We may read more bytes each time we're called. This way we can ensure we
    // consume buffers partially and across multiple buffers.
    test->mReadChunkSize += test->mReadChunkSizeIncrement;
>>>>>>> upstream-releases

<<<<<<< HEAD
    // fake calling AsyncOpen() due to IPC calls. must be non-null
    mSProxy->AssignChannel_DoNotUse(new WebrtcProxyChannelWrapper(nullptr));
||||||| merged common ancestors
  static int cancel(void *obj, void *handle) {
    return 0;
=======
    if (test->mReadAllowance > 0) {
      NR_ASYNC_WAIT(s, how, &NrSocketProxyTest::readable_cb, cb_arg);
    }
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  void TearDown() override {
    mSProxy->close();
||||||| merged common ancestors
  nr_resolver *get_nr_resolver() {
    return resolver_;
=======
  static void writable_cb(NR_SOCKET s, int how, void* cb_arg) {
    NrSocketProxyTest* test = (NrSocketProxyTest*)cb_arg;
    test->mConnected = true;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  static void readable_cb(NR_SOCKET s, int how, void *cb_arg) {
    NrSocketProxyTest* test = (NrSocketProxyTest*) cb_arg;
    size_t capacity = std::min(test->mReadChunkSize, test->mReadAllowance);
    nsTArray<uint8_t> array(capacity);
    size_t read;

    nr_socket_read(test->nr_socket_,
                   (char*)array.Elements(),
                   array.Capacity(),
                   &read,
                   0);

    ASSERT_TRUE(read <= array.Capacity());
    ASSERT_TRUE(test->mReadAllowance >= read);
||||||| merged common ancestors
 private:
  nr_resolver_vtbl *vtbl_;
  nr_resolver *resolver_;
};

class ProxyTunnelSocketTest : public MtransportTest {
 public:
  ProxyTunnelSocketTest()
      : socket_impl_(nullptr),
        nr_socket_(nullptr) {}

  ~ProxyTunnelSocketTest() {
    nr_socket_destroy(&nr_socket_);
    nr_proxy_tunnel_config_destroy(&config_);
  }
=======
  const std::string DataString() {
    return std::string((char*)mData.Elements(), mData.Length());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    array.SetLength(read);
    test->mData.AppendElements(array);
    test->mReadAllowance -= read;

    // We may read more bytes each time we're called. This way we can ensure we
    // consume buffers partially and across multiple buffers.
    test->mReadChunkSize += test->mReadChunkSizeIncrement;

    if (test->mReadAllowance > 0) {
      NR_ASYNC_WAIT(s, how, &NrSocketProxyTest::readable_cb, cb_arg);
    }
  }
||||||| merged common ancestors
  void SetUp() override {
    MtransportTest::SetUp();

    nr_resolver_ = resolver_impl_.get_nr_resolver();

    int r = nr_str_port_to_transport_addr(
        (char *)kRemoteAddr.c_str(), kRemotePort, IPPROTO_TCP, &remote_addr_);
    ASSERT_EQ(0, r);

    r = nr_str_port_to_transport_addr(
        (char *)kProxyAddr.c_str(), kProxyPort, IPPROTO_TCP, &proxy_addr_);
    ASSERT_EQ(0, r);

    nr_proxy_tunnel_config_create(&config_);
    nr_proxy_tunnel_config_set_resolver(config_, nr_resolver_);
    nr_proxy_tunnel_config_set_proxy(config_, kProxyAddr.c_str(), kProxyPort);

    Configure();
  }

  // This reconfigures the socket with the updated information in config_.
  void Configure() {
    if (nr_socket_) {
      EXPECT_EQ(0, nr_socket_destroy(&nr_socket_));
      EXPECT_EQ(nullptr, nr_socket_);
    }

    RefPtr<DummySocket> dummy(new DummySocket());
    int r = nr_socket_proxy_tunnel_create(
        config_,
        dummy->get_nr_socket(),
        &nr_socket_);
    ASSERT_EQ(0, r);

    socket_impl_ = dummy.forget();  // Now owned by nr_socket_.
  }
=======
 protected:
  RefPtr<NrSocketProxy> mSProxy;
  nr_socket* nr_socket_;

  nsTArray<uint8_t> mData;
  nsTArray<uint8_t> mEmptyArray;

  uint32_t mReadChunkSize;
  uint32_t mReadChunkSizeIncrement;
  uint32_t mReadAllowance;

  bool mConnected;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void writable_cb(NR_SOCKET s, int how, void *cb_arg) {
    NrSocketProxyTest* test = (NrSocketProxyTest*) cb_arg;
    test->mConnected = true;
  }
||||||| merged common ancestors
  void Connect(int expectedReturn = 0) {
    int r = nr_socket_connect(nr_socket_, &remote_addr_);
    EXPECT_EQ(expectedReturn, r);

    size_t written = 0;
    r = nr_socket_write(nr_socket_, kHelloMessage.c_str(), kHelloMessage.size(), &written, 0);
    EXPECT_EQ(0, r);
    EXPECT_EQ(kHelloMessage.size(), written);
  }
=======
TEST_F(NrSocketProxyTest, TestCreate) {}

TEST_F(NrSocketProxyTest, TestConnected) {
  ASSERT_TRUE(!mConnected);
>>>>>>> upstream-releases

<<<<<<< HEAD
  const std::string DataString() {
    return std::string((char*)mData.Elements(), mData.Length());
  }
||||||| merged common ancestors
  nr_socket *socket() { return nr_socket_; }
=======
  NR_ASYNC_WAIT(mSProxy, NR_ASYNC_WAIT_WRITE, &NrSocketProxyTest::writable_cb,
                this);
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  RefPtr<NrSocketProxy> mSProxy;
  nr_socket *nr_socket_;
||||||| merged common ancestors
 protected:
  RefPtr<DummySocket> socket_impl_;
  DummyResolver resolver_impl_;
  nr_socket *nr_socket_;
  nr_resolver *nr_resolver_;
  nr_proxy_tunnel_config *config_;
  nr_transport_addr proxy_addr_;
  nr_transport_addr remote_addr_;
};
=======
  // still not connected just registered for writes...
  ASSERT_TRUE(!mConnected);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsTArray<uint8_t> mData;
  nsTArray<uint8_t> mEmptyArray;
||||||| merged common ancestors
=======
  mSProxy->OnConnected();
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t mReadChunkSize;
  uint32_t mReadChunkSizeIncrement;
  uint32_t mReadAllowance;
||||||| merged common ancestors
TEST_F(ProxyTunnelSocketTest, TestCreate) {
}
=======
  ASSERT_TRUE(mConnected);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mConnected;
};
||||||| merged common ancestors
TEST_F(ProxyTunnelSocketTest, TestConnectProxyAddress) {
  int r = nr_socket_connect(nr_socket_, &remote_addr_);
  ASSERT_EQ(0, r);
=======
TEST_F(NrSocketProxyTest, TestRead) {
  nsTArray<uint8_t> array;
  array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(NrSocketProxyTest, TestCreate) {
}
||||||| merged common ancestors
  ASSERT_EQ(0, nr_transport_addr_cmp(socket_impl_->get_connect_addr(), &proxy_addr_,
        NR_TRANSPORT_ADDR_CMP_MODE_ALL));
}
=======
  NR_ASYNC_WAIT(mSProxy, NR_ASYNC_WAIT_READ, &NrSocketProxyTest::readable_cb,
                this);
  // this will read 0 bytes here
  mSProxy->OnRead(std::move(array));

  ASSERT_EQ(kHelloMessage.length(), mSProxy->CountUnreadBytes());
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(NrSocketProxyTest, TestConnected) {
  ASSERT_TRUE(!mConnected);
||||||| merged common ancestors
// TODO: Reenable once bug 1251212 is fixed
TEST_F(ProxyTunnelSocketTest, DISABLED_TestConnectProxyRequest) {
  Connect();
=======
  // callback is still set but terminated due to 0 byte read
  // start callbacks again (first read is 0 then 1,2,3,...)
  mSProxy->OnRead(std::move(mEmptyArray));
>>>>>>> upstream-releases

<<<<<<< HEAD
  NR_ASYNC_WAIT(mSProxy,
                NR_ASYNC_WAIT_WRITE,
                &NrSocketProxyTest::writable_cb,
                this);
||||||| merged common ancestors
  std::string msg = connect_message(kRemoteAddr, kRemotePort, "", kHelloMessage);
  socket_impl_->CheckWriteBuffer(reinterpret_cast<const uint8_t *>(msg.c_str()), msg.size());
}
=======
  ASSERT_EQ(kHelloMessage.length(), mData.Length());
  ASSERT_EQ(kHelloMessage, DataString());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // still not connected just registered for writes...
  ASSERT_TRUE(!mConnected);
||||||| merged common ancestors
// TODO: Reenable once bug 1251212 is fixed
TEST_F(ProxyTunnelSocketTest, DISABLED_TestAlpnConnect) {
  const std::string alpn = "this,is,alpn";
  int r = nr_proxy_tunnel_config_set_alpn(config_, alpn.c_str());
  EXPECT_EQ(0, r);
=======
TEST_F(NrSocketProxyTest, TestReadConstantConsumeSize) {
  std::string data;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mSProxy->OnConnected();
||||||| merged common ancestors
  Configure();
  Connect();
=======
  // triangle number
  const int kCount = 32;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ASSERT_TRUE(mConnected);
}
||||||| merged common ancestors
  std::string msg = connect_message(kRemoteAddr, kRemotePort, alpn, kHelloMessage);
  socket_impl_->CheckWriteBuffer(reinterpret_cast<const uint8_t *>(msg.c_str()), msg.size());
}
=======
  //  ~17kb
  // triangle number formula n*(n+1)/2
  for (int i = 0; i < kCount * (kCount + 1) / 2; ++i) {
    data += kHelloMessage;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(NrSocketProxyTest, TestRead) {
  nsTArray<uint8_t> array;
  array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
||||||| merged common ancestors
// TODO: Reenable once bug 1251212 is fixed
TEST_F(ProxyTunnelSocketTest, DISABLED_TestNullAlpnConnect) {
  int r = nr_proxy_tunnel_config_set_alpn(config_, nullptr);
  EXPECT_EQ(0, r);
=======
  // decreasing buffer sizes
  for (int i = 0, start = 0; i < kCount; ++i) {
    int length = (kCount - i) * kHelloMessage.length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  NR_ASYNC_WAIT(mSProxy,
                NR_ASYNC_WAIT_READ,
                &NrSocketProxyTest::readable_cb,
                this);
  // this will read 0 bytes here
  mSProxy->OnRead(std::move(array));
||||||| merged common ancestors
  Configure();
  Connect();
=======
    nsTArray<uint8_t> array;
    array.AppendElements(data.c_str() + start, length);
    start += length;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ASSERT_EQ(kHelloMessage.length(), mSProxy->CountUnreadBytes());
||||||| merged common ancestors
  std::string msg = connect_message(kRemoteAddr, kRemotePort, "", kHelloMessage);
  socket_impl_->CheckWriteBuffer(reinterpret_cast<const uint8_t *>(msg.c_str()), msg.size());
}
=======
    mSProxy->OnRead(std::move(array));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // callback is still set but terminated due to 0 byte read
  // start callbacks again (first read is 0 then 1,2,3,...)
  mSProxy->OnRead(std::move(mEmptyArray));

  ASSERT_EQ(kHelloMessage.length(), mData.Length());
  ASSERT_EQ(kHelloMessage, DataString());
}
||||||| merged common ancestors
// TODO: Reenable once bug 1251212 is fixed
TEST_F(ProxyTunnelSocketTest, DISABLED_TestConnectProxyHostRequest) {
  nr_proxy_tunnel_config_set_proxy(config_, kProxyHost.c_str(), kProxyPort);
  Configure();
  // Because kProxyHost is a domain name and not an IP address,
  // nr_socket_connect will need to resolve an IP address before continuing.  It
  // does that, and assumes that resolving the IP will take some time, so it
  // returns R_WOULDBLOCK.
  //
  // However, In this test setup, the resolution happens inline immediately, so
  // nr_socket_connect is called recursively on the inner socket in
  // nr_socket_proxy_tunnel_resolved_cb.  That also completes.  Thus, the socket
  // is actually successfully connected after this call, even though
  // nr_socket_connect reports an error.
  //
  // Arguably nr_socket_proxy_tunnel_connect() is busted, because it shouldn't
  // report an error when it doesn't need any further assistance from the
  // calling code, but that's pretty minor.
  Connect(R_WOULDBLOCK);

  std::string msg = connect_message(kRemoteAddr, kRemotePort, "", kHelloMessage);
  socket_impl_->CheckWriteBuffer(reinterpret_cast<const uint8_t *>(msg.c_str()), msg.size());
}
=======
  ASSERT_EQ(data.length(), mSProxy->CountUnreadBytes());
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(NrSocketProxyTest, TestReadConstantConsumeSize) {
  std::string data;

  // triangle number
  const int kCount = 32;
||||||| merged common ancestors
// TODO: Reenable once bug 1251212 is fixed
TEST_F(ProxyTunnelSocketTest, DISABLED_TestConnectProxyWrite) {
  Connect();
=======
  // read same amount each callback
  mReadChunkSize = 128;
  mReadChunkSizeIncrement = 0;
  NR_ASYNC_WAIT(mSProxy, NR_ASYNC_WAIT_READ, &NrSocketProxyTest::readable_cb,
                this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  //  ~17kb
  // triangle number formula n*(n+1)/2
  for(int i = 0; i < kCount * (kCount + 1) / 2; ++i) {
    data += kHelloMessage;
  }
||||||| merged common ancestors
  socket_impl_->ClearWriteBuffer();
=======
  ASSERT_EQ(data.length(), mSProxy->CountUnreadBytes());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // decreasing buffer sizes
  for(int i = 0, start = 0; i < kCount; ++i) {
    int length = (kCount - i) * kHelloMessage.length();
||||||| merged common ancestors
  size_t written = 0;
  int r = nr_socket_write(nr_socket_, kHelloMessage.c_str(), kHelloMessage.size(), &written, 0);
  EXPECT_EQ(0, r);
  EXPECT_EQ(kHelloMessage.size(), written);
=======
  // start callbacks
  mSProxy->OnRead(std::move(mEmptyArray));
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsTArray<uint8_t> array;
    array.AppendElements(data.c_str() + start, length);
    start += length;
||||||| merged common ancestors
  socket_impl_->CheckWriteBuffer(reinterpret_cast<const uint8_t *>(kHelloMessage.c_str()),
      kHelloMessage.size());
}
=======
  ASSERT_EQ(data.length(), mData.Length());
  ASSERT_EQ(data, DataString());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    mSProxy->OnRead(std::move(array));
  }

  ASSERT_EQ(data.length(), mSProxy->CountUnreadBytes());

  // read same amount each callback
  mReadChunkSize = 128;
  mReadChunkSizeIncrement = 0;
  NR_ASYNC_WAIT(mSProxy,
                NR_ASYNC_WAIT_READ,
                &NrSocketProxyTest::readable_cb,
                this);
||||||| merged common ancestors
TEST_F(ProxyTunnelSocketTest, TestConnectProxySuccessResponse) {
  int r = nr_socket_connect(nr_socket_, &remote_addr_);
  ASSERT_EQ(0, r);

  std::string resp = connect_response(200, kHelloMessage);
  socket_impl_->SetReadBuffer(reinterpret_cast<const uint8_t *>(resp.c_str()), resp.size());

  char buf[4096];
  size_t read = 0;
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
  ASSERT_EQ(0, r);
=======
TEST_F(NrSocketProxyTest, TestReadNone) {
  char buf[4096];
  size_t read = 0;
  int r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ASSERT_EQ(data.length(), mSProxy->CountUnreadBytes());
||||||| merged common ancestors
  ASSERT_EQ(kHelloMessage.size(), read);
  ASSERT_EQ(0, memcmp(buf, kHelloMessage.c_str(), kHelloMessage.size()));
}
=======
  ASSERT_EQ(R_WOULDBLOCK, r);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // start callbacks
  mSProxy->OnRead(std::move(mEmptyArray));
||||||| merged common ancestors
TEST_F(ProxyTunnelSocketTest, TestConnectProxyRead) {
  int r = nr_socket_connect(nr_socket_, &remote_addr_);
  ASSERT_EQ(0, r);
=======
  nsTArray<uint8_t> array;
  array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
  mSProxy->OnRead(std::move(array));
>>>>>>> upstream-releases

<<<<<<< HEAD
  ASSERT_EQ(data.length(), mData.Length());
  ASSERT_EQ(data, DataString());
}
||||||| merged common ancestors
  std::string resp = connect_response(200, kHelloMessage);
  socket_impl_->SetReadBuffer(reinterpret_cast<const uint8_t *>(resp.c_str()), resp.size());
=======
  ASSERT_EQ(kHelloMessage.length(), mSProxy->CountUnreadBytes());
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(NrSocketProxyTest, TestReadNone) {
  char buf[4096];
  size_t read = 0;
  int r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);

  ASSERT_EQ(R_WOULDBLOCK, r);

  nsTArray<uint8_t> array;
  array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
  mSProxy->OnRead(std::move(array));
||||||| merged common ancestors
  char buf[4096];
  size_t read = 0;
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
  ASSERT_EQ(0, r);
=======
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);

  ASSERT_EQ(0, r);
  ASSERT_EQ(kHelloMessage.length(), read);
  ASSERT_EQ(kHelloMessage, std::string(buf, read));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ASSERT_EQ(kHelloMessage.length(), mSProxy->CountUnreadBytes());
||||||| merged common ancestors
  socket_impl_->ClearReadBuffer();
  socket_impl_->SetReadBuffer(reinterpret_cast<const uint8_t *>(kHelloMessage.c_str()),
      kHelloMessage.size());
=======
TEST_F(NrSocketProxyTest, TestReadMultipleSizes) {
  using namespace std;
>>>>>>> upstream-releases

<<<<<<< HEAD
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
||||||| merged common ancestors
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
  ASSERT_EQ(0, r);
=======
  string data;
  // 515 * kHelloMessage.length() == 17510
  const size_t kCount = 515;
  // randomly generated numbers, sums to 17510, 20 numbers
  vector<int> varyingSizes = {404,  622, 1463, 1597, 1676, 389, 389,
                              1272, 781, 81,   1030, 1450, 256, 812,
                              1571, 29,  1045, 911,  643,  1089};
>>>>>>> upstream-releases

<<<<<<< HEAD
  ASSERT_EQ(0, r);
  ASSERT_EQ(kHelloMessage.length(), read);
  ASSERT_EQ(kHelloMessage, std::string(buf, read));
}
||||||| merged common ancestors
  ASSERT_EQ(kHelloMessage.size(), read);
  ASSERT_EQ(0, memcmp(buf, kHelloMessage.c_str(), kHelloMessage.size()));
}
=======
  // changing varyingSizes or the test message breaks this so check here
  ASSERT_EQ(kCount, 17510 / kHelloMessage.length());
  ASSERT_EQ(17510, accumulate(varyingSizes.begin(), varyingSizes.end(), 0));
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(NrSocketProxyTest, TestReadMultipleSizes) {
  using namespace std;
||||||| merged common ancestors
TEST_F(ProxyTunnelSocketTest, TestConnectProxyReadNone) {
  int r = nr_socket_connect(nr_socket_, &remote_addr_);
  ASSERT_EQ(0, r);
=======
  // ~17kb
  for (size_t i = 0; i < kCount; ++i) {
    data += kHelloMessage;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  string data;
  // 515 * kHelloMessage.length() == 17510
  const size_t kCount = 515;
  // randomly generated numbers, sums to 17510, 20 numbers
  vector<int> varyingSizes = { 404, 622, 1463, 1597, 1676, 389, 389, 1272, 781,
    81, 1030, 1450, 256, 812, 1571, 29, 1045, 911, 643, 1089 };
||||||| merged common ancestors
  std::string resp = connect_response(200);
  socket_impl_->SetReadBuffer(reinterpret_cast<const uint8_t *>(resp.c_str()), resp.size());
=======
  nsTArray<uint8_t> array;
  array.AppendElements(data.c_str(), data.length());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // changing varyingSizes or the test message breaks this so check here
  ASSERT_EQ(kCount, 17510 / kHelloMessage.length());
  ASSERT_EQ(17510, accumulate(varyingSizes.begin(), varyingSizes.end(), 0));
||||||| merged common ancestors
  char buf[4096];
  size_t read = 0;
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
  ASSERT_EQ(R_WOULDBLOCK, r);
=======
  for (int amountToRead : varyingSizes) {
    nsTArray<uint8_t> buffer;
    buffer.AppendElements(array.Elements(), amountToRead);
    array.RemoveElementsAt(0, amountToRead);
    mSProxy->OnRead(std::move(buffer));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ~17kb
  for(size_t i = 0; i < kCount; ++i) {
    data += kHelloMessage;
  }
||||||| merged common ancestors
  socket_impl_->ClearReadBuffer();
  socket_impl_->SetReadBuffer(reinterpret_cast<const uint8_t *>(kHelloMessage.c_str()),
      kHelloMessage.size());
=======
  ASSERT_EQ(data.length(), mSProxy->CountUnreadBytes());
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsTArray<uint8_t> array;
  array.AppendElements(data.c_str(), data.length());
||||||| merged common ancestors
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
  ASSERT_EQ(0, r);
}
=======
  // don't need to read 0 on the first read, so start at 1 and keep going
  mReadChunkSize = 1;
  NR_ASYNC_WAIT(mSProxy, NR_ASYNC_WAIT_READ, &NrSocketProxyTest::readable_cb,
                this);
  // start callbacks
  mSProxy->OnRead(std::move(mEmptyArray));

  ASSERT_EQ(data.length(), mData.Length());
  ASSERT_EQ(data, DataString());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  for(int amountToRead : varyingSizes) {
    nsTArray<uint8_t> buffer;
    buffer.AppendElements(array.Elements(), amountToRead);
    array.RemoveElementsAt(0, amountToRead);
    mSProxy->OnRead(std::move(buffer));
  }
||||||| merged common ancestors
TEST_F(ProxyTunnelSocketTest, TestConnectProxyFailResponse) {
  int r = nr_socket_connect(nr_socket_, &remote_addr_);
  ASSERT_EQ(0, r);
=======
TEST_F(NrSocketProxyTest, TestReadConsumeReadDrain) {
  std::string data;
  // ~26kb total; should be even
  const int kCount = 512;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ASSERT_EQ(data.length(), mSProxy->CountUnreadBytes());
||||||| merged common ancestors
  std::string resp = connect_response(500, kHelloMessage);
  socket_impl_->SetReadBuffer(reinterpret_cast<const uint8_t *>(resp.c_str()), resp.size());
=======
  // there's some division by 2 here so check that kCount is even
  ASSERT_EQ(0, kCount % 2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // don't need to read 0 on the first read, so start at 1 and keep going
  mReadChunkSize = 1;
  NR_ASYNC_WAIT(mSProxy,
                NR_ASYNC_WAIT_READ,
                &NrSocketProxyTest::readable_cb,
                this);
  // start callbacks
  mSProxy->OnRead(std::move(mEmptyArray));

  ASSERT_EQ(data.length(), mData.Length());
  ASSERT_EQ(data, DataString());
}
||||||| merged common ancestors
  char buf[4096];
  size_t read = 0;
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
  ASSERT_NE(0, r);
}
=======
  for (int i = 0; i < kCount; ++i) {
    data += kHelloMessage;
    nsTArray<uint8_t> array;
    array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
    mSProxy->OnRead(std::move(array));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(NrSocketProxyTest, TestReadConsumeReadDrain) {
  std::string data;
  // ~26kb total; should be even
  const int kCount = 512;
||||||| merged common ancestors
TEST_F(ProxyTunnelSocketTest, TestConnectProxyGarbageResponse) {
  int r = nr_socket_connect(nr_socket_, &remote_addr_);
  ASSERT_EQ(0, r);
=======
  // read half at first
  mReadAllowance = kCount / 2 * kHelloMessage.length();
  // start by reading 1 byte
  mReadChunkSize = 1;
  NR_ASYNC_WAIT(mSProxy, NR_ASYNC_WAIT_READ, &NrSocketProxyTest::readable_cb,
                this);
  mSProxy->OnRead(std::move(mEmptyArray));

  ASSERT_EQ(data.length() / 2, mSProxy->CountUnreadBytes());
  ASSERT_EQ(data.length() / 2, mData.Length());

  // fill read buffer back up
  for (int i = 0; i < kCount / 2; ++i) {
    data += kHelloMessage;
    nsTArray<uint8_t> array;
    array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
    mSProxy->OnRead(std::move(array));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // there's some division by 2 here so check that kCount is even
  ASSERT_EQ(0, kCount % 2);
||||||| merged common ancestors
  socket_impl_->SetReadBuffer(reinterpret_cast<const uint8_t *>(kGarbageMessage.c_str()),
      kGarbageMessage.size());
=======
  // remove read limit
  mReadAllowance = -1;
  // used entire read allowance so we need to setup a new await
  NR_ASYNC_WAIT(mSProxy, NR_ASYNC_WAIT_READ, &NrSocketProxyTest::readable_cb,
                this);
  // start callbacks
  mSProxy->OnRead(std::move(mEmptyArray));
>>>>>>> upstream-releases

<<<<<<< HEAD
  for(int i = 0; i < kCount; ++i) {
    data += kHelloMessage;
    nsTArray<uint8_t> array;
    array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
    mSProxy->OnRead(std::move(array));
  }

  // read half at first
  mReadAllowance = kCount / 2 * kHelloMessage.length();
  // start by reading 1 byte
  mReadChunkSize = 1;
  NR_ASYNC_WAIT(mSProxy,
                NR_ASYNC_WAIT_READ,
                &NrSocketProxyTest::readable_cb,
                this);
  mSProxy->OnRead(std::move(mEmptyArray));

  ASSERT_EQ(data.length() / 2, mSProxy->CountUnreadBytes());
  ASSERT_EQ(data.length() / 2, mData.Length());

  // fill read buffer back up
  for(int i = 0; i < kCount / 2; ++i) {
    data += kHelloMessage;
    nsTArray<uint8_t> array;
    array.AppendElements(kHelloMessage.c_str(), kHelloMessage.length());
    mSProxy->OnRead(std::move(array));
  }

  // remove read limit
  mReadAllowance = -1;
  // used entire read allowance so we need to setup a new await
  NR_ASYNC_WAIT(mSProxy,
                NR_ASYNC_WAIT_READ,
                &NrSocketProxyTest::readable_cb,
                this);
  // start callbacks
  mSProxy->OnRead(std::move(mEmptyArray));

  ASSERT_EQ(data.length(), mData.Length());
  ASSERT_EQ(data, DataString());
||||||| merged common ancestors
  char buf[4096];
  size_t read = 0;
  r = nr_socket_read(nr_socket_, buf, sizeof(buf), &read, 0);
  ASSERT_EQ(0ul, read);
=======
  ASSERT_EQ(data.length(), mData.Length());
  ASSERT_EQ(data, DataString());
>>>>>>> upstream-releases
}
