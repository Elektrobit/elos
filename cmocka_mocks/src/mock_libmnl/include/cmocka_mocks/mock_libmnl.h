// SPDX-License-Identifier: MIT

#ifndef __MOCK_LIBMNL_H__
#define __MOCK_LIBMNL_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_extensions/mock_func_wrap.h>
#include <libmnl/libmnl.h>

MOCK_FUNC_PROTOTYPE(mnl_socket_open, struct mnl_socket *, int bus)
MOCK_FUNC_PROTOTYPE(mnl_socket_bind, int, struct mnl_socket *nl, unsigned int groups, pid_t pid)
MOCK_FUNC_PROTOTYPE(mnl_socket_close, int, struct mnl_socket *nl)
MOCK_FUNC_PROTOTYPE(mnl_socket_recvfrom, ssize_t, const struct mnl_socket *nl, void *buf, size_t siz)
MOCK_FUNC_PROTOTYPE(mnl_socket_sendto, ssize_t, const struct mnl_socket *nl, const void *req, size_t siz)
MOCK_FUNC_PROTOTYPE(mnl_socket_get_portid, unsigned int, struct mnl_socket *nl)

#endif /* __MOCK_LIBMNL_H__ */
