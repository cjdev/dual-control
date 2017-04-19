#!/bin/bash

for f in *.h *.cc; do echo "" >> $f; { rm $f;cat -s >$f; }<$f; done
astyle -q --options=$(dirname $0)/astylerc *.h *.cc

