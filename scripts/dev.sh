#!/bin/bash

rm -rf build/debug

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -S . -B build/debug
make -C build/debug

# Needed for neovim clangd lsp
# Remove existing symlink if it exists
[ -L compile_commands.json ] && rm compile_commands.json
# Create symlink for neovim lspconfig clangd
ln -s build/debug/compile_commands.json compile_commands.json

if [ $? -eq 0 ]; then
    echo "Debug - Build succeeded!"
else
    echo "Debug - Build failed!"
    exit 1
fi

