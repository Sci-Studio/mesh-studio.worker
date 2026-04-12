#!/usr/bin/env bash
# Copies API schema from .cache into prisma/schema.prisma, pins generator output to
# src/generated-prisma (this project), then runs prisma generate.
# Override source schema: PRISMA_SCHEMA_PATH=path/to/schema.prisma
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"

CACHED_SCHEMA="${PRISMA_SCHEMA_PATH:-.cache/mesh-studio.api/prisma/schema.prisma}"
LOCAL_SCHEMA="prisma/schema.prisma"

if [ ! -f "$CACHED_SCHEMA" ]; then
  echo "Prisma schema not found: $CACHED_SCHEMA" >&2
  echo "Run npm run prisma:fetch first, or set PRISMA_SCHEMA_PATH if your API repo uses a different layout." >&2
  exit 1
fi

if [ ! -d "${ROOT}/node_modules/prisma" ]; then
  echo "Prisma CLI not installed. Run: npm install" >&2
  exit 1
fi

mkdir -p prisma src/generated-prisma
cp "$CACHED_SCHEMA" "$LOCAL_SCHEMA"

node ./src/tools/patch-prisma-schema-output.mjs

exec npx prisma generate --schema "$LOCAL_SCHEMA"
