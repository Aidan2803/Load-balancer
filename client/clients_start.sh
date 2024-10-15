#!/bin/bash

load_balancer_ip="127.0.0.1"
load_balancer_port="8090"

COUNTER=0
while [  $COUNTER -lt 5 ]; do
    let COUNTER=COUNTER+1 
    ./build/client $load_balancer_ip $load_balancer_port &
done

wait

echo "All client processes have finished"
