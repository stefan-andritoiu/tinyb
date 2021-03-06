find_package(Java REQUIRED)
include(UseJava)

if (NOT DEFINED $ENV{JAVA_HOME_NATIVE})
  set (JAVA_HOME_NATIVE $ENV{JAVA_HOME})
  set (JAVAC $ENV{JAVA_HOME}/bin/javac)
  set (JAR $ENV{JAVA_HOME}/bin/jar)
else ()
  set (JAVAC $ENV{JAVA_HOME_NATIVE}/bin/javac)
  set (JAR $ENV{JAVA_HOME_NATIVE}/bin/jar)
endif ()


# Check that we can use javah
if(NOT Java_JAVAH_EXECUTABLE)
    message(FATAL_ERROR "Cannot locate javah executable.")
endif(NOT Java_JAVAH_EXECUTABLE)

file(GLOB JAVA_SOURCES "*.java")
add_jar(tinybjar ${JAVA_SOURCES}
                  OUTPUT_NAME tinyb
)

set(JAVA_CLASSES tinyb.BluetoothAdapter
                 tinyb.BluetoothCallback
                 tinyb.BluetoothDevice
                 tinyb.BluetoothEvent
                 tinyb.BluetoothGattCharacteristic
                 tinyb.BluetoothGattService
                 tinyb.BluetoothGattDescriptor
                 tinyb.BluetoothManager
                 tinyb.BluetoothObject
                 tinyb.BluetoothType
                 tinyb.ObjectArgCallback
                 tinyb.ObjectArrayArgCallback)
add_custom_command (TARGET tinybjar
  POST_BUILD
  COMMAND cmake -E echo "Generating JNI headers.."
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/tinybjar.dir"
  COMMAND ${Java_JAVAH_EXECUTABLE} -d jni/ -jni ${JAVA_CLASSES}
)

set(JNI_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/tinybjar.dir/jni")

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/tinyb.jar DESTINATION ${CMAKE_INSTALL_LIBDIR}/java)

add_subdirectory (jni)
