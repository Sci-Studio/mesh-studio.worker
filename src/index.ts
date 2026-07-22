import { ServiceBusClient, ServiceBusReceivedMessage } from '@azure/service-bus';
import { BlobServiceClient } from '@azure/storage-blob';

import { env } from './config/env.js';

const jobsQueue = env.AZURE_SERVICE_BUS_QUEUE_NAME;
const resultsQueue = env.AZURE_SERVICE_BUS_RESULTS_QUEUE_NAME;

const serviceBusClient = new ServiceBusClient(
  env.AZURE_SERVICE_BUS_CONNECTION_STRING
);
const blobServiceClient = BlobServiceClient.fromConnectionString(
  env.AZURE_BLOB_CONNECTION_STRING
);
const containerClient = blobServiceClient.getContainerClient(env.AZURE_BLOB_CONTAINER);

async function processJob(jobId: string, payload: any): Promise<string> {
  console.log(`[worker] Processing job ${jobId}`);
  const userId = String(payload?.userId);
  const outputBlobPath = `results/${jobId}/hello-world.txt`;
  const blob = containerClient.getBlockBlobClient(outputBlobPath);
  await blob.uploadData(Buffer.from('Hello World'), {
    blobHTTPHeaders: { blobContentType: 'text/plain' },
  });

  return outputBlobPath;
}

function resolveJobId(message: ServiceBusReceivedMessage): string {
  if (message.messageId) {
    return String(message.messageId);
  }

  return `${Date.now()}-${Math.random().toString(36).slice(2, 10)}`;
}

async function main(): Promise<void> {
  await containerClient.createIfNotExists();

  const receiver = serviceBusClient.createReceiver(jobsQueue);
  const sender = serviceBusClient.createSender(resultsQueue);

  receiver.subscribe(
    {
      processMessage: async (message) => {
        const jobId = resolveJobId(message);

        try {
          const outputBlobPath = await processJob(jobId, message.body);

          await sender.sendMessages({
            messageId: jobId,
            body: { jobId, status: 'COMPLETED', outputBlobPath },
            contentType: 'application/json',
            subject: 'mesh.completed',
          });

          await receiver.completeMessage(message);
          console.log(`[worker] Job ${jobId} completed`);
        } catch (error) {
          console.error(`[worker] Job ${jobId} failed:`, error);

          await sender.sendMessages({
            messageId: jobId,
            body: { jobId, status: 'FAILED', error: String(error) },
            contentType: 'application/json',
            subject: 'mesh.failed',
          });

          await receiver.deadLetterMessage(message, {
            deadLetterReason: 'WorkerProcessingFailed',
            deadLetterErrorDescription: String(error),
          });
        }
      },
      processError: async (args) => {
        console.error('[worker] Service Bus error:', args.error);
      },
    },
    { autoCompleteMessages: false }
  );

  const shutdown = async (signal: NodeJS.Signals): Promise<void> => {
    console.log(`[worker] Received ${signal}. Shutting down...`);
    await receiver.close();
    await sender.close();
    await serviceBusClient.close();
    process.exit(0);
  };

  process.once('SIGINT', () => void shutdown('SIGINT'));
  process.once('SIGTERM', () => void shutdown('SIGTERM'));

  await new Promise<void>(() => {});
}

void main().catch(async (error) => {
  console.error('[worker] Fatal startup error:', error);
  await serviceBusClient.close();
  process.exit(1);
});
