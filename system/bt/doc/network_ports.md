Network Ports
===
This document lists all of the network ports used by the bluetooth stack.
It should be used as a reference and as a mechanism to avoid port
namespace conflicts.

* TCP 8872 (hci/src/btsnoop_net.cc) - read live btsnoop logs
* TCP 8873 (hci/src/hci_inject.cc) - inject HCI packets into HCI stream
