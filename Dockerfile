# ---- stage 1: build C++ mesh-gen ----
FROM alpine:3.20 AS cpp-build

RUN apk add --no-cache cmake g++ make

WORKDIR /src
COPY packages/mesh-gen/ ./

# Ensure no host-copied CMake cache is reused inside the image
RUN rm -rf build \
 && cmake -S . -B build \
 && cmake --build build --config Release

# ---- stage 2: Node worker ----
FROM node:24-alpine

WORKDIR /app

COPY package*.json ./
RUN npm ci --omit=dev

COPY dist/ ./dist/

# Binary path matches MESH_GEN_BIN=/bin/mesh-gen
COPY --from=cpp-build /src/build/mesh-gen /bin/mesh-gen
COPY --from=cpp-build /src/build/libMeshGen.so* /usr/local/lib/

RUN chmod +x /bin/mesh-gen

ENV MESH_GEN_BIN=/bin/mesh-gen
ENV LD_LIBRARY_PATH=/usr/local/lib

CMD ["node", "dist/index.js"]
