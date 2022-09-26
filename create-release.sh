#!/usr/bin/bash

mkdir -p releases
if [ -f "releases/ds-craft.nds" ]; then
    rm releases/ds-craft.nds
fi

if [ "$1" == "" ]; then
    echo "Usage: $0 <release version>"
    echo "Example: $0 alpha2.0.0"
    exit 1
fi

echo "Creating ds-craft-$1.nds in releases..."
make
cp ds-craft.nds releases/ds-craft-$1.nds
echo "ok!"