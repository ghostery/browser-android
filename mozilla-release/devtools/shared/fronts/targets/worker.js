/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
"use strict";

const { workerTargetSpec } = require("devtools/shared/specs/targets/worker");
const {
  FrontClassWithSpec,
  registerFront,
} = require("devtools/shared/protocol");
const { TargetMixin } = require("./target-mixin");

class WorkerTargetFront extends TargetMixin(
  FrontClassWithSpec(workerTargetSpec)
) {
  constructor(client) {
    super(client);

<<<<<<< HEAD
loader.lazyRequireGetter(this, "ThreadClient", "devtools/shared/client/thread-client");

const WorkerTargetFront = protocol.FrontClassWithSpec(workerTargetSpec, {
  initialize: function(client) {
    protocol.Front.prototype.initialize.call(this, client);

    this.thread = null;
||||||| merged common ancestors
loader.lazyRequireGetter(this, "ThreadClient", "devtools/shared/client/thread-client");

const WorkerTargetFront = protocol.FrontClassWithSpec(workerTargetSpec, {
  initialize: function(client, form) {
    protocol.Front.prototype.initialize.call(this, client, form);

    this.thread = null;
=======
>>>>>>> upstream-releases
    this.traits = {};

<<<<<<< HEAD
    // TODO: remove once ThreadClient becomes a front
    this.client = client;

    this._isClosed = false;

    this.destroy = this.destroy.bind(this);
    this.on("close", this.destroy);
  },

  form(json) {
    this.actorID = json.actor;

    // Save the full form for Target class usage.
    // Do not use `form` name to avoid colliding with protocol.js's `form` method
    this.targetForm = json;
    this.url = json.url;
    this.type = json.type;
    this.scope = json.scope;
    this.fetch = json.fetch;
  },

  get isClosed() {
    return this._isClosed;
  },

  destroy: function() {
    this.off("close", this.destroy);
    this._isClosed = true;

    if (this.thread) {
      this.client.unregisterClient(this.thread);
    }

    this.unmanage(this);

    protocol.Front.prototype.destroy.call(this);
  },

  attach: custom(async function() {
    const response = await this._attach();

    this.url = response.url;

    // Immediately call `connect` in other to fetch console and thread actors
    // that will be later used by Target.
    const connectResponse = await this.connect({});
    // Set the console actor ID on the form to expose it to Target.attach's attachConsole
    this.targetForm.consoleActor = connectResponse.consoleActor;
    this.threadActor = connectResponse.threadActor;

    return response;
  }, {
    impl: "_attach",
  }),

  detach: custom(async function() {
    if (this.isClosed) {
      return {};
    }
    let response;
    try {
      response = await this._detach();
    } catch (e) {
      console.warn(`Error while detaching the worker target front: ${e.message}`);
||||||| merged common ancestors
    // TODO: remove once ThreadClient becomes a front
    this.client = client;

    this._isClosed = false;

    this.destroy = this.destroy.bind(this);
    this.on("close", this.destroy);
  },

  get isClosed() {
    return this._isClosed;
  },

  destroy: function() {
    this.off("close", this.destroy);
    this._isClosed = true;

    if (this.thread) {
      this.client.unregisterClient(this.thread);
    }

    this.unmanage(this);

    protocol.Front.prototype.destroy.call(this);
  },

  attach: custom(async function() {
    const response = await this._attach();

    this.url = response.url;

    return response;
  }, {
    impl: "_attach",
  }),

  detach: custom(async function() {
    let response;
    try {
      response = await this._detach();
    } catch (e) {
      console.warn(`Error while detaching the worker target front: ${e.message}`);
=======
    // The actor sends a "close" event, which is translated to "worker-close" by
    // the specification in order to not conflict with Target's "close" event.
    // This event is similar to tabDetached and means that the worker is destroyed.
    // So that we should destroy the target in order to significate that the target
    // is no longer debuggable.
    this.once("worker-close", this.destroy.bind(this));
  }

  form(json) {
    this.actorID = json.actor;
    // `id` was added in Firefox 68 to the worker target actor. Fallback to the actorID
    // when debugging older clients.
    // Fallback can be removed when Firefox 68 will be in the Release channel.
    this.id = json.id || this.actorID;

    // Save the full form for Target class usage.
    // Do not use `form` name to avoid colliding with protocol.js's `form` method
    this.targetForm = json;
    this._url = json.url;
    this.type = json.type;
    this.scope = json.scope;
    this.fetch = json.fetch;
  }

  async attach() {
    if (this._attach) {
      return this._attach;
>>>>>>> upstream-releases
    }
    this._attach = (async () => {
      const response = await super.attach();

      this._url = response.url;

      // Immediately call `connect` in other to fetch console and thread actors
      // that will be later used by Target.
      const connectResponse = await this.connect({});
      // Set the console actor ID on the form to expose it to Target.attachConsole
      // Set the ThreadActor on the target form so it is accessible by getFront
      this.targetForm.consoleActor = connectResponse.consoleActor;
      this.targetForm.contextActor = connectResponse.threadActor;
      this._threadActor = connectResponse.threadActor;

      return this.attachConsole();
    })();
    return this._attach;
  }

  reconfigure() {
    // Toolbox and options panel are calling this method but Worker Target can't be
    // reconfigured. So we ignore this call here.
    return Promise.resolve();
<<<<<<< HEAD
  },

  attachThread: async function(options = {}) {
    if (this.thread) {
      const response = [{
        type: "connected",
        threadActor: this.thread._actor,
        consoleActor: this.targetForm.consoleActor,
      }, this.thread];
      return response;
    }

    const attachResponse = await this.client.request({
      to: this.threadActor,
      type: "attach",
      options,
    });
    this.thread = new ThreadClient(this, this.threadActor);
    this.client.registerClient(this.thread);

    return [attachResponse, this.thread];
  },

});
||||||| merged common ancestors
  },

  attachThread: async function(options = {}) {
    if (this.thread) {
      const response = [{
        type: "connected",
        threadActor: this.thread._actor,
        consoleActor: this.consoleActor,
      }, this.thread];
      return response;
    }

    // The connect call on server doesn't attach the thread as of version 44.
    const connectResponse = await this.connect(options);
    await this.client.request({
      to: connectResponse.threadActor,
      type: "attach",
      options,
    });
    this.thread = new ThreadClient(this, connectResponse.threadActor);
    this.consoleActor = connectResponse.consoleActor;
    this.client.registerClient(this.thread);

    return [connectResponse, this.thread];
  },

});
=======
  }
}
>>>>>>> upstream-releases

exports.WorkerTargetFront = WorkerTargetFront;
registerFront(exports.WorkerTargetFront);
