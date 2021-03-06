/*
 * Author: Andrei Vasiliu <andrei.vasiliu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <vector>
#include "tinyb/BluetoothObject.hpp"

jfieldID getInstanceField(JNIEnv *env, jobject obj);

jclass search_class(JNIEnv *env, const char *clazz_name);
jclass search_class(JNIEnv *env, tinyb::BluetoothObject &object);
jmethodID search_method(JNIEnv *env, jclass clazz, const char *method_name,
                        const char *prototype, bool is_static);
jfieldID search_field(JNIEnv *env, jclass clazz, const char *field_name,
                        const char *type, bool is_static);
bool from_jboolean_to_bool(jboolean val);
jobject get_bluetooth_type(JNIEnv *env, const char *field_name);
jobject get_new_arraylist(JNIEnv *env, unsigned int size, jmethodID *add);

template <typename T>
T *getInstance(JNIEnv *env, jobject obj)
{
    jlong instance = env->GetLongField(obj, getInstanceField(env, obj));
    return reinterpret_cast<T *>(instance);
}

template <typename T>
void setInstance(JNIEnv *env, jobject obj, T *t)
{
    jlong instance = reinterpret_cast<jlong>(t);
    env->SetLongField(obj, getInstanceField(env, obj), instance);
}

template <typename T>
jobject generic_clone(JNIEnv *env, jobject obj, const char *class_name)
{
    T *obj_generic = getInstance<T>(env, obj);
    T *copy_generic = new T(*obj_generic);

    jclass generic_class = search_class(env, class_name);
    jmethodID generic_ctor = search_method(env, generic_class, "<init>", "(J)V", false);

    jobject result = env->NewObject(generic_class, generic_ctor, (jlong)copy_generic);
    if (result == NULL)
    {
        throw std::runtime_error("cannot create instance of class\n");
    }

    return result;
}

template <typename T>
jobject convert_vector_to_jobject(JNIEnv *env, std::vector<std::unique_ptr<T>>& array,
                                const char *ctor_prototype)
{
    unsigned int array_size = array.size();

    jmethodID arraylist_add;
    jobject result = get_new_arraylist(env, array_size, &arraylist_add);

    if (array_size == 0)
        return result;

    jclass clazz = search_class(env, T::java_class().c_str());
    jmethodID clazz_ctor = search_method(env, clazz, "<init>", ctor_prototype, false);

    for (unsigned int i = 0; i < array_size; ++i)
    {
        T *elem = array.at(i).release();
        jobject object = env->NewObject(clazz, clazz_ctor, (long)elem);
        if (object == NULL)
        {
            throw std::runtime_error("cannot create instance of class\n");
        }
        env->CallBooleanMethod(result, arraylist_add, object);
    }
    return result;
}
