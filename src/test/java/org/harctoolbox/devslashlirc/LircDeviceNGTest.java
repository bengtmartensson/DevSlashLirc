/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.harctoolbox.devslashlirc;

import java.util.logging.Level;
import java.util.logging.Logger;
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.assertFalse;
import static org.testng.Assert.assertTrue;
import org.testng.annotations.AfterClass;
import org.testng.annotations.AfterMethod;
import org.testng.annotations.BeforeClass;
import org.testng.annotations.BeforeMethod;
import org.testng.annotations.Test;

/**
 *
 */
public class LircDeviceNGTest {
    private static LircDevice instance;

    @BeforeClass(groups = {"hardware"})
    public static void setUpClass() throws LircDeviceException {
        LircHardware.loadLibrary();
        Mode2LircDevice thing = new Mode2LircDevice();
        assertFalse(thing.isValid());
        thing.open();
        assertTrue(thing.isValid());
        instance = thing;
    }

    @AfterClass(groups = {"hardware"})
    public static void tearDownClass() throws Exception, Throwable {
        instance.close();
        assertFalse(instance.isValid());
    }

    public LircDeviceNGTest() {
    }

    @BeforeMethod
    public void setUpMethod() throws Exception {
    }

    @AfterMethod
    public void tearDownMethod() throws Exception {
    }

    /**
     * Test of getVersion method, of class LircDevice.
     */
    @Test(groups = {"hardware"})
    public void testGetVersion() {
        System.out.println("getVersion");

        String result = instance.getVersion();
        assertEquals(result, "LircDevice 0.2.0");
    }

    /**
     * Test of setTransmitterMask method, of class LircDevice.
     * @throws LircDeviceException
     */
    @Test(groups = {"hardware"})
    public void testSetTransmitterMask() throws LircDeviceException {
        System.out.println("setTransmitterMask");
        int mask = 1;
        try {
            instance.setTransmitterMask(mask);
        } catch (NotSupportedException | NonExistentTransmitterException ex) {
            Logger.getLogger(LircDeviceNGTest.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * Test of getNumberTransmitters method, of class LircDevice.
     */
    @Test(groups = {"hardware"})
    public void testGetNumberTransmitters() {
        System.out.println("getNumberTransmitters");
        int expResult = 0;
        int result = instance.getNumberTransmitters();
        assertEquals(result, expResult);
    }

    /**
     * Test of canSend method, of class LircDevice.
     */
    @Test(groups = {"hardware"})
    public void testCanSend() {
        System.out.println("canSend");
        boolean expResult = true;
        boolean result = instance.canSend();
        assertEquals(result, expResult);
    }

    /**
     * Test of canSetTransmitterMask method, of class LircDevice.
     */
    @Test(groups = {"hardware"})
    public void testCanSetTransmitterMask() {
        System.out.println("canSetTransmitterMask");
        boolean expResult = true;
        boolean result = instance.canSetTransmitterMask();
        assertEquals(result, expResult);
    }

    /**
     * Test of canRec method, of class LircDevice.
     */
    @Test(groups = {"hardware"})
    public void testCanRec() {
        System.out.println("canRec");
        boolean expResult = true;
        boolean result = instance.canRec();
        assertEquals(result, expResult);
    }

    /**
     * Test of toString method, of class LircDevice.
     */
    @Test(groups = {"hardware"})
    public void testToString() {
        System.out.println("toString");
        String expResult = "mode2  setCarr. send rec. setTM #xmtrs=0";
        String result = instance.toString();
        assertEquals(result, expResult);
    }

    /**
     * Test of setBeginTimeout method, of class LircDevice.
     */
    @Test(groups = {"hardware"})
    public void testSetBeginTimeout() {
        System.out.println("setBeginTimeout");
        instance.setBeginTimeout(1234);
    }
}
