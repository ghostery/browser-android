try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup


setup(
    name="redo",
<<<<<<< HEAD
    version="2.0.2",
||||||| merged common ancestors
    version="1.6",
=======
    version="2.0.3",
>>>>>>> upstream-releases
    description="Utilities to retry Python callables.",
    author="Ben Hearsum",
    author_email="ben@hearsum.ca",
    packages=["redo"],
    entry_points={"console_scripts": ["retry = redo.cmd:main"]},
    url="https://github.com/bhearsum/redo",
)
