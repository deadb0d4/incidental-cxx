import os
import sys

from codegen.common import (
    open_yaml,
    open_template,
    to_snake_case,
    parse_simple_flags,
    expect,
)


def get_option_desc(name: str, desc: dict) -> dict:
    expect("type" in desc, "type should be specified")
    typestr = desc["type"]
    default = desc.get("default", None)
    if default is None and typestr == "bool":
        default = False
    required = True
    if name[-1] == "?":
        name = name[:-1]
        required = False
    if default is not None:
        expect(required, "default value => required")
    return {
        "name": name,
        "default": default,
        "required": required,
        "type": typestr,
        "full": f"--{desc.get('full', name)}",
        "char": f"-{desc['char']}" if "char" in desc else None,
    }


class Option:
    def __init__(self, desc: dict):
        self.__dict__.update(desc)

    def cpp_name(self):
        return self.name

    def typename(self):
        return self.type

    def check_presense(self):
        if self.default is not None:
            return False
        return self.required

    def cpp_typename(self):
        t = self.typename()
        if t == "str":
            t = "std::string_view"
        elif t == ["str"]:
            t = "std::vector<std::string_view>"
        elif t == ["int"]:
            t = "std::vector<int>"
        if self.required:
            return t
        return f"std::optional<{t}>"

    def cpp_default_value(self):
        if self.default is None:
            return ""
        if self.typename() == "bool":
            return 1 if self.default else 0
        if self.typename() == "int":
            return self.default
        if self.typename() == "str":
            return f'"{self.default}"'
        if self.typename() == ["str"]:
            return ", ".join(map(lambda x: f'"{x}"', self.default))
        if self.typename() == ["int"]:
            return ", ".join(map(str, self.default))


class Parser:
    def __init__(self, name):
        self.name = name
        self.options = []

    def add_option(self, option):
        self.options.append(option)

    def cpp_include_name(self):
        return f"argparse/parsers/{to_snake_case(self.name)}.hpp"


# TODO: change interface to argc/argv
def main():
    jp = os.path.join

    my_dir = os.path.dirname(__file__)
    kwargs = parse_simple_flags(sys.argv)

    target_dir = jp(kwargs["codegen_build_dir"], "argparse", "parsers")
    os.makedirs(target_dir, exist_ok=True)

    parsers_dir = jp(my_dir, "parsers")
    for e in os.listdir(parsers_dir):
        if not e.endswith(".yaml") and not e.endswith(".yml"):
            continue

        got = open_yaml(jp(parsers_dir, e))
        for parser_name, parser_desc in got.items():
            parser = Parser(parser_name)
            for option_name, option_desc in parser_desc["options"].items():
                parser.add_option(Option(get_option_desc(option_name, option_desc)))

            header = open_template(jp(my_dir, "templates/parser.hpp.jinja"))
            with open(jp(target_dir, f"{to_snake_case(parser.name)}.hpp"), "w") as f:
                print(header.render(parser=parser), file=f)
            source = open_template(jp(my_dir, "templates/parser.cpp.jinja"))
            with open(jp(target_dir, f"{to_snake_case(parser.name)}.cpp"), "w") as f:
                print(source.render(parser=parser), file=f)


if __name__ == "__main__":
    main()
