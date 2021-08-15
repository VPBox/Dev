package com.android.rs.refocus;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.adobe.xmp.XMPConst;
import com.adobe.xmp.XMPException;
import com.adobe.xmp.XMPIterator;
import com.adobe.xmp.XMPMeta;
import com.adobe.xmp.XMPMetaFactory;
import com.adobe.xmp.properties.XMPPropertyInfo;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import com.android.rs.refocus.image.RangeInverseDepthTransform;
import com.android.rs.refocus.image.RangeLinearDepthTransform;


/**
 * Created by hoford on 5/15/15.
 */
public class XmpDepthDecode {
    private static final String TAG = "XmpUtil";
    private static final String XMP_DEPTHMAP = "http://ns.google.com/photos/1.0/depthmap/";
    private static final String XMP_FOCUS = "http://ns.google.com/photos/1.0/focus/";
    private static final String XMP_HEADER = "http://ns.adobe.com/xap/1.0/\0";
    private static final String XMP_EXTENSION_HEADER =
            "http://ns.adobe.com/xmp/extension/\0";
    private static final String XMP_HAS_EXTENSION = "HasExtendedXMP";
    private static final int XMP_EXTENSION_HEADER_GUID_SIZE =
            XMP_EXTENSION_HEADER.length() + 32 + 1; // 32 byte GUID + 1 byte null termination.
    private static final int XMP_EXTENSION_HEADER_OFFSET = 7;

    private static final int M_SOI = 0xd8; // File start marker.
    private static final int M_APP1 = 0xe1; // Marker for EXIF or XMP.
    private static final int M_SOS = 0xda; // Image data marker.

    private final String mFormat;
    private final double mFar;
    private final double mNear;
    private final Bitmap mDepthBitmap;
    private final double mBlurAtInfinity;
    private final double mFocalDistance;
    private final double mDepthOfFiled;
    private final double mFocalPointX;
    private final double mFocalPointY;
    private final DepthTransform mDepthTransform;

    public XmpDepthDecode(InputStream is) throws IOException {
        XMPMeta meta = read(is, false);
        try {
            mFormat = meta.getPropertyString(XMP_DEPTHMAP, "GDepth:Format");

            mFar = Double.parseDouble(meta.getPropertyString(XMP_DEPTHMAP, "GDepth:Far"));
            mNear = Double.parseDouble(meta.getPropertyString(XMP_DEPTHMAP, "GDepth:Near"));

            DepthTransform tDepthTransform = null;
            String format = meta.getPropertyString(
                    XMP_DEPTHMAP, "GDepth:Format");
            if (RangeInverseDepthTransform.FORMAT.equals(format)) {
              tDepthTransform = new RangeInverseDepthTransform((float)mNear, (float)mFar);
            } else if (RangeLinearDepthTransform.FORMAT.equals(format)) {
              tDepthTransform = new RangeLinearDepthTransform((float)mNear, (float)mFar);
            } else {
              Log.e(TAG, "Unknown GDepth format: " + format);
            }
            mDepthTransform = tDepthTransform;

            byte[] data = meta.getPropertyBase64(XMP_DEPTHMAP, "GDepth:Data");
            mDepthBitmap = BitmapFactory.decodeStream(new ByteArrayInputStream(data));

            mBlurAtInfinity = Double.parseDouble(meta.getPropertyString(XMP_FOCUS, "GFocus:BlurAtInfinity"));
            mFocalDistance = Double.parseDouble(meta.getPropertyString(XMP_FOCUS, "GFocus:FocalDistance"));
            mDepthOfFiled = Double.parseDouble(meta.getPropertyString(XMP_FOCUS, "GFocus:DepthOfField"));
            mFocalPointX = Double.parseDouble(meta.getPropertyString(XMP_FOCUS, "GFocus:FocalPointX"));
            mFocalPointY = Double.parseDouble(meta.getPropertyString(XMP_FOCUS, "GFocus:FocalPointY"));
        } catch (XMPException e) {
            throw new IOException("XMP data missing");
        }
    }

    public Bitmap getDepthBitmap() {
        return mDepthBitmap;
    }

    public DepthTransform getDepthTransform() { return  mDepthTransform; }

    public String getFormat() {
        return mFormat;
    }

    public double getFar() {
        return mFar;
    }

    public double getNear() {
        return mNear;
    }

    public double getBlurAtInfinity() {
        return mBlurAtInfinity;
    }

    public double getFocalDistance() {
        return mFocalDistance;
    }

    public double getDepthOfField() { return mDepthOfFiled; }

    public double getFocalPointX() {
        return mFocalPointX;
    }

    public double getFocalPointY() {
        return mFocalPointY;
    }


