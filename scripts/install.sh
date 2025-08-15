#!/usr/bin/env bash

## Copyright (c) 2025 Michael Navarro 
## MIT license, see LICENSE for more details.

## Currently this script, builds azile locally and moves the executable to /usr/local/bin
## TODO: update this script to download azile executable from github releases
##       to one of the following dirs:
##           ~/.local/bin
##           /usr/local/bin

set -e

cc ./build/nob.c -o ./build/nob
./build/nob

if [[ ! -f ./build/azile ]]; then
    exit 1
fi

echo "[INFO] Installing azile to /usr/local/bin/..."
if sudo cp ./build/azile /usr/local/bin/; then
    echo -e "[INFO] Installation completed!\n"
else
    echo "[ERROR] Failed to install azile. Aborting setup instructions."
    exit 1
fi

echo "To begin using Azile, run the appropriate command for your shell below."
echo -e "This will configure your shell to load Azile automatically in future sessions.\n"

echo "BASH:"
echo -e '  echo '\''eval "$(azile init bash)"'\'' >> ~/.bashrc && source ~/.bashrc \n'

echo "FISH:"
echo -e '  echo "azile init fish | source" >> ~/.config/fish/config.fish && source ~/.config/fish/config.fish \n'

echo "ZSH:"
echo -e '  echo '\''eval "$(azile init zsh)"'\'' >> ~/.zshrc && source ~/.zshrc \n'

echo "Thanks for using Azile!"
