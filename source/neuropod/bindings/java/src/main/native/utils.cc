/* Copyright (c) 2020 UATC, LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "utils.h"

#include "jclass_register.h"
#include "neuropod/neuropod.hh"

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <jni.h>

namespace neuropod::jni
{

const std::string TENSOR_TYPE = "Lcom/uber/neuropod/TensorType;";

std::string to_string(JNIEnv *env, jstring target)
{
    const char *raw = env->GetStringUTFChars(target, nullptr);
    if (!raw || env->ExceptionCheck())
    {
        throw std::runtime_error("invalid jstring in to_string");
    }

    std::string res(raw);
    env->ReleaseStringUTFChars(target, raw);
    return res;
}

jclass find_class(JNIEnv *env, const std::string &name)
{
    jclass ret = env->FindClass(name.c_str());
    if (!ret || env->ExceptionCheck())
    {
        throw std::runtime_error("class not found: " + name);
    }
    return ret;
}

jmethodID get_method_id(JNIEnv *env, jclass clazz, const std::string &name, const std::string &sig)
{
    jmethodID ret = env->GetMethodID(clazz, name.c_str(), sig.c_str());
    if (!ret || env->ExceptionCheck())
    {
        throw std::runtime_error("method ID not found: " + name + sig);
    }
    return ret;
}

jobject get_tensor_type_field(JNIEnv *env, const std::string &fieldName)
{
    jfieldID field = env->GetStaticFieldID(com_uber_neuropod_TensorType, fieldName.c_str(), TENSOR_TYPE.c_str());
    if (!field || env->ExceptionCheck())
    {
        throw std::runtime_error("field not found: " + fieldName);
    }
    return env->GetStaticObjectField(com_uber_neuropod_TensorType, field);
}

std::string tensor_type_to_string(TensorType type)
{
    std::string       typeString;
    std::stringstream ss;
    ss << type;
    ss >> typeString;
    return typeString;
}

void throw_java_exception(JNIEnv *env, const std::string &message)
{
    // We encountered an error condition. There can be active Java exception.
    if (env->ExceptionCheck())
    {
        // Prints an exception and a backtrace of the stack to stderr.
        // This is a convenience routine provided for debugging.
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    env->ThrowNew(com_uber_neuropod_NeuropodJNIException, message.c_str());
}

} // namespace neuropod::jni
