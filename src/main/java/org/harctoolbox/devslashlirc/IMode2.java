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
 * This interface specifies the commands of "mode2" Lirc hardware, device or driver.
 */
public interface IMode2 extends Closeable {

    boolean canGetRecResolution();

    boolean canSetSendCarrier();

    int getRecResolution() throws NotSupportedException;

    /**
     * Opens the hardware using the previously given device name.
     * @throws LircDeviceException Error occured.
     */
    void open() throws LircDeviceException;

    // The read() function, returing just a duration, is not supported.
    //int read() throws NotSupportedException;

    /**
     * Reads an IR signal as a sequence of durations.
     * @return durations as an integer array, all positive.
     * @throws NotSupportedException if the hardwared does not support receiving.
     */
    int[] receive() throws NotSupportedException;

    /**
     * Sends the provided data as a sequence of durations. The data is send
     * without setting the modulation frequency, so the exact outcome may depend
     * on the hardware, or on previously sent commands.
     *
     * @param data interger array of durations.
     * @throws NotSupportedException if the hardware does not support sending.
     */
    void send(int[] data) throws NotSupportedException;

    /**
     * Sends the provided data as a sequence of durations. The data is send
     * without setting the modulation frequency, so the exact outcome may depend
     * on the hardware, or on previously sent commands.
     *
     * @param data interger array of durations.
     * @param frequency Frequency in Hz.
     * @throws NotSupportedException if the hardware does not support sending,
     * or setting of the modulation frequency.
     */
    void send(int[] data, int frequency) throws NotSupportedException;

    void setSendCarrier(int frequency) throws NotSupportedException;

    boolean canRec();

    boolean canSend();

    boolean canSetTransmitterMask();

    /**
     * Returns the number of transmitters, as reported from the device.
     * @return integer
     */
    int getNumberTransmitters();

    /**
     * Returns version of underlying C++ driver.
     * @return String
     */
    String getVersion();

    /**
     * Returns true if the hardware is open and ready.
     * @return boolean
     */
    boolean isValid();

    void setTransmitterMask(int mask) throws NotSupportedException, NonExistentTransmitterException, LircDeviceException;

    void setMaxCaptureLength(int maxCaptureLength);

    void setEndTimeout(int timeout);

    @Override
    String toString();
}
