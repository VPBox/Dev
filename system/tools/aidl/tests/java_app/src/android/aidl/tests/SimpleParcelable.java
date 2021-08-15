/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.aidl.tests;

import android.os.Parcel;
import android.os.Parcelable;

public class SimpleParcelable implements Parcelable {
    private String mName;
    private int mNumber;

    SimpleParcelable() {}
    SimpleParcelable(String name, int number) {
        mName = name;
        mNumber = number;
    }

    public int describeContents() { return 0; }

    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(mName);
        dest.writeInt(mNumber);
    }

    public void readFromParcel(Parcel source) {
        mName = source.readString();
        mNumber = source.readInt();
    }

    public boolean equals(Object o) {
        if (o == null) {
            return false;
        }
        if (!(o instanceof SimpleParcelable)) {
            return false;
        }
        SimpleParcelable p = (SimpleParcelable)o;
        if ((mName == null && p.mName != null) ||
            (mName != null && !mName.equals(p.mName))) {
            return false;
        }
        return mNumber == p.mNumber;
    }

    public String toString() {
        return "SimpleParcelable(" + mName + ", " + mNumber + ")";
    }

    public static final Parcelable.Creator<SimpleParcelable> CREATOR =
            new Parcelable.Creator<SimpleParcelable>() {
        public SimpleParcelable createFromParcel(Parcel source) {
            String name = source.readString();
            int number = source.readInt();
            return new SimpleParcelable(name, number);
        }

        public SimpleParcelable[] newArray(int size) {
            return new SimpleParcelable[size];
        }
    };
}
