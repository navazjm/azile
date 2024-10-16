#!/bin/bash

CC=/opt/homebrew/bin/gcc-14 cmake -B build/release -DCMAKE_EXPORT_COMPILE_COMMANDS=1\
-DCMAKE_BUILD_TYPE=Release .

# Needed for neovim clangd lsp
# Remove existing symlink if it exists
[ -L ../../compile_commands.json ] && rm ../../compile_commands.json
# Create symlink for neovim lspconfig clangd
ln -s build/release/compile_commands.json ../../compile_commands.json

cd build/release
make azile
cd ../..
./build/release/azile
