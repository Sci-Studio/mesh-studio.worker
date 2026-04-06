export interface PubSubClientAccessOptions {
  userId?: string;
  groups?: string[];
  roles?: string[];
}

export interface PubSubService {
  publishToJobGroup(jobId: string, payload: unknown): Promise<void>;

  getClientAccessUrl(options?: PubSubClientAccessOptions): Promise<string>;
}