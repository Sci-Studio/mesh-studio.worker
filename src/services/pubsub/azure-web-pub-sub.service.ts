import { env } from '../../config/env.js';

import { WebPubSubServiceClient } from '@azure/web-pubsub';
import { PubSubClientAccessOptions, PubSubService } from './pub-sub.service.js';

export class AzureWebPubSubService implements PubSubService {
  private readonly webPubSubServiceClient: WebPubSubServiceClient;

  constructor() {
    this.webPubSubServiceClient = new WebPubSubServiceClient(
      env.AZURE_WEBPUBSUB_CONNECTION_STRING,
      env.AZURE_WEBPUBSUB_HUB
    );
  }

  async publishToJobGroup(jobId: string, payload: unknown): Promise<void> {
    const normalizedJobId = jobId.trim();
    if (!normalizedJobId) {
      throw new Error('jobId is required to publish to a job group.');
    }

    const groupClient = this.webPubSubServiceClient.group(`job:${normalizedJobId}`);
    const serializedPayload =
      typeof payload === 'string' ? payload : JSON.stringify(payload ?? null);

    await groupClient.sendToAll(serializedPayload, { contentType: 'text/plain' });
  }

  async getClientAccessUrl(options?: PubSubClientAccessOptions): Promise<string> {
    const token = await this.webPubSubServiceClient.getClientAccessToken({
      userId: options?.userId,
      groups: options?.groups,
      roles: options?.roles,
    });

    return token.url;
  }
}