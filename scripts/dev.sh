#!/bin/bash

mkdir -p build/release
cd build/release

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../..

# Needed for neovim clangd lsp
# Remove existing symlink if it exists
[ -L ../../compile_commands.json ] && rm ../../compile_commands.json
# Create symlink for neovim lspconfig clangd
ln -s build/release/compile_commands.json ../../compile_commands.json

make azile
cd ../..
./build/release/azile
