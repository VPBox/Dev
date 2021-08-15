/*
 * socket_common.h
 *
 * Common socket routines used by cell and celld
 *
 * Copyright (C) 2010-2013 Columbia University
 * Authors: Christoffer Dall <cdall@cs.columbia.edu>
 *		  Jeremy C. Andrus <jeremya@cs.columbia.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */
#ifndef SOCKET_COMMON_H
#define SOCKET_COMMON_H

#include <fcntl.h>
#include <libgen.h>

struct socket_client {
	char root_path[PATH_MAX];
	int init_pid;
	void *data;

	struct socket_client *next;
	struct socket_client *prev;
};

struct client_list {
	struct socket_client *head;
	struct socket_client *tail;
	pthread_mutex_t mutex;
};

/*
 * Adds a client to the list of active possible containers that may start
 * sending socket control requests.
 */
static inline void add_socket_client(struct socket_client *client,
					 struct client_list *list)
{
	pthread_mutex_lock(&list->mutex);
	client->next = client->prev = NULL;
	if (list->tail == NULL)
		list->head = list->tail = client;
	else {
		client->prev = list->tail;
		list->tail->next = client;
		list->tail = client;
	}
	pthread_mutex_unlock(&list->mutex);
}

/*
 * Find the client in the list of registered clients.
 * Return NULL if none is found.
 */
static inline struct socket_client *find_socket_client(char *root_path,
							   struct client_list *list)
{
	struct socket_client *ptr;

	pthread_mutex_lock(&list->mutex);
	ptr = list->head;
	while (ptr != NULL) {
		if (strncmp(root_path, ptr->root_path, PATH_MAX) == 0)
			break;
		ptr = ptr->next;
	}
	pthread_mutex_unlock(&list->mutex);
	return ptr;
}

/*
 * Remove the client from the list of registered clients
 */
static inline void del_socket_client(struct socket_client *client,
					 struct client_list *list)
{
	if (client != NULL) {
		pthread_mutex_lock(&list->mutex);
		if (client->prev != NULL)
			client->prev->next = client->next;
		if (client->next != NULL)
			client->next->prev = client->prev;

		if (list->head == client)
			list->head = client->next;
		if (list->tail == client)
			list->tail = client->prev;
		pthread_mutex_unlock(&list->mutex);
	}
}

/*
 * Create a unix socket and bind to it.
 */
static inline int cell_socket(char *path)
{
	struct sockaddr_un local;
	size_t len;
	int sock;

	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return -1;
	}

	fcntl(sock, F_SETFD, FD_CLOEXEC);

	local.sun_family = AF_UNIX;
	strncpy(local.sun_path, path, sizeof(local.sun_path));
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(sock, (struct sockaddr *)&local, len) == -1) {
		return -1;
	}

	return sock;
}

/*
 * Look at the registered list of clients and ask celld for each of them if the
 * client is the active_cell - return the client if that is the case.
 *
 * TODO: This suggests that the data in socket_client could go in the cell_node
 * and some funcionality be consolidated.
 */
static inline struct socket_client*
active_socket_client(struct client_list *list)
{
	struct socket_client *client;
	struct cell_node *node;

	pthread_mutex_lock(&list->mutex);

	client = list->head;
	while (client != NULL) {
		node = search_cells_path(client->root_path);
		if (node == active_cell)
			break;
		client = client->next;
	}

	pthread_mutex_unlock(&list->mutex);
	return client;
}

/*
 * Map a socket client (cell instance) to a client ID 
 */
static inline int socket_client_id(struct socket_client *client)
{
	struct cell_node *cell;
	if (!client)
		return -1;

	cell = search_cells_path(client->root_path);
	if (cell == NULL)
		return -1;
	return cell->id;
}

/*
 * Obtains the cell ID from its configuration file
 *
 * This lookup is what effectively chooses a cell into which an incoming call
 * or SMS, etc. will be placed. The digit must correspond to the digit
 * appended to the caller ID from the calling service
 */
static inline struct socket_client *
socket_client_from_id(int cellID, struct client_list *list)
{
	struct socket_client *client;
	char cellID_str[0];

	cellID_str[1] = 0;
	pthread_mutex_lock(&list->mutex);

	client = list->head;
	while (client != NULL) {
		int cid = socket_client_id(client);
		if (cid == cellID)
			break;
		client = client->next;
	}

	pthread_mutex_unlock(&list->mutex);
	return client;
}

static size_t number_of_clients(struct client_list *list)
{
	struct socket_client *client;
	size_t cnt = 0;

	pthread_mutex_lock(&list->mutex);

	client = list->head;
	while (client != NULL) {
		cnt++;
		client = client->next;
	}

	pthread_mutex_unlock(&list->mutex);
	return cnt;

}

#endif
