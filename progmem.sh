#!/bin/bash

shopt -s nullglob
declare -u name
declare -a var_names=()
declare -a var_data=()

tmp_path="$(mktemp -d)"

cp web/* $tmp_path

for file in $tmp_path/*; do
    if [[ -f "$file" ]]; then
        basename="$(basename "$file")"
        extension="${file##*.}"
        name="PAGE_${basename//./_}_GZ[]"

        gz_data=$(gzip -9 -c -n $file | xxd -i | tr -d '\n' | sed 's/}/}\n/g'; printf "\n")
            
        var_names+=("$name")    
        var_data+=("$gz_data")
    fi
done

cat <<EOF > src/core/webfiles.h
#pragma once
#include <Arduino.h>

EOF

for i in "${!var_names[@]}"; do
    echo "const uint8_t ${var_names[$i]} PROGMEM = {${var_data[$i]}};" >> src/core/webfiles.h
done

if [[ -d "$tmp_path" ]]; then
    rm -rf $tmp_path
fi