import { StorageService } from './storage.service.js';
import { env } from '../../config/env.js';
import { AzureStorageAccountService } from './azure-storage-account.service.js';

export function createStorageService(): StorageService {
  switch (env.STORAGE_SERVICE) {
    case 'azure':
      return new AzureStorageAccountService();
    default:
      throw new Error(`Unsupported storage service: ${env.STORAGE_SERVICE}`);
  }
}