package android.aidl.loggable;

interface ILoggableInterface {
    String[] LogThis(boolean boolValue, inout boolean[] boolArray,
                   byte byteValue, inout byte[] byteArray,
                   char charValue, inout char[] charArray,
                   int intValue, inout int[] intArray,
                   long longValue, inout long[] longArray,
                   float floatValue, inout float[] floatArray,
                   double doubleValue, inout double[] doubleArray,
                   String stringValue, inout String[] stringArray,
                   inout List<String> listValue,
                   inout Map mapValue,
                   IBinder binderValue,
                   inout ParcelFileDescriptor pfdValue, inout ParcelFileDescriptor[] pfdArray);
}
