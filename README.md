演示画布用法 演示qt webassembly用法

# Webassembly功能测试

## CLion下测试

```shell
# 配置了以下cmake参数
-DCMAKE_PREFIX_PATH=~/Qt/6.2.0/macos/lib/cmake
-DCMAKE_BUILD_TYPE:STRING=MinSizeRel
-DCMAKE_TOOLCHAIN_FILE:PATH=~/Qt/6.2.0/wasm_32/lib/cmake/Qt6/qt.toolchain.cmake
```

## qt creator下测试

```shell
# 配置以下cmake参数
-GNinja
-DCMAKE_BUILD_TYPE:STRING=MinSizeRel
-DCMAKE_PROJECT_INCLUDE_BEFORE:PATH=%{IDE:ResourcePath}/package-manager/auto-setup.cmake
-DQT_QMAKE_EXECUTABLE:STRING=%{Qt:qmakeExecutable}
-DCMAKE_PREFIX_PATH:STRING=%{Qt:QT_INSTALL_PREFIX}
-DCMAKE_C_COMPILER:STRING=%{Compiler:Executable:C}
-DCMAKE_CXX_COMPILER:STRING=%{Compiler:Executable:Cxx}
-DCMAKE_TOOLCHAIN_FILE:PATH=%{Qt:QT_INSTALL_PREFIX}/lib/cmake/Qt6/qt.toolchain.cmake
```

## 其它说明

* 修改了CMakeLists.txt文件，通过条件变量设置在LINUX或MAC下编译main.cc文件，在WebAssembly环境下不编译
* public目录是一个指向cmake-build-webassembly的软链接，为了方便通过vite演示生成的wasm页面

