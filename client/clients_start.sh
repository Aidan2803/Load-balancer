#!/bin/bash

load_balancer_ip="127.0.0.1"
load_balancer_port="8090"

COUNTER=0
while [  $COUNTER -lt 10 ]; do
    echo The counter is $COUNTER
    let COUNTER=COUNTER+1 
    ./build/client $load_balancer_ip $load_balancer_port &
done

wait

echo "All client processes have finished"
