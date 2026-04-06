import { PubSubService } from './pub-sub.service.js';
import { env } from '../../config/env.js';
import { AzureWebPubSubService } from './azure-web-pub-sub.service.js';

export function createPubSubService(): PubSubService {
  switch (env.PUBSUB_SERVICE) {
    case 'azure':
      return new AzureWebPubSubService();
    default:
      throw new Error(`Unsupported pub sub service: ${env.PUBSUB_SERVICE}`);
  }
}