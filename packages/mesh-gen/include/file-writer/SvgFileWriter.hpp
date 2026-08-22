#pragma once

#include "FileWriter.hpp"
#include "geometry/Mesh.hpp"

namespace writer {

    class SvgFileWriter : FileWriter {

      public:
        using FileWriter::FileWriter;

        bool saveMeshToOutputFile(const geometry::Mesh& mesh) override;
    };
} // namespace writer
