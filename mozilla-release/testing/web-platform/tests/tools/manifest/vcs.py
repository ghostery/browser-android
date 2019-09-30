<<<<<<< HEAD
import json
||||||| merged common ancestors
=======
import abc
import json
>>>>>>> upstream-releases
import os
<<<<<<< HEAD
import platform
import stat
import subprocess
from collections import deque
||||||| merged common ancestors
import subprocess
import platform
=======
import stat
from collections import deque
from collections import MutableMapping

from six import with_metaclass, PY2
>>>>>>> upstream-releases

from .sourcefile import SourceFile
from .utils import git

try:
    from ..gitignore import gitignore
except ValueError:
    # relative import beyond toplevel throws *ValueError*!
    from gitignore import gitignore  # type: ignore

<<<<<<< HEAD
def get_tree(tests_root, manifest, manifest_path, cache_root,
             working_copy=False, rebuild=False):
    tree = None
    if cache_root is None:
        cache_root = os.path.join(tests_root, ".wptcache")
    if not os.path.exists(cache_root):
        try:
            os.makedirs(cache_root)
        except IOError:
            cache_root = None

    if not working_copy:
        tree = Git.for_path(tests_root,
                            manifest.url_base,
                            manifest_path=manifest_path,
                            cache_path=cache_root,
                            rebuild=rebuild)
    if tree is None:
        tree = FileSystem(tests_root,
                          manifest.url_base,
                          manifest_path=manifest_path,
                          cache_path=cache_root,
                          rebuild=rebuild)
    return tree


class Git(object):
    def __init__(self, repo_root, url_base, cache_path, manifest_path=None,
                 rebuild=False):
        self.root = repo_root
        self.git = Git.get_func(repo_root)
        self.url_base = url_base
        # rebuild is a noop for now since we don't cache anything
||||||| merged common ancestors
class Git(object):
    def __init__(self, repo_root, url_base):
        self.root = os.path.abspath(repo_root)
        self.git = Git.get_func(repo_root)
        self.url_base = url_base
=======
>>>>>>> upstream-releases

MYPY = False
if MYPY:
    # MYPY is set to True when run under Mypy.
    from typing import Dict, Optional, List, Set, Text, Iterable, Any, Tuple, Union, Iterator
    from .manifest import Manifest  # cyclic import under MYPY guard
    if PY2:
        stat_result = Any
    else:
        stat_result = os.stat_result

<<<<<<< HEAD
    @classmethod
    def for_path(cls, path, url_base, cache_path, manifest_path=None, rebuild=False):
        git = Git.get_func(path)
||||||| merged common ancestors
    @classmethod
    def for_path(cls, path, url_base):
        git = Git.get_func(path)
=======

def get_tree(tests_root, manifest, manifest_path, cache_root,
             working_copy=True, rebuild=False):
    # type: (bytes, Manifest, Optional[bytes], Optional[bytes], bool, bool) -> FileSystem
    tree = None
    if cache_root is None:
        cache_root = os.path.join(tests_root, b".wptcache")
    if not os.path.exists(cache_root):
>>>>>>> upstream-releases
        try:
<<<<<<< HEAD
            return cls(git("rev-parse", "--show-toplevel").rstrip(), url_base, cache_path,
                       manifest_path=manifest_path, rebuild=rebuild)
        except (subprocess.CalledProcessError, OSError):
            return None
||||||| merged common ancestors
            return cls(git("rev-parse", "--show-toplevel").rstrip(), url_base)
        except subprocess.CalledProcessError:
            return None
=======
            os.makedirs(cache_root)
        except IOError:
            cache_root = None

    if not working_copy:
        raise ValueError("working_copy=False unsupported")

    if tree is None:
        tree = FileSystem(tests_root,
                          manifest.url_base,
                          manifest_path=manifest_path,
                          cache_path=cache_root,
                          rebuild=rebuild)
    return tree


class GitHasher(object):
    def __init__(self, path):
        # type: (bytes) -> None
        self.git = git(path)
