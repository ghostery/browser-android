#!/usr/bin/env python
import argparse
import os

from . import manifest
from . import vcs
from .log import get_logger
from .download import download_from_github

here = os.path.dirname(__file__)

wpt_root = os.path.abspath(os.path.join(here, os.pardir, os.pardir))

logger = get_logger()

<<<<<<< HEAD

def update(tests_root,
           manifest,
           manifest_path=None,
           working_copy=False,
           cache_root=None,
           rebuild=False):
    logger.warning("Deprecated; use manifest.load_and_update instead")
||||||| merged common ancestors
def update(tests_root, manifest, working_copy=False):
=======
MYPY = False
if MYPY:
    # MYPY is set to True when run under Mypy.
    from typing import Any
    from typing import Optional
    from .manifest import Manifest  # avoid cyclic import


def update(tests_root,  # type: str
           manifest,  # type: Manifest
           manifest_path=None,  # type: Optional[str]
           working_copy=True,  # type: bool
           cache_root=None,  # type: Optional[str]
           rebuild=False  # type: bool
           ):
    # type: (...) -> bool
    logger.warning("Deprecated; use manifest.load_and_update instead")
>>>>>>> upstream-releases
    logger.info("Updating manifest")

    tree = vcs.get_tree(tests_root, manifest, manifest_path, cache_root,
                        working_copy, rebuild)
    return manifest.update(tree)


def update_from_cli(**kwargs):
    # type: (**Any) -> None
    tests_root = kwargs["tests_root"]
    path = kwargs["path"]
    assert tests_root is not None

<<<<<<< HEAD
    if kwargs["download"]:
||||||| merged common ancestors
    m = None

    if kwargs["download"]:
=======
    if not kwargs["rebuild"] and kwargs["download"]:
>>>>>>> upstream-releases
        download_from_github(path, tests_root)

<<<<<<< HEAD
    manifest.load_and_update(tests_root,
                             path,
                             kwargs["url_base"],
                             update=True,
                             rebuild=kwargs["rebuild"],
                             cache_root=kwargs["cache_root"],
                             working_copy=kwargs["work"])
||||||| merged common ancestors
    if not kwargs.get("rebuild", False):
        try:
            m = manifest.load(tests_root, path)
        except manifest.ManifestVersionMismatch:
            logger.info("Manifest version changed, rebuilding")
            m = None

    if m is None:
        m = manifest.Manifest(kwargs["url_base"])

    changed = update(tests_root,
                     m,
                     working_copy=kwargs["work"])
    if changed:
        manifest.write(m, path)
=======
    manifest.load_and_update(tests_root,
                             path,
                             kwargs["url_base"],
                             update=True,
                             rebuild=kwargs["rebuild"],
                             cache_root=kwargs["cache_root"])
>>>>>>> upstream-releases


def abs_path(path):
    # type: (str) -> str
    return os.path.abspath(os.path.expanduser(path))


def create_parser():
    # type: () -> argparse.ArgumentParser
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-p", "--path", type=abs_path, help="Path to manifest file.")
    parser.add_argument(
        "--tests-root", type=abs_path, default=wpt_root, help="Path to root of tests.")
    parser.add_argument(
        "-r", "--rebuild", action="store_true", default=False,
        help="Force a full rebuild of the manifest.")
    parser.add_argument(
        "--url-base", action="store", default="/",
        help="Base url to use as the mount point for tests in this manifest.")
    parser.add_argument(
        "--no-download", dest="download", action="store_false", default=True,
        help="Never attempt to download the manifest.")
    parser.add_argument(
        "--cache-root", action="store", default=os.path.join(wpt_root, ".wptcache"),
        help="Path in which to store any caches (default <tests_root>/.wptcache/")
    return parser


<<<<<<< HEAD
def find_top_repo():
    path = here
    rv = None
    while path != "/":
        if vcs.is_git_repo(path):
            rv = path
        path = os.path.abspath(os.path.join(path, os.pardir))

    return rv


def run(*args, **kwargs):
||||||| merged common ancestors
def find_top_repo():
    path = here
    rv = None
    while path != "/":
        if vcs.is_git_repo(path):
            rv = path
        path = os.path.abspath(os.path.join(path, os.pardir))

    return rv


def run(**kwargs):
=======
def run(*args, **kwargs):
    # type: (*Any, **Any) -> None
>>>>>>> upstream-releases
    if kwargs["path"] is None:
        kwargs["path"] = os.path.join(kwargs["tests_root"], "MANIFEST.json")
    update_from_cli(**kwargs)


def main():
    # type: () -> None
    opts = create_parser().parse_args()

    run(**vars(opts))
