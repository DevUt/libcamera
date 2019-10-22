/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * ipa_proxy_linux_worker.cpp - Default Image Processing Algorithm proxy worker for Linux
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>

#include <ipa/ipa_interface.h>
#include <libcamera/event_dispatcher.h>
#include <libcamera/logging.h>

#include "ipa_module.h"
#include "ipc_unixsocket.h"
#include "log.h"
#include "thread.h"
#include "utils.h"

using namespace libcamera;

LOG_DEFINE_CATEGORY(IPAProxyLinuxWorker)

void readyRead(IPCUnixSocket *ipc)
{
	IPCUnixSocket::Payload message;
	int ret;

	ret = ipc->receive(&message);
	if (ret) {
		LOG(IPAProxyLinuxWorker, Error)
			<< "Receive message failed: " << ret;
		return;
	}

	LOG(IPAProxyLinuxWorker, Debug) << "Received a message!";
}

int main(int argc, char **argv)
{
	/* Uncomment this for debugging. */
#if 0
	std::string logPath = "/tmp/libcamera.worker." +
			      std::to_string(getpid()) + ".log";
	logSetFile(logPath.c_str());
#endif

	if (argc < 3) {
		LOG(IPAProxyLinuxWorker, Debug)
			<< "Tried to start worker with no args";
		return EXIT_FAILURE;
	}

	int fd = std::stoi(argv[2]);
	LOG(IPAProxyLinuxWorker, Debug)
		<< "Starting worker for IPA module " << argv[1]
		<< " with IPC fd = " << fd;

	std::unique_ptr<IPAModule> ipam = utils::make_unique<IPAModule>(argv[1]);
	if (!ipam->isValid() || !ipam->load()) {
		LOG(IPAProxyLinuxWorker, Error)
			<< "IPAModule " << argv[1] << " should be valid but isn't";
		return EXIT_FAILURE;
	}

	IPCUnixSocket socket;
	if (socket.bind(fd) < 0) {
		LOG(IPAProxyLinuxWorker, Error) << "IPC socket binding failed";
		return EXIT_FAILURE;
	}
	socket.readyRead.connect(&readyRead);

	struct ipa_context *ipac = ipam->createContext();
	if (!ipac) {
		LOG(IPAProxyLinuxWorker, Error) << "Failed to create IPA context";
		return EXIT_FAILURE;
	}

	LOG(IPAProxyLinuxWorker, Debug) << "Proxy worker successfully started";

	/* \todo upgrade listening loop */
	EventDispatcher *dispatcher = Thread::current()->eventDispatcher();
	while (1)
		dispatcher->processEvents();

	ipac->ops->destroy(ipac);

	return 0;
}
