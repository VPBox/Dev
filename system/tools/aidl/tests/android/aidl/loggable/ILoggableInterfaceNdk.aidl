package android.aidl.loggable;

interface ILoggableInterfaceNdk {
    String[] LogThis(boolean boolValue, inout boolean[] boolArray,
                   byte byteValue, inout byte[] byteArray,
                   char charValue, inout char[] charArray,
                   int intValue, inout int[] intArray,
                   long longValue, inout long[] longArray,
                   float floatValue, inout float[] floatArray,
                   double doubleValue, inout double[] doubleArray,
                   String stringValue, inout String[] stringArray,
                   IBinder binderValue,
                   inout ParcelFileDescriptor pfdValue);
}
