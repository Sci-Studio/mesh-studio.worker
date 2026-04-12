# mesh-studio.worker

Worker service for Mesh Studio (Azure Container Jobs and related integrations).

## Fetching from mesh-studio.api (OpenAPI + Prisma)

Scripts such as `npm run openapi:fetch` or `npm run prisma:fetch` clone [mesh-studio.api](https://github.com/Sci-Studio/mesh-studio.api) into `.cache/mesh-studio.api` with a **sparse checkout** of **`openapi/`** and **`prisma/`** by default. If that repository is private, Git must authenticate.

Override checked-out paths with `SPARSE_PATHS` (space-separated), e.g. `SPARSE_PATHS="openapi prisma"` (default).

## Prisma client (generate only; no migrations here)

After fetching, generate the Prisma client from the API schema (this worker does **not** run migrations):

```bash
npm run prisma:generate
```

What this does:

1. Copies the cached API schema to **`prisma/schema.prisma`** in this repo (ignored by git).
2. Patches the `generator client` block so **`output`** is **`../src/generated-prisma`** (relative to `prisma/schema.prisma`), i.e. generated code lives under **`src/generated-prisma/`** in this project—not under `.cache`.
3. Runs `prisma generate` against that local schema.

Import the client in application code (path alias in `tsconfig.json`):

```ts
import { PrismaClient } from '@prisma-db';
```

The default source schema is `.cache/mesh-studio.api/prisma/schema.prisma`. If your API repo uses a different layout, set **`PRISMA_SCHEMA_PATH`** and adjust **`SPARSE_PATHS`** so the file is present under `.cache/` before generate.

## Fetching the OpenAPI spec (private GitHub repository)

Use the same authentication steps below for any fetch.

### Option A: GitHub CLI (recommended)

1. Install the [GitHub CLI](https://cli.github.com/) (`gh`).
2. Log in to GitHub.com:

   ```bash
   gh auth login -h github.com
   ```

   Follow the prompts (HTTPS, authenticate via browser or token, and grant access as needed).

3. Confirm you are logged in:

   ```bash
   gh auth status -h github.com
   ```

4. Run the fetch (or full generate) from this repo:

   ```bash
   npm run openapi:fetch
   ```

   The fetch script uses Git’s credential helper from `gh` when no `GITHUB_TOKEN` / `GH_TOKEN` is set.

If your organization uses **SAML SSO**, you may still need to authorize the token or SSO session for that org in the GitHub web UI after login.

### Option B: Personal access token

Create a [Personal Access Token](https://github.com/settings/tokens) with read access to the repository (for fine-grained tokens: **Contents: Read** on the target repo). Then:

```bash
export GITHUB_TOKEN=ghp_your_token_here
npm run openapi:fetch
```

`GH_TOKEN` is also supported.

### Option C: SSH

Use an SSH remote URL and your usual SSH key:

```bash
REPO_URL=git@github.com:Sci-Studio/mesh-studio.api.git npm run openapi:fetch
```

## Related npm scripts

| Script | Purpose |
|--------|---------|
| `npm run openapi:fetch` | Sparse-clone `openapi/` + `prisma/` into `.cache/mesh-studio.api` |
| `npm run openapi:generate` | Fetch, then run `@hey-api/openapi-ts` |
| `npm run prisma:fetch` | Same fetch as above (alias) |
| `npm run prisma:generate` | Fetch, then `prisma generate` from cached API schema |
| `npm run openapi:lint` | Lint the bundled OpenAPI file with Spectral |
