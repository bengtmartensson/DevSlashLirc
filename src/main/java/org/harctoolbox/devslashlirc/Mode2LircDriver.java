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

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Note: this class does not attempt at "normalizing" the idiosyncrasies of
 * the Lirc driver API.
 */
public class Mode2LircDriver extends LircDriver implements IMode2 {

    public static final int defaultFrequency = 38000;

    private static native long newInstance(String name, String fileName);

    private static native long newInstance(String name);

    private Mode2LircDriver(String name, long nativePointer) {
        super(name, nativePointer);
    }

    private Mode2LircDriver(String name) {
        this(name, newInstance(name));
    }

    private Mode2LircDriver(String name, String fileName) {
        this(name, newInstance(name, fileName));
    }

    // TODO syncronized...
    public static Mode2LircDriver newMode2LircDriver(String name, String fileName) {
        if (instance != null)
            return null;
        // RACE cndtn!!
        instance = new Mode2LircDriver(name, fileName);
        return instance;
    }

    public static Mode2LircDriver newMode2LircDriver(String name) {
        if (instance != null)
            return null;

        instance = new Mode2LircDriver(name);
        return instance.nativePointer == 0 ? null : instance;
    }

    @Override
    public native boolean canGetRecResolution();

    // Questionable
    @Override
    public native boolean canSetSendCarrier();

    private native void setSendCarrierNative(int frequency);

    @Override
    public void setSendCarrier(int frequency) throws NotSupportedException {
        if (!canSetSendCarrier())
            throw new NotSupportedException("setSendCarrier not supported by driver");
        setSendCarrierNative(frequency);
    }

    @Override
    public native int getRecResolution();

    private native int[] receiveNative();

    @Override
    public int[] receive() throws NotSupportedException {
        if (!canRec())
            throw new NotSupportedException("Hardware does not support receiving");
        return receiveNative();
    }

    //public native void report();

    private native boolean sendNative(int[] data, int frequency);

    @Override
    public void send(int[] data) throws NotSupportedException {
        send(data, defaultFrequency);
    }

    @Override
    public void send(int[] data, int frequency) throws NotSupportedException {
        if (!canSend())
            throw new NotSupportedException("Hardware does not support sending");
        sendNative(data, frequency);
    }

    @Override
    public String toString() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    public static void main(String[] args) {
        /*try {
            Thread.sleep(20000);
        } catch (InterruptedException ex) {
            Logger.getLogger(Mode2LircDriver.class.getName()).log(Level.SEVERE, null, ex);
        }*/

        if (System.console() != null) {
            System.out.println("Press return");
            System.console().readLine();
        }
        String driverName = "default";
        String deviceName = "auto";
        setupLog();
        //LircDriver.setPluginDir("/usr/local/lib/lirc/pluginsx");
        //Mode2LircDriver driver = newMode2LircDriver("girs", "/usr/local/lib/lirc/plugins/girs.so");
        //Mode2LircDriver driver = newMode2LircDriver("girs");
        //Mode2LircDriver driver = newMode2LircDriver("default");
//        if (driver == null) {
//            System.err.println("driver == null, aborting");
//            closeLog();
//            System.exit(1);
//        }


        try (Mode2LircDriver driver = newMode2LircDriver(driverName)) {
            if (driver == null) {
                //System.err.println("driver == null, aborting");
                throw new LircDeviceException("driver == null");
            }
            System.out.println(driver.getVersion());
            driver.open(deviceName);
            //driver.open("/dev/lirc0");
            driver.init();
            System.out.println(driver.isReady());
            System.out.println(driver.canRec());
            //driver.report();
            int[] data = driver.receive();
            if (data != null && data.length > 0) {
                for (int d : data)
                    System.out.print(d + " ");
            }
        } catch (LircDeviceException ex) {
            Logger.getLogger(Mode2LircDriver.class.getName()).log(Level.SEVERE, null, ex);
        }
        closeLog();
        System.exit(0);
    }
}
