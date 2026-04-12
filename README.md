# mesh-studio.worker

Worker service for Mesh Studio (Azure Container Jobs and related integrations).

## Fetching the OpenAPI spec (private GitHub repository)

Scripts such as `npm run openapi:fetch` clone [mesh-studio.api](https://github.com/Sci-Studio/mesh-studio.api) with a **sparse checkout** of the `openapi/` directory. If that repository is private, Git must authenticate.

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
| `npm run openapi:fetch` | Sparse-clone OpenAPI files into `.cache/mesh-studio.api` |
| `npm run openapi:generate` | Fetch spec, then run `@hey-api/openapi-ts` |
| `npm run openapi:lint` | Lint the bundled OpenAPI file with Spectral |
