---
layout: post
title:  "DLangUI on Windows"
date:   2020-07-10 10:00:00 +0900
categories: dlangui
---

# System and Tools Version

- Windows 8.1
- Visual Studio Community Edition 2019
- DUB version 1.21.0, built on Jun 11 2020

# dub link error

Visual Studio Build Tools 2019를 설치했으나 `libcmt.lib`을 찾을 수 없다는
오류가 났다. 관련 환경 변수가 전역으로 설정되지 않는 것 같다.

```
Linking...
lld-link: error: could not open 'libcmt.lib': no such file or directory
lld-link: error: could not open 'OLDNAMES.lib': no such file or directory
Error: linker exited with status 1
C:\D\dmd2\windows\bin\dmd.exe failed with exit code 1.
```

**Developer Command Prompt for VS 2019**를 참조하여 다음과 같이
환경변수 **VCINSTALLDIR**을 설정하니 제데로 동작한다.
```
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\
```

# dlangui의 한글

![DLangUI CJK text on Windows](/image/dlang_windows_hello.png)

# Notes

## monodevelop
mododevelop으로 컴파일 하는 설명이 많아 monodevelop을 설치하려고 시도했다.
예전 바이너리 버전은 어디서 받는지 알수 없고 최신 버전은 source로만 배포한다.
소스를 받아 컴파일하려고 시도하니 너무 많은 오류가 난다.
초심자가 할 일은 아니것 같아 포기한다.


## useful dub usage
```
dub describe -c unittest --nodeps --skip-registry=all --arch=x86_64
dub --arch=x86_64 --build=debug
```