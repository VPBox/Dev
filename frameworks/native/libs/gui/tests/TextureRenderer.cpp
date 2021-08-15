/*
 * Copyright 2013 The Android Open Source Project
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

#include "TextureRenderer.h"

#include "GLTest.h"

#include <gui/GLConsumer.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <gtest/gtest.h>

namespace android {

TextureRenderer::TextureRenderer(GLuint texName,
        const sp<GLConsumer>& st) : mTexName(texName), mST(st), mPgm(0),
        mPositionHandle(-1), mTexSamplerHandle(-1), mTexMatrixHandle(-1) {
}

void TextureRenderer::SetUp() {
    const char vsrc[] =
        "attribute vec4 vPosition;\n"
        "varying vec2 texCoords;\n"
        "uniform mat4 texMatrix;\n"
        "void main() {\n"
        "  vec2 vTexCoords = 0.5 * (vPosition.xy + vec2(1.0, 1.0));\n"
        "  texCoords = (texMatrix * vec4(vTexCoords, 0.0, 1.0)).xy;\n"
        "  gl_Position = vPosition;\n"
        "}\n";

    const char fsrc[] =
        "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float;\n"
        "uniform samplerExternalOES texSampler;\n"
        "varying vec2 texCoords;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(texSampler, texCoords);\n"
        "}\n";

    {
        SCOPED_TRACE("creating shader program");
        ASSERT_NO_FATAL_FAILURE(GLTest::createProgram(vsrc, fsrc, &mPgm));
    }

    mPositionHandle = glGetAttribLocation(mPgm, "vPosition");
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    ASSERT_NE(-1, mPositionHandle);
    mTexSamplerHandle = glGetUniformLocation(mPgm, "texSampler");
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    ASSERT_NE(-1, mTexSamplerHandle);
    mTexMatrixHandle = glGetUniformLocation(mPgm, "texMatrix");
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    ASSERT_NE(-1, mTexMatrixHandle);
}

// drawTexture draws the GLConsumer over the entire GL viewport.
void TextureRenderer::drawTexture() {
    static const GLfloat triangleVertices[] = {
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
    };

    glVertexAttribPointer(mPositionHandle, 2, GL_FLOAT, GL_FALSE, 0,
            triangleVertices);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    glEnableVertexAttribArray(mPositionHandle);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());

    glUseProgram(mPgm);
    glUniform1i(mTexSamplerHandle, 0);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTexName);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());

    // XXX: These calls are not needed for GL_TEXTURE_EXTERNAL_OES as
    // they're setting the defautls for that target, but when hacking
    // things to use GL_TEXTURE_2D they are needed to achieve the same
    // behavior.
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());

    GLfloat texMatrix[16];
    mST->getTransformMatrix(texMatrix);
    glUniformMatrix4fv(mTexMatrixHandle, 1, GL_FALSE, texMatrix);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
}

} // namespace android
