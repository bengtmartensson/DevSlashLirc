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
 * Due to limitations in Lirc, this can be instantiated only once, so here it
 * is a singleton class.
 */
public abstract class LircDriver extends LircHardware {

    public static final String defaultDeviceName = "auto";
    public static final String defaultPluginDir = "/usr/local/lib/lirc/plugins";
    public static final String defaultLogfile = "/tmp/lircDriver.log";
    public static final int defaultLoglevel = 9;

    public static native void setupLog(String filename, int loglevel, boolean append);

    public static void setupLog() {
        setupLog(defaultLogfile, defaultLoglevel, false);
    }

    public static native void closeLog();

    public native static void setPluginDir(String pluginPath);

    public static void setPluginDir() {
        setPluginDir(defaultPluginDir);
    }

    protected LircDriver(String deviceName, long nativePointer) {
        super(deviceName, nativePointer);
    }

    protected static Mode2LircDriver instance = null;

    protected native boolean openNative(String deviceName);

    public final void open(String deviceName) throws LircDeviceException {
        boolean status = openNative(deviceName);
        if (!status)
            throw new LircDeviceException("opened failed");
    }

    @Override
    public void open() throws LircDeviceException {
        open(defaultDeviceName);
    }

    @Override
    protected native boolean openNative();

    @Override
    protected native boolean setTransmitterMaskNative(int mask);

    protected native boolean initNative();

    public final void init() throws LircDeviceException {
        boolean status = initNative();
        if (!status)
            throw new LircDeviceException("init failed");
    }

    // ???
    protected native boolean deinitNative();

    public final void deinit() throws LircDeviceException {
        boolean status = deinitNative();
        if (!status)
            throw new LircDeviceException("deinit failed");
    }

    public native boolean isReady();

    @Override
    public native boolean canRec();

    @Override
    public native boolean canSend();

    @Override
    public native boolean canSetTransmitterMask();

    @Override
    public native void close();

    @Override
    public native int getNumberTransmitters();

    @Override
    public native String getVersion();

    @Override
    public native boolean isValid();
}
