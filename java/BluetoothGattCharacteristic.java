import java.util.*;

public class BluetoothGattCharacteristic extends BluetoothObject
{
    private long nativeInstance;

    public native BluetoothType getBluetoothType();
    public native BluetoothGattCharacteristic clone();
    public native List<Byte> readValue();
    public native boolean writeValue(List<Byte> argValue);
    public native boolean startNotify();
    public native boolean stopNotify();
    public native String getUuid();
    public native BluetoothGattService getService();
    public native List<Byte> getValue();
    public native boolean getNotifying();
    public native String[] getFlags();
    public native List<BluetoothGattDescriptor> getDescriptors();

    private native void init(BluetoothGattCharacteristic obj);
    private native void delete();

    private BluetoothGattCharacteristic(long instance)
    {
        super(instance);
        nativeInstance = instance;
    }

    protected void finalize()
    {
        delete();
    }
}
