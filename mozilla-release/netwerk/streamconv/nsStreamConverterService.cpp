/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsStreamConverterService.h"
#include "nsIComponentRegistrar.h"
#include "nsAutoPtr.h"
#include "nsString.h"
#include "nsAtom.h"
#include "nsDeque.h"
#include "nsIInputStream.h"
#include "nsIStreamConverter.h"
#include "nsICategoryManager.h"
#include "nsXPCOM.h"
#include "nsISupportsPrimitives.h"
#include "nsCOMArray.h"
#include "nsTArray.h"
#include "nsServiceManagerUtils.h"
#include "nsISimpleEnumerator.h"

///////////////////////////////////////////////////////////////////
// Breadth-First-Search (BFS) algorithm state classes and types.

// Used to establish discovered verticies.
enum BFScolors { white, gray, black };

// BFS hashtable data class.
struct BFSTableData {
<<<<<<< HEAD
  nsCString key;
  BFScolors color;
  int32_t distance;
  nsAutoPtr<nsCString> predecessor;

  explicit BFSTableData(const nsACString &aKey)
      : key(aKey), color(white), distance(-1) {}
||||||| merged common ancestors
    nsCString key;
    BFScolors color;
    int32_t distance;
    nsAutoPtr<nsCString> predecessor;

    explicit BFSTableData(const nsACString& aKey)
      : key(aKey), color(white), distance(-1)
    {
    }
=======
  nsCString key;
  BFScolors color;
  int32_t distance;
  nsAutoPtr<nsCString> predecessor;

  explicit BFSTableData(const nsACString& aKey)
      : key(aKey), color(white), distance(-1) {}
>>>>>>> upstream-releases
};

////////////////////////////////////////////////////////////
// nsISupports methods
NS_IMPL_ISUPPORTS(nsStreamConverterService, nsIStreamConverterService)

////////////////////////////////////////////////////////////
// nsIStreamConverterService methods

////////////////////////////////////////////////////////////
// nsStreamConverterService methods

// Builds the graph represented as an adjacency list (and built up in
// memory using an nsObjectHashtable and nsCOMArray combination).
//
// :BuildGraph() consults the category manager for all stream converter
// CONTRACTIDS then fills the adjacency list with edges.
// An edge in this case is comprised of a FROM and TO MIME type combination.
//
// CONTRACTID format:
// @mozilla.org/streamconv;1?from=text/html&to=text/plain
// XXX curently we only handle a single from and to combo, we should repeat the
// XXX registration process for any series of from-to combos.
// XXX can use nsTokenizer for this.
//

nsresult nsStreamConverterService::BuildGraph() {
  nsresult rv;

  nsCOMPtr<nsICategoryManager> catmgr(
      do_GetService(NS_CATEGORYMANAGER_CONTRACTID, &rv));
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsISimpleEnumerator> entries;
  rv = catmgr->EnumerateCategory(NS_ISTREAMCONVERTER_KEY,
                                 getter_AddRefs(entries));
  if (NS_FAILED(rv)) return rv;

  // go through each entry to build the graph
  nsCOMPtr<nsISupports> supports;
  nsCOMPtr<nsISupportsCString> entry;
  rv = entries->GetNext(getter_AddRefs(supports));
  while (NS_SUCCEEDED(rv)) {
    entry = do_QueryInterface(supports);

    // get the entry string
    nsAutoCString entryString;
    rv = entry->GetData(entryString);
    if (NS_FAILED(rv)) return rv;

    // cobble the entry string w/ the converter key to produce a full
    // contractID.
    nsAutoCString contractID(NS_ISTREAMCONVERTER_KEY);
    contractID.Append(entryString);

    // now we've got the CONTRACTID, let's parse it up.
    rv = AddAdjacency(contractID.get());
    if (NS_FAILED(rv)) return rv;

    rv = entries->GetNext(getter_AddRefs(supports));
  }

  return NS_OK;
}

