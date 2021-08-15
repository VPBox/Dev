#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <pwd.h>
#define LOG_TAG "RILDEBUG"
#define LOG_NDDEBUG 0
#include <utils/Log.h>
#include <cutils/sockets.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "socketproxy.h"
#include <private/android_filesystem_config.h>
#include <getopt.h>

const char *debug_path = "/dev/socket";
const char *debug_sock_name_ril = "ril_debug";
const char *debug_sock_name_oem = "oem_debug";
const char *debug_sock_name_urs = "urs_debug";
const char *debug_sock_name_ims = "ims_debug";
const char *debug_sock_name_ul  = "ul_debug";
const char *debug_sock_name_qrc = "qrc_debug";
const char *debug_sock_name_urc = "urc_debug";


char buff_debug[] = {0, 0, 0, 5, 'd', 'e', 'b', 'u', 'g'};

int get_debug_info(const char *path, const char *name, const char *processname, int index)
{
	int fd;
	uint32_t header;
	int datasize;
	struct debug_info d_info;
	int ret = 0;

	fd = connect_debug_server(path, name, index);
	if (fd < 0) {
		printf("connect debug server error, please try again later.\n");
		return -1;
	}

	send_buf_to_socket(fd, buff_debug, sizeof(buff_debug));

	ret = recv_len_data(fd, (char *)&header, 4);
	if (ret < 0) {
		printf("recv_len_data error errno:%d strerror:%s\n", errno, strerror(errno));
		goto error;
	}

	datasize = ntohl(header);

	ret = recv_len_data(fd, (char *)&d_info, datasize);
	if (ret < 0) {
		printf("recv_len_data error errno:%d strerror:%s\n", errno, strerror(errno));
		goto error;
	}

	printf("--------------------------------process name:%s sim:%d pid:%d-----------------------------------------\n", processname, (index+1), d_info.sdi.pid);
	printf("name   \tstatus\tfd\tpkg_count\trx_count\ttx_count\tdisconnect_count\tlast_time\n");
	printf("server \t%d    \t%d\t%d       \t%d      \t%d      \t%d              \t\t%s\n",
		d_info.sdi.server_status, d_info.sdi.server_fd, d_info.sdi.server_pkg, d_info.sdi.server_rx, d_info.sdi.server_tx, d_info.sdi.server_disc_count, ctime(&d_info.sdi.last_time));
	printf("client1\t%d     \t%d\t%d      \t%d      \t%d      \t%d              \t\t%s\n",
		d_info.cdi[0].client_status, d_info.cdi[0].client_fd, d_info.cdi[0].client_pkg, d_info.cdi[0].client_rx, d_info.cdi[0].client_tx, d_info.cdi[0].client_disc_count, ctime(&d_info.cdi[0].last_time));
	printf("client2\t%d     \t%d\t%d      \t%d      \t%d      \t%d              \t\t%s\n",
		d_info.cdi[1].client_status, d_info.cdi[1].client_fd, d_info.cdi[1].client_pkg, d_info.cdi[1].client_rx, d_info.cdi[1].client_tx, d_info.cdi[1].client_disc_count, ctime(&d_info.cdi[1].last_time));

error:
	close(fd);

	return ret;
}

static void print_usage(const char *name)
{
	static const char *usage = "\
    -d             Default display rilproxyd debug info\n\
    -a             Display all debug info \n\
    --ril          Display rilproxyd debug info\n\
    --oem          Display riloemproxyd debug info\n\
    --ims          Display rilimsproxyd debug info\n\
    --urs          Display rilursproxyd debug info\n\
    --ul           Display rilulproxyd debug info\n\
    --qrc          Display rilqrcproxyd debug info\n\
    --urc          Display rilurcproxyd debug info\n\
    -h             Help\n";

	printf("Usage: %s [options]\n", name);
	printf("%s", usage);
}

static struct option opts[] = {
	{ "ril",	no_argument,		NULL, 1},
	{ "oem",	no_argument,		NULL, 2},
	{ "ims",	no_argument,		NULL, 3},
	{ "urs",	no_argument,		NULL, 4},
	{ "ul",	    no_argument,		NULL, 5},
	{ "qrc",	no_argument,		NULL, 6},
	{ "urc",	no_argument,		NULL, 7},
	{ NULL, 	0,			        NULL, 0}
};


int main(int argc, char **argv)
{
	int ret = 0;
	int c;
	int ril = 0;
	int oem = 0;
	int ims = 0;
	int urs = 0;
	int ul  = 0;
	int qrc = 0;
	int urc = 0;
	int all = 0;

	if (argc < 2) {
        print_usage(argv[0]);
        exit(0);
    }

	while ((c = getopt_long(argc, argv, "adh", opts, NULL)) != -1) {
		switch (c) {
			case 'a':
				all = 1;
				break;
			case 'd':
				ril = 1;
				break;
			case 1:
				ril = 1;
				break;
			case 2:
				oem = 1;
				break;
			case 3:
				ims = 1;
				break;
			case 4:
				urs = 1;
				break;
			case 5:
				ul = 1;
				break;
			case 6:
				qrc = 1;
				break;
			case 7:
				urc = 1;
				break;
			case 'h':
				print_usage(argv[0]);
				exit(0);
			case ':':
				printf("Option %c needs a value", optopt);
			default:
				print_usage(argv[0]);
				exit(1);
		}
	}

	if (all) {
		ril=oem=ims=urs=ul=qrc=urc=1;
	}

	if (ril) {
		ret += get_debug_info(debug_path, debug_sock_name_ril, "rilproxyd", 0);
		ret += get_debug_info(debug_path, debug_sock_name_ril, "rilproxyd", 1);
	}
	
	if (oem) {
		ret += get_debug_info(debug_path, debug_sock_name_oem, "riloemproxyd", 0);
		ret += get_debug_info(debug_path, debug_sock_name_oem, "riloemproxyd", 1);
	}

	if (ims) {
		ret += get_debug_info(debug_path, debug_sock_name_ims, "rilimsproxyd", 0);
		ret += get_debug_info(debug_path, debug_sock_name_ims, "rilimsproxyd", 1);
	}

	if (urs) {
		ret += get_debug_info(debug_path, debug_sock_name_urs, "rilursproxyd", 0);
		ret += get_debug_info(debug_path, debug_sock_name_urs, "rilursproxyd", 1);
	}

	if (ul) {
		ret += get_debug_info(debug_path, debug_sock_name_ul, "rilulproxyd", 0);
		ret += get_debug_info(debug_path, debug_sock_name_ul, "rilulproxyd", 1);
	}

	if (qrc) {
		ret += get_debug_info(debug_path, debug_sock_name_qrc, "rilqrcproxyd", 0);
		ret += get_debug_info(debug_path, debug_sock_name_qrc, "rilqrcproxyd", 1);
	}

	if (urc) {
		ret += get_debug_info(debug_path, debug_sock_name_urc, "rilurcproxyd", 0);
		ret += get_debug_info(debug_path, debug_sock_name_urc, "rilurcproxyd", 1);
	}

	return ret;
}
