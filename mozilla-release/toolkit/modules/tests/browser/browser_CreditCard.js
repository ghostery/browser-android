/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

<<<<<<< HEAD
ChromeUtils.import("resource://gre/modules/CreditCard.jsm");
ChromeUtils.import("resource://formautofill/OSKeyStore.jsm");
ChromeUtils.import("resource://testing-common/OSKeyStoreTestUtils.jsm");
||||||| merged common ancestors
ChromeUtils.import("resource://gre/modules/CreditCard.jsm");
ChromeUtils.import("resource://formautofill/MasterPassword.jsm");
=======
const { CreditCard } = ChromeUtils.import(
  "resource://gre/modules/CreditCard.jsm"
);
const { OSKeyStore } = ChromeUtils.import(
  "resource://formautofill/OSKeyStore.jsm"
);
const { OSKeyStoreTestUtils } = ChromeUtils.import(
  "resource://testing-common/OSKeyStoreTestUtils.jsm"
);
>>>>>>> upstream-releases

let oldGetters = {};
let gFakeLoggedIn = true;

add_task(function setup() {
<<<<<<< HEAD
  OSKeyStoreTestUtils.setup();
  oldGetters.isLoggedIn = Object.getOwnPropertyDescriptor(OSKeyStore, "isLoggedIn").get;
  OSKeyStore.__defineGetter__("isLoggedIn", () => gFakeLoggedIn);
  registerCleanupFunction(async () => {
    OSKeyStore.__defineGetter__("isLoggedIn", oldGetters.isLoggedIn);
    await OSKeyStoreTestUtils.cleanup();

    // CreditCard.jsm, OSKeyStore.jsm, and OSKeyStoreTestUtils.jsm are imported
    // into the global scope -- the window -- above. If they're not deleted,
    // they outlive the test and are reported as a leak.
    delete window.OSKeyStore;
||||||| merged common ancestors
  oldGetters._token = Object.getOwnPropertyDescriptor(MasterPassword, "_token").get;
  oldGetters.isEnabled = Object.getOwnPropertyDescriptor(MasterPassword, "isEnabled").get;
  oldGetters.isLoggedIn = Object.getOwnPropertyDescriptor(MasterPassword, "isLoggedIn").get;
  MasterPassword.__defineGetter__("_token", () => { return {hasPassword: true}; });
  MasterPassword.__defineGetter__("isEnabled", () => true);
  MasterPassword.__defineGetter__("isLoggedIn", () => gFakeLoggedIn);
  registerCleanupFunction(() => {
    MasterPassword.__defineGetter__("_token", oldGetters._token);
    MasterPassword.__defineGetter__("isEnabled", oldGetters.isEnabled);
    MasterPassword.__defineGetter__("isLoggedIn", oldGetters.isLoggedIn);

    // CreditCard.jsm and MasterPassword.jsm are imported into the global scope
    // -- the window -- above. If they're not deleted, they outlive the test and
    // are reported as a leak.
    delete window.MasterPassword;
=======
  OSKeyStoreTestUtils.setup();
  oldGetters.isLoggedIn = Object.getOwnPropertyDescriptor(
    OSKeyStore,
    "isLoggedIn"
  ).get;
  OSKeyStore.__defineGetter__("isLoggedIn", () => gFakeLoggedIn);
  registerCleanupFunction(async () => {
    OSKeyStore.__defineGetter__("isLoggedIn", oldGetters.isLoggedIn);
    await OSKeyStoreTestUtils.cleanup();

    // CreditCard.jsm, OSKeyStore.jsm, and OSKeyStoreTestUtils.jsm are imported
    // into the global scope -- the window -- above. If they're not deleted,
    // they outlive the test and are reported as a leak.
    delete window.OSKeyStore;
>>>>>>> upstream-releases
    delete window.CreditCard;
    delete window.OSKeyStoreTestUtils;
  });
});

<<<<<<< HEAD
add_task(async function test_getLabel_withOSKeyStore() {
  ok(OSKeyStore.isLoggedIn, "Confirm that OSKeyStore is faked and thinks it is logged in");
||||||| merged common ancestors
add_task(async function test_getLabel_withMasterPassword() {
  ok(MasterPassword.isEnabled, "Confirm that MasterPassword is faked and thinks it is enabled");
  ok(MasterPassword.isLoggedIn, "Confirm that MasterPassword is faked and thinks it is logged in");
=======
add_task(async function test_getLabel_withOSKeyStore() {
  ok(
    OSKeyStore.isLoggedIn,
    "Confirm that OSKeyStore is faked and thinks it is logged in"
  );
>>>>>>> upstream-releases

  const ccNumber = "4111111111111111";
  const encryptedNumber = await OSKeyStore.encrypt(ccNumber);
  const decryptedNumber = await OSKeyStore.decrypt(encryptedNumber);
  is(decryptedNumber, ccNumber, "Decrypted CC number should match original");

  const name = "Foxkeh";
<<<<<<< HEAD
  const label = CreditCard.getLabel({name: "Foxkeh", number: ccNumber});
  is(label, `**** 1111, ${name}`, "Label matches");
||||||| merged common ancestors
  const creditCard = new CreditCard({encryptedNumber, name: "Foxkeh"});
  const label = await creditCard.getLabel({showNumbers: true});
  is(label, `${ccNumber}, ${name}`);
=======
  const label = CreditCard.getLabel({ name: "Foxkeh", number: ccNumber });
  is(label, `**** 1111, ${name}`, "Label matches");
>>>>>>> upstream-releases
});
