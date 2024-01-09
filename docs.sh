#!/usr/bin//bash

echo "Running Doxygen"
doxygen

if [ "$?" != "0" ]; then
    echo "Doxygen error, please see log above"
    exit 1
fi

clear
echo "Doxygen run successfully, warning log:"
cat doxygen-warn.log

