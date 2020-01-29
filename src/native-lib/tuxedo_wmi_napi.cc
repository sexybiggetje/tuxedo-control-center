/*!
 * Copyright (c) 2020 TUXEDO Computers GmbH <tux@tuxedocomputers.com>
 *
 * This file is part of TUXEDO Control Center.
 *
 * TUXEDO Control Center is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TUXEDO Control Center is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TUXEDO Control Center.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <napi.h>
#include "tuxedo_wmi_lib/tuxedo_wmi_api.hh"

using namespace Napi;

Boolean webcamOn(const CallbackInfo &info) {
    TuxedoWmiAPI wmi;
    bool result = wmi.setWebcam(true);
    return Boolean::New(info.Env(), result);
}

Boolean webcamOff(const CallbackInfo &info) {
    TuxedoWmiAPI wmi;
    bool result = wmi.setWebcam(false);
    return Boolean::New(info.Env(), result);
}

Boolean setFanAuto(const CallbackInfo &info) {
    if (info.Length() != 1 || !info[0].IsNumber()) return Boolean::New(info.Env(), false);
    int fanNumber = info[0].As<Number>();
    if (fanNumber < 1 || fanNumber > 4) return Boolean::New(info.Env(), false);

    bool result = false; // set_fan_auto(fanNumber);

    return Boolean::New(info.Env(), result);
}

Boolean setFanSpeedPercent(const CallbackInfo &info) {
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) return Boolean::New(info.Env(), false);
    int fanNumber = info[0].As<Number>();
    int speedPercent = info[1].As<Number>();
    if (fanNumber < 1 || fanNumber > 4) return Boolean::New(info.Env(), false);
    if (speedPercent < 0 || speedPercent > 100) return Boolean::New(info.Env(), false);

    bool result = false; // set_fan_speed(fanNumber, speedPercent);

    return Boolean::New(info.Env(), result);
}

Number getFanSpeedPercent(const CallbackInfo &info) {
    if (info.Length() != 1 || !info[0].IsNumber()) return Number::New(info.Env(), -1);
    int fanNumber = info[0].As<Number>();
    if (fanNumber < 1 || fanNumber > 4) return Number::New(info.Env(), -1);

    int fanSpeedPercent = 1; // get_fan_speed_percent(fanNumber);

    return Number::New(info.Env(), fanSpeedPercent);
}

Number getFanTemperature(const CallbackInfo &info) {
    if (info.Length() != 1 || !info[0].IsNumber()) return Number::New(info.Env(), -1);
    int fanNumber = info[0].As<Number>();
    if (fanNumber < 1 || fanNumber > 4) return Number::New(info.Env(), -1);

    int fanTemperature = 1; //get_fan_temperature(fanNumber);

    return Number::New(info.Env(), fanTemperature);
}

Object Init(Env env, Object exports) {
    // Webcam
    exports.Set(String::New(env, "webcamOn"), Function::New(env, webcamOn));
    exports.Set(String::New(env, "webcamOff"), Function::New(env, webcamOff));

    // Fan control
    exports.Set(String::New(env, "setFanAuto"), Function::New(env, setFanAuto));
    exports.Set(String::New(env, "setFanSpeedPercent"), Function::New(env, setFanSpeedPercent));
    exports.Set(String::New(env, "getFanSpeedPercent"), Function::New(env, getFanSpeedPercent));
    exports.Set(String::New(env, "getFanTemperature"), Function::New(env, getFanTemperature));

    return exports;
}

NODE_API_MODULE(TuxedoWMIAPI, Init);
