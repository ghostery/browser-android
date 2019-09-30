"use strict";

/*
  Test the permutations of calling complete() on the payment response and handling the case
  where the timeout is exceeded before it is called
*/

async function setup() {
  await setupFormAutofillStorage();
  await cleanupFormAutofillStorage();
  let billingAddressGUID = await addAddressRecord(PTU.Addresses.TimBL);
  let card = Object.assign({}, PTU.BasicCards.JohnDoe, { billingAddressGUID });
  let card1GUID = await addCardRecord(card);
  return { address1GUID: billingAddressGUID, card1GUID };
}

add_task(async function test_complete_success() {
<<<<<<< HEAD
  if (!OSKeyStoreTestUtils.canTestOSKeyStoreLogin()) {
    todo(false, "Cannot test OS key store login on official builds.");
    return;
  }
  await setup();
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: BLANK_PAGE_URL,
  }, async browser => {
    let {win, frame} =
      await setupPaymentDialog(browser, {
||||||| merged common ancestors
  await setup();
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: BLANK_PAGE_URL,
  }, async browser => {
    let {win, frame} =
      await setupPaymentDialog(browser, {
=======
  if (!OSKeyStoreTestUtils.canTestOSKeyStoreLogin()) {
    todo(false, "Cannot test OS key store login on official builds.");
    return;
  }
  let prefilledGuids = await setup();
  await BrowserTestUtils.withNewTab(
    {
      gBrowser,
      url: BLANK_PAGE_URL,
    },
    async browser => {
      let { win, frame } = await setupPaymentDialog(browser, {
>>>>>>> upstream-releases
        methodData: [PTU.MethodData.basicCard],
        details: Object.assign({}, PTU.Details.total60USD),
        merchantTaskFn: PTU.ContentTasks.createAndShowRequest,
<<<<<<< HEAD
      }
    );

    await spawnPaymentDialogTask(frame, PTU.DialogContentTasks.setSecurityCode, {
      securityCode: "123",
    });

    await loginAndCompletePayment(frame);

    // Add a handler to complete the payment above.
    info("acknowledging the completion from the merchant page");
    let {completeException} = await ContentTask.spawn(browser,
                                                      { result: "success" },
                                                      PTU.ContentTasks.addCompletionHandler);

    ok(!completeException, "Expect no exception to be thrown when calling complete()");

    await BrowserTestUtils.waitForCondition(() => win.closed, "dialog should be closed");
  });
||||||| merged common ancestors
      }
    );

    await spawnPaymentDialogTask(frame, PTU.DialogContentTasks.setSecurityCode, {
      securityCode: "123",
    });

    spawnPaymentDialogTask(frame, PTU.DialogContentTasks.completePayment);

    // Add a handler to complete the payment above.
    info("acknowledging the completion from the merchant page");
    let {completeException} = await ContentTask.spawn(browser,
                                                      { result: "success" },
                                                      PTU.ContentTasks.addCompletionHandler);

    ok(!completeException, "Expect no exception to be thrown when calling complete()");

    await BrowserTestUtils.waitForCondition(() => win.closed, "dialog should be closed");
  });
=======
      });

      await spawnPaymentDialogTask(
        frame,
        async ({ prefilledGuids: guids }) => {
          let paymentMethodPicker = content.document.querySelector(
            "payment-method-picker"
          );
          content.fillField(
            Cu.waiveXrays(paymentMethodPicker).dropdown.popupBox,
            guids.card1GUID
          );
        },
        { prefilledGuids }
      );

      await spawnPaymentDialogTask(
        frame,
        PTU.DialogContentTasks.setSecurityCode,
        {
          securityCode: "123",
        }
      );

      await loginAndCompletePayment(frame);

      // Add a handler to complete the payment above.
      info("acknowledging the completion from the merchant page");
      let { completeException } = await ContentTask.spawn(
        browser,
        { result: "success" },
        PTU.ContentTasks.addCompletionHandler
      );

      ok(
        !completeException,
        "Expect no exception to be thrown when calling complete()"
      );

      await BrowserTestUtils.waitForCondition(
        () => win.closed,
        "dialog should be closed"
      );
    }
  );
>>>>>>> upstream-releases
});

