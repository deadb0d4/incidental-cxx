import os
import sys
import subprocess

from colorize import warn_style
from walk import walk_dir


def get_binary_name(filename):
    assert filename[0] == "/"
    filename = filename.rsplit(".", 1)[0]
    filename = filename.replace("/", "_")
    return f"benchmark{filename}"


def get_result_name(filename, results_dir):
    return os.path.join(results_dir, f"{filename}.json")


def main():
    bench_dir = sys.argv[1]
    build_dir = sys.argv[2]
    results_dir = build_dir
    for filename in walk_dir(bench_dir):
        if not filename.endswith(".cpp"):
            continue
        print(warn_style(f"> Running {bench_dir}{filename}\n"))
        bin_name = get_binary_name(filename)
        job = os.path.join(build_dir, bin_name)
        out_file = get_result_name(bin_name, results_dir)
        subprocess.run(
            [job, "--benchmark_out_format=json", f"--benchmark_out={out_file}"]
        )


if __name__ == "__main__":
    main()
