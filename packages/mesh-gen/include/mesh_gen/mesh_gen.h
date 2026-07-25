#pragma once

namespace mesh_gen {

/**
 * Writes "Hello World" to the file called mesh.node.
 * Returns 0 on success, non-zero on failure.
 */
int write_mesh_to_file(const char* output_path);

}  // namespace mesh_gen