add_task(async function test_complete_fail() {
<<<<<<< HEAD
  if (!OSKeyStoreTestUtils.canTestOSKeyStoreLogin()) {
    todo(false, "Cannot test OS key store login on official builds.");
    return;
  }
  await setup();
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: BLANK_PAGE_URL,
  }, async browser => {
    let {win, frame} =
      await setupPaymentDialog(browser, {
||||||| merged common ancestors
  await setup();
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: BLANK_PAGE_URL,
  }, async browser => {
    let {win, frame} =
      await setupPaymentDialog(browser, {
=======
  if (!OSKeyStoreTestUtils.canTestOSKeyStoreLogin()) {
    todo(false, "Cannot test OS key store login on official builds.");
    return;
  }
  let prefilledGuids = await setup();
  await BrowserTestUtils.withNewTab(
    {
      gBrowser,
      url: BLANK_PAGE_URL,
    },
    async browser => {
      let { win, frame } = await setupPaymentDialog(browser, {
>>>>>>> upstream-releases
        methodData: [PTU.MethodData.basicCard],
        details: Object.assign({}, PTU.Details.total60USD),
        merchantTaskFn: PTU.ContentTasks.createAndShowRequest,
<<<<<<< HEAD
      }
    );

    await spawnPaymentDialogTask(frame, PTU.DialogContentTasks.setSecurityCode, {
      securityCode: "456",
    });

    info("clicking pay");
    await loginAndCompletePayment(frame);

    info("acknowledging the completion from the merchant page");
    let {completeException} = await ContentTask.spawn(browser,
                                                      { result: "fail" },
                                                      PTU.ContentTasks.addCompletionHandler);
    ok(!completeException, "Expect no exception to be thrown when calling complete()");

    ok(!win.closed, "dialog shouldn't be closed yet");

    spawnPaymentDialogTask(frame, PTU.DialogContentTasks.clickPrimaryButton);
    await BrowserTestUtils.waitForCondition(() => win.closed, "dialog should be closed");
  });
||||||| merged common ancestors
      }
    );

    await spawnPaymentDialogTask(frame, PTU.DialogContentTasks.setSecurityCode, {
      securityCode: "456",
    });

    info("clicking pay");
    spawnPaymentDialogTask(frame, PTU.DialogContentTasks.completePayment);

    info("acknowledging the completion from the merchant page");
    let {completeException} = await ContentTask.spawn(browser,
                                                      { result: "fail" },
                                                      PTU.ContentTasks.addCompletionHandler);
    ok(!completeException, "Expect no exception to be thrown when calling complete()");

    ok(!win.closed, "dialog shouldn't be closed yet");

    spawnPaymentDialogTask(frame, PTU.DialogContentTasks.clickPrimaryButton);
    await BrowserTestUtils.waitForCondition(() => win.closed, "dialog should be closed");
  });
=======
      });

      await spawnPaymentDialogTask(
        frame,
        async ({ prefilledGuids: guids }) => {
          let paymentMethodPicker = content.document.querySelector(
            "payment-method-picker"
          );
          content.fillField(
            Cu.waiveXrays(paymentMethodPicker).dropdown.popupBox,
            guids.card1GUID
          );
        },
        { prefilledGuids }
      );

      await spawnPaymentDialogTask(
        frame,
        PTU.DialogContentTasks.setSecurityCode,
        {
          securityCode: "456",
        }
      );

      info("clicking pay");
      await loginAndCompletePayment(frame);

      info("acknowledging the completion from the merchant page");
      let { completeException } = await ContentTask.spawn(
        browser,
        { result: "fail" },
        PTU.ContentTasks.addCompletionHandler
      );
      ok(
        !completeException,
        "Expect no exception to be thrown when calling complete()"
      );

      ok(!win.closed, "dialog shouldn't be closed yet");

      spawnPaymentDialogTask(frame, PTU.DialogContentTasks.clickPrimaryButton);
      await BrowserTestUtils.waitForCondition(
        () => win.closed,
        "dialog should be closed"
      );
    }
  );
>>>>>>> upstream-releases
});