    // JPEG file is composed of many sections and image data. This class is used
    // to hold the section data from image file.
    private static class Section {
        public int marker;
        public int length;
        public byte[] data;
    }

    static XMPMeta read(InputStream is, boolean skipExtendedContent) {
        List<Section> sections = parse(is, true, skipExtendedContent);
        if (sections == null) {
            return null;
        }

        XMPMeta xmpMeta = parseFirstValidXMPSection(sections);
        if (xmpMeta == null ||
                !xmpMeta.doesPropertyExist(XMPConst.NS_XMP_NOTE, XMP_HAS_EXTENSION)) {
            return xmpMeta;
        }

        String extensionName = null;
        try {
            extensionName = (String) xmpMeta.getProperty(
                    XMPConst.NS_XMP_NOTE, XMP_HAS_EXTENSION).getValue();
        } catch (XMPException e) {
            e.printStackTrace();
            return null;
        }

        if (skipExtendedContent) {
            if (!checkExtendedSectionExists(sections, extensionName)) {
                // The main XMP section referenced an extended section that is not present.
                // This is an error.
                return null;
            }
            return xmpMeta;
        }

        XMPMeta xmpExtended = parseExtendedXMPSections(sections, extensionName);
        if (xmpExtended == null) {
            // The main XMP section referenced an extended section that is not present.
            // This is an error.
            return null;
        }

        // Merge the extended properties into the main one.
        try {
            XMPIterator iterator = xmpExtended.iterator();
            while (true) {
                XMPPropertyInfo info = (XMPPropertyInfo) iterator.next();
                if (info.getPath() != null) {
                    xmpMeta.setProperty(info.getNamespace(), info.getPath(),
                            info.getValue(), info.getOptions());
                }
            }
        } catch (Exception e) {
            // Catch XMPException and NoSuchElementException.
        }
        return xmpMeta;
    }

