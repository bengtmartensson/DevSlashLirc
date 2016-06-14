package org.harctoolbox.devslashlirc;

import java.util.logging.Level;
import java.util.logging.Logger;
import static org.testng.Assert.assertEquals;
import static org.testng.Assert.fail;
import org.testng.annotations.AfterClass;
import org.testng.annotations.AfterMethod;
import org.testng.annotations.BeforeClass;
import org.testng.annotations.BeforeMethod;
import org.testng.annotations.Test;


public class Mode2LircDeviceNGTest {

    private static Mode2LircDevice instance;

    public Mode2LircDeviceNGTest() {
    }

    @BeforeClass
    public static void setUpClass() {
        try {
            instance = new Mode2LircDevice();
            instance.open();
        } catch (LircDeviceException ex) {
            Logger.getLogger(Mode2LircDeviceNGTest.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @AfterClass
    public static void tearDownClass() throws Exception {
        instance.close();
    }

    @BeforeMethod
    public void setUpMethod() throws Exception {
    }

    @AfterMethod
    public void tearDownMethod() throws Exception {
    }

    /**
     * Test of canSetSendCarrier method, of class Mode2LircDevice.
     */
    @Test
    public void testCanSetSendCarrier() {
        System.out.println("canSetSendCarrier");
        boolean expResult = true;
        boolean result = instance.canSetSendCarrier();
        assertEquals(result, expResult);
    }

    /**
     * Test of canGetRecResolution method, of class Mode2LircDevice.
     */
    @Test
    public void testCanGetRecResolution() {
        System.out.println("canGetRecResolution");
        boolean expResult = false;
        boolean result = instance.canGetRecResolution();
        assertEquals(result, expResult);
    }

    /**
     * Test of send method, of class Mode2LircDevice.
     */
    @Test
    public void testSend_intArr() {
        System.out.println("send");
        int[] data = new int[1];
        data[0] = 1;
        try {
            instance.send(data);
        } catch (NotSupportedException ex) {
            fail();
        }
    }

    /**
     * Test of send method, of class Mode2LircDevice.
     * /
    @Test
    public void testSend_intArr_int() {
        System.out.println("send");
        int[] data = {
            9024, 4512, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 564, 564, 564, 564, 564, 564, 1692, 564, 1692, 564, 1692, 564, 39756
        };
        int frequency = 38400;
        try {
            instance.send(data, frequency);
        } catch (NotSupportedException ex) {
            fail();
        }
    }

    /**
     * Test of setSendCarrier method, of class Mode2LircDevice.
     */
    @Test
    public void testSetSendCarrier() {
        System.out.println("setSendCarrier");
        int frequency = 40000;
        try {
            instance.setSendCarrier(frequency);
        } catch (NotSupportedException ex) {
            Logger.getLogger(Mode2LircDeviceNGTest.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * Test of getResolution method, of class Mode2LircDevice.
     */
    @Test
    public void testGetResolution() {
        System.out.println("getResolution");
        try {
            instance.getRecResolution();
            fail();
        } catch (NotSupportedException ex) {
        }
    }

    /**
     * Test of receive method, of class Mode2LircDevice.
     * /
    @Test
    public void testReceive() {
        try {
            System.out.println("receive");
            System.out.println(">>>>>>>>>>>>> Now shoot an IR signal at the receiver");
            int[] result = instance.receive();
            for (int i = 0; i < result.length; i++)
                System.out.print((i%2==0 ? "+" : "-") + result[i] + " ");
            System.out.println();
            //assertEquals(result, expResult);
        } catch (NotSupportedException ex) {
            fail();
        }
    }

    /**
     * Test of report method, of class Mode2LircDevice.
     */
    @Test
    public void testReport() {
        System.out.println("report");
        instance.report();
    }

    /**
     * Test of toString method, of class Mode2LircDevice.
     */
    @Test
    public void testToString() {
        System.out.println("toString");
        String expResult = "Mode2LircDevice  setCarrierdeviceName=/dev/lirc0 send setTransmitterMask record, # transmitters = 0";
        String result = instance.toString();
        assertEquals(result, expResult);
    }
}
