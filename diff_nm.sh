#!/bin/bash

/Users/amansour/42File/nm_otool/ft_nm "$1" > file
echo '==========='
nm "$1" > file1
echo `diff file file1`
