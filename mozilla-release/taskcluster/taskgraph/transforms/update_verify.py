# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""
Transform the beetmover task into an actual task description.
"""

from __future__ import absolute_import, print_function, unicode_literals

from copy import deepcopy

from taskgraph.transforms.base import TransformSequence
<<<<<<< HEAD
from taskgraph.util.schema import resolve_keyed_by
from taskgraph.util.treeherder import add_suffix
||||||| merged common ancestors
from taskgraph.util.schema import resolve_keyed_by
from taskgraph.util.taskcluster import get_taskcluster_artifact_prefix
from taskgraph.util.treeherder import add_suffix
=======
from taskgraph.util.treeherder import add_suffix, inherit_treeherder_from_dep
>>>>>>> upstream-releases

transforms = TransformSequence()


@transforms.add
def add_command(config, tasks):
    config_tasks = {}
    for dep in config.kind_dependencies_tasks:
        if 'update-verify-config' in dep.kind or 'update-verify-next-config' in dep.kind:
            config_tasks[dep.name] = dep

    for task in tasks:
        config_task = config_tasks[task['name']]
        total_chunks = task["extra"]["chunks"]
        task['worker'].setdefault('env', {}).update(
            CHANNEL=config_task.task['extra']['channel'],
        )
        task.setdefault('fetches', {})[config_task.label] = [
            "update-verify.cfg",
        ]
        task['treeherder'] = inherit_treeherder_from_dep(task, config_task)

        for this_chunk in range(1, total_chunks+1):
            chunked = deepcopy(task)
            chunked["treeherder"]["symbol"] = add_suffix(
                chunked["treeherder"]["symbol"], this_chunk)
            chunked["label"] = "release-update-verify-{}-{}/{}".format(
                chunked["name"], this_chunk, total_chunks
            )
            if not chunked["worker"].get("env"):
                chunked["worker"]["env"] = {}
<<<<<<< HEAD
            chunked["run"] = {
                'using': 'run-task',
                'command': 'cd /builds/worker/checkouts/gecko && '
                           'tools/update-verify/scripts/chunked-verify.sh '
                           '{} {}'.format(
                               total_chunks,
                               this_chunk,
                           ),
                'sparse-profile': 'update-verify',
            }
            for thing in ("CHANNEL", "VERIFY_CONFIG", "BUILD_TOOLS_REPO"):
                thing = "worker.env.{}".format(thing)
                resolve_keyed_by(
                    chunked, thing, thing,
                    **{
                        'project': config.params['project'],
                        'release-type': config.params['release_type'],
                    }
                )

            for upstream in chunked.get("dependencies", {}).keys():
                if 'update-verify-config' in upstream:
                    chunked.setdefault('fetches', {})[upstream] = [
                        "update-verify.cfg",
                    ]
                    break
            else:
                raise Exception("Couldn't find upate verify config")

||||||| merged common ancestors
            chunked["worker"]["command"] = [
                "/bin/bash",
                "-c",
                "hg clone $BUILD_TOOLS_REPO tools && " +
                "tools/scripts/release/updates/chunked-verify.sh " +
                "UNUSED UNUSED {} {}".format(
                    total_chunks,
                    this_chunk,
                )
            ]
            for thing in ("CHANNEL", "VERIFY_CONFIG", "BUILD_TOOLS_REPO"):
                thing = "worker.env.{}".format(thing)
                resolve_keyed_by(
                    chunked, thing, thing,
                    **{
                        'project': config.params['project'],
                        'release-type': config.params['release_type'],
                    }
                )

            update_verify_config = None
            for upstream in chunked.get("dependencies", {}).keys():
                if 'update-verify-config' in upstream:
                    update_verify_config = "{}update-verify.cfg".format(
                        get_taskcluster_artifact_prefix(task, "<{}>".format(upstream))
                    )
            if not update_verify_config:
                raise Exception("Couldn't find upate verify config")

            chunked["worker"]["env"]["TASKCLUSTER_VERIFY_CONFIG"] = {
                "task-reference": update_verify_config
            }

=======
            chunked["run"] = {
                'using': 'run-task',
                'command': 'cd /builds/worker/checkouts/gecko && '
                           'tools/update-verify/scripts/chunked-verify.sh '
                           '{} {}'.format(
                               total_chunks,
                               this_chunk,
                           ),
                'sparse-profile': 'update-verify',
            }

>>>>>>> upstream-releases
            yield chunked
