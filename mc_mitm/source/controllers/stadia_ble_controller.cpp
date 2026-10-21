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
#include "stadia_ble_controller.hpp"
#include <stratosphere.hpp>

namespace ams::controller {

    namespace {

        // HOGP strips report ids from notifications; Stadia uses report id 0x03
        constexpr u8 InputReportId = 0x03;

    }

    Result StadiaBleController::HandleInputReport(const u8 *data, size_t size) {
        // Only the 9.0.0+ event layout is filled in; BLE controllers only appeared after that
        R_UNLESS(hos::GetVersion() >= hos::Version_9_0_0, sf::ResultNotSupported());

        auto &event = m_event_info.data_report.v9;
        R_UNLESS(size + 1 <= sizeof(event.report.data), svc::ResultInvalidSize());

        event.res = 0;
        event.proto_mode = 0;
        event.addr = m_address;
        event.pad = 0;
        event.report.size = size + 1;
        event.report.data[0] = InputReportId;
        std::memcpy(&event.report.data[1], data, size);

        R_RETURN(StadiaController::HandleDataReportEvent(&m_event_info));
    }

}
