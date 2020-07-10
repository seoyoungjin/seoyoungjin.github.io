---
layout: post
title:  "DLangUI on Windows"
date:   2020-07-11 10:00:00 +0900
categories: dlangui
---

# System and Tools Version

- Windows 8.1
- Visual Studio Community Edition 2019
- DUB version 1.21.0, built on Jun 11 2020

# dub link error

Visual Studio Build Tools 2019 를 설치했으나 `libcmt.lib`을 찾을 수 없다는
오류가 났다. 관련 환경 변수가 전역으로 설정되지 않는 것 같다.

```
Linking...
lld-link: error: could not open 'libcmt.lib': no such file or directory
lld-link: error: could not open 'OLDNAMES.lib': no such file or directory
Error: linker exited with status 1
C:\D\dmd2\windows\bin\dmd.exe failed with exit code 1.
```

VS 콘솔을 참조하여 다음과 같이 VCINSTALLDIR을 설정하니 제데로 동작한다.
```
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\
```

# dlangui의 한글

![DLangUI CJK text on Windows](/image/dlang_windows_hello.png)