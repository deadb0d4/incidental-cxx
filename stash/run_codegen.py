import os
import sys
import copy
import subprocess
import argparse

from walk import list_dir
from colorize import error_style, warn_style


def prep_parser():
    parser = argparse.ArgumentParser(description="Codegen runner")
    parser.add_argument(
        "--codegen_build_dir", help="Path to the codegen build directory", required=True
    )
    parser.add_argument("--build_type", help="cmake build type", required=True)
    return parser.parse_args()


def prep_simple_args(parser):
    args = vars(parser)
    return sum([[k, v] for k, v in args.items()], [])


def run_cmd_list(p, simple_args):
    args = ["python3", "-m", None] + simple_args
    args[2] = str(p).replace(os.path.sep, ".")
    return args


def main():
    simple_args = prep_simple_args(prep_parser())
    for e in list_dir("codegen/"):
        if os.path.isfile(e) or "__" in e:
            continue
        print(warn_style(f"> Running {e}"))
        fp = subprocess.run(run_cmd_list(os.path.join(e, "run"), simple_args))
        if fp.returncode != 0:
            print(error_style(f"Error in {e}"))
            quit()
    print(warn_style("> Codegen finished"))


if __name__ == "__main__":
    main()
