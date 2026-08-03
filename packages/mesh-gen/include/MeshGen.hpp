#pragma once


/**
 * Writes "Hello World" to the file called mesh.node.
 * Returns 0 on success, non-zero on failure.
 */
int writeMeshToFile(const char* output_path);

int generateMesh(const char* input_path);