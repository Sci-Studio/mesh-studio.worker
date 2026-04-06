#!/usr/bin/env bash
set -euo pipefail

ORIGINAL_REPO_URL="${REPO_URL:-https://github.com/Sci-Studio/mesh-studio.api.git}"
REPO_URL="$ORIGINAL_REPO_URL"
DEST_DIR="${DEST_DIR:-.cache/mesh-studio.api}"
SPARSE_PATH="${SPARSE_PATH:-openapi}"

if [ -n "${GITHUB_TOKEN:-}" ] && [[ "$REPO_URL" =~ ^https://github\.com/ ]]; then
  REPO_URL="${REPO_URL/https:\/\/github.com\//https:\/\/x-access-token:${GITHUB_TOKEN}@github.com\/}"
fi

DEFAULT_REF="$(
  git ls-remote --symref "$REPO_URL" HEAD \
    | awk '/^ref:/ { sub("refs/heads/", "", $2); print $2; exit }'
)"
if [ -z "$DEFAULT_REF" ]; then
  echo "Failed to resolve default branch for repository: $ORIGINAL_REPO_URL" >&2
  echo "If this is a private repository, set GITHUB_TOKEN or use an SSH REPO_URL." >&2
  exit 1
fi

REF="${REF:-$DEFAULT_REF}"

mkdir -p "$(dirname "$DEST_DIR")"

if [ ! -d "$DEST_DIR/.git" ]; then
  git clone --filter=blob:none --no-checkout "$REPO_URL" "$DEST_DIR"
fi

git -C "$DEST_DIR" sparse-checkout init --no-cone
git -C "$DEST_DIR" sparse-checkout set "/$SPARSE_PATH/"
git -C "$DEST_DIR" fetch --depth 1 origin "$REF"
git -C "$DEST_DIR" checkout --detach FETCH_HEAD

echo "Fetched '$SPARSE_PATH' from '$ORIGINAL_REPO_URL' (ref: $REF) into '$DEST_DIR'."