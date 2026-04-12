import path from 'node:path';

import { BlobServiceClient, ContainerClient } from '@azure/storage-blob';
import { PostSaveResponse } from '@generated/types.gen.js';

import { env } from '../../config/env.js';
import { StorageService, UploadFileInput } from './storage.service.js';

export class AzureStorageAccountService implements StorageService {
  private readonly blobServiceClient: BlobServiceClient;
  private readonly containerClient: ContainerClient;

  constructor() {
    this.blobServiceClient = BlobServiceClient.fromConnectionString(
      env.AZURE_BLOB_CONNECTION_STRING
    );

    this.containerClient = this.blobServiceClient.getContainerClient(
      env.AZURE_BLOB_CONTAINER
    );
  }

  private normalizePrefix(prefix: string): string {
    return prefix.replace(/^\/+|\/+$/g, '');
  }

  async uploadFile(uploadRequest: UploadFileInput): Promise<PostSaveResponse> {
    const safeFileName = path.basename(uploadRequest.fileName ?? 'upload.off');
    const normalizedPrefix = this.normalizePrefix(env.AZURE_BLOB_PREFIX);
    const blobName = normalizedPrefix
      ? `${normalizedPrefix}/${safeFileName}`
      : safeFileName;

    const blockBlobClient = this.containerClient.getBlockBlobClient(blobName);
    const fileBuffer = Buffer.from(await uploadRequest.file.arrayBuffer());

    await blockBlobClient.uploadData(fileBuffer, {
      blobHTTPHeaders: {
        blobContentType: uploadRequest.contentType ?? 'application/octet-stream',
      },
    });

    return {
      blobName,
      blobUrl: blockBlobClient.url,
    };
  }

}