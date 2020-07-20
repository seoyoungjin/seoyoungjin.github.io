---
title:  "DLangUI OpenGL"
date:   2020-07-11 10:00:00 +0900
categories:
  - dlangui
---

## Compile error

**DerelictGL3**를 사용하는데 패키지 구성이 많이 바뀐것 같다.
예전 소스를 보면 다음과 같은 줄이 있는데 컴파일시 에러가 난다.
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
문법이 바뀌었나 보다. 마이너 언어다 보니 문법이 바뀌어도 예제에 적용되지 않는다.
하긴 Python 같은 메이저 언어도 같은 문제를 격는데 D는 말할것도 없겠다.

아무튼 대충 수정후 실행해서 다음 화면을 보았다.

- GLFW 예제 실행 화면

![GLFW sample](/image/dlang_glfw.png)

## Derelict 패키지 버전 문제

DerelictGL3 가 1,x에서 2.x로 넘어 오면서 API 및 파일 구조가 바뀌었다.
이전 버전에는 Window나 X 같은 플랫폼 기능이 있는데
이는 또 **derect-util**에 의존성이 있다.

패키지가 몇개 안되는데로 이 정도면 조금만 더 지나면 의존성 헬이 우려된다.
