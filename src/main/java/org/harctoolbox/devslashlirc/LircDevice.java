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
 * This class is an abstract class for hardware communicating through /dev/lirc.
 * <p>
 * As opposed to standard C/Lirc, where a device is opened, and it "turns out"
 * that it is "mode2" or "lirccode", here the user has to decide if he wants
 * a mode2 or lirccode device, and open it with the appropriate class. This will
 * fail if the connected hardware does not have the expected properties.
 * <p>
 * This class (and its derived classes) can be instantiated several times,
 * but of course only one can open a particular device file.
 */
public abstract class LircDevice extends LircHardware {
    public static final String defaultDeviceName = "/dev/lirc0";
    public static final int  defaultBeginTimeout = 5000;

    public LircDevice(String deviceName, int beginTimeout, long nativePointer) {
        super(deviceName, nativePointer);
        setBeginTimeout(beginTimeout);
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
    public final native void setBeginTimeout(int timeout);

    @Override
    public String toString() {
        return //"deviceName=" + deviceName
                  (canSend() ? " send" : "")
                + (canRec() ? " rec." : "")
                + (canSetTransmitterMask() ? " setTM" : "")
                + " #xmtrs=" + getNumberTransmitters();
    }

    /**
     * Invoke the destructor from Java.
     */
    public native void delete();

    @Override
    protected void finalize() throws Throwable {
        close();
        delete();
        super.finalize();

    }
}
