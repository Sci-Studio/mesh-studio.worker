FROM node:24-alpine

WORKDIR /app

COPY package*.json ./
RUN npm ci --omit=dev

COPY dist/ ./dist/

# Later: copy your C++ binary
# COPY bin/mesh-gen ./bin/mesh-gen
# RUN chmod +x ./bin/mesh-gen

CMD ["node", "dist/index.js"]
