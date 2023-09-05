#!/bin/bash
set -e -u -o pipefail

if [ $# -ne 2 ]; then
    echo "create_weak_library.sh needs exactly two paramters:"
    echo "$0 <new library name> <object files separated by ';'>"
    exit 1
fi

library=${1}
objects=${2//;/ }
objects_weak=""

do_ar=${CMAKE_AR:-${AR:-ar}}
do_nm=${CMAKE_NM:-${NM:-nm}}
do_objcopy=${CMAKE_OBJCOPY:-${OBJCOPY:-objcopy}}
ar_param=${AR_PARAM:-qc}

for object in $objects
do
    local_prefix="$(basename ${object%%.c.o})"
    object_name="${object%%.o}"

    object_weak="${object_name}.weak.o"
    sym_weak="${object_name}.weak.txt"
    sym_rename="${object_name}.rename.txt"
    sym_global="${object_name}.global.txt"

    object_nm=$($do_nm $object)
    # get a list of all global symbols
    sym_gfunc=$(echo "$object_nm" | sed -e '/\sT\s/!d; s/\sT\s/ /g')
    # generate the list of static symbols that should be renamed with the local prefix and turnt global
    echo "$object_nm" | sed -e "/\st\s/!d; /_sub_[DI]/d; s/\st\s/ /g; s/\S*\s\(\S*\)/\1 _$local_prefix\1/g" > $sym_rename
    echo "$sym_gfunc" | awk '{ print $2 }' > $sym_weak
    awk '{ print $2 }' $sym_rename > $sym_global

    param_global=""
    param_rename=""
    if [ -s $sym_global ]; then param_global=" --globalize-symbols=$sym_global"; fi
    if [ -s $sym_rename ]; then param_rename=" --redefine-syms=$sym_rename"; fi

    param_add="$(echo "$sym_gfunc" | sed -e 's/\(\S\+\)\s\(.\+\)/--add-symbol __real_\2=.text:0x\1,function,global/g')"

    # We need to check if we can properly feed objcopy via a pipe from a file
    $do_objcopy $param_rename $param_add $param_global $object $object_weak
    $do_objcopy --weaken-symbols=$sym_weak $object_weak

    objects_weak="$objects_weak $object_weak"
done

if [ -f "$library" ]
then
    rm $library
fi

$do_ar $ar_param $library $objects_weak
