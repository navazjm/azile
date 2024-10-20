#!/bin/bash

cmake -S . -B build/release
sudo make install -C build/release
