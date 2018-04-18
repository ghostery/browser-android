package org.mozilla.gecko.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Copyright © Cliqz 2018
 */

@SuppressWarnings("TryFinallyCanBeTryWithResources")
public class FileUtils {

    private FileUtils() {}

    public static String readTextFromFile(final File input) throws IOException {
        if (input == null) {
            return "";
        }

        try {
            final FileInputStream is = new FileInputStream(input);
            final String result = StreamUtils.readTextStream(is);
            is.close();
            return result;
        } catch (FileNotFoundException|SecurityException e) {
            throw new IOException(e);
        }
    }

    public static void writeTextToFile(File output, String str) throws IOException {
        if (output == null || str == null || str.isEmpty()) {
            return;
        }

        final FileOutputStream os = new FileOutputStream(output);
        try {
            os.write(str.getBytes());
        } finally {
            os.close();
        }
    }
}
