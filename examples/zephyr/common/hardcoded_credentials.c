/*
 * Copyright (c) 2023 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(golioth_hardcoded_credentials, LOG_LEVEL_DBG);

#include "golioth.h"

#ifdef CONFIG_GOLIOTH_AUTH_METHOD_CERT

static const uint8_t tls_client_crt[] = {
#include "golioth-systemclient-crt.inc"
    0x00
};

static const uint8_t tls_client_key[] = {
#include "golioth-systemclient-key.inc"
    0x00
};

static const uint8_t tls_ca_crt[] = {
#include "golioth-systemclient-ca_crt.inc"
    0x00
};

/* Awaiting certificate support in Zephyr port */

/* FIXME: define start/end addresses for PKI files */
golioth_client_config_t _golioth_client_config_psk = {
        .credentials = {
                .auth_type = GOLIOTH_TLS_AUTH_TYPE_PKI,
                .pki = {
                        .ca_cert = tls_ca_crt,
                        .ca_cert_len = sizeof(tls_ca_crt),
                        .public_cert = tls_client_crt,
                        .public_cert_len = sizeof(tls_client_crt),
                        .private_key = tls_client_key,
                        .private_key_len = sizeof(tls_client_key),
                }}};

#else /* Using PSK Authentication */

golioth_client_config_t _golioth_client_config_psk = {
        .credentials = {
                .auth_type = GOLIOTH_TLS_AUTH_TYPE_PSK,
                .psk = {
                        .psk_id = CONFIG_GOLIOTH_SAMPLE_PSK_ID,
                        .psk_id_len = strlen(CONFIG_GOLIOTH_SAMPLE_PSK_ID),
                        .psk = CONFIG_GOLIOTH_SAMPLE_PSK,
                        .psk_len = strlen(CONFIG_GOLIOTH_SAMPLE_PSK),
                }}};
#endif

golioth_client_config_t* hardcoded_credentials_get(void) {
    return &_golioth_client_config_psk;
}