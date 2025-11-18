#!/usr/bin/env bash
set -euo pipefail

# Script: compila e executa testes.
# - Sem argumentos: compila tudo (make -B) e executa todos os binários em build/
# - Com 1 argumento: trata-se do nome do teste (com ou sem .cpp) — compila e executa só ele

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$SCRIPT_DIR/.."
cd "$REPO_ROOT" || exit 1

BUILD_DIR="build"
SRC_DIR="tests"

usage() {
  echo "Usage: $0 [test_name]"
  echo "If test_name is provided (optionally ending with .cpp) the script will compile and run only that test."
  echo "If omitted, the script compiles all targets and runs all executables in '$BUILD_DIR'."
  exit 1
}

if [ "$#" -gt 1 ]; then
  usage
fi

if [ "$#" -eq 1 ]; then
  # Single test mode
  name="$1"
  # Allow passing either 'q7' or 'q7.cpp'
  name="${name%.cpp}"
  src="$SRC_DIR/$name.cpp"
  bin="$BUILD_DIR/$name"

  if [ ! -f "$src" ]; then
    echo "Arquivo de teste '$src' não encontrado." >&2
    exit 1
  fi

  echo "Compilando (forçado) o alvo '$name'..."
  make -B "$name"

  if [ ! -f "$bin" ]; then
    echo "Binário '$bin' não encontrado após compilação." >&2
    exit 1
  fi
  if [ ! -x "$bin" ]; then
    echo "Binário '$bin' não é executável." >&2
    exit 1
  fi

  echo "Executando $bin..."
  "$bin"
  exit $?
else
  # No-arg mode: compile all and run all executables in build/
  echo "Compilando (forçado) todos os alvos..."
  make -B

  echo "Procurando binários em '$BUILD_DIR'..."
  shopt -s nullglob
  bins=("$BUILD_DIR"/*)
  if [ "${#bins[@]}" -eq 0 ]; then
    echo "Nenhum arquivo encontrado em '$BUILD_DIR'." >&2
    exit 1
  fi

  for bin in "${bins[@]}"; do
    if [ -f "$bin" ] && [ -x "$bin" ]; then
      echo
      echo "==== Executando: $bin ===="
      "$bin"
    else
      echo "Pulando '$bin' (não é executável regular)."
    fi
  done

  exit 0
fi
