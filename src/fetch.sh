#!/bin/bash

day="${1?}"
if ! [[ -f .cookie ]]; then
  # Download the cookie by:
  #   * Logging into the website
  #   * Retrieving the session cookie via chrome dev tools
  #   * Storing its value in a file called .cookie
  >&2 echo 'no cookie'
exit 1
fi
cookie="Cookie: $(cat .cookie)"
directory="$(printf 'puzzles/day%02d' "$day")"
file="$directory/$USER.input"
mkdir -p "$directory"
curl -H "$cookie" "https://adventofcode.com/2015/day/$day/input" > "$file"
touch "$directory/$USER.output"
