/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/logging/log.h>

#include <nrf_rpc.h>

#include <target_comm.h>
#include <tca_transport.h>
#include <tca_nrf_error.h>
#include <tca_zephyr_log_backend.h>

target_comm_t g_target_comm;
const int TCA_LOG_EVENT_TYPE = 100;

LOG_MODULE_REGISTER(nrf_ps_client, CONFIG_NRF_PS_CLIENT_LOG_LEVEL);

static void err_handler(const struct nrf_rpc_err_report *report)
{
	LOG_ERR("nRF RPC error %d ocurred. See nRF RPC logs for more details", report->code);
}

int main(void)
{
	int ret;

	tca_transport_config_t transport_config = TCA_TRANSPORT_CONFIG_DEFAULT;
	tca_init(&g_target_comm, &transport_config);

	#ifdef CONFIG_TCA_LOG_BACKEND
		// disable all backends in zephyr which have autostart enabled
		tca_zephyr_log_disable_all_backends();
		tca_zephyr_log_enable_backend(&g_target_comm, TCA_LOG_EVENT_TYPE, LOG_LEVEL_ERR);
	#endif


	LOG_INF("Initializing RPC client");

	ret = nrf_rpc_init(err_handler);

	if (ret != 0) {
		LOG_ERR("RPC init failed");
	}

	LOG_INF("RPC client ready");

	while (1)
    {
        tca_send_and_receive(&g_target_comm);
        k_yield();
    }

	return 0;
}
