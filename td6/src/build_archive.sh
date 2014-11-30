#!/bin/sh
##
## build_archive.sh
## 
## Made by Benjamin Negrevergne
## Login   <bengreve@neb.imag.fr>
## 
## Started on  Wed Nov 17 13:22:25 2010 Benjamin Negrevergne
## Last update Wed Nov 17 13:22:25 2010 Benjamin Negrevergne
##

FILES="linked_list.h linked_list.c linked_list_simple_test.c Makefile reader_writer.h reader_writer_0.c reader_writer_tracing.h reader_writer_tracing.c README ../td6.pdf"

rm -rf td6/ && mkdir td6 &&
for i in $FILES; do  echo "Coping $i"; cp $i td6/.; done

tar czvf td6.tar.gz td6/