>>>>>>> upstream-releases

    def _local_changes(self):
        # type: () -> Set[bytes]
        """get a set of files which have changed between HEAD and working copy"""
        # note that git runs the command with tests_root as the cwd, which may
        # not be the root of the git repo (e.g., within a browser repo)
        cmd = [b"diff-index", b"--relative", b"--no-renames", b"--name-only", b"-z", b"HEAD"]
        data = self.git(*cmd)
        return set(data.split(b"\0"))

    def hash_cache(self):
        # type: () -> Dict[bytes, Optional[bytes]]
        """
        A dict of rel_path -> current git object id if the working tree matches HEAD else None
        """
        hash_cache = {}  # type: Dict[bytes, Optional[bytes]]

        if self.git is None:
            return hash_cache

        cmd = [b"ls-tree", b"-r", b"-z", b"HEAD"]
        
        local_changes = self._local_changes()
<<<<<<< HEAD
        for result in self.git(*cmd).split("\0")[:-1]:
            rel_path = result.split("\t")[-1]
            hash = result.split()[2]
            if not os.path.isdir(os.path.join(self.root, rel_path)):
                if rel_path in local_changes:
                    contents = self._show_file(rel_path)
                else:
                    contents = None
                yield SourceFile(self.root,
                                 rel_path,
                                 self.url_base,
                                 hash,
                                 contents=contents), True

    def dump_caches(self):
        pass
||||||| merged common ancestors
        for result in self.git(*cmd).split("\0")[:-1]:
            rel_path = result.split("\t")[-1]
            hash = result.split()[2]
            if not os.path.isdir(os.path.join(self.root, rel_path)):
                if rel_path in local_changes:
                    contents = self._show_file(rel_path)
                else:
                    contents = None
                yield SourceFile(self.root,
                                 rel_path,
                                 self.url_base,
                                 hash,
                                 contents=contents)
=======
        for result in self.git(*cmd).split(b"\0")[:-1]:  # type: bytes
            data, rel_path = result.rsplit(b"\t", 1)
            hash_cache[rel_path] = None if rel_path in local_changes else data.split(b" ", 3)[2]

        return hash_cache

>>>>>>> upstream-releases


class FileSystem(object):
<<<<<<< HEAD
    def __init__(self, root, url_base, cache_path, manifest_path=None, rebuild=False):
        from gitignore import gitignore
        self.root = os.path.abspath(root)
        self.url_base = url_base
        self.ignore_cache = None
        self.mtime_cache = None
        if cache_path is not None:
            if manifest_path is not None:
                self.mtime_cache = MtimeCache(cache_path, root, manifest_path, rebuild)
            if gitignore.has_ignore(root):
                self.ignore_cache = GitIgnoreCache(cache_path, root, rebuild)
        self.path_filter = gitignore.PathFilter(self.root,
                                                extras=[".git/"],
                                                cache=self.ignore_cache)
||||||| merged common ancestors
    def __init__(self, root, url_base):
        self.root = root
        self.url_base = url_base
        from gitignore import gitignore
        self.path_filter = gitignore.PathFilter(self.root, extras=[".git/"])
=======
    def __init__(self, root, url_base, cache_path, manifest_path=None, rebuild=False):
        # type: (bytes, Text, Optional[bytes], Optional[bytes], bool) -> None
        self.root = os.path.abspath(root)
        self.url_base = url_base
        self.ignore_cache = None
        self.mtime_cache = None
        if cache_path is not None:
            if manifest_path is not None:
                self.mtime_cache = MtimeCache(cache_path, root, manifest_path, rebuild)
            if gitignore.has_ignore(root):
                self.ignore_cache = GitIgnoreCache(cache_path, root, rebuild)
        self.path_filter = gitignore.PathFilter(self.root,
                                                extras=[".git/"],
                                                cache=self.ignore_cache)
        git = GitHasher(root)
        if git.git is not None:
            self.hash_cache = git.hash_cache()
        else:
            self.hash_cache = {}
