#include "Server.h"


int main(int argc, char *argv[])	{
	
    Server server = Server(SERVER_DEFAULT_ADDRESS, SERVER_DEFAULT_PORT);

	char buf[MSG_BUFFER_MAX_SIZE];
	double seconds_per_tick = 1.0f / TICKS_PER_SECOND;

	/** 
	 * 1. Read incoming packets (messages from clients)
	 * 2. Update game state accordingly
	 * 3. Send state updates to clients
	 * State updates should always be sent at 1 / TICKS_PER_SECOND intervals.
	 * Once a period of 1 / TICKS_PER_SECOND elapses, push state update packets
	 * to clients (go to step 3) and restart timing cycle.
	 */
	while(server.running())	{
		
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point finish = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> processing_time = start - finish;
        
		// 1. Get and action new packets
		while (processing_time.count() < seconds_per_tick) {
            
            start = std::chrono::system_clock::now();
            processing_time = start - finish;

			// This should be consistend once timed_listen() implemented
            std::cout << processing_time.count() << std::endl;    
			
			// TODO add remauining tick time as timeout to listen()
			server.listen(buf, MSG_BUFFER_MAX_SIZE);
			if (buf[0] != '\0') {
				std::cout << buf;

				// 2. Update server-side game state
				// TODO
			}
		}
		finish = std::chrono::system_clock::now(); 
				
        // 3. Send state updates to clients
		// TODO
	}

	close(server.s);

	return EXIT_SUCCESS;
}

Server::Server(const char* address, int port) {

	// Init socket
	s = sizeof(s_info_client);
	send_len = sizeof(s_info_client);
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)	{
		is_running = false;
		std::cout << "[server] failed to create socket" << std::endl;
	} else {is_running = true;}
	
	// Store address/port details in addrinfo struct
	memset((char *) &s_info_server, 0, sizeof(s_info_server));
	s_info_server.sin_family = AF_INET;
	s_info_server.sin_port = htons(port);
	s_info_server.sin_addr.s_addr = inet_addr(address);  // Specified address
	// s_info.sin_addr.s_addr = htonl(INADDR_ANY);	// Localhost

	// Bind socket to specified port
	if( bind(s, (struct sockaddr*)&s_info_server, sizeof(s_info_server) ) == -1){
		is_running = false;
		std::cout << "[server] failed to bind socket" << std::endl;
	} else {is_running = true;}

}
    
Server::~Server() {
	close(s);
}

Server::Server() {is_running = false;}

int Server::listen(char* buf, int max_size) {
	
	// Clear buffer and receive message.
	memset(buf,'\0', MSG_BUFFER_MAX_SIZE);
	recv_len = 0;
	if ((recv_len = recvfrom(s, buf, MSG_BUFFER_MAX_SIZE, 0, (struct sockaddr *) &s_info_client,  (socklen_t*)&send_len)) == -1)	{
		std::cout << "[server] error when reading incoming packet" << std::endl;
	}

	// Send response message.
	if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &s_info_client, send_len) == -1)	{
		std::cout << "[server] error when sending response packet" << std::endl;
	}	

	return EXIT_SUCCESS;
} 

int Server::timed_listen(char* buf, int max_size, int ms) {
	return EXIT_SUCCESS;
}

bool Server::running()  {return is_running;}