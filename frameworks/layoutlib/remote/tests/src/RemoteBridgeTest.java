/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import com.android.ide.common.rendering.api.Bridge;
import com.android.ide.common.rendering.api.Result;
import com.android.ide.common.rendering.api.SessionParams;
import com.android.layoutlib.bridge.intensive.RenderResult;
import com.android.layoutlib.bridge.intensive.RenderTestBase;
import com.android.layoutlib.bridge.intensive.setup.ConfigGenerator;
import com.android.layoutlib.bridge.intensive.setup.LayoutLibTestCallback;
import com.android.layoutlib.bridge.intensive.setup.LayoutPullParser;
import com.android.layoutlib.bridge.intensive.util.ImageUtils;
import com.android.layoutlib.bridge.remote.client.RemoteBridgeClient;
import com.android.layoutlib.bridge.remote.server.ServerMain;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.File;
import java.io.IOException;
import java.rmi.NotBoundException;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;


public class RemoteBridgeTest extends RenderTestBase {
    private ServerMain mServerMain;
    private RemoteBridgeClient mClient;

    /**
     * Copy of RenderTestBase.renderAndVerify that allows using a different Bridge. TODO: Merge back
     * into RenderTestBase
     */
    protected static RenderResult renderAndVerify(Bridge bridge, SessionParams params,
            String goldenFileName, long frameTimeNanos) throws ClassNotFoundException {
        RenderResult result = RenderTestBase.render(bridge, params, frameTimeNanos);
        try {
            String goldenImagePath = APP_TEST_DIR + "/golden/" + goldenFileName;
            assertNotNull(result.getImage());
            ImageUtils.requireSimilar(goldenImagePath, result.getImage());
        } catch (IOException e) {
            getLogger().error(e, e.getMessage());
        }

        return result;
    }

    @Before
    public void setupServer() throws IOException, NotBoundException, InterruptedException {
        long startTime = System.currentTimeMillis();
        mServerMain = ServerMain.forkAndStartServer(ServerMain.REGISTRY_BASE_PORT, 10);
        mClient = RemoteBridgeClient.getRemoteBridge(mServerMain.getPort());
        System.out.printf("Server started in %dms\n", System.currentTimeMillis() - startTime);
        startTime = System.currentTimeMillis();

        File data_dir = new File(PLATFORM_DIR, "data");
        File res = new File(data_dir, "res");
        File fontLocation = new File(data_dir, "fonts");
        File buildProp = new File(PLATFORM_DIR, "build.prop");
        File attrs = new File(res, "values" + File.separator + "attrs.xml");

        mClient.init(ConfigGenerator.loadProperties(buildProp), fontLocation, null,
                ConfigGenerator.getEnumMap(attrs), getLayoutLog());
        System.out.printf("Remote client init took %dms\n",
                System.currentTimeMillis() - startTime);
    }

    @After
    public void stopServer() {
        mClient.dispose();
        mServerMain.stop();
    }

    /**
     * Same test as RenderTest#testActivity but using the remote bridge
     */
    @Test
    public void testActivity() throws IOException, ClassNotFoundException {
        SessionParams params = createSessionParams("activity.xml", ConfigGenerator.NEXUS_5);
        RenderResult result = renderAndVerify(mClient, params, "activity.png", 250);
        assertEquals(Result.Status.SUCCESS, result.getResult().getStatus());
        if (result.getResult().getException() != null) {
            result.getResult().getException().printStackTrace();
            fail("Unexpected exception");
        }
    }

    /**
     * Same test as RenderTest#testActivity but using the remote bridge
     */
    @Test
    public void testCustomClassLoading() throws ClassNotFoundException {
        LayoutLibTestCallback layoutLibCallback =
                new LayoutLibTestCallback(getLogger(), mDefaultClassLoader);
        layoutLibCallback.initResources();

        LayoutPullParser parser = LayoutPullParser.createFromString(
                "<CustomComponent xmlns:android=\"http://schemas" +
                        ".android.com/apk/res/android\"\n" +
                        "                android:layout_width=\"match_parent\"\n" +
                        "                android:layout_height=\"match_parent\"\n>" +
                        "</CustomComponent>");
        SessionParams params =
                getSessionParamsBuilder().setParser(parser).setCallback(layoutLibCallback).setTheme(
                        "Theme.NoTitleBar", false).build();

        RenderResult result = renderAndVerify(mClient, params, "remote_component_load.png", 250);
        assertEquals(Result.Status.SUCCESS, result.getResult().getStatus());
        if (result.getResult().getException() != null) {
            result.getResult().getException().printStackTrace();
            fail("Unexpected exception");
        }

        parser = LayoutPullParser.createFromString(
                "<MissingCustomComponent xmlns:android=\"http://schemas" +
                        ".android.com/apk/res/android\"\n" +
                        "                android:layout_width=\"match_parent\"\n" +
                        "                android:layout_height=\"match_parent\"\n>" +
                        "</MissingCustomComponent>");
        params =
                getSessionParamsBuilder().setParser(parser).setCallback(layoutLibCallback).setTheme(
                        "Theme.NoTitleBar", false).build();
        result = renderAndVerify(mClient, params, "remote_component_load_fail.png", 250);
        assertEquals(Result.Status.SUCCESS, result.getResult().getStatus());
        if (result.getResult().getException() != null) {
            result.getResult().getException().printStackTrace();
            fail("Unexpected exception");
        }
    }
}