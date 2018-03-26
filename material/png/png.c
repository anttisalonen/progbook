#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (16 * 1024 * 1024)

/* crc - from rfc2083 */

/* Table of CRCs of all 8-bit messages. */
unsigned long crc_table[256];

/* Flag: has the table been computed? Initially false. */
int crc_table_computed = 0;

/* Make the table for a fast CRC. */
void make_crc_table(void)
{
	unsigned long c;
	int n, k;
	for (n = 0; n < 256; n++) {
		c = (unsigned long) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)). */

unsigned long update_crc(unsigned long crc, const char *buf,
		int len)
{
	unsigned long c = crc;
	int n;

	if (!crc_table_computed)
		make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned long calculate_crc(const char *buf, int len)
{
	return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

/* png handling */

struct png_data {
	int color_type;
};

typedef void (*handler_ptr)(const char *buf, int len, struct png_data *png);

struct handler {
	const char  *type;
	handler_ptr func;
};

void validate(int val, const char *msg)
{
	if(!val) {
		fprintf(stderr, "Invalid file: %s\n", msg);
		exit(1);
	}
}

int get_2_byte_big_endian(const char *buf)
{
	return ((unsigned char)buf[0] << 8) |
	       (unsigned char)buf[1];
}

int get_big_endian(const char *buf)
{
	return ((unsigned char)buf[0] << 24) |
	       ((unsigned char)buf[1] << 16) | 
	       ((unsigned char)buf[2] << 8) | 
	       (unsigned char)buf[3];
}

void header_handler(const char *buf, int len, struct png_data *png)
{
	validate(len == 13, "header must be 13 bytes");
	printf("Width:              %d\n", get_big_endian(buf));
	printf("Height:             %d\n", get_big_endian(buf + 4));
	printf("Bit depth:          %d\n", (unsigned char)buf[8]);
	png->color_type = (unsigned char)buf[9];
	printf("Color type:         %d\n", png->color_type);
	printf("Compression method: %d\n", (unsigned char)buf[10]);
	printf("Filter method:      %d\n", (unsigned char)buf[11]);
	printf("Interlace method:   %d\n", (unsigned char)buf[12]);
}

void background_handler(const char *buf, int len, struct png_data *png)
{
	switch(png->color_type) {
		case 3:
			validate(len == 1, "color type 3 must have 1 byte");
			printf("Palette index: %d\n", (unsigned char)buf[0]);
			break;

		case 0:
		case 4:
			validate(len == 2, "color type 0/4 must have 2 bytes");
			printf("Gray: %d\n", get_2_byte_big_endian(buf));
			break;

		case 2:
		case 6:
			validate(len == 6, "color type 2/6 must have 6 bytes");
			printf("R: %d\n", get_2_byte_big_endian(buf));
			printf("G: %d\n", get_2_byte_big_endian(buf + 2));
			printf("B: %d\n", get_2_byte_big_endian(buf + 4));
			break;

		default:
			printf("Unknown background color type\n");
			break;
	}
}

void phys_handler(const char *buf, int len, struct png_data *png)
{
	validate(len == 9, "phys len==9");
	printf("pixels/unit, x: %d\n", get_big_endian(buf));
	printf("pixels/unit, y: %d\n", get_big_endian(buf + 4));
	printf("unit:           %d\n", (unsigned char)buf[8]);
}

void time_handler(const char *buf, int len, struct png_data *png)
{
	validate(len == 7, "time len==7");
	printf("Year:   %d\n", get_2_byte_big_endian(buf));
	printf("Month:  %d\n", (unsigned char)buf[2]);
	printf("Day:    %d\n", (unsigned char)buf[3]);
	printf("Hour:   %d\n", (unsigned char)buf[4]);
	printf("Minute: %d\n", (unsigned char)buf[5]);
	printf("Second: %d\n", (unsigned char)buf[6]);
}

const struct handler handlers[] = {
	{ "IHDR", header_handler },
	{ "bKGD", background_handler },
	{ "pHYs", phys_handler },
	{ "tIME", time_handler },
	{ NULL,   NULL }
};

void check_header(const char *buf)
{
	validate((unsigned char)buf[0] == 0x89, "header byte 1");
	validate((unsigned char)buf[1] == 'P', "header byte 2");
	validate((unsigned char)buf[2] == 'N', "header byte 3");
	validate((unsigned char)buf[3] == 'G', "header byte 4");
	validate((unsigned char)buf[4] == '\r', "header byte 5");
	validate((unsigned char)buf[5] == '\n', "header byte 6");
	validate((unsigned char)buf[6] == 0x1a, "header byte 7");
	validate((unsigned char)buf[7] == '\n', "header byte 8");
}

void check_crc(const char *crcbuf, const char *buf, int len)
{
	int found_crc = get_big_endian(crcbuf);
	int calc_crc = calculate_crc(buf, len);
	printf("CRC correct: %d\n", found_crc == calc_crc);
}

int main(int argc, char **argv)
{
	if(argc != 2) {
		printf("Usage: %s <png file>\n", argv[0]);
		return 1;
	}
	char *buf = (char *)malloc(MAX_SIZE);
	if(!buf) {
		fprintf(stderr, "Couldn't allocate memory\n");
		return 1;
	}
	FILE *f = fopen(argv[1], "r");
	if(!f) {
		perror("fopen");
		free(buf);
		return 1;
	}
	int size = fread(buf, 1, MAX_SIZE, f);
	if(size < 8) {
		fprintf(stderr, "Too small file\n");
		fclose(f);
		free(buf);
		return 1;
	}
	check_header(buf);
	int pos = 8;
	struct png_data png;
	memset(&png, 0x00, sizeof(png));
	while(pos + 12 <= size) {
		char lenbuf[4];
		memcpy(lenbuf, buf + pos, 4);
		int len = get_big_endian(lenbuf);
		char chunkbuf[5];
		chunkbuf[4] = 0;
		memcpy(chunkbuf, buf + pos + 4, 4);
		printf("chunk: %s - len: %d (%d)\n", chunkbuf, len, size - (pos + len + 12));
		for(int i = 0; handlers[i].type != NULL; i++) {
			if(!strcmp(chunkbuf, handlers[i].type)) {
				handlers[i].func(buf + pos + 8, len, &png);
				break;
			}
		}
		check_crc(buf + pos + 8 + len, buf + pos + 4, len + 4);
		pos += 12 + len;
	}
	fclose(f);
	free(buf);
	return 0;
}
