/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _XFRM_CONTROLLER_H
#define _XFRM_CONTROLLER_H

#include <atomic>
#include <list>
#include <map>
#include <string>
#include <utility> // for pair

#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/if_tunnel.h>
#include <linux/netlink.h>
#include <linux/udp.h>
#include <linux/xfrm.h>
#include <unistd.h>

#include "NetdConstants.h"
#include "android-base/unique_fd.h"
#include "netdutils/DumpWriter.h"
#include "netdutils/Slice.h"
#include "netdutils/Status.h"
#include "sysutils/SocketClient.h"

namespace android {
namespace net {

// Exposed for testing
extern const uint32_t ALGO_MASK_AUTH_ALL;
// Exposed for testing
extern const uint32_t ALGO_MASK_CRYPT_ALL;
// Exposed for testing
extern const uint32_t ALGO_MASK_AEAD_ALL;
// Exposed for testing
extern const uint8_t REPLAY_WINDOW_SIZE;

// Suggest we avoid the smallest and largest ints
class XfrmMessage;
class TransportModeSecurityAssociation;

class XfrmSocket {
public:
    virtual void close() {
        if (mSock >= 0) {
            ::close(mSock);
        }
        mSock = -1;
    }

    virtual netdutils::Status open() = 0;

    virtual ~XfrmSocket() { close(); }

    // Sends the netlink message contained in iovecs. This populates iovecs[0] with
    // a valid netlink message header.
    virtual netdutils::Status sendMessage(uint16_t nlMsgType, uint16_t nlMsgFlags,
                                          uint16_t nlMsgSeqNum,
                                          std::vector<iovec>* iovecs) const = 0;

protected:
    int mSock;
};

enum struct XfrmDirection : uint8_t {
    IN = XFRM_POLICY_IN,
    OUT = XFRM_POLICY_OUT,
    FORWARD = XFRM_POLICY_FWD,
    MASK = XFRM_POLICY_MASK,
};

enum struct XfrmMode : uint8_t {
    TRANSPORT = XFRM_MODE_TRANSPORT,
    TUNNEL = XFRM_MODE_TUNNEL,
};

enum struct XfrmEncapType : uint16_t {
    NONE = 0,
    ESPINUDP_NON_IKE = UDP_ENCAP_ESPINUDP_NON_IKE,
    ESPINUDP = UDP_ENCAP_ESPINUDP
};

struct XfrmAlgo {
    std::string name;
    std::vector<uint8_t> key;
    uint16_t truncLenBits;
};

struct XfrmEncap {
    XfrmEncapType type;
    uint16_t srcPort;
    uint16_t dstPort;
};

// minimally sufficient structure to match either an SA or a Policy
struct XfrmCommonInfo {
    xfrm_address_t dstAddr; // network order
    xfrm_address_t srcAddr;
    int addrFamily;  // AF_INET or AF_INET6
    int transformId; // requestId
    int spi;
    xfrm_mark mark;
    int xfrm_if_id;
};

struct XfrmSaInfo : XfrmCommonInfo {
    XfrmAlgo auth;
    XfrmAlgo crypt;
    XfrmAlgo aead;
    int netId;
    XfrmMode mode;
    XfrmEncap encap;
};

struct XfrmSpInfo : XfrmSaInfo {
    // Address family in XfrmCommonInfo used for template/SA matching, need separate addrFamily
    // for selectors
    int selAddrFamily;  // AF_INET or AF_INET6
};

/*
 * This is a workaround for a kernel bug in the 32bit netlink compat layer
 * that has been present on x86_64 kernels since 2010 with no fix on the
 * horizon.
 *
 * Below is a redefinition of the xfrm_usersa_info struct that is part
 * of the Linux uapi <linux/xfrm.h> to align the structures to a 64-bit
 * boundary.
 *
 * Note that we turn this on for all x86 32bit targets, under the assumption
 * that nowadays all x86 targets are running 64bit kernels.
 */
#if defined(__i386__)
// Shadow the kernel definition of xfrm_usersa_info with a 64-bit aligned version
struct xfrm_usersa_info : ::xfrm_usersa_info {
} __attribute__((aligned(8)));
// Shadow the kernel's version, using the aligned version of xfrm_usersa_info
struct xfrm_userspi_info {
    struct xfrm_usersa_info info;
    __u32 min;
    __u32 max;
};
struct xfrm_userpolicy_info : ::xfrm_userpolicy_info {
} __attribute__((aligned(8)));

/*
 * Anyone who encounters a failure when sending netlink messages should look here
 * first. Hitting the static_assert() below should be a strong hint that Android
 * IPsec will probably not work with your current settings.
 *
 * Again, experimentally determined, the "flags" field should be the first byte in
 * the final word of the xfrm_usersa_info struct. The check validates the size of
 * the padding to be 7.
 *
 * This padding is verified to be correct on gcc/x86_64 kernel, and clang/x86 userspace.
 */
static_assert(sizeof(::xfrm_usersa_info) % 8 != 0,
              "struct xfrm_usersa_info has changed "
              "alignment. Please consider whether this "
              "patch is needed.");
static_assert(sizeof(xfrm_usersa_info) - offsetof(xfrm_usersa_info, flags) == 8,
              "struct xfrm_usersa_info probably misaligned with kernel struct.");
static_assert(sizeof(xfrm_usersa_info) % 8 == 0,
              "struct xfrm_usersa_info_t is not 64-bit  "
              "aligned. Please consider whether this patch "
              "is needed.");
static_assert(sizeof(::xfrm_userspi_info) - sizeof(::xfrm_usersa_info) ==
                      sizeof(xfrm_userspi_info) - sizeof(xfrm_usersa_info),
              "struct xfrm_userspi_info has changed and does not match the kernel struct.");
static_assert(sizeof(::xfrm_userpolicy_info) % 8 != 0,
              "struct xfrm_userpolicy_info has changed "
              "alignment. Please consider whether this "
              "patch is needed.");
static_assert(sizeof(xfrm_userpolicy_info) - offsetof(xfrm_userpolicy_info, share) == 5,
              "struct xfrm_userpolicy_info probably misaligned with kernel struct.");
static_assert(sizeof(xfrm_userpolicy_info) % 8 == 0,
              "struct xfrm_userpolicy_info is not 64-bit "
              "aligned. Please consider whether this patch "
              "is needed.");
#endif

class XfrmController {
public:
    XfrmController();

