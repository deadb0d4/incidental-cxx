import jinja2
import yaml
import re


def parse_simple_flags(args):
    """
    Expect call to be
    <script_name> flag_name_1 flag_value_1 etc.
    """
    kwargs = dict()
    for i in range(1, len(args) - 1, 2):
        kwargs[args[i]] = args[i + 1]
    return kwargs


def open_template(path):
    template = []
    with open(path, "r") as f:
        for line in f:
            template.append(line[:-1])
    return jinja2.Template("\n".join(template))


def open_yaml(path):
    with open(path, "r") as f:
        return yaml.safe_load(f)


def to_snake_case(name):
    name = re.sub("(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub("([a-z0-9])([A-Z])", r"\1_\2", name).lower()


def to_camel_case(s):
    return "".join(map(lambda x: x.title(), s.split("_")))


def expect(ok: bool, message: str):
    if not ok:
        raise RuntimeError(message)
