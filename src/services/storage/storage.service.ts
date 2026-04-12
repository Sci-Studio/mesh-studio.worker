import { PostSaveResponse } from '@generated/types.gen.js';

export interface UploadFileInput {
  fileName: string;
  file: Blob;
  contentType: string;
}

export interface StorageService {
  uploadFile(uploadRequest: UploadFileInput): Promise<PostSaveResponse>;

}