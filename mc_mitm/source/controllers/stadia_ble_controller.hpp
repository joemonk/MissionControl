/*
 * Copyright (c) 2020-2026 ndeadly
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include "stadia_controller.hpp"

namespace ams::controller {

    /*
     * Stadia controller connected over Bluetooth LE (HID over GATT).
     *
     * HOGP strips the report id from notifications; input is re-framed with
     * the Stadia's fixed report id (0x03) and handed to StadiaController::ProcessInputData.
     * The Stadia controller has no output report (no rumble over BLE).
     */
    class StadiaBleController final : public StadiaController {

        public:
            StadiaBleController(bluetooth::Address address, u32 conn_id, const BtdrvGattId &service_id, bool service_primary, const BtdrvGattId &output_report_id)
            : StadiaController(address, {0x18d1, 0x9400})
            , m_conn_id(conn_id)
            , m_service_id(service_id)
            , m_service_primary(service_primary)
            , m_output_report_id(output_report_id) { }

            // Payload of a notification from the HID input Report characteristic
            Result HandleInputReport(const u8 *data, size_t size);

        private:
            u32 m_conn_id;
            BtdrvGattId m_service_id;
            bool m_service_primary;
            BtdrvGattId m_output_report_id;

            bluetooth::HidReportEventInfo m_event_info;

    };

}