>>>>>>> upstream-releases

    def __iter__(self):
<<<<<<< HEAD
        mtime_cache = self.mtime_cache
        for dirpath, dirnames, filenames in self.path_filter(walk(self.root)):
            for filename, path_stat in filenames:
                path = os.path.join(dirpath, filename)
                if mtime_cache is None or mtime_cache.updated(path, path_stat):
                    yield SourceFile(self.root, path, self.url_base), True
                else:
                    yield path, False

    def dump_caches(self):
        for cache in [self.mtime_cache, self.ignore_cache]:
            if cache is not None:
                cache.dump()


class CacheFile(object):
    file_name = None

    def __init__(self, cache_root, tests_root, rebuild=False):
        self.tests_root = tests_root
        if not os.path.exists(cache_root):
            os.makedirs(cache_root)
        self.path = os.path.join(cache_root, self.file_name)
        self.modified = False
        self.data = self.load(rebuild)

    def dump(self):
        if not self.modified:
            return
        with open(self.path, 'w') as f:
            json.dump(self.data, f, indent=1)

    def load(self, rebuild=False):
        data = {}
        try:
            if not rebuild:
                with open(self.path, 'r') as f:
                    data = json.load(f)
                data = self.check_valid(data)
        except IOError:
            pass
        return data

    def check_valid(self, data):
        """Check if the cached data is valid and return an updated copy of the
        cache containing only data that can be used."""
        return data


class MtimeCache(CacheFile):
    file_name = "mtime.json"

    def __init__(self, cache_root, tests_root, manifest_path, rebuild=False):
        self.manifest_path = manifest_path
        super(MtimeCache, self).__init__(cache_root, tests_root, rebuild=False)

    def updated(self, rel_path, stat):
        """Return a boolean indicating whether the file changed since the cache was last updated.

        This implicitly updates the cache with the new mtime data."""
        mtime = stat.st_mtime
        if mtime != self.data.get(rel_path):
            self.modified = True
            self.data[rel_path] = mtime
            return True
        return False

    def check_valid(self, data):
        if data.get("/tests_root") != self.tests_root:
            self.modified = True
        else:
            if self.manifest_path is not None and os.path.exists(self.manifest_path):
                mtime = os.path.getmtime(self.manifest_path)
                if data.get("/manifest_path") != [self.manifest_path, mtime]:
                    self.modified = True
            else:
                self.modified = True
        if self.modified:
            data = {}
            data["/tests_root"] = self.tests_root
        return data

    def dump(self):
        if self.manifest_path is None:
            raise ValueError
        if not os.path.exists(self.manifest_path):
            return
        mtime = os.path.getmtime(self.manifest_path)
        self.data["/manifest_path"] = [self.manifest_path, mtime]
        self.data["/tests_root"] = self.tests_root
        super(MtimeCache, self).dump()


class GitIgnoreCache(CacheFile):
    file_name = "gitignore.json"

    def check_valid(self, data):
        ignore_path = os.path.join(self.tests_root, ".gitignore")
        mtime = os.path.getmtime(ignore_path)
        if data.get("/gitignore_file") != [ignore_path, mtime]:
            self.modified = True
            data = {}
            data["/gitignore_file"] = [ignore_path, mtime]
        return data

    def __contains__(self, key):
        return key in self.data

    def __getitem__(self, key):
        return self.data[key]

    def __setitem__(self, key, value):
        if self.data.get(key) != value:
            self.modified = True
            self.data[key] = value


