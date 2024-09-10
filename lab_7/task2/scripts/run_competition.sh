#!/bin/bash

COMPETITION_EXEC="./competition"

function echo_ok {
  echo -e "\033[32m$1\033[0m"
}

function echo_info {
  echo -e "\033[95m$1\033[0m"
}

function echo_warn {
  echo -e "\033[33m$1\033[0m"
}

function echo_err {
  echo -e "\033[91m$1\033[0m"
}

function check_and_run_exec {
  if test -f "$1"; then
    echo_ok "Executable file '$1' found"
    echo_ok "Running..."
    $1 $2
  else
    echo_err "Executable file '$1' not found"
    exit 1
  fi
}

cd ../competition || exit
echo_info "\n[COMPETITION]"
echo_info "\nProject 'COMPETITION' files"
ls
echo_info "\nEnter path to file"
read -r filename
echo_info "\nStarting './competition'"
check_and_run_exec "$COMPETITION_EXEC" "$filename"