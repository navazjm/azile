#!/bin/bash

rm -rf build/release

# Set the necessary environment variables
export PKG_CONFIG_PATH="/opt/homebrew/opt/libgit2/lib/pkgconfig"

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release -S . -B build/release

# Needed for neovim clangd lsp
# Remove existing symlink if it exists
[ -L compile_commands.json ] && rm compile_commands.json
# Create symlink for neovim lspconfig clangd
ln -s build/release/compile_commands.json compile_commands.json

make -C build/release

# Run the executable if the build was successful
if [ $? -eq 0 ]; then
    echo "Build succeeded!"
else
    echo "Build failed!"
    exit 1
fi

