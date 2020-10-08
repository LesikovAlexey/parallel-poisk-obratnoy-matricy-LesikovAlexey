#!/usr/bin/env bash
clang-tidy --checks=-*,bugprone-*,performance-*,readability-*,misc-* --warnings-as-errors=* "$@" --
