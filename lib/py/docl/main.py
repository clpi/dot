import click
from typing import List, Tuple, Optional

# @click.command()
# @click.option("--count", default=1, help="Number of greetings.")
# @click.option("--name", prompt="Your name",
#               help="The person to greet.")
@click.group()
@click.version_option(version='1.0.0')
def greet():
    pass


@greet.command()
@click.argument('name')  # add the name argument
@click.option('--excited', is_flag=True)
@click.option('--greeting', default='Hello', help='word to use for the greeting')
def hello(**kwargs):
    out = f"{kwargs['greeting']} {kwargs['name']}"
    if kwargs['excited']: print(f"{out}!!")
    else:
        print(f"{out} wut du...")


@greet.command()
@click.argument('name')  # add the name argument
@click.option('--verbose', default=True)
def goodbye(**kwargs):
    print('Goodbye, {0}!'.format(kwargs['name']))
    pass

# -*- encoding: utf-8 -*-
# kb v0.1.6
# A knowledge base organizer
# Copyright © 2020, gnc.
# See /LICENSE for licensing information.

"""
kb filesystem module

:Copyright: © 2020, gnc.
:License: GPLv3 (see /LICENSE).
"""

__all__ = ()

import os
import re
import shutil
import tempfile
from pathlib import Path
from typing import List


def list_files(directory: str) -> List[str]:
    """
    List all files contained in a directory recursively,
    similarly to the "find" UNIX command

    Args:
    directory       - a string representing the target directory

    Returns:
    A list of strings representing the path of files contained
    in the directory
    """

    # Get kbdir path
    dirpath = Path(directory)

    # Get list of files in the form: file1, dir1/file2, ...
    files = [str(f.relative_to(dirpath))
             for f in dirpath.rglob("*") if f.is_file()]
    return files


def list_dirs(directory: str) -> List[str]:
    """
    List all sub-directories contained in a directory

    Args:
    directory       - a string representing the path to a directory

    Returns:
    A list of strings representing the path of directories contained
    in the provided directory
    """
    # Get kbdir path
    dirpath = Path(directory)

    # Get list of files in the form: file1, dir1/file2, ...
    files = [str(f.relative_to(dirpath))
             for f in dirpath.rglob("*") if f.is_dir()]
    return files


def touch_file(filename: str):
    """
    Creates a new empty file, in the style of the UNIX
    touch program.

    Arguments:

    filename    - a path to a filename
    """
    Path(filename).touch()


def get_basename(filename: str) -> str:
    """
    Get basename for a file

    Arguments:
    filename    - a path to a filename

    Returns:
    The basename of the provided file
    """
    return Path(filename).name


def copy_file(source: str, dest: str) -> None:
    """
    Copies a file to the provided destination

    Arguments:
    source    - the path to the source file to copy
    dest      - the destination path of the copy
    """
    shutil.copy2(Path(source), Path(dest))


def remove_file(filename: str) -> None:
    """
    Removes a file from the filesystem

    Arguments:
    filename    - the file to remove from the kb directory
    """
    try:
        Path(filename).unlink()
    except FileNotFoundError:
        pass


def remove_directory(directory: str) -> None:
    """
    Removes a directory from the filesystem

    Arguments:
    directory    - the directory to remove from the kb system
    """
    shutil.rmtree(directory)


def create_directory(directory: str) -> None:
    """
    Create a directory if it does not exist.

    Arguments:
    directory    - the directory path to be created
    """
    os.makedirs(Path(directory), exist_ok=True)


def is_directory(path: str) -> bool:
    """
    Checks if the provided path is a directory.

    Arguments:
    path        - the path to check

    Returns:
    A boolean, if true, the path corresponds to a directory
    """
    return os.path.isdir(path)


def is_file(path: str) -> bool:
    """
    Checks if the provided path corresponds to a regular file.

    Arguments:
    path        - the path to check

    Returns:
    A boolean, if true, the path corresponds to a regular file
    """
    return os.path.isfile(path)


def count_files(directory: str) -> int:
    """
    Count the number of files in a directory

    Arguments:
    directory    - the directory where to count files

    Returns:
    the number of files contained in the directory
    """
    return len(list(Path(directory).iterdir()))


def move_file(source: str, dest: str) -> None:
    """
    Moves a file to the provided destination

    Arguments:
    source    - the path to the source file to copy
    dest      - the destination path of the copy
    """
    shutil.move(source, dest)


def get_temp_filepath() -> str:
    """
    Generates a temporary file path.

    Returns:
    A boolean, True if the file is of type text.
    """
    tmpfilename = None
    while tmpfilename is None:
        random_tmp_path = str(Path(tempfile.gettempdir(),
                                   os.urandom(24).hex()))
        if not os.path.isfile(random_tmp_path):
            tmpfilename = random_tmp_path
    return tmpfilename


def is_text_file(filename: str) -> bool:
    """
    Determines if a file is textual (that can be
    nicely viewed in a text editor) or belonging
    to other types.

    Arguments:
    filename        - the file name/path to check

    Returns:
    A boolean, True if the file is of type text.
    """
    txt_extensions = ("", ".conf", ".ini", ".txt",
                      ".md", ".rst", ".ascii", ".org", ".tex")

    file_ext = os.path.splitext(filename)[1]

    return file_ext in txt_extensions


def get_filename_parts_wo_prefix(
        filename: str,
        prefix_to_remove: str) -> Optional[Tuple[str, ...]]:
    """
    Get filename parts without the provided prefix.
    E.g., if the filename is "/path/to/data/dir1/file2.txt"
    and the prefix to remove is "/path/to/data" then the
    returned will be a tuple containing ("dir1","file2.txt")

    Arguments:
    filename          - a string or path provided by pathlib
    prefix_to_remove  - a string or path provided by pathlib that
                        will be removed from the filename

    Returns:
    The provided filename without the provided prefix
    """
    prefix_path = Path(prefix_to_remove)
    file_path = Path(filename)

    try:
        return file_path.relative_to(prefix_path).parts
    except ValueError:
        file_path.parts


def grep_in_files(
        filelist: str,
        regex: str,
        case_insensitive: bool = False
) -> List[str]:
    """
    grep -nr 'regex' file1 file2 ...
    Arguments:
    filelist     - the file list where to match the regex
    regex        - the regex to match
    Returns
    A list of tuples containing (filepath, line number, matched string)
    """
    if case_insensitive:
        pattern = re.compile(regex, re.IGNORECASE)
    else:
        pattern = re.compile(regex)

    matches = list()
    for fname in filelist:
        try:
            with open(fname) as handle:
                for i, line in enumerate(handle):
                    match = pattern.search(line)
                    linenumber = i + 1
                    if match:
                        matches.append((fname, linenumber, line.strip()))
        except UnicodeDecodeError:
            continue
    return matches


def grep_in_files_uniq(
        filelist: str,
        regex: str,
        case_insensitive=False
) -> List[str]:
    """
    grep -lnr 'regex' file1 file2 ...

    Arguments:
    filelist     - the file list where to match the regex
    regex        - the regex to match
    Returns
    A list of file paths matching the regex.
    """
    if case_insensitive:
        pattern = re.compile(regex, re.IGNORECASE)
    else:
        pattern = re.compile(regex)

    matches = list()
    for fname in filelist:
        try:
            with open(fname) as handle:
                for line in handle:
                    match = pattern.search(line)
                    if match:
                        matches.append(fname)
        except UnicodeDecodeError:
            continue
    return list(set(matches))

if __name__ == '__main__':
    greet()

