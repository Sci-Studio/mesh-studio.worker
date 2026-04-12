/// <reference types="multer" />
import type { Express } from 'express';

import { PostSaveResponse } from '@generated/types.gen.js';

/** Result of reading a blob from storage (e.g. for in-memory processing). */
export interface DownloadedBlob {
  buffer: Buffer;
  contentType: string | undefined;
  /** Blob path within the container (same as upload `blobName`). */
  blobName: string;
}

/** Temp file on disk for CLI tools such as TetGen; call `cleanup()` when done. */
export interface TempBlobDownload {
  path: string;
  cleanup: () => Promise<void>;
}

export interface StorageService {
  uploadFile(file: File | Express.Multer.File): Promise<PostSaveResponse>;

  /**
   * Downloads blob bytes by container object name (e.g. `PostSaveResponse.blobName` from upload).
   */
  downloadFile(blobName: string): Promise<DownloadedBlob>;

  /**
   * Downloads a blob to a temp file and returns its path for subprocess/CLI input (e.g. tetgen).
   */
  downloadFileToTemp(blobName: string): Promise<TempBlobDownload>;
}
