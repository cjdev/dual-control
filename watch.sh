#!/bin/bash

if ! which fswatch > /dev/null; then
    echo 'fswatch is required https://github.com/emcrisostomo/fswatch'
    exit 1
fi
fswatch -E --exclude '.' --include='\.(cc|h)$' -o $(dirname $0) | xargs -n1 -I{} make test

