#!/bin/bash 

BDE_SOURCE_ROOT=$PWD/../
BDE_BUILD_DIR=$PWD/../build/Debug/

BSL_HEADERS=`\ls -d1 $BDE_SOURCE_ROOT/groups/bsl/*/`
#echo $BSL_HEADERS 
export BSL_HEADERS_WITH_I=
BSL_HEADERS_WITH_I= 
for i in $BSL_HEADERS
do 
    BSL_HEADERS_WITH_I="$BSL_HEADERS_WITH_I -I$i"
done
export BSL_HEADERS_WITH_I
export BSL_LIB=$BDE_BUILD_DIR/groups/bsl/libbsl.a

echo "BSL Headers = ${BSL_HEADERS_WITH_I}"
echo
echo "BSL LIB     = ${BSL_LIB}"
echo
