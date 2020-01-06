// How to get Logitech F710 address by piotr_go

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <glob.h>

#define VID_LOGITECH		0x046d
#define PID_F710		0xc21f

int main(void){
	unsigned int id;

	glob_t matches;
	if(!glob("/sys/bus/usb/devices/*/serial", 0, NULL, &matches)){
		int i;
		for(i=0; i<matches.gl_pathc; i++){
			char *name = matches.gl_pathv[i];
			char buf[1024];

			*(strrchr(name, '/'))=0;

			FILE *fp;
			uint32_t vid = 0, pid = 0;

			snprintf(buf, sizeof buf, "%s/idVendor", name);
			if((fp = fopen(buf, "r"))){
				int m = fscanf(fp, "%04X", &vid);
				fclose(fp);
				if(m != 1 || vid != VID_LOGITECH) continue;
			}

			snprintf(buf, sizeof buf, "%s/idProduct", name);
			if((fp = fopen(buf, "r"))){
				int m = fscanf(fp, "%04X", &pid);
				fclose(fp);
				if(m != 1 || pid != PID_F710) continue;
			}

			snprintf(buf, sizeof buf, "%s/serial", name);
			if((fp = fopen(buf, "r"))){
				int m = fscanf(fp, "%08X", &id);
				fclose(fp);
				if(m == 1){
					printf("F710 address:   0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", (id>>24)&0xFF, (id>>16)&0xFF, (id>>8)&0xFF, (id>>0)&0xFF, 0x07);
					printf("TX channel: 5\nSpeed: 2 Mbps\nCRC: 2 Bytes\nEnable auto acknowledgement\n");


				}
			}
		}
	}
	globfree(&matches);

	return 0;
}
