Simple Load-Balancer to distribute load on servers

THis repository contains Load-Balancer itself, HTTP Client and HTTP Server

HTTP Client makes a request to Load Balancer (thinking it is an actual server), Load-Balancer distributes request among it`s servers based on chosen algorithm. On accepted request from Load-Balancer (rounted from HTTP Client), HTTP Server will send current date and time as a response <br>

HTTP Client accepts ip and port from command line, it is general purpose client, but here it is used to communicate with Load-Balancer, thus, emulate real client of a Load-Balancer<br>
HTTP Server is used as servers, for which Load-Balancer distributes load. HTTP Server accepts port and time delay from command line, if no port was provided, default 8080 will be used, if no time delay provided, random in range from 0 to 9 will be used. This is done to emulate complex work of server side.<br>
