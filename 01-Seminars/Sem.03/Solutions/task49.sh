find songs -mindepth 1| cut -d '-' -f2 | cut -c 2- | cut -d '(' -f1 | tr A-Z a-z | tr ' ' '_' | sort
#or
find songs -iname '*.ogg' | sed -E 's/(.*)\/(.*) - (.*) \((.*), (.*)\).(.*)/\3/g' | sed -E 's/[A-Z]/\L&/g' | sed -E 's/ /_/g' | sort
#or
find songs -mindepth 1 -printf "%f\n" | sed -E 's/.* - (.*) \(.*/\1/g' | sed -E 's/[A-Z]/\L&/g' | sed -E 's/ /_/g' | sort