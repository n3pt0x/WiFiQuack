#!/bin/bash

shopt -s nullglob
declare -u name
declare -a var_names=()
declare -a var_data=()

tmp_path="$(mktemp -d)"

cp web/* $tmp_path

if ! [[ -d venv ]]; then
    pip_install="Y"
    read -p "Python package jsmin not founded. Do you want install it ? [Y/n]" pip_install
    if [[ "$pip_install" =~ ^[Yy]$ ]]; then
        python3 -m venv venv
        source venv/bin/activate
        pip install jsmin
    else
        echo "jsmin is require !"
        exit 1;
    fi
else
    source venv/bin/activate
fi

for file in $tmp_path/*; do
    if [[ -f "$file" ]]; then
        basename="$(basename "$file")"
        extension="${file##*.}"
        name="PAGE_${basename//./_}_GZ[]"

        if [[ "$extension" == "js" ]]; then
            python3 -m jsmin "$file" > "${file}.tmp" && mv "${file}.tmp" "$file"
        fi

        gz_data=$(gzip -9 -c $file | xxd -i | tr -d '\n' | sed 's/}/}\n/g'; printf "\n")
            
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

if [[ -d "$tmp_path" ]]; then
    rm -rf $tmp_path
fi