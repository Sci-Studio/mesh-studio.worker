/// <reference types="multer" />
import type { Express } from 'express';

import { PostSaveResponse } from '@generated/types.gen.js';

export interface StorageService {
  uploadFile(file: File | Express.Multer.File): Promise<PostSaveResponse>;
}