    /**
     * Parses the JPEG image file. If readMetaOnly is true, only keeps the EXIF
     * and XMP sections (with marker M_APP1) and ignore others; otherwise, keep
     * all sections. The last section with image data will have -1 length.
     *
     * @param is                  Input image data stream
     * @param readMetaOnly        Whether only reads the metadata in jpg
     * @param skipExtendedContent Whether to skip the content of extended sections
     * @return The parse result
     */
    private static List<Section> parse(InputStream is, boolean readMetaOnly,
                                       boolean skipExtendedContent) {
        List<Section> sections = new ArrayList<Section>();
        if (is == null) {
            return sections;
        }

        try {
            if (is.read() != 0xff || is.read() != M_SOI) {
                return sections;
            }
            int c;
            while ((c = is.read()) != -1) {
                if (c != 0xff) {
                    return sections;
                }
                // Skip padding bytes.
                while ((c = is.read()) == 0xff) {
                }
                if (c == -1) {
                    return sections;
                }
                int marker = c;
                if (marker == M_SOS) {
                    // M_SOS indicates the image data will follow and no metadata after
                    // that, so read all data at one time.
                    if (!readMetaOnly) {
                        Section section = new Section();
                        section.marker = marker;
                        section.length = -1;
                        section.data = new byte[is.available()];
                        is.read(section.data, 0, section.data.length);
                        sections.add(section);
                    }
                    return sections;
                }
                int lh = is.read();
                int ll = is.read();
                if (lh == -1 || ll == -1) {
                    return sections;
                }
                int length = lh << 8 | ll;
                if (!readMetaOnly || marker == M_APP1) {
                    sections.add(readSection(is, length, marker, skipExtendedContent));
                } else {
                    // Skip this section since all EXIF/XMP meta will be in M_APP1
                    // section.
                    is.skip(length - 2);
                }
            }
            return sections;
        } catch (IOException e) {
            System.out.println("Could not parse file." + e);
            return sections;
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException e) {
                    // Ignore.
                }
            }
        }
    }

    /**
     * Checks whether the byte array has XMP header. The XMP section contains
     * a fixed length header XMP_HEADER.
     *
     * @param data   XMP metadata
     * @param header The header to look for
     */
    private static boolean hasHeader(byte[] data, String header) {
        if (data.length < header.length()) {
            return false;
        }
        try {
            byte[] buffer = new byte[header.length()];
            System.arraycopy(data, 0, buffer, 0, header.length());
            if (new String(buffer, "UTF-8").equals(header)) {
                return true;
            }
        } catch (UnsupportedEncodingException e) {
            return false;
        }
        return false;
    }

    private static Section readSection(InputStream is, int length,
                                       int marker, boolean skipExtendedContent) throws IOException {
        if (length - 2 < XMP_EXTENSION_HEADER_GUID_SIZE || !skipExtendedContent) {
            Section section = new Section();
            section.marker = marker;
            section.length = length;
            section.data = new byte[length - 2];
            is.read(section.data, 0, length - 2);
            return section;
        }

        byte[] header = new byte[XMP_EXTENSION_HEADER_GUID_SIZE];
        is.read(header, 0, header.length);

        if (hasHeader(header, XMP_EXTENSION_HEADER) && skipExtendedContent) {
            Section section = new Section();
            section.marker = marker;
            section.length = header.length + 2;
            section.data = header;
            is.skip(length - 2 - header.length);
            return section;
        }

        Section section = new Section();
        section.marker = marker;
        section.length = length;
        section.data = new byte[length - 2];
        System.arraycopy(header, 0, section.data, 0, header.length);
        is.read(section.data, header.length, length - 2 - header.length);
        return section;
    }

    /**
     * Gets the end of the XMP meta content. If there is no packet wrapper,
     * return data.length, otherwise return 1 + the position of last '>'
     * without '?' before it.
     * Usually the packet wrapper end is "<?xpacket end="w"?> but
     * javax.xml.parsers.DocumentBuilder fails to parse it in android.
     *
     * @param data XMP metadata bytes
     * @return The end of the XMP metadata content
     */
    private static int getXMPContentEnd(byte[] data) {
        for (int i = data.length - 1; i >= 1; --i) {
            if (data[i] == '>') {
                if (data[i - 1] != '?') {
                    return i + 1;
                }
            }
        }
        // It should not reach here for a valid XMP meta.
        return data.length;
    }

    /**
     * Parses the first valid XMP section. Any other valid XMP section will be
     * ignored.
     *
     * @param sections The list of sections parse
     * @return The parsed XMPMeta object
     */
    private static XMPMeta parseFirstValidXMPSection(List<Section> sections) {
        for (Section section : sections) {
            if (hasHeader(section.data, XMP_HEADER)) {
                int end = getXMPContentEnd(section.data);
                byte[] buffer = new byte[end - XMP_HEADER.length()];
                System.arraycopy(
                        section.data, XMP_HEADER.length(), buffer, 0, buffer.length);
                try {
                    XMPMeta result = XMPMetaFactory.parseFromBuffer(buffer);
                    return result;
                } catch (XMPException e) {
                    System.out.println("XMP parse error " + e);
                    return null;
                }
            }
        }
        return null;
    }

    /**
     * Checks there is an extended section with the given name.
     *
     * @param sections    The list of sections to parse
     * @param sectionName The name of the extended sections
     * @return Whether there is an extended section with the given name
     */
    private static boolean checkExtendedSectionExists(List<Section> sections,
                                                      String sectionName) {
        String extendedHeader = XMP_EXTENSION_HEADER + sectionName + "\0";
        for (Section section : sections) {
            if (hasHeader(section.data, extendedHeader)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Parses the extended XMP sections with the given name. All other sections
     * will be ignored.
     *
     * @param sections    The list of sections to parse
     * @param sectionName The name of the extended sections
     * @return The parsed XMPMeta object
     */
    private static XMPMeta parseExtendedXMPSections(List<Section> sections,
                                                    String sectionName) {
        String extendedHeader = XMP_EXTENSION_HEADER + sectionName + "\0";

        // Compute the size of the buffer to parse the extended sections.
        List<Section> xmpSections = new ArrayList<Section>();
        List<Integer> xmpStartOffset = new ArrayList<Integer>();
        List<Integer> xmpEndOffset = new ArrayList<Integer>();
        int bufferSize = 0;
        for (Section section : sections) {
            if (hasHeader(section.data, extendedHeader)) {
                int startOffset = extendedHeader.length() + XMP_EXTENSION_HEADER_OFFSET;
                int endOffset = section.data.length;
                bufferSize += Math.max(0, section.data.length - startOffset);
                xmpSections.add(section);
                xmpStartOffset.add(startOffset);
                xmpEndOffset.add(endOffset);
            }
        }
        if (bufferSize == 0) {
            return null;
        }

        // Copy all the relevant sections' data into a buffer.
        byte buffer[] = new byte[bufferSize];
        int offset = 0;
        for (int i = 0; i < xmpSections.size(); ++i) {
            Section section = xmpSections.get(i);
            int startOffset = xmpStartOffset.get(i);
            int endOffset = xmpEndOffset.get(i);
            int length = endOffset - startOffset;
            System.arraycopy(
                    section.data, startOffset, buffer, offset, length);
            offset += length;
        }

        XMPMeta xmpExtended = null;
        try {
            xmpExtended = XMPMetaFactory.parseFromBuffer(buffer);
        } catch (XMPException e) {
            System.out.println("Extended XMP parse error " + e);
            return null;
        }
        return xmpExtended;
    }
}
