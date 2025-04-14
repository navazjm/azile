#!/bin/bash

## Copyright (c) 2025 Michael Navarro 
## MIT license, see LICENSE for more details.

cmake -S . -B build/release
sudo make install -C build/release
