import jinja2


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
