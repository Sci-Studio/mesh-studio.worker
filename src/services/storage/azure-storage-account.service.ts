/// <reference types="multer" />
import path from 'node:path';

import type { Express } from 'express';
import { BlobServiceClient, ContainerClient } from '@azure/storage-blob';
import { PostSaveResponse } from '@generated/types.gen.js';

import { env } from '../../config/env.js';
import { StorageService } from './storage.service.js';

interface blobFile {
  blob: Blob;
  originalName: string;
  contentType: string;
}
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

  async uploadFile(file: File | Express.Multer.File): Promise<PostSaveResponse> {
    const { blob, originalName, contentType } =
      await this.resolveUploadBuffer(file);

    const safeFileName = path.basename(originalName || 'upload.bin');
    const normalizedPrefix = this.normalizePrefix(env.AZURE_BLOB_PREFIX);
    const blobName = normalizedPrefix
      ? `${normalizedPrefix}/${safeFileName}`
      : safeFileName;

    const blockBlobClient = this.containerClient.getBlockBlobClient(blobName);

    await blockBlobClient.uploadData(blob, {
      blobHTTPHeaders: {
        blobContentType: contentType,
      },
    });

    return {
      blobName,
      blobUrl: blockBlobClient.url,
    };
  }

  private async resolveUploadBuffer(
    file: File | Express.Multer.File
  ): Promise<blobFile> {
    let blob: Blob;
    if ('originalname' in file && typeof file.originalname === 'string') {
      const multerFile = file as Express.Multer.File;
      const originalName = multerFile.originalname;
      const contentType = multerFile.mimetype || 'application/octet-stream';
      blob = new Blob([Uint8Array.from(multerFile.buffer)], {
        type: contentType,
      })

      return { blob, originalName, contentType };
    }

    const webFile = file as File;
    blob = new Blob([await webFile.arrayBuffer()], {
      type: webFile.type || 'application/octet-stream',
    })
    return {
      blob,
      originalName: webFile.name,
      contentType: webFile.type || 'application/octet-stream',
    };
  }
}
