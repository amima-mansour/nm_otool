#!/bin/bash

/Users/amansour/42File/nm_otool/nm/ft_nm "$1" > file
#/Users/amansour/Desktop/42Projects/test/ft_nm  "$1" > file
/Users/amansour/42File/nm_otool/ft_nm_ex "$1" > file1
#nm "$1" > file1
echo `diff file file1`
