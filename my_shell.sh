set -e

(
  cmake -B local_build -S .
  cmake --build ./local_build
)

exec $(dirname "$0")/local_build/shell "$@"