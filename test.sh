#!/bin/bash

SUCCESS=0

for file in tests/test_right/*
do
    echo "TEST FILE $file"
    cat "$file" | make exec -s
    result=$?
    if [ $result -eq $SUCCESS ]
    then
        echo "SUCCESS!"
    else
        echo "TEST FAILED!"
    fi
  
done

echo "STARTING WRONG TESTS"

for file in tests/test_wrong/*
do
    echo "TEST FILE $file"
    cat "$file" | make exec -s
    result=$?
    if [ $result -ne $SUCCESS ]
    then
        echo "SUCCESS!"
    else
        echo "TEST FAILED!"
    fi
  
done

