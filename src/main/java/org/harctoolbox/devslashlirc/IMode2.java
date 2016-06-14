package org.harctoolbox.devslashlirc;

/**
 * This interface models a "mode2" Lirc hardware, device or driver.
 */
public interface IMode2 {

    boolean canGetRecResolution();

    boolean canSetSendCarrier();

    int getRecResolution() throws NotSupportedException;

    void open() throws LircDeviceException;

    //int read() throws NotSupportedException;

    int[] receive() throws NotSupportedException;

    //void report();

    void send(int[] data) throws NotSupportedException;

    void send(int[] data, int frequency) throws NotSupportedException;

    void setSendCarrier(int frequency) throws NotSupportedException;

    boolean canRec();

    boolean canSend();

    boolean canSetTransmitterMask();

    //@Override
    //void close();

    int getNumberTransmitters();

    String getVersion();

    boolean isValid();

    void setTransmitterMask(int mask) throws NotSupportedException, NonExistentTransmitterException, LircDeviceException;

    @Override
    String toString();
}