def walk(root):
    """Re-implementation of os.walk. Returns an iterator over
    (dirpath, dirnames, filenames), with some semantic differences
    to os.walk.

    This has a similar interface to os.walk, with the important difference
    that instead of lists of filenames and directory names, it yields
    lists of tuples of the form [(name, stat)] where stat is the result of
    os.stat for the file. That allows reusing the same stat data in the
    caller. It also always returns the dirpath relative to the root, with
    the root iself being returned as the empty string.

    Unlike os.walk the implementation is not recursive."""

    listdir = os.listdir
    get_stat = os.stat
    listdir = os.listdir
    join = os.path.join
    is_dir = stat.S_ISDIR
    is_link = stat.S_ISLNK
    relpath = os.path.relpath

    root = os.path.abspath(root)
    stack = deque([(root, "")])

    while stack:
        dir_path, rel_path = stack.popleft()
        try:
            # Note that listdir and error are globals in this module due
            # to earlier import-*.
            names = listdir(dir_path)
        except OSError:
            continue

        dirs, non_dirs = [], []
        for name in names:
            path = join(dir_path, name)
            try:
                path_stat = get_stat(path)
            except OSError:
                continue
            if is_dir(path_stat.st_mode):
                dirs.append((name, path_stat))
            else:
                non_dirs.append((name, path_stat))

        yield rel_path, dirs, non_dirs
        for name, path_stat in dirs:
            new_path = join(dir_path, name)
            if not is_link(path_stat.st_mode):
                stack.append((new_path, relpath(new_path, root)))
||||||| merged common ancestors
        paths = self.get_paths()
        for path in paths:
            yield SourceFile(self.root, path, self.url_base)

    def get_paths(self):
        for dirpath, dirnames, filenames in os.walk(self.root):
            for filename in filenames:
                path = os.path.relpath(os.path.join(dirpath, filename), self.root)
                if self.path_filter(path):
                    yield path

            dirnames[:] = [item for item in dirnames if self.path_filter(
                           os.path.relpath(os.path.join(dirpath, item), self.root) + "/")]
=======
        # type: () -> Iterator[Tuple[Union[bytes, SourceFile], bool]]
        mtime_cache = self.mtime_cache
        for dirpath, dirnames, filenames in self.path_filter(walk(self.root)):
            for filename, path_stat in filenames:
                path = os.path.join(dirpath, filename)
                if mtime_cache is None or mtime_cache.updated(path, path_stat):
                    hash = self.hash_cache.get(path, None)
                    yield SourceFile(self.root, path, self.url_base, hash), True
                else:
                    yield path, False

    def dump_caches(self):
        # type: () -> None
        for cache in [self.mtime_cache, self.ignore_cache]:
            if cache is not None:
                cache.dump()


class CacheFile(with_metaclass(abc.ABCMeta)):
    def __init__(self, cache_root, tests_root, rebuild=False):
        # type: (bytes, bytes, bool) -> None
        self.tests_root = tests_root
        if not os.path.exists(cache_root):
            os.makedirs(cache_root)
        self.path = os.path.join(cache_root, self.file_name)
        self.modified = False
        self.data = self.load(rebuild)

    @abc.abstractproperty
    def file_name(self):
        # type: () -> bytes
        pass

    def dump(self):
        # type: () -> None
        if not self.modified:
            return
        with open(self.path, 'w') as f:
            json.dump(self.data, f, indent=1)

    def load(self, rebuild=False):
        # type: (bool) -> Dict[Any, Any]
        data = {}  # type: Dict[Any, Any]
        try:
            if not rebuild:
                with open(self.path, 'r') as f:
                    try:
                        data = json.load(f)
                    except ValueError:
                        pass
                data = self.check_valid(data)
        except IOError:
            pass
        return data

    def check_valid(self, data):
        # type: (Dict[Any, Any]) -> Dict[Any, Any]
        """Check if the cached data is valid and return an updated copy of the
        cache containing only data that can be used."""
        return data


