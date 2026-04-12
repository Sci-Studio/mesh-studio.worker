import { MeshJobStatus } from '@generated/types.gen.js';

export interface DatabaseService {
    updateMeshJobStatus(jobId: string, status: MeshJobStatus): Promise<void>;
}