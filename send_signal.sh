#!/bin/bash

PID=$1

if [ -z "$PID" ]; then
    echo "Please provide the child PID as the first argument."
    exit 1
fi

echo "Sending SIGTSTP to child process (PID: $PID) every 3 seconds..."

for i in {1..5}
do
    echo "Sending SIGTSTP to $PID"
    kill -SIGTSTP $PID
    sleep 3
done

echo "Done sending signals. Now sending SIGTERM to child."
kill -SIGTERM $PID
