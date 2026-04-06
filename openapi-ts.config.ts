import { defineConfig } from "@hey-api/openapi-ts";

export default defineConfig({
    input: '.cache/mesh-studio.api/openapi/ms/openapi.yaml',
    output: 'src/generated',
    plugins: ['@hey-api/typescript', 'zod']
})