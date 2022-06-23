import os


def walk_dir(dirname):
    for root, _, files in os.walk(dirname):
        for filename in files:
            path = os.path.join(root, filename)
            yield path[len(dirname) :]


def list_dir(dirname):
    for e in os.listdir(dirname):
        yield os.path.join(dirname, e)