    // Initializer to override XFRM-I support for unit-testing purposes
    explicit XfrmController(bool xfrmIntfSupport);

    static netdutils::Status Init();

    static netdutils::Status ipSecSetEncapSocketOwner(int socketFd, int newUid, uid_t callerUid);

    static netdutils::Status ipSecAllocateSpi(int32_t transformId, const std::string& localAddress,
                                              const std::string& remoteAddress, int32_t inSpi,
                                              int32_t* outSpi);

    static netdutils::Status ipSecAddSecurityAssociation(
            int32_t transformId, int32_t mode, const std::string& sourceAddress,
            const std::string& destinationAddress, int32_t underlyingNetId, int32_t spi,
            int32_t markValue, int32_t markMask, const std::string& authAlgo,
            const std::vector<uint8_t>& authKey, int32_t authTruncBits,
            const std::string& cryptAlgo, const std::vector<uint8_t>& cryptKey,
            int32_t cryptTruncBits, const std::string& aeadAlgo,
            const std::vector<uint8_t>& aeadKey, int32_t aeadIcvBits, int32_t encapType,
            int32_t encapLocalPort, int32_t encapRemotePort, int32_t xfrmInterfaceId);

    static netdutils::Status ipSecDeleteSecurityAssociation(int32_t transformId,
                                                            const std::string& sourceAddress,
                                                            const std::string& destinationAddress,
                                                            int32_t spi, int32_t markValue,
                                                            int32_t markMask,
                                                            int32_t xfrmInterfaceId);

    static netdutils::Status ipSecApplyTransportModeTransform(int socketFd, int32_t transformId,
                                                              int32_t direction,
                                                              const std::string& localAddress,
                                                              const std::string& remoteAddress,
                                                              int32_t spi);

    static netdutils::Status ipSecRemoveTransportModeTransform(int socketFd);

