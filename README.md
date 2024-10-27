## Overview
This repository contains <b>Load-Balancer</b> itself, <b>HTTP Client</b> and <b>HTTP Server</b>

<b>HTTP Client</b> makes a request to Load Balancer (thinking it is an actual server), Load-Balancer distributes request among it`s servers based on chosen algorithm. On accepted request from Load-Balancer (rounted from <b>HTTP Client</b>), <b>HTTP Server</b> will send current date and time as a response <br>

<b>HTTP Clien</b>t accepts ip and port from command line, it is general purpose client, but here it is used to communicate with <b>Load-Balancer</b>, thus, emulate real client of a <b>Load-Balancer</b><br>

<b>HTTP Server</b> is used as servers, for which <b>Load-Balancer</b> distributes load. <b>HTTP Server</b> accepts port and time delay from command line, if no port was provided, default 8080 will be used, if no time delay provided, random in range from 0 to 9 will be used. This is done to emulate complex work of server side.<br>

## Implemented algorithms
Load-Balancer currently implements only two algorithms:
<li> <b>Pseudo (Proxy)</b> - this is not actually an algorithm, this is the simplest version of Load-Balancer implementation that does not actually do load balancing, here Load-Balancer behaves like a Proxy. It accepts reuqest from client and forwards it to only server that Load-Balancer knows about.</li>
<li> <b>Round-robin</b> - forwards each request to the next server in a queue in round-robin maner, does not care about any dynamic load indicators (such as CPU load, RAM load etc.)
This is done in multithreaded way, so Load-Balancer wont wait for each server at a time, if 3 request received at a one time and we have 3 servers available, Load-Balancer will sent all 3 request to all servers simultaneously.<br></li>
