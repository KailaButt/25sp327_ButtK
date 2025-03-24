#!/bin/bash


CHILD_PID=$(pgrep -f child)

echo "Sending SIGTSTP to child process (PID: $CHILD_PID) every 3 seconds..."
for i in {1..5}
do
  kill -SIGTSTP $CHILD_PID
  echo "Sending SIGTSTP to $CHILD_PID"
  sleep 3
done

echo "Done sending signals. Now sending SIGTERM to child."
kill -SIGTERM $CHILD_PID
