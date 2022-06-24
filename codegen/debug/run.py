import sys
import os
import yaml

from codegen.common import parse_simple_flags, open_template


def render_template_to_file(template_filename, kwargs, f):
    print(open_template(template_filename).render(**kwargs), file=f)


def read_config(p):
    with open(p, "r") as f:
        return yaml.safe_load(f)


def main():
    jp = os.path.join
    kwargs = parse_simple_flags(sys.argv)
    my_dir = os.path.dirname(__file__)

    config = read_config(jp(my_dir, "config.yaml"))
    for k, v in config.items():
        kwargs[k] = v.format(**kwargs)

    target_dir = jp(kwargs["codegen_build_dir"], "debug")

    os.makedirs(target_dir, exist_ok=True)
    with open(jp(target_dir, "debug.hpp"), "w") as f:
        render_template_to_file(jp(my_dir, "debug.hpp.jinja"), kwargs, f)
    with open(jp(target_dir, "debug.cpp"), "w") as f:
        render_template_to_file(jp(my_dir, "debug.cpp.jinja"), kwargs, f)


if __name__ == "__main__":
    main()
