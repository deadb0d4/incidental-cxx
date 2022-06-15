import os


def walk_dir(dirname):
    for root, _, files in os.walk(dirname):
        for filename in files:
            path = os.path.join(root, filename)
            yield path[len(dirname):]
