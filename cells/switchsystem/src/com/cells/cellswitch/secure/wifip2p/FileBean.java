package com.cells.cellswitch.secure.wifip2p;

import java.io.Serializable;

/**
 * description: File details
 */

public class FileBean implements Serializable{

    public static final String serialVersionUID = "6321689524634663223356";

    public String filePath;

    public long fileLength;

    //MD5 code: to ensure the integrity of the file
    public String md5;

    public FileBean(String filePath, long fileLength, String md5) {
        this.filePath = filePath;
        this.fileLength = fileLength;
        this.md5 = md5;
    }
}
