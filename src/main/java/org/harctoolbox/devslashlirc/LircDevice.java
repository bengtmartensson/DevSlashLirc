/*
Copyright (C) 2016 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

package org.harctoolbox.devslashlirc;

/**
 *
 */
public abstract class LircDevice extends LircHardware {
    public static final String defaultDeviceName = "/dev/lirc0";

    public LircDevice(String deviceName, long nativePointer) {
        super(deviceName, nativePointer);
    }

    @Override
    protected native boolean setTransmitterMaskNative(int mask);

    @Override
    public native void close();

    @Override
    public native String getVersion();

    @Override
    public native int getNumberTransmitters();

    @Override
    public native boolean canSend();

    @Override
    public native boolean canSetTransmitterMask();

    @Override
    public native boolean canRec();

    @Override
    public native boolean isValid();

    @Override
    public String toString() {
        return "deviceName=" + deviceName
                + (canSend() ? " send" : "")
                + (canSetTransmitterMask() ? " setTransmitterMask" : "")
                + (canRec() ? " record" : "")
                + ", # transmitters = " + getNumberTransmitters();
    }
}
