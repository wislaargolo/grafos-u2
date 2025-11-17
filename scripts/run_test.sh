#!/usr/bin/env bash
set -euo pipefail

# Script para compilar e executar um único teste em tests/
# Uso:
#   ./scripts/run_test.sh q7            # compila (make q7) e executa build/q7
#   ./scripts/run_test.sh q7 -- arg1    # passa argumentos para o executável

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$SCRIPT_DIR/.."
cd "$REPO_ROOT" || exit 1

SRC_DIR="tests"
BUILD_DIR="build"

usage() {
  echo "Usage: $0 test_name [-- args...]"
  echo "Ex: $0 q7 -- arg1 arg2"
  exit 1
}

if [ "$#" -lt 1 ]; then
  usage
fi

name="$1"
shift || true
src="$SRC_DIR/$name.cpp"
bin="$BUILD_DIR/$name"
if [ ! -f "$src" ]; then
  echo "Arquivo de teste '$src' não encontrado." >&2
  exit 1
fi
echo "Compilando $name..."
make "$name"
echo "Executando $bin..."
"$bin" "$@"
exit $?
