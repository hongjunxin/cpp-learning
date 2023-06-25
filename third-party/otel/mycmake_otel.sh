TARGET_SYSTEM="linux"

while [ $# -gt 0 ] ; do
    case "$1" in
        --target-system)
            if [ $# -lt 2 ]; then
                echo "--target-system missing value"
                exit -1
            fi
            TARGET_SYSTEM=$2
            shift 2
            ;;
        -h|--help|*)
            echo "--target-system linux|android"
            exit 0
            ;;
    esac
done

if [ ${TARGET_SYSTEM} == "linux" ]; then
    cmake -B build/linux \
    -DBUILD_TESTING=OFF \
    -DWITH_EXAMPLES_HTTP=ON \
    -DWITH_JAEGER=ON \
    -DWITH_STL=ON \
    -DWITH_ABSEIL=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=17
else 
    cmake -B build/android \
    -DCMAKE_TOOLCHAIN_FILE=~/android/android-ndk-r21e/build/cmake/android.toolchain.cmake \
    -DANDROID_NDK=~/android/android-ndk-r21e \
    -DCMAKE_SYSTEM_NAME=Android \
    -DANDROID_PLATFORM=android-28 \
    -DANDROID_NATIVE_API_LEVEL=28 \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_STL=c++_static \
    -DANDROID=ON \
    -DBUILD_TESTING=OFF \
    -DWITH_EXAMPLES_HTTP=ON \
    -DWITH_JAEGER=ON \
    -DWITH_STL=ON \
    -DWITH_ABSEIL=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_INSTALL_PREFIX="~/android/android-ndk-r21e/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/local"
fi