---
layout: post
title:  "DLangUI OpenGL"
date:   2020-07-11 10:00:00 +0900
categories: dlangui
---

# dub compile error

**DerelictGL3**를 사용하는데 패키지 구성이 많이 바뀐것 같다.
예전 소스를 보면 다음과 같은 줄이 있는데 컴파일시 다음 에러가 난다.
```
import derelict.opengl3.gl;
import derelict.opengl3.gl3;
```

```
openglexample.d(37,8): Error: module gl is in file 'derelict\opengl3\gl.d'
```

다음과 같이 변경후 컴파일하면 Link 에러가 난다.
```
import derelict.opengl;
```

```
src\openglexample.d(141,31): Error: undefined identifier glLightfv
src\openglexample.d(158,18): Error: undefined identifier GL_LIGHTING
src\openglexample.d(159,18): Error: undefined identifier GL_LIGHT0
...
src\openglexample.d(428,5): Error: undefined identifier glEnd
src\openglexample.d(430,5): Error: undefined identifier glNormal3f
C:\D\dmd2\windows\bin\dmd.exe failed with exit code 1.
```

DerelictGL3 의 간단한 샘플을 돌려보려고 했더니 그동안 배열 관련 D 언어의
문법이 바뀌었나 보다. 설상가상이다. D는 너무 마이너 언어인가 보다.

아무튼 대충 수정후 실행해서 다음 화면을 보았다. 찬찬히 살펴보자. ^^

- GLFW 샘플

![GLFW 샘](/image/dlang_glfw.png)