// XXX currently you can not add the same adjacency (i.e. you can't have
// multiple
// XXX stream converters registering to handle the same from-to combination.
// It's
// XXX not programatically prohibited, it's just that results are un-predictable
// XXX right now.
<<<<<<< HEAD
nsresult nsStreamConverterService::AddAdjacency(const char *aContractID) {
  nsresult rv;
  // first parse out the FROM and TO MIME-types.
||||||| merged common ancestors
nsresult
nsStreamConverterService::AddAdjacency(const char *aContractID) {
    nsresult rv;
    // first parse out the FROM and TO MIME-types.
=======
nsresult nsStreamConverterService::AddAdjacency(const char* aContractID) {
  nsresult rv;
  // first parse out the FROM and TO MIME-types.
>>>>>>> upstream-releases

  nsAutoCString fromStr, toStr;
  rv = ParseFromTo(aContractID, fromStr, toStr);
  if (NS_FAILED(rv)) return rv;

  // Each MIME-type is a vertex in the graph, so first lets make sure
  // each MIME-type is represented as a key in our hashtable.

<<<<<<< HEAD
  nsTArray<RefPtr<nsAtom>> *fromEdges = mAdjacencyList.Get(fromStr);
  if (!fromEdges) {
    // There is no fromStr vertex, create one.
    fromEdges = new nsTArray<RefPtr<nsAtom>>();
    mAdjacencyList.Put(fromStr, fromEdges);
  }
||||||| merged common ancestors
    nsTArray<RefPtr<nsAtom>>* fromEdges = mAdjacencyList.Get(fromStr);
    if (!fromEdges) {
        // There is no fromStr vertex, create one.
        fromEdges = new nsTArray<RefPtr<nsAtom>>();
        mAdjacencyList.Put(fromStr, fromEdges);
    }
=======
  nsTArray<RefPtr<nsAtom>>* fromEdges = mAdjacencyList.Get(fromStr);
  if (!fromEdges) {
    // There is no fromStr vertex, create one.
    fromEdges = new nsTArray<RefPtr<nsAtom>>();
    mAdjacencyList.Put(fromStr, fromEdges);
  }
>>>>>>> upstream-releases

  if (!mAdjacencyList.Get(toStr)) {
    // There is no toStr vertex, create one.
    mAdjacencyList.Put(toStr, new nsTArray<RefPtr<nsAtom>>());
  }

  // Now we know the FROM and TO types are represented as keys in the hashtable.
  // Let's "connect" the verticies, making an edge.

  RefPtr<nsAtom> vertex = NS_Atomize(toStr);
  if (!vertex) return NS_ERROR_OUT_OF_MEMORY;

  NS_ASSERTION(fromEdges, "something wrong in adjacency list construction");
  if (!fromEdges) return NS_ERROR_FAILURE;

  return fromEdges->AppendElement(vertex) ? NS_OK : NS_ERROR_FAILURE;
}

<<<<<<< HEAD
nsresult nsStreamConverterService::ParseFromTo(const char *aContractID,
                                               nsCString &aFromRes,
                                               nsCString &aToRes) {
  nsAutoCString ContractIDStr(aContractID);
||||||| merged common ancestors
nsresult
nsStreamConverterService::ParseFromTo(const char *aContractID, nsCString &aFromRes, nsCString &aToRes) {

    nsAutoCString ContractIDStr(aContractID);
=======
nsresult nsStreamConverterService::ParseFromTo(const char* aContractID,
                                               nsCString& aFromRes,
                                               nsCString& aToRes) {
  nsAutoCString ContractIDStr(aContractID);
>>>>>>> upstream-releases

  int32_t fromLoc = ContractIDStr.Find("from=");
  int32_t toLoc = ContractIDStr.Find("to=");
  if (-1 == fromLoc || -1 == toLoc) return NS_ERROR_FAILURE;

  fromLoc = fromLoc + 5;
  toLoc = toLoc + 3;

  nsAutoCString fromStr, toStr;

  ContractIDStr.Mid(fromStr, fromLoc, toLoc - 4 - fromLoc);
  ContractIDStr.Mid(toStr, toLoc, ContractIDStr.Length() - toLoc);

  aFromRes.Assign(fromStr);
  aToRes.Assign(toStr);

  return NS_OK;
}

typedef nsClassHashtable<nsCStringHashKey, BFSTableData> BFSHashTable;

// nsObjectHashtable enumerator functions.

class CStreamConvDeallocator : public nsDequeFunctor {
<<<<<<< HEAD
 public:
  void operator()(void *anObject) override {
    nsCString *string = (nsCString *)anObject;
    delete string;
  }
||||||| merged common ancestors
public:
    void operator()(void* anObject) override {
        nsCString *string = (nsCString*)anObject;
        delete string;
    }
=======
 public:
  void operator()(void* anObject) override {
    nsCString* string = (nsCString*)anObject;
    delete string;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
// walks the graph using a breadth-first-search algorithm which generates a
// discovered verticies tree. This tree is then walked up (from destination
// vertex, to origin vertex) and each link in the chain is added to an
// nsStringArray. A direct lookup for the given CONTRACTID should be made prior
// to calling this method in an attempt to find a direct converter rather than
// walking the graph.
nsresult nsStreamConverterService::FindConverter(
    const char *aContractID, nsTArray<nsCString> **aEdgeList) {
  nsresult rv;
  if (!aEdgeList) return NS_ERROR_NULL_POINTER;
  *aEdgeList = nullptr;

  // walk the graph in search of the appropriate converter.

  uint32_t vertexCount = mAdjacencyList.Count();
  if (0 >= vertexCount) return NS_ERROR_FAILURE;

  // Create a corresponding color table for each vertex in the graph.
  BFSHashTable lBFSTable;
  for (auto iter = mAdjacencyList.Iter(); !iter.Done(); iter.Next()) {
    const nsACString &key = iter.Key();
    MOZ_ASSERT(iter.UserData(), "no data in the table iteration");
    lBFSTable.Put(key, new BFSTableData(key));
  }

  NS_ASSERTION(lBFSTable.Count() == vertexCount,
               "strmconv BFS table init problem");

  // This is our source vertex; our starting point.
  nsAutoCString fromC, toC;
  rv = ParseFromTo(aContractID, fromC, toC);
  if (NS_FAILED(rv)) return rv;

  BFSTableData *data = lBFSTable.Get(fromC);
  if (!data) {
    return NS_ERROR_FAILURE;
  }

  data->color = gray;
  data->distance = 0;
  auto *dtorFunc = new CStreamConvDeallocator();

  nsDeque grayQ(dtorFunc);

  // Now generate the shortest path tree.
  grayQ.Push(new nsCString(fromC));
  while (0 < grayQ.GetSize()) {
    nsCString *currentHead = (nsCString *)grayQ.PeekFront();
    nsTArray<RefPtr<nsAtom>> *data2 = mAdjacencyList.Get(*currentHead);
    if (!data2) return NS_ERROR_FAILURE;

    // Get the state of the current head to calculate the distance of each
    // reachable vertex in the loop.
    BFSTableData *headVertexState = lBFSTable.Get(*currentHead);
    if (!headVertexState) return NS_ERROR_FAILURE;

    int32_t edgeCount = data2->Length();

    for (int32_t i = 0; i < edgeCount; i++) {
      nsAtom *curVertexAtom = data2->ElementAt(i);
      auto *curVertex = new nsCString();
      curVertexAtom->ToUTF8String(*curVertex);

      BFSTableData *curVertexState = lBFSTable.Get(*curVertex);
      if (!curVertexState) {
        delete curVertex;
||||||| merged common ancestors
// walks the graph using a breadth-first-search algorithm which generates a discovered
// verticies tree. This tree is then walked up (from destination vertex, to origin vertex)
// and each link in the chain is added to an nsStringArray. A direct lookup for the given
// CONTRACTID should be made prior to calling this method in an attempt to find a direct
// converter rather than walking the graph.
nsresult
nsStreamConverterService::FindConverter(const char *aContractID, nsTArray<nsCString> **aEdgeList) {
    nsresult rv;
    if (!aEdgeList) return NS_ERROR_NULL_POINTER;
    *aEdgeList = nullptr;

    // walk the graph in search of the appropriate converter.

    uint32_t vertexCount = mAdjacencyList.Count();
    if (0 >= vertexCount) return NS_ERROR_FAILURE;

    // Create a corresponding color table for each vertex in the graph.
    BFSHashTable lBFSTable;
    for (auto iter = mAdjacencyList.Iter(); !iter.Done(); iter.Next()) {
        const nsACString &key = iter.Key();
        MOZ_ASSERT(iter.UserData(), "no data in the table iteration");
        lBFSTable.Put(key, new BFSTableData(key));
    }

    NS_ASSERTION(lBFSTable.Count() == vertexCount, "strmconv BFS table init problem");

    // This is our source vertex; our starting point.
    nsAutoCString fromC, toC;
    rv = ParseFromTo(aContractID, fromC, toC);
    if (NS_FAILED(rv)) return rv;

    BFSTableData *data = lBFSTable.Get(fromC);
    if (!data) {
=======
// walks the graph using a breadth-first-search algorithm which generates a
// discovered verticies tree. This tree is then walked up (from destination
// vertex, to origin vertex) and each link in the chain is added to an
// nsStringArray. A direct lookup for the given CONTRACTID should be made prior
// to calling this method in an attempt to find a direct converter rather than
// walking the graph.
nsresult nsStreamConverterService::FindConverter(
    const char* aContractID, nsTArray<nsCString>** aEdgeList) {
  nsresult rv;
  if (!aEdgeList) return NS_ERROR_NULL_POINTER;
  *aEdgeList = nullptr;

  // walk the graph in search of the appropriate converter.

  uint32_t vertexCount = mAdjacencyList.Count();
  if (0 >= vertexCount) return NS_ERROR_FAILURE;

  // Create a corresponding color table for each vertex in the graph.
  BFSHashTable lBFSTable;
  for (auto iter = mAdjacencyList.Iter(); !iter.Done(); iter.Next()) {
    const nsACString& key = iter.Key();
    MOZ_ASSERT(iter.UserData(), "no data in the table iteration");
    lBFSTable.Put(key, new BFSTableData(key));
  }

  NS_ASSERTION(lBFSTable.Count() == vertexCount,
               "strmconv BFS table init problem");

  // This is our source vertex; our starting point.
  nsAutoCString fromC, toC;
  rv = ParseFromTo(aContractID, fromC, toC);
  if (NS_FAILED(rv)) return rv;

  BFSTableData* data = lBFSTable.Get(fromC);
  if (!data) {
    return NS_ERROR_FAILURE;
  }

  data->color = gray;
  data->distance = 0;
  auto* dtorFunc = new CStreamConvDeallocator();

  nsDeque grayQ(dtorFunc);

  // Now generate the shortest path tree.
  grayQ.Push(new nsCString(fromC));
  while (0 < grayQ.GetSize()) {
    nsCString* currentHead = (nsCString*)grayQ.PeekFront();
    nsTArray<RefPtr<nsAtom>>* data2 = mAdjacencyList.Get(*currentHead);
    if (!data2) return NS_ERROR_FAILURE;

    // Get the state of the current head to calculate the distance of each
    // reachable vertex in the loop.
    BFSTableData* headVertexState = lBFSTable.Get(*currentHead);
    if (!headVertexState) return NS_ERROR_FAILURE;

    int32_t edgeCount = data2->Length();

    for (int32_t i = 0; i < edgeCount; i++) {
      nsAtom* curVertexAtom = data2->ElementAt(i);
      auto* curVertex = new nsCString();
      curVertexAtom->ToUTF8String(*curVertex);

      BFSTableData* curVertexState = lBFSTable.Get(*curVertex);
      if (!curVertexState) {
        delete curVertex;
>>>>>>> upstream-releases
        return NS_ERROR_FAILURE;
      }

      if (white == curVertexState->color) {
        curVertexState->color = gray;
        curVertexState->distance = headVertexState->distance + 1;
        curVertexState->predecessor = new nsCString(*currentHead);
        grayQ.Push(curVertex);
      } else {
        delete curVertex;  // if this vertex has already been discovered, we
                           // don't want to leak it. (non-discovered vertex's
                           // get cleaned up when they're popped).
      }
    }
<<<<<<< HEAD
    headVertexState->color = black;
    nsCString *cur = (nsCString *)grayQ.PopFront();
    delete cur;
    cur = nullptr;
  }
  // The shortest path (if any) has been generated and is represented by the
  // chain of BFSTableData->predecessor keys. Start at the bottom and work our
  // way up.

  // first parse out the FROM and TO MIME-types being registered.

  nsAutoCString fromStr, toMIMEType;
  rv = ParseFromTo(aContractID, fromStr, toMIMEType);
  if (NS_FAILED(rv)) return rv;

  // get the root CONTRACTID
  nsAutoCString ContractIDPrefix(NS_ISTREAMCONVERTER_KEY);
  auto *shortestPath = new nsTArray<nsCString>();

  data = lBFSTable.Get(toMIMEType);
  if (!data) {
    // If this vertex isn't in the BFSTable, then no-one has registered for it,
    // therefore we can't do the conversion.
    delete shortestPath;
    return NS_ERROR_FAILURE;
  }

  while (data) {
    if (fromStr.Equals(data->key)) {
      // found it. We're done here.
      *aEdgeList = shortestPath;
      return NS_OK;
||||||| merged common ancestors

    data->color = gray;
    data->distance = 0;
    auto *dtorFunc = new CStreamConvDeallocator();

    nsDeque grayQ(dtorFunc);

    // Now generate the shortest path tree.
    grayQ.Push(new nsCString(fromC));
    while (0 < grayQ.GetSize()) {
        nsCString *currentHead = (nsCString*)grayQ.PeekFront();
        nsTArray<RefPtr<nsAtom>>* data2 = mAdjacencyList.Get(*currentHead);
        if (!data2) return NS_ERROR_FAILURE;

        // Get the state of the current head to calculate the distance of each
        // reachable vertex in the loop.
        BFSTableData *headVertexState = lBFSTable.Get(*currentHead);
        if (!headVertexState) return NS_ERROR_FAILURE;

        int32_t edgeCount = data2->Length();

        for (int32_t i = 0; i < edgeCount; i++) {
            nsAtom* curVertexAtom = data2->ElementAt(i);
            auto *curVertex = new nsCString();
            curVertexAtom->ToUTF8String(*curVertex);

            BFSTableData *curVertexState = lBFSTable.Get(*curVertex);
            if (!curVertexState) {
                delete curVertex;
                return NS_ERROR_FAILURE;
            }

            if (white == curVertexState->color) {
                curVertexState->color = gray;
                curVertexState->distance = headVertexState->distance + 1;
                curVertexState->predecessor = new nsCString(*currentHead);
                grayQ.Push(curVertex);
            } else {
                delete curVertex; // if this vertex has already been discovered, we don't want
                                  // to leak it. (non-discovered vertex's get cleaned up when
                                  // they're popped).
            }
        }
        headVertexState->color = black;
        nsCString *cur = (nsCString*)grayQ.PopFront();
        delete cur;
        cur = nullptr;
    }
    // The shortest path (if any) has been generated and is represented by the chain of
    // BFSTableData->predecessor keys. Start at the bottom and work our way up.

    // first parse out the FROM and TO MIME-types being registered.

    nsAutoCString fromStr, toMIMEType;
    rv = ParseFromTo(aContractID, fromStr, toMIMEType);
    if (NS_FAILED(rv)) return rv;

    // get the root CONTRACTID
    nsAutoCString ContractIDPrefix(NS_ISTREAMCONVERTER_KEY);
    auto *shortestPath = new nsTArray<nsCString>();

    data = lBFSTable.Get(toMIMEType);
    if (!data) {
        // If this vertex isn't in the BFSTable, then no-one has registered for it,
        // therefore we can't do the conversion.
        delete shortestPath;
        return NS_ERROR_FAILURE;
=======
    headVertexState->color = black;
    nsCString* cur = (nsCString*)grayQ.PopFront();
    delete cur;
    cur = nullptr;
  }
  // The shortest path (if any) has been generated and is represented by the
  // chain of BFSTableData->predecessor keys. Start at the bottom and work our
  // way up.

  // first parse out the FROM and TO MIME-types being registered.

  nsAutoCString fromStr, toMIMEType;
  rv = ParseFromTo(aContractID, fromStr, toMIMEType);
  if (NS_FAILED(rv)) return rv;

  // get the root CONTRACTID
  nsAutoCString ContractIDPrefix(NS_ISTREAMCONVERTER_KEY);
  auto* shortestPath = new nsTArray<nsCString>();

  data = lBFSTable.Get(toMIMEType);
  if (!data) {
    // If this vertex isn't in the BFSTable, then no-one has registered for it,
    // therefore we can't do the conversion.
    delete shortestPath;
    return NS_ERROR_FAILURE;
  }

  while (data) {
    if (fromStr.Equals(data->key)) {
      // found it. We're done here.
      *aEdgeList = shortestPath;
      return NS_OK;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // reconstruct the CONTRACTID.
    // Get the predecessor.
    if (!data->predecessor) break;  // no predecessor
    BFSTableData *predecessorData = lBFSTable.Get(*data->predecessor);
||||||| merged common ancestors
    while (data) {
        if (fromStr.Equals(data->key)) {
            // found it. We're done here.
            *aEdgeList = shortestPath;
            return NS_OK;
        }

        // reconstruct the CONTRACTID.
        // Get the predecessor.
        if (!data->predecessor) break; // no predecessor
        BFSTableData *predecessorData = lBFSTable.Get(*data->predecessor);
=======
    // reconstruct the CONTRACTID.
    // Get the predecessor.
    if (!data->predecessor) break;  // no predecessor
    BFSTableData* predecessorData = lBFSTable.Get(*data->predecessor);

    if (!predecessorData) break;  // no predecessor, chain doesn't exist.
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!predecessorData) break;  // no predecessor, chain doesn't exist.
||||||| merged common ancestors
        if (!predecessorData) break; // no predecessor, chain doesn't exist.
=======
    // build out the CONTRACTID.
    nsAutoCString newContractID(ContractIDPrefix);
    newContractID.AppendLiteral("?from=");
>>>>>>> upstream-releases

<<<<<<< HEAD
    // build out the CONTRACTID.
    nsAutoCString newContractID(ContractIDPrefix);
    newContractID.AppendLiteral("?from=");
||||||| merged common ancestors
        // build out the CONTRACTID.
        nsAutoCString newContractID(ContractIDPrefix);
        newContractID.AppendLiteral("?from=");
=======
    newContractID.Append(predecessorData->key);
>>>>>>> upstream-releases

<<<<<<< HEAD
    newContractID.Append(predecessorData->key);
||||||| merged common ancestors
        newContractID.Append(predecessorData->key);
=======
    newContractID.AppendLiteral("&to=");
    newContractID.Append(data->key);
>>>>>>> upstream-releases

<<<<<<< HEAD
    newContractID.AppendLiteral("&to=");
    newContractID.Append(data->key);
||||||| merged common ancestors
        newContractID.AppendLiteral("&to=");
        newContractID.Append(data->key);
=======
    // Add this CONTRACTID to the chain.
    rv = shortestPath->AppendElement(newContractID)
             ? NS_OK
             : NS_ERROR_FAILURE;  // XXX this method incorrectly returns a bool
    NS_ASSERTION(NS_SUCCEEDED(rv), "AppendElement failed");
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Add this CONTRACTID to the chain.
    rv = shortestPath->AppendElement(newContractID)
             ? NS_OK
             : NS_ERROR_FAILURE;  // XXX this method incorrectly returns a bool
    NS_ASSERTION(NS_SUCCEEDED(rv), "AppendElement failed");

    // move up the tree.
    data = predecessorData;
  }
  delete shortestPath;
  return NS_ERROR_FAILURE;  // couldn't find a stream converter or chain.
||||||| merged common ancestors
        // Add this CONTRACTID to the chain.
        rv = shortestPath->AppendElement(newContractID) ? NS_OK : NS_ERROR_FAILURE;  // XXX this method incorrectly returns a bool
        NS_ASSERTION(NS_SUCCEEDED(rv), "AppendElement failed");

        // move up the tree.
        data = predecessorData;
    }
    delete shortestPath;
    return NS_ERROR_FAILURE; // couldn't find a stream converter or chain.
=======
    // move up the tree.
    data = predecessorData;
  }
  delete shortestPath;
  return NS_ERROR_FAILURE;  // couldn't find a stream converter or chain.
>>>>>>> upstream-releases
}

/////////////////////////////////////////////////////
// nsIStreamConverterService methods
NS_IMETHODIMP
<<<<<<< HEAD
nsStreamConverterService::CanConvert(const char *aFromType, const char *aToType,
                                     bool *_retval) {
  nsCOMPtr<nsIComponentRegistrar> reg;
  nsresult rv = NS_GetComponentRegistrar(getter_AddRefs(reg));
  if (NS_FAILED(rv)) return rv;

  nsAutoCString contractID;
  contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
  contractID.Append(aFromType);
  contractID.AppendLiteral("&to=");
  contractID.Append(aToType);

  // See if we have a direct match
  rv = reg->IsContractIDRegistered(contractID.get(), _retval);
  if (NS_FAILED(rv)) return rv;
  if (*_retval) return NS_OK;

  // Otherwise try the graph.
  rv = BuildGraph();
  if (NS_FAILED(rv)) return rv;

  nsTArray<nsCString> *converterChain = nullptr;
  rv = FindConverter(contractID.get(), &converterChain);
  *_retval = NS_SUCCEEDED(rv);

  delete converterChain;
  return NS_OK;
||||||| merged common ancestors
nsStreamConverterService::CanConvert(const char* aFromType,
                                     const char* aToType,
                                     bool* _retval) {
    nsCOMPtr<nsIComponentRegistrar> reg;
    nsresult rv = NS_GetComponentRegistrar(getter_AddRefs(reg));
    if (NS_FAILED(rv))
        return rv;

    nsAutoCString contractID;
    contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
    contractID.Append(aFromType);
    contractID.AppendLiteral("&to=");
    contractID.Append(aToType);

    // See if we have a direct match
    rv = reg->IsContractIDRegistered(contractID.get(), _retval);
    if (NS_FAILED(rv))
        return rv;
    if (*_retval)
        return NS_OK;

    // Otherwise try the graph.
    rv = BuildGraph();
    if (NS_FAILED(rv))
        return rv;

    nsTArray<nsCString> *converterChain = nullptr;
    rv = FindConverter(contractID.get(), &converterChain);
    *_retval = NS_SUCCEEDED(rv);

    delete converterChain;
    return NS_OK;
=======
nsStreamConverterService::CanConvert(const char* aFromType, const char* aToType,
                                     bool* _retval) {
  nsCOMPtr<nsIComponentRegistrar> reg;
  nsresult rv = NS_GetComponentRegistrar(getter_AddRefs(reg));
  if (NS_FAILED(rv)) return rv;

  nsAutoCString contractID;
  contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
  contractID.Append(aFromType);
  contractID.AppendLiteral("&to=");
  contractID.Append(aToType);

  // See if we have a direct match
  rv = reg->IsContractIDRegistered(contractID.get(), _retval);
  if (NS_FAILED(rv)) return rv;
  if (*_retval) return NS_OK;

  // Otherwise try the graph.
  rv = BuildGraph();
  if (NS_FAILED(rv)) return rv;

  nsTArray<nsCString>* converterChain = nullptr;
  rv = FindConverter(contractID.get(), &converterChain);
  *_retval = NS_SUCCEEDED(rv);

  delete converterChain;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsStreamConverterService::Convert(nsIInputStream *aFromStream,
                                  const char *aFromType, const char *aToType,
                                  nsISupports *aContext,
                                  nsIInputStream **_retval) {
  if (!aFromStream || !aFromType || !aToType || !_retval)
    return NS_ERROR_NULL_POINTER;
  nsresult rv;

  // first determine whether we can even handle this conversion
  // build a CONTRACTID
  nsAutoCString contractID;
  contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
  contractID.Append(aFromType);
  contractID.AppendLiteral("&to=");
  contractID.Append(aToType);
  const char *cContractID = contractID.get();

  nsCOMPtr<nsIStreamConverter> converter(do_CreateInstance(cContractID, &rv));
  if (NS_FAILED(rv)) {
    // couldn't go direct, let's try walking the graph of converters.
    rv = BuildGraph();
    if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
nsStreamConverterService::Convert(nsIInputStream *aFromStream,
                                  const char *aFromType,
                                  const char *aToType,
                                  nsISupports *aContext,
                                  nsIInputStream **_retval) {
    if (!aFromStream || !aFromType || !aToType || !_retval) return NS_ERROR_NULL_POINTER;
    nsresult rv;

    // first determine whether we can even handle this conversion
    // build a CONTRACTID
    nsAutoCString contractID;
    contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
    contractID.Append(aFromType);
    contractID.AppendLiteral("&to=");
    contractID.Append(aToType);
    const char *cContractID = contractID.get();

    nsCOMPtr<nsIStreamConverter> converter(do_CreateInstance(cContractID, &rv));
    if (NS_FAILED(rv)) {
        // couldn't go direct, let's try walking the graph of converters.
        rv = BuildGraph();
        if (NS_FAILED(rv)) return rv;

        nsTArray<nsCString> *converterChain = nullptr;

        rv = FindConverter(cContractID, &converterChain);
        if (NS_FAILED(rv)) {
            // can't make this conversion.
            // XXX should have a more descriptive error code.
            return NS_ERROR_FAILURE;
        }
=======
nsStreamConverterService::Convert(nsIInputStream* aFromStream,
                                  const char* aFromType, const char* aToType,
                                  nsISupports* aContext,
                                  nsIInputStream** _retval) {
  if (!aFromStream || !aFromType || !aToType || !_retval)
    return NS_ERROR_NULL_POINTER;
  nsresult rv;

  // first determine whether we can even handle this conversion
  // build a CONTRACTID
  nsAutoCString contractID;
  contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
  contractID.Append(aFromType);
  contractID.AppendLiteral("&to=");
  contractID.Append(aToType);
  const char* cContractID = contractID.get();

  nsCOMPtr<nsIStreamConverter> converter(do_CreateInstance(cContractID, &rv));
  if (NS_FAILED(rv)) {
    // couldn't go direct, let's try walking the graph of converters.
    rv = BuildGraph();
    if (NS_FAILED(rv)) return rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
    nsTArray<nsCString> *converterChain = nullptr;
||||||| merged common ancestors
        int32_t edgeCount = int32_t(converterChain->Length());
        NS_ASSERTION(edgeCount > 0, "findConverter should have failed");
=======
    nsTArray<nsCString>* converterChain = nullptr;
>>>>>>> upstream-releases

    rv = FindConverter(cContractID, &converterChain);
    if (NS_FAILED(rv)) {
      // can't make this conversion.
      // XXX should have a more descriptive error code.
      return NS_ERROR_FAILURE;
    }

    int32_t edgeCount = int32_t(converterChain->Length());
    NS_ASSERTION(edgeCount > 0, "findConverter should have failed");

    // convert the stream using each edge of the graph as a step.
    // this is our stream conversion traversal.
    nsCOMPtr<nsIInputStream> dataToConvert = aFromStream;
    nsCOMPtr<nsIInputStream> convertedData;

<<<<<<< HEAD
    for (int32_t i = edgeCount - 1; i >= 0; i--) {
      const char *lContractID = converterChain->ElementAt(i).get();
||||||| merged common ancestors
            converter = do_CreateInstance(lContractID, &rv);
=======
    for (int32_t i = edgeCount - 1; i >= 0; i--) {
      const char* lContractID = converterChain->ElementAt(i).get();
>>>>>>> upstream-releases

      converter = do_CreateInstance(lContractID, &rv);

      if (NS_FAILED(rv)) {
        delete converterChain;
        return rv;
      }

      nsAutoCString fromStr, toStr;
      rv = ParseFromTo(lContractID, fromStr, toStr);
      if (NS_FAILED(rv)) {
        delete converterChain;
        return rv;
      }

      rv = converter->Convert(dataToConvert, fromStr.get(), toStr.get(),
                              aContext, getter_AddRefs(convertedData));
      dataToConvert = convertedData;
      if (NS_FAILED(rv)) {
        delete converterChain;
        return rv;
      }
    }

    delete converterChain;
    convertedData.forget(_retval);
  } else {
    // we're going direct.
    rv = converter->Convert(aFromStream, aFromType, aToType, aContext, _retval);
  }

  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsStreamConverterService::AsyncConvertData(const char *aFromType,
                                           const char *aToType,
                                           nsIStreamListener *aListener,
                                           nsISupports *aContext,
                                           nsIStreamListener **_retval) {
  if (!aFromType || !aToType || !aListener || !_retval)
    return NS_ERROR_NULL_POINTER;

  nsresult rv;

  // first determine whether we can even handle this conversion
  // build a CONTRACTID
  nsAutoCString contractID;
  contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
  contractID.Append(aFromType);
  contractID.AppendLiteral("&to=");
  contractID.Append(aToType);
  const char *cContractID = contractID.get();

  nsCOMPtr<nsIStreamConverter> listener(do_CreateInstance(cContractID, &rv));
  if (NS_FAILED(rv)) {
    // couldn't go direct, let's try walking the graph of converters.
    rv = BuildGraph();
    if (NS_FAILED(rv)) return rv;

    nsTArray<nsCString> *converterChain = nullptr;

    rv = FindConverter(cContractID, &converterChain);
||||||| merged common ancestors
nsStreamConverterService::AsyncConvertData(const char *aFromType,
                                           const char *aToType,
                                           nsIStreamListener *aListener,
                                           nsISupports *aContext,
                                           nsIStreamListener **_retval) {
    if (!aFromType || !aToType || !aListener || !_retval) return NS_ERROR_NULL_POINTER;

    nsresult rv;

    // first determine whether we can even handle this conversion
    // build a CONTRACTID
    nsAutoCString contractID;
    contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
    contractID.Append(aFromType);
    contractID.AppendLiteral("&to=");
    contractID.Append(aToType);
    const char *cContractID = contractID.get();

    nsCOMPtr<nsIStreamConverter> listener(do_CreateInstance(cContractID, &rv));
=======
nsStreamConverterService::AsyncConvertData(const char* aFromType,
                                           const char* aToType,
                                           nsIStreamListener* aListener,
                                           nsISupports* aContext,
                                           nsIStreamListener** _retval) {
  if (!aFromType || !aToType || !aListener || !_retval)
    return NS_ERROR_NULL_POINTER;

  nsresult rv;

  // first determine whether we can even handle this conversion
  // build a CONTRACTID
  nsAutoCString contractID;
  contractID.AssignLiteral(NS_ISTREAMCONVERTER_KEY "?from=");
  contractID.Append(aFromType);
  contractID.AppendLiteral("&to=");
  contractID.Append(aToType);
  const char* cContractID = contractID.get();

  nsCOMPtr<nsIStreamConverter> listener(do_CreateInstance(cContractID, &rv));
  if (NS_FAILED(rv)) {
    // couldn't go direct, let's try walking the graph of converters.
    rv = BuildGraph();
    if (NS_FAILED(rv)) return rv;

    nsTArray<nsCString>* converterChain = nullptr;

    rv = FindConverter(cContractID, &converterChain);
>>>>>>> upstream-releases
    if (NS_FAILED(rv)) {
      // can't make this conversion.
      // XXX should have a more descriptive error code.
      return NS_ERROR_FAILURE;
    }

<<<<<<< HEAD
    // aListener is the listener that wants the final, converted, data.
    // we initialize finalListener w/ aListener so it gets put at the
    // tail end of the chain, which in the loop below, means the *first*
    // converter created.
    nsCOMPtr<nsIStreamListener> finalListener = aListener;

    // convert the stream using each edge of the graph as a step.
    // this is our stream conversion traversal.
    int32_t edgeCount = int32_t(converterChain->Length());
    NS_ASSERTION(edgeCount > 0, "findConverter should have failed");
    for (int i = 0; i < edgeCount; i++) {
      const char *lContractID = converterChain->ElementAt(i).get();

      // create the converter for this from/to pair
      nsCOMPtr<nsIStreamConverter> converter(do_CreateInstance(lContractID));
      NS_ASSERTION(converter,
                   "graph construction problem, built a contractid that wasn't "
                   "registered");

      nsAutoCString fromStr, toStr;
      rv = ParseFromTo(lContractID, fromStr, toStr);
      if (NS_FAILED(rv)) {
        delete converterChain;
        return rv;
      }
||||||| merged common ancestors
    return rv;
=======
    // aListener is the listener that wants the final, converted, data.
    // we initialize finalListener w/ aListener so it gets put at the
    // tail end of the chain, which in the loop below, means the *first*
    // converter created.
    nsCOMPtr<nsIStreamListener> finalListener = aListener;

    // convert the stream using each edge of the graph as a step.
    // this is our stream conversion traversal.
    int32_t edgeCount = int32_t(converterChain->Length());
    NS_ASSERTION(edgeCount > 0, "findConverter should have failed");
    for (int i = 0; i < edgeCount; i++) {
      const char* lContractID = converterChain->ElementAt(i).get();

      // create the converter for this from/to pair
      nsCOMPtr<nsIStreamConverter> converter(do_CreateInstance(lContractID));
      NS_ASSERTION(converter,
                   "graph construction problem, built a contractid that wasn't "
                   "registered");

      nsAutoCString fromStr, toStr;
      rv = ParseFromTo(lContractID, fromStr, toStr);
      if (NS_FAILED(rv)) {
        delete converterChain;
        return rv;
      }
>>>>>>> upstream-releases

      // connect the converter w/ the listener that should get the converted
      // data.
      rv = converter->AsyncConvertData(fromStr.get(), toStr.get(),
                                       finalListener, aContext);
      if (NS_FAILED(rv)) {
        delete converterChain;
        return rv;
      }

      // the last iteration of this loop will result in finalListener
      // pointing to the converter that "starts" the conversion chain.
      // this converter's "from" type is the original "from" type. Prior
      // to the last iteration, finalListener will continuously be wedged
      // into the next listener in the chain, then be updated.
      finalListener = converter;
    }
    delete converterChain;
    // return the first listener in the chain.
    finalListener.forget(_retval);
  } else {
    // we're going direct.
    rv = listener->AsyncConvertData(aFromType, aToType, aListener, aContext);
    listener.forget(_retval);
  }

  return rv;
}

<<<<<<< HEAD
nsresult NS_NewStreamConv(nsStreamConverterService **aStreamConv) {
  MOZ_ASSERT(aStreamConv != nullptr, "null ptr");
  if (!aStreamConv) return NS_ERROR_NULL_POINTER;
||||||| merged common ancestors
nsresult
NS_NewStreamConv(nsStreamConverterService** aStreamConv)
{
    MOZ_ASSERT(aStreamConv != nullptr, "null ptr");
    if (!aStreamConv) return NS_ERROR_NULL_POINTER;
=======
nsresult NS_NewStreamConv(nsStreamConverterService** aStreamConv) {
  MOZ_ASSERT(aStreamConv != nullptr, "null ptr");
  if (!aStreamConv) return NS_ERROR_NULL_POINTER;
>>>>>>> upstream-releases

  *aStreamConv = new nsStreamConverterService();
  NS_ADDREF(*aStreamConv);

  return NS_OK;
}
