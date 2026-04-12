import { DatabaseService } from './database.service.js';
import { PrismaDatabaseService } from './prisma-database.service.js';

export function createDatabaseService(): DatabaseService {
  return new PrismaDatabaseService();
}
