Simple Load-Balancer to distribute load on servers

THis repository contains <b>Load-Balancer</b> itself, <b>HTTP Client</b> and <b>HTTP Server</b>

<b>HTTP Client</b> makes a request to Load Balancer (thinking it is an actual server), Load-Balancer distributes request among it`s servers based on chosen algorithm. On accepted request from Load-Balancer (rounted from <b>HTTP Client</b>), <b>HTTP Server</b> will send current date and time as a response <br>

<b>HTTP Clien</b>t accepts ip and port from command line, it is general purpose client, but here it is used to communicate with <b>Load-Balancer</b>, thus, emulate real client of a <b>Load-Balancer<br>
<b>HTTP Server</b> is used as servers, for which Load-Balancer</b> distributes load. <b>HTTP Server</b> accepts port and time delay from command line, if no port was provided, default 8080 will be used, if no time delay provided, random in range from 0 to 9 will be used. This is done to emulate complex work of server side.<br>
