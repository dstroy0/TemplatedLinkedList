#!/usr/bin/env bash
# run_tests.sh - Build and run the TemplatedLinkedList test suite
# Usage: bash ./run_tests.sh
#        bash ./run_tests.sh --clean    (remove compiled binary before building)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_SRC="$SCRIPT_DIR/test_main.cpp"
LIB_SRC="$SCRIPT_DIR/../src"
OUT_BIN="$SCRIPT_DIR/test_bin"

CLEAN=0
if [[ "${1:-}" == "--clean" ]]; then
    CLEAN=1
fi

# ─── Locate g++ ──────────────────────────────────────────────────────────────
GXX=""
for candidate in g++ g++-14 g++-13 g++-12 g++-11 g++-10; do
    if command -v "$candidate" >/dev/null 2>&1; then
        GXX="$candidate"
        break
    fi
done

if [[ -z "$GXX" ]]; then
    echo "Error: g++ not found." >&2
    echo "Install build-essential (Debian/Ubuntu: sudo apt install build-essential)" >&2
    echo "or Xcode Command Line Tools (macOS: xcode-select --install) and retry." >&2
    exit 1
fi

echo "Compiler : $(command -v "$GXX")"
echo "Version  : $("$GXX" --version | head -1)"
echo ""

# ─── Optional clean ──────────────────────────────────────────────────────────
if [[ $CLEAN -eq 1 && -f "$OUT_BIN" ]]; then
    rm -f "$OUT_BIN"
    echo "Removed existing binary."
fi

# ─── Compile ─────────────────────────────────────────────────────────────────
echo "Compiling $TEST_SRC ..."

if ! "$GXX" \
        -std=c++11 -Wall -Wextra -Wpedantic \
        -I"$LIB_SRC" \
        -I"$SCRIPT_DIR" \
        -o "$OUT_BIN" \
        "$TEST_SRC"; then
    echo ""
    echo "Error: compilation failed." >&2
    exit 1
fi

echo "Compilation succeeded."
echo ""

# ─── Run ─────────────────────────────────────────────────────────────────────
echo "Running tests ..."
echo ""

"$OUT_BIN"
EXIT_CODE=$?

# ─── Cleanup ─────────────────────────────────────────────────────────────────
rm -f "$OUT_BIN"

echo ""
if [[ $EXIT_CODE -eq 0 ]]; then
    echo "Result: PASSED"
else
    echo "Result: FAILED (exit $EXIT_CODE)" >&2
fi

exit $EXIT_CODE
