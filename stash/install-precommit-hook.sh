#!/bin/sh

set -e -x

cp ./stash/pre-commit-hook.sh .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
