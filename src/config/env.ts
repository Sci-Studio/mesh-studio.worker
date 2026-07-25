import dotenv from 'dotenv';
import { z } from "zod";

dotenv.config();
dotenv.config({
  path: `.env.${process.env.NODE_ENV ?? 'development'}`,
  override: true,
});
const envSchema = z.object({
  NODE_ENV: z
    .enum(["development", "test", "production"])
    .default("development"),
  AZURE_BLOB_CONNECTION_STRING: z.string().min(1),
  AZURE_BLOB_CONTAINER: z.string().default('mesh-studio'),
  AZURE_SERVICE_BUS_CONNECTION_STRING: z.string().min(1),
  AZURE_SERVICE_BUS_QUEUE_NAME: z.string().min(1).default('mesh-jobs'),
  AZURE_SERVICE_BUS_RESULTS_QUEUE_NAME: z.string().min(1).default('mesh-jobs-results'),
  MESH_GEN_BIN: z.string().min(1),
});

const parsed = envSchema.safeParse(process.env);

if (!parsed.success) {
  const errors = parsed.error.issues
    .map((issue) => `${issue.path.join('.')}: ${issue.message}`)
    .join(', ');

  throw new Error(`Invalid environment variables: ${errors}`);
}

export const env = parsed.data;
