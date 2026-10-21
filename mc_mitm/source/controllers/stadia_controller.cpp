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
#include "stadia_controller.hpp"
#include <stratosphere.hpp>

namespace ams::controller {

    namespace {

        constexpr u8 TriggerMax = 0xff;

    }

    void StadiaController::ProcessInputData(const bluetooth::HidReport *report) {
        auto stadia_report = reinterpret_cast<const StadiaReportData *>(&report->data);

        switch (stadia_report->id) {
            case 0x03:
                this->MapInputReport0x03(stadia_report);
                break;
            default:
                break;
        }
    }

    void StadiaController::MapInputReport0x03(const StadiaReportData *src) {
        auto dpad = static_cast<StadiaDpadDirection>(src->input0x03.dpad);

        m_left_stick  = PackAnalogStickValues(src->input0x03.left_stick.x,  InvertAnalogStickValue(src->input0x03.left_stick.y));
        m_right_stick = PackAnalogStickValues(src->input0x03.right_stick.x, InvertAnalogStickValue(src->input0x03.right_stick.y));

        m_buttons.ZL = src->input0x03.left_trigger  > (m_trigger_threshold * TriggerMax);
        m_buttons.ZR = src->input0x03.right_trigger > (m_trigger_threshold * TriggerMax);

        m_buttons.dpad_up    = (dpad == StadiaDpad_N)  || (dpad == StadiaDpad_NE) || (dpad == StadiaDpad_NW);
        m_buttons.dpad_down  = (dpad == StadiaDpad_S)  || (dpad == StadiaDpad_SE) || (dpad == StadiaDpad_SW);
        m_buttons.dpad_left  = (dpad == StadiaDpad_W)  || (dpad == StadiaDpad_NW) || (dpad == StadiaDpad_SW);
        m_buttons.dpad_right = (dpad == StadiaDpad_E)  || (dpad == StadiaDpad_NE) || (dpad == StadiaDpad_SE);

        // Face buttons: map by position to match Switch convention (A=right, B=bottom, X=top, Y=left)
        m_buttons.A = src->input0x03.buttons1.b;
        m_buttons.B = src->input0x03.buttons1.a;
        m_buttons.X = src->input0x03.buttons1.y;
        m_buttons.Y = src->input0x03.buttons1.x;

        m_buttons.R = src->input0x03.buttons1.rb;
        m_buttons.L = src->input0x03.buttons1.lb;

        m_buttons.lstick_press = src->input0x03.buttons0.lstick;
        m_buttons.rstick_press = src->input0x03.buttons1.rstick;

        m_buttons.plus    = src->input0x03.buttons0.start;
        m_buttons.minus   = src->input0x03.buttons0.select;
        m_buttons.home    = src->input0x03.buttons0.home;
        m_buttons.capture = src->input0x03.buttons0.capture;
    }

}
