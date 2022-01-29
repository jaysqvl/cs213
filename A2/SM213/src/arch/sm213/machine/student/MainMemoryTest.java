package arch.sm213.machine.student;

import jdk.jfr.internal.tool.Main;
import machine.AbstractMainMemory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class MainMemoryTest {

    @Test
    void isAccessAlignedTest() {
        MainMemory m = new MainMemory(0); // doesn't need to be able to hold any bytes
        // Test that a 4 bit byte is aligned starting at address 0
        assertTrue(m.isAccessAligned(0, 4));

        // Test false that a 4 bit byte is aligned starting at address 2
        assertFalse(m.isAccessAligned(2, 4));

        // Test true that a 4 bit byte is aligned starting at address 4
        assertTrue(m.isAccessAligned(4, 4));

        // Test true that a 4 bit byte is aligned starting at address 8
        assertTrue(m.isAccessAligned(8, 4));

        // Test false that a 4 bit byte is aligned starting at address 1
        assertFalse(m.isAccessAligned(1, 4));

        // Test false that a 2 bit byte is aligned starting at address 1
        assertFalse(m.isAccessAligned(1,2));

        // Test true that a 2 bit byte is aligned starting at address 2
        assertTrue(m.isAccessAligned(2, 2));

        // Test true that a 1 bit byte is aligned starting at address 1
        assertTrue(m.isAccessAligned(1,1));
    }

    @Test
    void bytesToIntegerTest() {
        MainMemory m = new MainMemory(0);
        // Tests that 0x00000000 (correctly) equals integer 0
        assertEquals(0, m.bytesToInteger((byte)0,(byte)0,(byte)0,(byte)0));

        // Tests that 0x00000001 (correctly) equals integer 1 above 0
        assertEquals(1, m.bytesToInteger((byte)0,(byte)0,(byte)0,(byte)1));

        // Tests that 0x80 (correctly) = 0000 0000 0000 0000 0000 0000 1000 0000 <- 2^7 in integers
        assertEquals(128, m.bytesToInteger((byte)0,(byte)0,(byte)0,(byte)0x80));

        // Tests below 0
        // 0xFF FF FF FF = 1111 1111 | 1111 1111 | 1111 1111 | 1111 1111 <- signed bit -1 = Int Min Value + Max Value
        assertEquals(-1, m.bytesToInteger((byte)0xFF,(byte)0xFF,(byte)0xFF,(byte)0xFF));

        // Tests highest bound of signed integers
        // 0x7F FF FF FF = 01111111 11111111 11111111 11111111 <- signed bit max int
        assertEquals(Integer.MAX_VALUE, m.bytesToInteger((byte)0x7F, (byte)0xFF, (byte)0xFF, (byte)0xFF));

        // Tests lowest bound of signed integers and negative numbers
        // 0x80 00 00 00 = 10000000 00000000 00000000 00000000 <- signed bit min int
        assertEquals(Integer.MIN_VALUE, m.bytesToInteger((byte)0x80, (byte)0, (byte)0, (byte)0));
    }

    @Test
    void integerToBytesTest() {
        MainMemory m = new MainMemory(0);
        // Tests that 0 (correctly) equals 0x00000000
        assertByteEquals(0,0,0,0, m.integerToBytes(0));

        // Tests that 0 (correctly) equals 0x00000000
        assertByteEquals(0,0,0,1, m.integerToBytes(1));

        // Tests that 128 (correctly) equals 0x00000080 or 0000 0000 | 0000 0000 | 0000 0000 | 1000 0000
        assertByteEquals(0,0,0,0x80, m.integerToBytes(128));

        // Tests that 2147483647 (correctly) equals 0x7F FF FF FF or 0111 1111 | 1111 1111 | 1111 1111 | 1111 1111
        assertByteEquals(0x7F, 0xFF, 0xFF,0xFF, m.integerToBytes(Integer.MAX_VALUE));

        // Tests that -2147483648 (correctly) equals 0x80000000 or 1000 0000 | 0000 0000 | 0000 0000 | 0000 0000
        assertByteEquals(0x80, 0,0,0, m.integerToBytes(Integer.MIN_VALUE));

        // Tests that -1 (correctly) equals 0xFFFFFFFF or 1111 1111 | 1111 1111 | 1111 1111 | 1111 1111
        assertByteEquals(0xFF,0xFF,0xFF,0xFF, m.integerToBytes(-1));
    }

    @Test
    void setGetTest() {
        MainMemory m = new MainMemory(64);
        byte[] intSizeBytes = {0, 1, 2, 3};
        byte[] charSizeBytes = {1};
        byte[] shortSizeBytes = {0, 1};
        byte[] longSizeBytes = {0, 1, 2, 3, 4, 5, 6, 7};

        // Fail cases for get and set
        // Get upper bound exception test
        try {
            m.get(64, 4);
            fail();
        } catch (Exception e) {}

        // Get lower bound exception test
        try {
            m.get(-4, 4);
            fail();
        } catch(Exception e) {}

        // Set lower bound exception test
        try {
            m.set(-4, intSizeBytes);
            fail();
        } catch(Exception e) {}

        // Set upper bound exception test
        try {
            m.set(64, intSizeBytes);
            fail();
        } catch(Exception e) {}

        // Pass cases for get and set
        // Upper and lower bounds for int num of bytes (4)
        try {
            m.set(0, intSizeBytes);
            assertByteEqualsArr(intSizeBytes, m.get(0, 4));
        } catch(Exception e) {fail();}

        try {
            m.set(60, intSizeBytes);
            assertByteEqualsArr(intSizeBytes, m.get(0, 4));
        } catch(Exception e) {fail();}

        // get and set for long num of bytes (8) in middle of memory
        try {
            m.set(4, longSizeBytes);
            assertByteEqualsArr(longSizeBytes, m.get(4, 8));
        } catch(Exception e) {fail();}

        // get and set for short num of bytes (2) in middle of memory
        try {
            m.set(12, shortSizeBytes);
            assertByteEqualsArr(shortSizeBytes, m.get(12, 2));
        } catch(Exception e) {fail();}

        // extra get and set for char num of bytes (1) in middle of memory
        try {
            m.set(14, charSizeBytes);
            assertByteEqualsArr(charSizeBytes, m.get(14, 1));
        } catch(Exception e) {fail();}
    }

    private void assertByteEquals(int byte0, int byte1, int byte2, int byte3, byte[] runAgainst) {
        assertEquals((byte)byte0, runAgainst[0]);
        assertEquals((byte)byte1, runAgainst[1]);
        assertEquals((byte)byte2, runAgainst[2]);
        assertEquals((byte)byte3, runAgainst[3]);
    }

    private void assertByteEqualsArr(byte[] expected, byte[] runAgainst) {
        for(int i = 0; i < runAgainst.length; i++) {
            assertEquals(expected[i], runAgainst[i]);
        }
    }
}
