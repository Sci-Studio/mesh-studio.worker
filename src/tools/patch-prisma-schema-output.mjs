/**
 * Ensures prisma/schema.prisma generator block uses a client output under src/generated-prisma
 * (paths are relative to prisma/schema.prisma).
 */
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.resolve(__dirname, '../..');
const schemaPath = path.join(root, 'prisma', 'schema.prisma');

const OUTPUT_LINE = '  output   = "../src/generated-prisma"';

let text = fs.readFileSync(schemaPath, 'utf8');

// Drop existing client output lines (safe: Prisma uses "output =", not datasource "url")
text = text.replace(/^\s*output\s*=\s*.+$/gm, '');

if (!/generator\s+client\s*\{/s.test(text)) {
  console.error('prisma/schema.prisma: no generator client { } block found.');
  process.exit(1);
}

const replaced = text.replace(
  /(generator\s+client\s*\{)(\r?\n)/,
  `$1$2${OUTPUT_LINE}$2`
);

if (replaced === text) {
  const alt = text.replace(
    /(generator\s+client\s*\{)(\s*)(provider)/,
    `$1\n${OUTPUT_LINE}\n  $3`
  );
  if (alt === text) {
    console.error('Could not inject output into generator client block.');
    process.exit(1);
  }
  fs.writeFileSync(schemaPath, alt);
} else {
  fs.writeFileSync(schemaPath, replaced);
}
