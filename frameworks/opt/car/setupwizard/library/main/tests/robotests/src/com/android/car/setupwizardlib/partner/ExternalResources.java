/*
 * Copyright (C) 2019 The Android Open Source Project
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

package com.android.car.setupwizardlib.partner;

import static org.robolectric.RuntimeEnvironment.application;
import static org.robolectric.Shadows.shadowOf;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.drawable.Drawable;
import android.util.DisplayMetrics;
import android.util.TypedValue;

import androidx.annotation.AnyRes;
import androidx.annotation.ArrayRes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import org.robolectric.res.ResName;
import org.robolectric.res.ResType;
import org.robolectric.res.TypedResource;

import java.util.HashMap;
import java.util.Map;

/**
 * Utility class to inject resources for an "external" application in Robolectric tests. This can be
 * used with {@link org.robolectric.shadows.ShadowPackageManager#resources} to simulate loading
 * resources from another package.
 */
public class ExternalResources {

    public static Resources injectExternalResources(String packageName) {
        return injectExternalResources(createPackageInfo(packageName));
    }

    public static Resources injectExternalResources(PackageInfo packageInfo) {
        try {
            application.getPackageManager().getPackageInfo(packageInfo.packageName, 0);
        } catch (PackageManager.NameNotFoundException e) {
            // Add the package if it does not exist
            shadowOf(application.getPackageManager()).addPackage(packageInfo);
        }
        Resources resources = Resources.forPackageName(packageInfo.packageName);
        shadowOf(application.getPackageManager()).resources.put(packageInfo.packageName, resources);
        return resources;
    }

    /**
     * Constructed resources for testing, representing resources external to the current package
     * under test.
     */
    public static class Resources extends android.content.res.Resources {

        private final String mPackageName;
        private final Map<Integer, TypedResource<?>> mOverrideResources = new HashMap<>();
        private final Map<ResName, Integer> mResourceIds = new HashMap<>();
        private int mNextId = 1;

        public static Resources forPackageName(String packageName) {
            android.content.res.Resources res = application.getResources();
            return new Resources(
                    packageName, res.getAssets(), res.getDisplayMetrics(), res.getConfiguration());
        }

        @Override
        public String getResourcePackageName(@AnyRes int resId) {
            TypedResource<?> typedResource = mOverrideResources.get(resId);
            if (typedResource != null) {
                return mPackageName;
            }
            throw new NotFoundException();
        }

        @Override
        public int getIdentifier(String name, String defType, String defPackage) {
            Integer resourceId =
                    mResourceIds.get(ResName.qualifyResName(name, defPackage, defType));
            if (resourceId == null) {
                return 0;
            }
            return resourceId;
        }

        @Override
        public void getValue(int id, TypedValue outValue, boolean resolveRefs) {
            TypedResource<?> typedResource = mOverrideResources.get(id);
            if (typedResource != null) {
                Object override = get(id, typedResource.getResType());
                if (override instanceof TypedValue) {
                    outValue.setTo((TypedValue) override);
                } else if (override instanceof Integer) {
                    outValue.data = (int) override;
                }
                return;
            }
            throw new NotFoundException();
        }

        public void putValue(String name, int value, String defaultType, ResType resType) {
            put(
                    ResName.qualifyResName(name, mPackageName, defaultType),
                    new TypedResource<>(value, resType, null));
        }

        public void putValue(String name, TypedValue value, String defaultType, ResType resType) {
            put(
                    ResName.qualifyResName(name, mPackageName, defaultType),
                    new TypedResource<>(value, resType, null));
        }

        @Override
        public int getInteger(int id) {
            return (int) get(id, ResType.INTEGER);
        }

        public void putInteger(String name, int value) {
            put(
                    ResName.qualifyResName(name, mPackageName, "integer"),
                    new TypedResource<>(value, ResType.INTEGER, null));
        }

        @Override
        public int getColor(int id) {
            return (int) get(id, ResType.COLOR);
        }

        @Override
        public int getColor(int id, @Nullable Theme theme) {
            return (int) get(id, ResType.COLOR);
        }

        public void putColor(String name, int value) {
            put(
                    ResName.qualifyResName(name, mPackageName, "color"),
                    new TypedResource<>(value, ResType.COLOR, null));
        }

        @Override
        public Drawable getDrawable(int id) {
            return (Drawable) get(id, ResType.DRAWABLE);
        }

        @Override
        public Drawable getDrawable(int id, @Nullable Theme theme) {
            return (Drawable) get(id, ResType.DRAWABLE);
        }

        public void putDrawable(String name, Drawable value) {
            put(
                    ResName.qualifyResName(name, mPackageName, "drawable"),
                    new TypedResource<>(value, ResType.DRAWABLE, null));
        }

        public void putBoolean(String name, boolean value) {
            put(
                    ResName.qualifyResName(name, mPackageName, "bool"),
                    new TypedResource<>(value, ResType.BOOLEAN, null));
        }

        @Override
        public float getDimension(int id) {
            return (float) get(id, ResType.DIMEN);
        }

        public void putDimension(String name, float value) {
            put(
                    ResName.qualifyResName(name, mPackageName, "dimen"),
                    new TypedResource<>(value, ResType.DIMEN, null));
        }

        @NonNull
        @Override
        public CharSequence getText(int id) {
            return (CharSequence) get(id, ResType.CHAR_SEQUENCE);
        }

        @NonNull
        @Override
        public String getString(int id) {
            return get(id, ResType.CHAR_SEQUENCE).toString();
        }

        public void putText(String name, CharSequence value) {
            put(
                    ResName.qualifyResName(name, mPackageName, "string"),
                    new TypedResource<>(value, ResType.CHAR_SEQUENCE, null));
        }

        @NonNull
        @Override
        public String[] getStringArray(@ArrayRes int id) {
            return (String[]) get(id, ResType.CHAR_SEQUENCE_ARRAY);
        }

        public void putStringArray(String name, String[] value) {
            put(
                    ResName.qualifyResName(name, mPackageName, "string-array"),
                    new TypedResource<>(value, ResType.CHAR_SEQUENCE_ARRAY, null));
        }

        private Resources(String packageName, AssetManager assets, DisplayMetrics metrics,
                Configuration config) {
            super(assets, metrics, config);
            this.mPackageName = packageName;
        }

        private <T> void put(ResName resName, TypedResource<T> value) {
            int id = mNextId++;
            mOverrideResources.put(id, value);
            mResourceIds.put(resName, id);
        }

        private Object get(@AnyRes int id, ResType type) {
            TypedResource<?> override = mOverrideResources.get(id);
            if (override != null && override.getResType() == type) {
                return override.getData();
            }
            throw new NotFoundException();
        }
    }

    private static PackageInfo createPackageInfo(String packageName) {
        PackageInfo packageInfo = new PackageInfo();
        packageInfo.packageName = packageName;
        return packageInfo;
    }

    private ExternalResources() {}
}