add_task(async function test_complete_timeout() {
<<<<<<< HEAD
  if (!OSKeyStoreTestUtils.canTestOSKeyStoreLogin()) {
    todo(false, "Cannot test OS key store login on official builds.");
    return;
  }
  await setup();
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: BLANK_PAGE_URL,
  }, async browser => {
    // timeout the response asap
    Services.prefs.setIntPref(RESPONSE_TIMEOUT_PREF, 60);

    let {win, frame} =
      await setupPaymentDialog(browser, {
||||||| merged common ancestors
  await setup();
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: BLANK_PAGE_URL,
  }, async browser => {
    // timeout the response asap
    Services.prefs.setIntPref(RESPONSE_TIMEOUT_PREF, 60);

    let {win, frame} =
      await setupPaymentDialog(browser, {
=======
  if (!OSKeyStoreTestUtils.canTestOSKeyStoreLogin()) {
    todo(false, "Cannot test OS key store login on official builds.");
    return;
  }
  let prefilledGuids = await setup();
  await BrowserTestUtils.withNewTab(
    {
      gBrowser,
      url: BLANK_PAGE_URL,
    },
    async browser => {
      // timeout the response asap
      Services.prefs.setIntPref(RESPONSE_TIMEOUT_PREF, 60);

      let { win, frame } = await setupPaymentDialog(browser, {
>>>>>>> upstream-releases
        methodData: [PTU.MethodData.basicCard],
        details: Object.assign({}, PTU.Details.total60USD),
        merchantTaskFn: PTU.ContentTasks.createAndShowRequest,
<<<<<<< HEAD
      }
    );

    await spawnPaymentDialogTask(frame, PTU.DialogContentTasks.setSecurityCode, {
      securityCode: "789",
    });

    info("clicking pay");
    await loginAndCompletePayment(frame);

    info("acknowledging the completion from the merchant page after a delay");
    let {completeException} = await ContentTask.spawn(browser,
                                                      { result: "fail", delayMs: 1000 },
                                                      PTU.ContentTasks.addCompletionHandler);
    ok(completeException,
       "Expect an exception to be thrown when calling complete() too late");

    ok(!win.closed, "dialog shouldn't be closed");

    spawnPaymentDialogTask(frame, PTU.DialogContentTasks.clickPrimaryButton);
    await BrowserTestUtils.waitForCondition(() => win.closed, "dialog should be closed");
  });
||||||| merged common ancestors
      }
    );

    await spawnPaymentDialogTask(frame, PTU.DialogContentTasks.setSecurityCode, {
      securityCode: "789",
    });

    info("clicking pay");
    await spawnPaymentDialogTask(frame, PTU.DialogContentTasks.completePayment);

    info("acknowledging the completion from the merchant page after a delay");
    let {completeException} = await ContentTask.spawn(browser,
                                                      { result: "fail", delayMs: 1000 },
                                                      PTU.ContentTasks.addCompletionHandler);
    ok(completeException,
       "Expect an exception to be thrown when calling complete() too late");

    ok(!win.closed, "dialog shouldn't be closed");

    spawnPaymentDialogTask(frame, PTU.DialogContentTasks.clickPrimaryButton);
    await BrowserTestUtils.waitForCondition(() => win.closed, "dialog should be closed");
  });
=======
      });

      await spawnPaymentDialogTask(
        frame,
        async ({ prefilledGuids: guids }) => {
          let paymentMethodPicker = content.document.querySelector(
            "payment-method-picker"
          );
          content.fillField(
            Cu.waiveXrays(paymentMethodPicker).dropdown.popupBox,
            guids.card1GUID
          );
        },
        { prefilledGuids }
      );

      await spawnPaymentDialogTask(
        frame,
        PTU.DialogContentTasks.setSecurityCode,
        {
          securityCode: "789",
        }
      );

      info("clicking pay");
      await loginAndCompletePayment(frame);

      info("acknowledging the completion from the merchant page after a delay");
      let { completeException } = await ContentTask.spawn(
        browser,
        { result: "fail", delayMs: 1000 },
        PTU.ContentTasks.addCompletionHandler
      );
      ok(
        completeException,
        "Expect an exception to be thrown when calling complete() too late"
      );

      ok(!win.closed, "dialog shouldn't be closed");

      spawnPaymentDialogTask(frame, PTU.DialogContentTasks.clickPrimaryButton);
      await BrowserTestUtils.waitForCondition(
        () => win.closed,
        "dialog should be closed"
      );
    }
  );
>>>>>>> upstream-releases
});
