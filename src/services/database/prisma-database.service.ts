import { MeshJobStatus } from '@generated/types.gen.js';
import { DatabaseService } from './database.service.js';

export class PrismaDatabaseService implements DatabaseService {
    updateMeshJobStatus(jobId: string, status: MeshJobStatus): Promise<void> {
        throw new Error('Method not implemented.');
    }
}