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

import java.io.Closeable;

/**
 * An abstract base class for Lirc Drivers and Devices
 */
public abstract class LircHardware implements Closeable {
    static {
//        System.load("/usr/local/lib/liblirc.so.0.0.0");
//        System.load("/usr/local/lib/liblirc_client.so.0.4.0");
//        System.load("/usr/local/lib/liblirc_driver.so.0.0.0");
//          System.loadLibrary("lirc_driver");
//          System.loadLibrary("lirc");


        System.loadLibrary("devslashlirc");

//        System.loadLibrary("lirc_client");
//        System.loadLibrary("lirc_driver");
    }

    protected long nativePointer;
    protected String deviceName;
    //protected int numberTransmitters = -1;

    private LircHardware() {
        this.nativePointer = 0;
    }

    protected LircHardware(String deviceName, long nativePointer) {
        this.nativePointer = nativePointer;
        this.deviceName = deviceName;
    }

    public void open() throws LircDeviceException {
        boolean status = openNative();
        if (!status)
            throw new LircDeviceException("open failed");
    }

    //public abstract void setTransmitterMask(int mask) throws NotSupportedException, NonExistentTransmitterException, LircDeviceException;

    public final void setTransmitterMask(int mask) throws NotSupportedException, NonExistentTransmitterException, LircDeviceException {
        if (!canSetTransmitterMask())
            throw new NotSupportedException("Setting transmitter mask not supported");
        int noTransmitters = getNumberTransmitters();

        // If noTransmitters == 0, probably it is just incorrectly reported;
        // ignore this "information".
        if (noTransmitters > 0 && (mask >= (1 << noTransmitters)))
            throw new NonExistentTransmitterException("No such transmitter");
        boolean result = setTransmitterMaskNative(mask);
        if (!result)
            throw new LircDeviceException("Error setting transmitters");
    }

    public abstract int getNumberTransmitters();

    protected abstract boolean setTransmitterMaskNative(int mask);

    protected abstract boolean openNative();

    //@Override
    //public abstract void close();

    public abstract String getVersion();

    public abstract boolean canSend();

    public abstract boolean canSetTransmitterMask();

    public abstract boolean canRec();

    public abstract boolean isValid();

    @Override
    public abstract String toString();
}
