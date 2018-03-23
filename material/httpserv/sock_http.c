#include <stdio.h>
#include <string.h>
#include <unistd.h>

void try_handle_http(int fd, const char *buf, int len)
{
	const char *loc = buf;
	if(len < 20) {
		return;
	}
	if(strncmp(loc, "GET /", 5) != 0) {
		printf("no GET\n");
		return;
	}
	loc += 5;

	char *found = strstr(loc, " HTTP/1.1");
	if(!found) {
		printf("no HTTP found\n");
		return;
	}

	int file_len = found - loc;
	char file[256];
	memset(file, 0x00, sizeof(file));
	if(file_len > 255) {
		printf("length too long\n");
		return;
	}
	strncpy(file, loc, file_len);
	printf("Searching for: '%s'\n", file);
	loc += file_len;

	if(strncmp(loc, " HTTP/1.1\r\n", 11) != 0) {
		printf("no HTTP\n");
		return;
	}
	if(!strncmp(file, "page.html", 9) || !strncmp(file, "guess2.html", 11)) {
		char file_contents[4096];
		memset(file_contents, 0x00, sizeof(file_contents));
		FILE* html_file;
		if(!strncmp(file, "page.html", 9)) {
			html_file = fopen("sock_index.html", "r");
		} else {
			html_file = fopen("guess2.html", "r");
		}
		int content_length = fread(file_contents, 1, 4096, html_file);
		printf("content length: %d\n", content_length);
		fclose(html_file);

		const char* resp1 = "HTTP/1.1 200 OK\r\n"
				   "Date: Mon, 5 Feb 2018 12:28:53 GMT\r\n"
				   "Server: Simple\r\n"
				   "Last-Modified: Mon, 5 Feb 2018 12:28:53 GMT\r\n"
				   "ETag: \"34aa387-d-1568eb00\"\r\n"
				   "Accept-Ranges: bytes\r\n"
				   "Content-Length: ";
		const char* resp2 = "\r\n"
				   "Vary: Accept-Encoding\r\n"
				   "Content-Type: text/html\r\n"
				   "\r\n";
		write(fd, resp1, strlen(resp1));
		char len_buf[32];
		sprintf(len_buf, "%d", content_length);
		write(fd, len_buf, strlen(len_buf));
		write(fd, resp2, strlen(resp2));
		write(fd, file_contents, content_length);
		write(fd, "\r\n", 2);
	}
	else {
		const char* resp = "HTTP/1.1 200 OK\r\n"
				   "Date: Mon, 5 Feb 2018 12:28:53 GMT\r\n"
				   "Server: Simple\r\n"
				   "Last-Modified: Mon, 5 Feb 2018 12:28:53 GMT\r\n"
				   "ETag: \"34aa387-d-1568eb00\"\r\n"
				   "Accept-Ranges: bytes\r\n"
				   "Content-Length: 51\r\n"
				   "Vary: Accept-Encoding\r\n"
				   "Content-Type: text/plain\r\n"
				   "\r\n"
				   "Hello World! My payload includes a trailing CRLF.\r\n\r\n";
		int resp_len = strlen(resp);
		int ret = write(fd, resp, resp_len);
		printf("Wrote %d/%d bytes.\n", ret, resp_len);
		return;
	}
}

void handle_client(int fd)
{
	for(;;) {
		char buf[1024];
		memset(buf, 0x00, sizeof(buf));
		int ret = read(fd, buf, 1023);
		if(ret == 0) {
			return;
		}
		if(ret == -1) {
			perror("read");
			return;
		}
		printf("%s\n", buf);
		try_handle_http(fd, buf, ret);
	}
}
