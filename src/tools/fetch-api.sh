#!/usr/bin/env bash
# Sparse-checkout selected directories from mesh-studio.api (OpenAPI specs, Prisma schema, etc.).
#
# Default paths: openapi prisma
# Override with space-separated SPARSE_PATHS, e.g.:
#   SPARSE_PATHS="openapi prisma packages/db/prisma" ./src/tools/fetch-api.sh
#
# Authentication (pick one):
#   - GITHUB_TOKEN or GH_TOKEN — fine-grained or classic PAT with repo read
#   - GitHub CLI: run `gh auth login` (uses `gh auth git-credential` for HTTPS)
#   - SSH: REPO_URL=git@github.com:OWNER/REPO.git (uses your SSH key)
set -euo pipefail

ORIGINAL_REPO_URL="${REPO_URL:-https://github.com/Sci-Studio/mesh-studio.api.git}"
REPO_URL="$ORIGINAL_REPO_URL"
DEST_DIR="${DEST_DIR:-.cache/mesh-studio.api}"
# Space-separated directory paths relative to repo root (no leading/trailing slashes).
# Prefer SPARSE_PATHS; SPARSE_PATH is legacy (single path only).
if [ -n "${SPARSE_PATHS:-}" ]; then
  :
elif [ -n "${SPARSE_PATH:-}" ]; then
  SPARSE_PATHS="$SPARSE_PATH"
else
  SPARSE_PATHS="openapi prisma"
fi

# Extra `git -c ...` args for HTTPS when using GitHub CLI (no token in URL).
GIT_AUTH=()
TOKEN="${GITHUB_TOKEN:-${GH_TOKEN:-}}"

if [ -n "$TOKEN" ] && [[ "$REPO_URL" =~ ^https://github\.com/ ]]; then
  REPO_URL="${REPO_URL/https:\/\/github.com\//https:\/\/x-access-token:${TOKEN}@github.com\/}"
elif [[ "$REPO_URL" =~ ^https://github\.com/ ]]; then
  if command -v gh >/dev/null 2>&1 && gh auth status -h github.com >/dev/null 2>&1; then
    GIT_AUTH=(-c "credential.helper=!gh auth git-credential")
  fi
fi

auth_hint() {
  echo "" >&2
  echo "Private repo or auth required. Do one of:" >&2
  echo "  export GITHUB_TOKEN=...   # or GH_TOKEN (repo read access)" >&2
  echo "  gh auth login -h github.com" >&2
  echo "  REPO_URL=git@github.com:Sci-Studio/mesh-studio.api.git $0 (SSH key)" >&2
}

LS_REMOTE_OUT=""
if ! LS_REMOTE_OUT=$(git "${GIT_AUTH[@]}" ls-remote --symref "$REPO_URL" HEAD 2>&1); then
  echo "Failed to read remote refs for: $ORIGINAL_REPO_URL" >&2
  echo "$LS_REMOTE_OUT" >&2
  auth_hint
  exit 1
fi

DEFAULT_REF=$(echo "$LS_REMOTE_OUT" | awk '/^ref:/ { sub("refs/heads/", "", $2); print $2; exit }')
if [ -z "$DEFAULT_REF" ]; then
  echo "Could not determine default branch for: $ORIGINAL_REPO_URL" >&2
  echo "$LS_REMOTE_OUT" >&2
  auth_hint
  exit 1
fi

REF="${REF:-$DEFAULT_REF}"

mkdir -p "$(dirname "$DEST_DIR")"

if [ ! -d "$DEST_DIR/.git" ]; then
  if ! git "${GIT_AUTH[@]}" clone --filter=blob:none --no-checkout "$REPO_URL" "$DEST_DIR"; then
    echo "git clone failed." >&2
    auth_hint
    exit 1
  fi
fi

read -r -a PATHS_ARR <<< "$SPARSE_PATHS"
if [ "${#PATHS_ARR[@]}" -eq 0 ]; then
  echo "SPARSE_PATHS is empty." >&2
  exit 1
fi

SPARSE_ARGS=()
for p in "${PATHS_ARR[@]}"; do
  p="${p#/}"
  p="${p%/}"
  if [ -z "$p" ]; then
    continue
  fi
  SPARSE_ARGS+=("/${p}/")
done

if [ "${#SPARSE_ARGS[@]}" -eq 0 ]; then
  echo "No valid paths in SPARSE_PATHS: $SPARSE_PATHS" >&2
  exit 1
fi

git -C "$DEST_DIR" sparse-checkout init --no-cone
git -C "$DEST_DIR" sparse-checkout set "${SPARSE_ARGS[@]}"
if ! git "${GIT_AUTH[@]}" -C "$DEST_DIR" fetch --depth 1 origin "$REF"; then
  echo "git fetch failed." >&2
  auth_hint
  exit 1
fi
git -C "$DEST_DIR" checkout --detach FETCH_HEAD

echo "Fetched [${SPARSE_PATHS}] from '$ORIGINAL_REPO_URL' (ref: $REF) into '$DEST_DIR'."
