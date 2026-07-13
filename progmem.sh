#!/bin/bash

shopt -s nullglob
declare -u name
declare -a var_names=()
declare -a var_data=()

for file in web/*; do
    if [[ -f "$file" ]]; then
        basename="$(basename "$file")"
        name="PAGE_${basename//./_}_GZ[]"
        
        gz_data=$(gzip -9 -c $file | xxd -i | tr -d '\n' | sed 's/}/}\n/g'; echo -e "\n")
            
        var_names+=("$name")    
        var_data+=("$gz_data")
    fi
done

cat <<EOF > src/core/webfiles.h
#pragma once
#include <Arduino.h>

EOF

for i in "${!var_names[@]}"; do
    echo "const char ${var_names[$i]} PROGMEM = {${var_data[$i]}};" >> src/core/webfiles.h
done
