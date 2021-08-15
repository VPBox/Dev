package com.cells.cellswitch.secure.wifip2p;

import java.io.File;
import java.io.FileInputStream;
import java.security.MessageDigest;

/**
 * MD5 generation of files
 */
public class Md5Util {

    public static String getMd5(File filePath) {
        try {
            MessageDigest digest = MessageDigest.getInstance("MD5");
            FileInputStream in = new FileInputStream(filePath);
            int len = 0;
            byte[] buffer = new byte[1024];
            while ((len = in.read(buffer)) != -1) {
                digest.update(buffer, 0, len);//Traverse the file and calculate md5
            }
            byte[] bs = digest.digest();
            StringBuffer sb = new StringBuffer();

            for (byte b : bs) {
                int i = b & 0xff;
                String hexString = Integer.toHexString(i);
                if (hexString.length() < 2) {
                    hexString = "0" + hexString;
                }
                sb.append(hexString);
            }
            String md5 = sb.toString();
            return md5;
        } catch (Exception e) {
            e.printStackTrace();
        }

        return "";
    }
}
