/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef KEYSTORE_PERMISSIONS_H_
#define KEYSTORE_PERMISSIONS_H_

#include <unistd.h>

/* Here are the permissions, actions, users, and the main function. */
enum perm_t {
    P_GET_STATE = 1 << 0,
    P_GET = 1 << 1,
    P_INSERT = 1 << 2,
    P_DELETE = 1 << 3,
    P_EXIST = 1 << 4,
    P_LIST = 1 << 5,
    P_RESET = 1 << 6,
    P_PASSWORD = 1 << 7,
    P_LOCK = 1 << 8,
    P_UNLOCK = 1 << 9,
    P_IS_EMPTY = 1 << 10,
    P_SIGN = 1 << 11,
    P_VERIFY = 1 << 12,
    P_GRANT = 1 << 13,
    P_DUPLICATE = 1 << 14,
    P_CLEAR_UID = 1 << 15,
    P_ADD_AUTH = 1 << 16,
    P_USER_CHANGED = 1 << 17,
    P_GEN_UNIQUE_ID = 1 << 18,
};

const char* get_perm_label(perm_t perm);

/**
 * Returns the UID that the callingUid should act as. This is here for
 * legacy support of the WiFi and VPN systems and should be removed
 * when WiFi can operate in its own namespace.
 */
uid_t get_keystore_euid(uid_t uid);

/**
 * Returns true if the uid/pid/sid has a permission. Checks based on sid if available.
 *
 * sid may be null on older kernels
 */
bool has_permission(uid_t uid, perm_t perm, pid_t spid, const char* sid);

/**
 * Returns true if the callingUid is allowed to interact in the targetUid's
 * namespace.
 */
bool is_granted_to(uid_t callingUid, uid_t targetUid);

int configure_selinux();

/*
 * Keystore grants.
 *
 * What are keystore grants?
 *
 * Keystore grants are a mechanism that allows an app to grant the permission to use one of its
 * keys to an other app.
 *
 * Liftime of a grant:
 *
 * A keystore grant is ephemeral in that is never persistently stored. When the keystore process
 * exits, all grants are lost. Also, grants can be explicitly revoked by the granter by invoking
 * the ungrant operation.
 *
 * What happens when a grant is created?
 *
 * The grant operation expects a valid key alias and the uid of the grantee, i.e., the app that
 * shall be allowed to use the key denoted by the alias. It then makes an entry in the grant store
 * which generates a new alias of the form <alias>_KEYSTOREGRANT_<random_grant_no_>. This grant
 * alias is returned to the caller which can pass the new alias to the grantee. For every grantee,
 * the grant store keeps a set of grants, an entry of which holds the following information:
 *  - the owner of the key by uid, aka granter uid,
 *  - the original alias of the granted key, and
 *  - the random grant number.
 * (See "grant_store.h:class Grant")
 *
 * What happens when a grant is used?
 *
 * Upon any keystore operation that expects an alias, the alias and the caller's uid are used
 * to retrieve a key file. If that fails some operations try to retrieve a key file indirectly
 * through a grant. These operations include:
 *  - attestKey
 *  - begin
 *  - exportKey
 *  - get
 *  - getKeyCharacteristics
 *  - del
 *  - exist
 *  - getmtime
 * Operations that DO NOT follow the grant indirection are:
 *  - import
 *  - generate
 *  - grant
 *  - ungrant
 * Especially, the latter two mean that neither can a grantee transitively grant a granted key
 * to a third, nor can they relinquish access to the key or revoke access to the key by a third.
 */

#endif  // KEYSTORE_PERMISSIONS_H_
