#!/bin/bash


for f in "${@}"; do
  astyle -q --options=$(dirname $0)/astylerc $f
  echo "" >> $f
  if ! grep -q 'Copyright.*CJ' $f; then
      cat << EOF > $f.temp_
/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the  GNU General Public License  version 2  or
 * later.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

EOF
  fi
  cat -s $f >> $f.temp_
  mv $f.temp_ $f
done

