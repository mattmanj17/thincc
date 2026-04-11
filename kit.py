import argparse
import subprocess
import sys
import atexit
from dataclasses import dataclass
from pathlib import Path
from typing import Callable

@dataclass(frozen=True)
class Option:
    short: str
    long: str
    info: str
    fn: Callable[[], None]

def require_file(path):
    if not path.is_file():
        sys.exit(f"File not found: {path}")

def run_checked(cmd):
    def show_cmd():
        print(" ".join(cmd), file=sys.stderr)
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
        )
    except Exception as e:
        show_cmd()
        sys.exit(f"failed to run {cmd[0]}: {type(e).__name__}: {e}")
    if result.returncode != 0:
        show_cmd()
        if result.stderr:
            print(result.stderr, file=sys.stderr, end="")
        if result.stdout:
            print(result.stdout, file=sys.stderr, end="")
        sys.exit(f"{cmd[0]} failed with exit code {result.returncode}")
    return result

NAME = "thincc"
SRC = Path(NAME + ".c")
OBJ = Path(NAME + ".obj")
EXE = Path(NAME + ".exe")

def preprocess():
    require_file(SRC)
    result = run_checked(
        [
            "cl",
            "/nologo",
            "/EP",
            "/TC",
            str(SRC),
        ]
    )
    text = "\n".join(
        line for line in result.stdout.splitlines()
        if line.strip() != ""
    )
    print(text)

def build():

    # clean
    OBJ.unlink(missing_ok=True)
    EXE.unlink(missing_ok=True)
    
    # compile
    require_file(SRC)
    run_checked(
        [
            "cl", "/nologo",
            "/c", "/TC", 
            "/GS-", "/Zl", "/Za",
            "/Wall", "/WX",
            str(SRC),
        ]
    )
    
    # link
    require_file(OBJ)
    run_checked(
        [
            "link", "/nologo",
            "/NODEFAULTLIB",
            "/SUBSYSTEM:CONSOLE",
            "/ENTRY:entry",
            "/WX",
            str(OBJ),
            "kernel32.lib",
        ]
    )
    OBJ.unlink(missing_ok=True)

    # got exe?
    require_file(EXE)

def test():
    build()
    run_checked([str(EXE)])

def at_exit():
    OBJ.unlink(missing_ok=True)
    EXE.unlink(missing_ok=True)

def main():
    atexit.register(at_exit)

    arg1_name = "command"
    arg1_ref = "<" + arg1_name + ">"

    parser = argparse.ArgumentParser(
        description=f"Driver script for {NAME}.",
        usage=f"%(prog)s [-h] {arg1_ref}"
    )

    options = [
        Option(
            "h", "help", "print help", 
            parser.print_help
        ),
        Option(
            "pp", "preprocess", "dump preprocessed source to stdout", 
            preprocess
        ),
        Option(
            "b", "build", "build the executable", 
            build
        ),
        Option(
            "t", "test", "build and test the executable", 
            test
        ),
    ]

    valid = (
        {op.short for op in options} | 
        {op.long for op in options} |
        {"-h", "--help"}
    )

    if len(sys.argv) > 1 and sys.argv[1] not in valid:
        parser.print_usage(sys.stderr)
        sys.exit(
            f"{parser.prog}: error: unknown {arg1_name} '{sys.argv[1]}'\n"
            f"Try '{parser.prog} -h' for a list of commands.\n"
        )
    
    subparsers = parser.add_subparsers(
        dest=arg1_name, 
        required=True,
        metavar=arg1_ref
    )
    for op in options:
        subparsers.add_parser(
            op.long,
            aliases=[op.short],
            help=op.info
        ).set_defaults(
            fn=op.fn
        )

    args = parser.parse_args()
    args.fn()

if __name__ == "__main__":
    main()