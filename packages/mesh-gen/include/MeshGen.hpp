#pragma once


/**
 * Writes "Hello World" to the file called mesh.node.
 * Returns 0 on success, non-zero on failure.
 */
int writeMeshToFile(const char* outputPath);

int generateMesh(const char* inputPath);