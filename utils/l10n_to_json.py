#!/usr/bin/env python

"""
This script generate a set of json files from the Mozilla locales in the current
working directory. It also generate a single index file (a dictionary of file
names to list of string keys) to be able to reconstruct the non-en-US locale
files.
"""

import os
from collections import defaultdict
import json

# The list of file we need to translate
src_dirs = [ "mobile/android/base/locales/en-US",
             "mobile/android/locales/en-US/chrome",
             "mobile/locales/en-US/chrome",
             "mobile/locales/en-US/overrides" ]

# The locales we want to translate to
locales = [ "de", "es-ES", "fr", "hu", "it", "ja", "ko", "nl", "pl", "pt-BR",
            "ru", "zh-CN", "zh-TW", ]

root_path = os.path.abspath(os.path.join(os.getcwd(), os.path.pardir))
try:
    root_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir))
except:
    pass

moz_src = os.path.join(root_path, "mozilla-release")
l10n_src = os.path.join(root_path, "l10n")

def list_files(root, dirs):
    """
    Given a root and a list of inner folders, generates a list of .dtd and
    .properties files.
    """
    files = []
    for dirname in dirs:
        dir = os.path.join(root, dirname)
        files.extend([ os.path.join(moz_src, dirname, f) for f in os.listdir(dir) ])
    return files

def list_l10n_files(locale, files):
    """
    Given a locale name and a list of .dtd and .properties files, generates a
    list of locale specific files.
    """
    loc_repl = os.path.join("locales", "en-US")
    dest_dir = os.path.join("l10n", locale)
    return [ f.replace("mozilla-release", dest_dir).replace(loc_repl, "").replace("//", "/") for f in files ]

def parse_file(file, out_index=defaultdict(lambda: [])):
    """
    Handles non existing files errors and forward the parsing to the proper
    function by file extension.
    """
    (_, ext) = os.path.splitext(file)
    if not os.path.isfile(file):
        return {}
    ext = ext.lower()
    if ext == ".dtd":
        return parse_dtd(file, out_index)
    elif ext == ".properties":
        return parse_properties(file, out_index)
    else:
        return {}

def safeEncode(str):
    str = str if str else u""
    try:
        return str.encode("utf8")
    except UnicodeEncodeError:
        return u""

def parse_dtd(file, out_index=defaultdict(lambda: [])):
    """
    Generates a dictionary from a .dtd file, it also add the file and its keys
    to the out_index.
    """
    from lxml import etree
    fname = file.replace(root_path, "")
    out={}
    try:
        dtd = etree.DTD(file)
        for e in dtd.entities():
            if e.name in out:
                raise Exception("Overwriting {}".format(e.name))
            msg = safeEncode(e.content)
            org = safeEncode(e.orig)
            out[e.name] = { 'message': msg, 'orig': org }
            out_index[fname].append(e.name)
    except Exception as e:
        print "Can't parse {}".format(file)
        raise e

    return out

def parse_properties(file, out_index=defaultdict(lambda: [])):
    """
    Generates a dictionary from a .properties file, it also add the file and its
    keys to the out_index.
    """
    out={}
    fname = file.replace(root_path, "")
    f = open(file, "r")
    for line in f:
        sline = line.strip()
        if sline.startswith("#") or len(sline) == 0:
            continue
        ei = line.find("=")
        key = sline[:ei]
        out.update({ key : { 'message': sline[ei+1:] } })
        out_index[fname].append(key)
    f.close()
    return out

def generate_json(locale, files, gen_index=False):
    """
    Given a locale name and a list of the associated files, it creates a json
    file (named <locale>.json). If gen_indes is True, it will also generate an
    index file (named <locale>_index.json).
    """
    fname = "{}.json".format(locale)
    iname = "{}_index.json".format(locale)
    out = {}
    out_index = defaultdict(lambda: [])
    for f in files:
        out.update(parse_file(f, out_index))
    f = open(fname, "w")
    f.write(json.dumps(out, indent=4, ensure_ascii=False))
    f.close()
    if gen_index:
        fi = open(iname, "we")
        fi.write(json.dumps(out_index, indent=4,  ensure_ascii=False))
        fi.close()

if __name__ == "__main__":
    in_files = list_files(moz_src, src_dirs)
    generate_json("en-US", in_files, True)
    for loc in locales:
        generate_json(loc, list_l10n_files(loc, in_files))

