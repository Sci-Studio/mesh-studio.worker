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
  MESSAGE_BROKER_SERVICE: z.enum(['azure']).default('azure'),
  AZURE_SERVICE_BUS_CONNECTION_STRING: z.string().min(1),
  AZURE_SERVICE_BUS_QUEUE_NAME: z.string().min(1).default('mesh-jobs'),
  PUBSUB_SERVICE: z.enum(['azure']).default('azure'),
  AZURE_WEBPUBSUB_CONNECTION_STRING: z.string().min(1),
  AZURE_WEBPUBSUB_HUB: z.string().min(1).default('mesh-jobs'),
});

const parsed = envSchema.safeParse(process.env);

if (!parsed.success) {
  const errors = parsed.error.issues
    .map((issue) => `${issue.path.join('.')}: ${issue.message}`)
    .join(', ');

  throw new Error(`Invalid environment variables: ${errors}`);
}

export const env = parsed.data;
