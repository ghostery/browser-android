/* -*- Mode: indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim: set sts=2 sw=2 et tw=80: */
"use strict";

// The ext-* files are imported into the same scopes.
/* import-globals-from ext-browserAction.js */

function historyResultsForQuery(queryText, resolve) {
  let listener = {
    onEvent: function (event, query) {
      switch(event) {
        case "Search:SetHistory":
          /*
            query.text = query
            query.results = [{ id, url, title, lastVisitTime, visitCount}, ....]
          */
          if (query.text === queryText) {
            GlobalEventDispatcher.unregisterListener(listener, [
              "Search:SetHistory",
            ]);

            resolve(query.results);
          }
          break;
        }
    }
  };

  GlobalEventDispatcher.registerListener(listener, [
    "Search:SetHistory",
  ]);
}

this.history = class extends ExtensionAPI {
  getAPI(context) {
    return {
      history: {
        search: function(query) {
          GlobalEventDispatcher.sendRequest({
            type: "Search:GetHistory",
            data: query
          });

          return new Promise((resolve, reject) => {
            historyResultsForQuery(query.text, resolve)
          })
        },
      },
    };
  }
};