class MtimeCache(CacheFile):
    file_name = b"mtime.json"

    def __init__(self, cache_root, tests_root, manifest_path, rebuild=False):
        # type: (bytes, bytes, bytes, bool) -> None
        self.manifest_path = manifest_path
        super(MtimeCache, self).__init__(cache_root, tests_root, rebuild)

    def updated(self, rel_path, stat):
        # type: (bytes, stat_result) -> bool
        """Return a boolean indicating whether the file changed since the cache was last updated.

        This implicitly updates the cache with the new mtime data."""
        mtime = stat.st_mtime
        if mtime != self.data.get(rel_path):
            self.modified = True
            self.data[rel_path] = mtime
            return True
        return False

    def check_valid(self, data):
        # type: (Dict[Any, Any]) -> Dict[Any, Any]
        if data.get("/tests_root") != self.tests_root:
            self.modified = True
        else:
            if self.manifest_path is not None and os.path.exists(self.manifest_path):
                mtime = os.path.getmtime(self.manifest_path)
                if data.get("/manifest_path") != [self.manifest_path, mtime]:
                    self.modified = True
            else:
                self.modified = True
        if self.modified:
            data = {}
            data["/tests_root"] = self.tests_root
        return data

    def dump(self):
        # type: () -> None
        if self.manifest_path is None:
            raise ValueError
        if not os.path.exists(self.manifest_path):
            return
        mtime = os.path.getmtime(self.manifest_path)
        self.data["/manifest_path"] = [self.manifest_path, mtime]
        self.data["/tests_root"] = self.tests_root
        super(MtimeCache, self).dump()


class GitIgnoreCache(CacheFile, MutableMapping):  # type: ignore
    file_name = b"gitignore.json"

    def check_valid(self, data):
        # type: (Dict[Any, Any]) -> Dict[Any, Any]
        ignore_path = os.path.join(self.tests_root, b".gitignore")
        mtime = os.path.getmtime(ignore_path)
        if data.get(b"/gitignore_file") != [ignore_path, mtime]:
            self.modified = True
            data = {}
            data[b"/gitignore_file"] = [ignore_path, mtime]
        return data

    def __contains__(self, key):
        # type: (Any) -> bool
        return key in self.data

    def __getitem__(self, key):
        # type: (bytes) -> bool
        v = self.data[key]
        assert isinstance(v, bool)
        return v

    def __setitem__(self, key, value):
        # type: (bytes, bool) -> None
        if self.data.get(key) != value:
            self.modified = True
            self.data[key] = value

    def __delitem__(self, key):
        # type: (bytes) -> None
        del self.data[key]

    def __iter__(self):
        # type: () -> Iterator[bytes]
        return iter(self.data)

    def __len__(self):
        # type: () -> int
        return len(self.data)


def walk(root):
    # type: (bytes) -> Iterable[Tuple[bytes, List[Tuple[bytes, stat_result]], List[Tuple[bytes, stat_result]]]]
    """Re-implementation of os.walk. Returns an iterator over
    (dirpath, dirnames, filenames), with some semantic differences
    to os.walk.

    This has a similar interface to os.walk, with the important difference
    that instead of lists of filenames and directory names, it yields
    lists of tuples of the form [(name, stat)] where stat is the result of
    os.stat for the file. That allows reusing the same stat data in the
    caller. It also always returns the dirpath relative to the root, with
    the root iself being returned as the empty string.

    Unlike os.walk the implementation is not recursive."""

    listdir = os.listdir
    get_stat = os.stat
    listdir = os.listdir
    join = os.path.join
    is_dir = stat.S_ISDIR
    is_link = stat.S_ISLNK
    relpath = os.path.relpath

    root = os.path.abspath(root)
    stack = deque([(root, b"")])

    while stack:
        dir_path, rel_path = stack.popleft()
        try:
            # Note that listdir and error are globals in this module due
            # to earlier import-*.
            names = listdir(dir_path)
        except OSError:
            continue

        dirs, non_dirs = [], []
        for name in names:
            path = join(dir_path, name)
            try:
                path_stat = get_stat(path)
            except OSError:
                continue
            if is_dir(path_stat.st_mode):
                dirs.append((name, path_stat))
            else:
                non_dirs.append((name, path_stat))

        yield rel_path, dirs, non_dirs
        for name, path_stat in dirs:
            new_path = join(dir_path, name)
            if not is_link(path_stat.st_mode):
                stack.append((new_path, relpath(new_path, root)))
>>>>>>> upstream-releases
