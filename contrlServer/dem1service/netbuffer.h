#pragma once
typedef struct netbuffer
{
	char* buffer;
    //char buffer[1024];
	int buffer_len;
	int buffer_offset;

	netbuffer(){
		buffer_offset = 0;
		buffer_len = 0;
		buffer = NULL;
	}

}netbuffer_t;



