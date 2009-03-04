#!/bin/bash
rm ../mangos.sql
for i in `ls *.sql | grep -v eventai`; do
    cat $i >> ../mangos.sql
done
cp eventai.sql ../scriptdev2.sql