    static netdutils::Status ipSecAddSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                                    int32_t direction,
                                                    const std::string& tmplSrcAddress,
                                                    const std::string& tmplDstAddress, int32_t spi,
                                                    int32_t markValue, int32_t markMask,
                                                    int32_t xfrmInterfaceId);

    static netdutils::Status ipSecUpdateSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                                       int32_t direction,
                                                       const std::string& tmplSrcAddress,
                                                       const std::string& tmplDstAddress,
                                                       int32_t spi, int32_t markValue,
                                                       int32_t markMask, int32_t xfrmInterfaceId);

    static netdutils::Status ipSecDeleteSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                                       int32_t direction, int32_t markValue,
                                                       int32_t markMask, int32_t xfrmInterfaceId);

    static netdutils::Status ipSecAddTunnelInterface(const std::string& deviceName,
                                                     const std::string& localAddress,
                                                     const std::string& remoteAddress, int32_t ikey,
                                                     int32_t okey, int32_t interfaceId,
                                                     bool isUpdate);

    static netdutils::Status ipSecRemoveTunnelInterface(const std::string& deviceName);

    void dump(netdutils::DumpWriter& dw);

    // Some XFRM netlink attributes comprise a header, a struct, and some data
    // after the struct. We wrap all of those in one struct for easier
    // marshalling. The structs below must be ABI compatible with the kernel and
    // are composed from kernel structures; thus, they use the kernel naming
    // convention.

    // Exposed for testing
    static constexpr size_t MAX_KEY_LENGTH = 128;

    // Container for the content of an XFRMA_ALG_CRYPT netlink attribute.
    // Exposed for testing
    struct nlattr_algo_crypt {
        nlattr hdr;
        xfrm_algo crypt;
        uint8_t key[MAX_KEY_LENGTH];
    };

    // Container for the content of an XFRMA_ALG_AUTH_TRUNC netlink attribute.
    // Exposed for testing
    struct nlattr_algo_auth {
        nlattr hdr;
        xfrm_algo_auth auth;
        uint8_t key[MAX_KEY_LENGTH];
    };

    // Container for the content of an XFRMA_TMPL netlink attribute.
    // Exposed for testing
    struct nlattr_algo_aead {
        nlattr hdr;
        xfrm_algo_aead aead;
        uint8_t key[MAX_KEY_LENGTH];
    };

    // Exposed for testing
    struct nlattr_user_tmpl {
        nlattr hdr;
        xfrm_user_tmpl tmpl;
    };

    // Container for the content of an XFRMA_ENCAP netlink attribute.
    // Exposed for testing
    struct nlattr_encap_tmpl {
        nlattr hdr;
        xfrm_encap_tmpl tmpl;
    };

    // Container for the content of an XFRMA_MARK netlink attribute.
    // Exposed for testing
    struct nlattr_xfrm_mark {
        nlattr hdr;
        xfrm_mark mark;
    };

    // Container for the content of an XFRMA_OUTPUT_MARK netlink attribute.
    // Exposed for testing
    struct nlattr_xfrm_output_mark {
        nlattr hdr;
        __u32 outputMark;
    };

    // Container for the content of an XFRMA_IF_ID netlink attribute.
    // Exposed for testing
    struct nlattr_xfrm_interface_id {
        nlattr hdr;
        __u32 if_id;
    };

    // Exposed for testing
    struct nlattr_payload_u32 {
        nlattr hdr;
        uint32_t value;
    };

  private:
    static bool isXfrmIntfSupported();

    // helper functions for filling in the XfrmCommonInfo (and XfrmSaInfo) structure
    static netdutils::Status fillXfrmCommonInfo(const std::string& sourceAddress,
                                                const std::string& destinationAddress, int32_t spi,
                                                int32_t markValue, int32_t markMask,
                                                int32_t transformId, int32_t xfrmInterfaceId,
                                                XfrmCommonInfo* info);
    static netdutils::Status fillXfrmCommonInfo(int32_t spi, int32_t markValue, int32_t markMask,
                                                int32_t transformId, int32_t xfrmInterfaceId,
                                                XfrmCommonInfo* info);

    // Top level functions for managing a Transport Mode Transform
    static netdutils::Status addTransportModeTransform(const XfrmSaInfo& record);
    static int removeTransportModeTransform(const XfrmSaInfo& record);

    // TODO(messagerefactor): FACTOR OUT ALL MESSAGE BUILDING CODE BELOW HERE
    // Shared between SA and SP
    static void fillXfrmSelector(const int record, xfrm_selector* selector);

    // Shared between Transport and Tunnel Mode
    static int fillNlAttrXfrmAlgoEnc(const XfrmAlgo& in_algo, nlattr_algo_crypt* algo);
    static int fillNlAttrXfrmAlgoAuth(const XfrmAlgo& in_algo, nlattr_algo_auth* algo);
    static int fillNlAttrXfrmAlgoAead(const XfrmAlgo& in_algo, nlattr_algo_aead* algo);
    static int fillNlAttrXfrmEncapTmpl(const XfrmSaInfo& record, nlattr_encap_tmpl* tmpl);

    // Functions for updating a Transport Mode SA
    static netdutils::Status updateSecurityAssociation(const XfrmSaInfo& record,
                                                       const XfrmSocket& sock);
    static int fillUserSaInfo(const XfrmSaInfo& record, xfrm_usersa_info* usersa);

    // Functions for deleting a Transport Mode SA
    static netdutils::Status deleteSecurityAssociation(const XfrmCommonInfo& record,
                                                       const XfrmSocket& sock);
    static int fillUserSaId(const XfrmCommonInfo& record, xfrm_usersa_id* said);
    static int fillUserTemplate(const XfrmSpInfo& record, xfrm_user_tmpl* tmpl);

    static int fillUserSpInfo(const XfrmSpInfo& record, XfrmDirection direction,
                              xfrm_userpolicy_info* usersp);
    static int fillNlAttrUserTemplate(const XfrmSpInfo& record, nlattr_user_tmpl* tmpl);
    static int fillUserPolicyId(const XfrmSpInfo& record, XfrmDirection direction,
                                xfrm_userpolicy_id* policy_id);
    static int fillNlAttrXfrmMark(const XfrmCommonInfo& record, nlattr_xfrm_mark* mark);
    static int fillNlAttrXfrmOutputMark(const __u32 underlyingNetId,
                                        nlattr_xfrm_output_mark* output_mark);
    static int fillNlAttrXfrmIntfId(const __u32 intf_id_value, nlattr_xfrm_interface_id* intf_id);

    static netdutils::Status allocateSpi(const XfrmSaInfo& record, uint32_t minSpi, uint32_t maxSpi,
                                         uint32_t* outSpi, const XfrmSocket& sock);

    static netdutils::Status processSecurityPolicy(int32_t transformId, int32_t selAddrFamily,
                                                   int32_t direction,
                                                   const std::string& tmplSrcAddress,
                                                   const std::string& tmplDstAddress, int32_t spi,
                                                   int32_t markValue, int32_t markMask,
                                                   int32_t xfrmInterfaceId, int32_t msgType);
    static netdutils::Status updateTunnelModeSecurityPolicy(const XfrmSpInfo& record,
                                                            const XfrmSocket& sock,
                                                            XfrmDirection direction,
                                                            uint16_t msgType);
    static netdutils::Status deleteTunnelModeSecurityPolicy(const XfrmSpInfo& record,
                                                            const XfrmSocket& sock,
                                                            XfrmDirection direction);
    static netdutils::Status flushInterfaces();
    static netdutils::Status flushSaDb(const XfrmSocket& s);
    static netdutils::Status flushPolicyDb(const XfrmSocket& s);

    static netdutils::Status ipSecAddXfrmInterface(const std::string& deviceName,
                                                   int32_t interfaceId, uint16_t flags);
    static netdutils::Status ipSecAddVirtualTunnelInterface(const std::string& deviceName,
                                                            const std::string& localAddress,
                                                            const std::string& remoteAddress,
                                                            int32_t ikey, int32_t okey,
                                                            uint16_t flags);
    // END TODO(messagerefactor)
};

} // namespace net
} // namespace android

#endif /* !defined(XFRM_CONTROLLER_H) */
