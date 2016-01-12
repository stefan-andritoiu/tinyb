#include "tinyb/BluetoothAdapter.hpp"
#include "tinyb/BluetoothDevice.hpp"
#include "tinyb/BluetoothGattService.hpp"
#include "tinyb/BluetoothObject.hpp"

#include "BluetoothDevice.h"

#include "helper.h"

using namespace tinyb;

jobject Java_BluetoothAdapter_getBluetoothType(JNIEnv *env, jobject obj)
{
    (void)obj;

    return get_bluetooth_type(env, "DEVICE");
}

jobject Java_BluetoothDevice_clone(JNIEnv *env, jobject obj)
{
    return generic_clone<BluetoothDevice>(env, obj, "BluetoothDevice");
}

jboolean Java_BluetoothDevice_disconnect(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->disconnect() ? JNI_TRUE : JNI_FALSE;
}

jboolean Java_BluetoothDevice_connect(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->connect() ? JNI_TRUE : JNI_FALSE;
}

jboolean Java_BluetoothDevice_connectProfile(JNIEnv *env, jobject obj, jstring str)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    jboolean is_copy = JNI_TRUE;
    const char *str_chars = (char *)env->GetStringUTFChars(str, &is_copy);
    const std::string string_to_write = std::string(str_chars);

    env->ReleaseStringUTFChars(str, str_chars);

    return obj_device->connect_profile(string_to_write);
}

jboolean Java_BluetoothDevice_disconnectProfile(JNIEnv *env, jobject obj, jstring str)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    jboolean is_copy = JNI_TRUE;
    const char *str_chars = (char *)env->GetStringUTFChars(str, &is_copy);
    const std::string string_to_write = std::string(str_chars);

    env->ReleaseStringUTFChars(str, str_chars);

    return obj_device->disconnect_profile(string_to_write);
}

jboolean Java_BluetoothDevice_pair(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->pair() ? JNI_TRUE : JNI_FALSE;
}

jboolean Java_BluetoothDevice_cancelPairing(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->cancel_pairing() ? JNI_TRUE : JNI_FALSE;
}

jobject Java_BluetoothDevice_getServices(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    std::vector<std::unique_ptr<BluetoothGattService>> array = obj_device->get_services();
    jobject result = convert_vector_to_jobject<BluetoothGattService>(env, array,
                                                                    "BluetoothGattService",
                                                                    "(J)V");

    return result;
}

jstring Java_BluetoothDevice_getAddress(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    std::string address = obj_device->get_address();

    return env->NewStringUTF((const char *)address.c_str());
}

jstring Java_BluetoothDevice_getName(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    std::string name = obj_device->get_name();

    return env->NewStringUTF((const char *)name.c_str());
}

jstring Java_BluetoothDevice_getAlias(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    std::string alias = obj_device->get_alias();

    return env->NewStringUTF((const char *)alias.c_str());
}

void Java_BluetoothDevice_setAlias(JNIEnv *env, jobject obj, jstring str)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    jboolean is_copy = JNI_TRUE;
    const char *str_chars = (char *)env->GetStringUTFChars(str, &is_copy);
    const std::string string_to_write = std::string(str_chars);

    env->ReleaseStringUTFChars(str, str_chars);

    obj_device->set_alias(string_to_write);
}

jint Java_BluetoothDevice_getBluetoothClass(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return (jlong)obj_device->get_class();
}

jshort Java_BluetoothDevice_getAppearance(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return (jshort)obj_device->get_appearance();
}

jstring Java_BluetoothDevice_getIcon(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    std::string icon = obj_device->get_icon();

    return env->NewStringUTF((const char *)icon.c_str());
}

jboolean Java_BluetoothDevice_getPaired(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->get_paired() ? JNI_TRUE : JNI_FALSE;
}

jboolean Java_BluetoothDevice_getTrusted(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->get_trusted() ? JNI_TRUE : JNI_FALSE;
}

void Java_BluetoothDevice_setTrusted(JNIEnv *env, jobject obj, jboolean val)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    bool val_to_write = from_jboolean_to_bool(val);
    obj_device->set_trusted(val_to_write);
}

jboolean Java_BluetoothDevice_getBlocked(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->get_blocked() ? JNI_TRUE : JNI_FALSE;
}

void Java_BluetoothDevice_setBlocked(JNIEnv *env, jobject obj, jboolean val)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    bool val_to_write = from_jboolean_to_bool(val);
    obj_device->set_blocked(val_to_write);
}

jboolean Java_BluetoothDevice_getLegacyPairing(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->get_legacy_pairing() ? JNI_TRUE : JNI_FALSE;
}

jshort Java_BluetoothDevice_getRssi(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return (jshort)obj_device->get_rssi();
}

jboolean Java_BluetoothDevice_getConnected(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);

    return obj_device->get_connected() ? JNI_TRUE : JNI_FALSE;
}

jobjectArray Java_BluetoothDevice_getUuids(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    std::vector<std::string> uuids = obj_device->get_uuids();
    unsigned int uuids_size = uuids.size();

    jclass string_class = search_class(env, "Ljava/lang/String;");
    jobjectArray result = env->NewObjectArray(uuids_size, string_class, 0);

    for (unsigned int i = 0; i < uuids_size; ++i)
    {
        std::string str_elem = uuids.at(i);
        jobject elem = env->NewStringUTF((const char *)str_elem.c_str());
        env->SetObjectArrayElement(result, i, elem);
    }

    return result;
}

jstring Java_BluetoothDevice_getModalias(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    std::string modalias = obj_device->get_modalias();

    return env->NewStringUTF((const char *)modalias.c_str());
}

jobject Java_BluetoothDevice_getAdapter(JNIEnv *env, jobject obj)
{
    BluetoothDevice *obj_device = getInstance<BluetoothDevice>(env, obj);
    BluetoothAdapter *obj_adapter = obj_device->get_adapter().clone();

    jclass b_adapter_class = search_class(env, "BluetoothAdapter");
    jmethodID b_adapter_ctor = search_method(env, b_adapter_class, "<init>",
                                            "(J)V", false);
    jobject result = env->NewObject(b_adapter_class, b_adapter_ctor, (jlong)obj_adapter);
    if (result == NULL)
    {
        throw std::runtime_error("cannot create instance of class\n");
    }

    return result;
}

void Java_BluetoothDevice_delete(JNIEnv *env, jobject obj)
{
    BluetoothDevice *b_device = getInstance<BluetoothDevice>(env, obj);
    delete b_device;
}

