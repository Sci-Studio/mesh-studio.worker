#pragma once

#include "geometry/Mesh.hpp"

#include <filesystem>

namespace writer {

    class FileWriter {
        public:
            FileWriter() = default;
            explicit FileWriter(std::filesystem::path outputPath) : mOutputPath(std::move(outputPath)) {}
            virtual ~FileWriter() = default;

            virtual bool saveMeshToOutputFile(const geometry::Mesh& mesh) = 0;
        
            const std::filesystem::path& outputPath() const { return mOutputPath; }

        private:
            std::filesystem::path mOutputPath = "tmp/deluanay.svg";
    };
}