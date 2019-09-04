#!/usr/bin/bash

GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
DEF="\033[0m"

nm_bin_ex="/Users/amansour/42file/nm_otool/ft_nm_example"
nm_bin="/Users/amansour/42File/nm_otool/nm/ft_nm"
otool_bin="/Users/amansour/42File/nm_otool/otool/ft_otool"

printf "\n%.-78s $YELLOW%s   %s$DEF\n" "" "OUTPUT" "RETURN"

for arg in "$@"
do

    echo ""
    if [[ -d "$arg" ]]; then
        # echo "FOLDER : $arg"
        loop=$arg/*
    elif [[ -f "$arg" ]]; then
        loop=$arg
    fi

    for file in $loop; do

        # CHECK SEGFAULT
        $("$nm_bin" $file > /dev/null 2>&1)
        if [[ "$?" -eq "139" ]]; then
            printf "%-80s $RED%s$DEF\n" "$file" "SEGFAULT"
            continue
        fi

        # CHECK NM
        my_nm="$("$nm_bin" $file 2>&1)"
        real_nm="$(nm $file 2>&1)"
        if [[ "$real_nm" == "$my_nm" ]]; then
            printf "%-80s $GREEN%s$DEF" "$file" "OK"
        else
            printf "%-80s $RED%s$DEF" "$file" "KO"
        fi

		# CHECK OTOOL
        my_otool="$("$otool_bin" $file 2>&1)"
        real_otool="$(otool -t $file 2>&1)"
        if [[ "$real_otool" == "$my_otool" ]]; then
            printf "%-80s $GREEN" "OK"
        else
            printf "%-80s $RED" "KO"
        fi


        # CHECK RETURN CODE
        #$("$nm_bin" $file > /dev/null 2>&1)
        #ret_my_nm="$?"
        #$(nm $file > /dev/null 2>&1)
        #ret_real_nm="$?"
        #if [[ "$ex_nm" == "$my_nm" ]]; then
        #    printf "       $GREEN%s$DEF\n" "OK"
        #else
            printf "       $RED%s$DEF\n" "KO"
        #fi

    done
done

# NO CORRUPT
# bash tests/nm_test.sh  ../testnm-master/64 ../testnm-master/32 ../testnm-master/macho/32 ../testnm-master/macho/64 ../testnm-master/macho/64/dylib ../testnm-master/fat ../testnm-master/lib_stat ../testnm-master/fat_lib ../testnm-master/archive

# CORRUPT
# bash tests/nm_test.sh ../testnm-master/corrupt ../testnm-master/corrupted_obj

# .h
# /usr/include/mach/machine.h
