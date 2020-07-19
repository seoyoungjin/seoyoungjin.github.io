---
layout: post
title:  "DLangUI FontManager and FontConfig"
date:   2020-07-13 11:50:00 +0900
categories: dlangui
---

### System and Tools Version

- Ubuntu 20.04 LTS
- DLangUI 0.9.182 

## FontManager Problem

다음과 같이 FontMaanger에서 한글이 확실히 나오는 D2Coding 글꼴을
선택했는데 돌려주는 글꼴의 이름을 출력해 보니 다른 글꼴이었다.

```d
        FontRef font = FontManager.instance.getFont(25, FontWeight.Normal,
            false, FontFamily.SansSerif, "D2Coding");
```

FontManager 초기화 부분에서 글꼴을 나일해보니 다음과 갈았디.

```
2020-07-13 12:34:40.965 I  Getting list of fonts using FontConfig
2020-07-13 12:34:40.979 I  Font: 0 Ubuntu Mono
2020-07-13 12:34:40.980 I  Font: 1 KacstTitle
2020-07-13 12:34:40.980 I  Font: 2 Abyssinica SIL
2020-07-13 12:34:40.980 I  Font: 3 DejaVu Sans Mono
...

2020-07-13 12:34:41.001 I  Font: 336 Umpush
2020-07-13 12:34:41.002 I  FontConfig: 337 font files registered in 37ms
```

## libfontconfig 
C 언어로 libfontconfig를 사용하여 다음 예제로 폰트 경로와 폰트 페이스를
나열해 보았다. 다음과 같이 총 75개의 경로와 449개의 글꼴이 나열되었다.

FontManager의 결과와 다른데 제외된 글꼴 중에 D2Coding 글꼴이 들어있다.

**그 이유는 DLangUI에서는 글꼴의 확장자를 검색하는데
77개의 `.ttc` 글꼴과 35개의 `.t1` 글꼴이 제외되있다.**

```c
{% include_relative fontlist.c %}
```

```
/usr/share/fonts
/usr/local/share/fonts
/home/yjseo/.local/share/fonts
/home/yjseo/.fonts
/usr/share/fonts/X11
/usr/share/fonts/cMap
/usr/share/fonts/cmap
/usr/share/fonts/opentype
/usr/share/fonts/truetype
/usr/share/fonts/type1
/usr/share/fonts/X11/Type1
/usr/share/fonts/X11/encodings
/usr/share/fonts/X11/misc
/usr/share/fonts/X11/util
/usr/share/fonts/cmap/adobe-cns1
/usr/share/fonts/cmap/adobe-gb1
/usr/share/fonts/cmap/adobe-japan1
/usr/share/fonts/cmap/adobe-japan2
/usr/share/fonts/cmap/adobe-korea1
/usr/share/fonts/opentype/malayalam
/usr/share/fonts/opentype/mathjax
...
/usr/share/fonts/truetype/naver-d2coding
/usr/share/fonts/truetype/noto
...
/usr/share/fonts/type1/gsfonts
/usr/share/fonts/type1/urw-base35
/usr/share/fonts/X11/encodings/large
```

```
Total fonts: 449
Family: Samyak Malayalam
...
```

## python-fontconfig 

C로 예제를 만들다가 문뜩 어리석은 짓을 하고 있었다는 생각이 들었다.
`python-fontconfig`라는 좋은 툴이 있었다.

```
[GCC 9.3.0] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import fontconfig
>>> fonts = fontconfig.query(family='', lang='en')
>>> len(fonts)
300
>>> fonts = fontconfig.query(family='', lang='')
>>> len(fonts)
387
>>> 
```

`python-fontconfig` 소스를 살펴 보니 다음과 같았다.

```
    os = FcObjectSetBuild (FC_CHARSET, FC_FILE, NULL);
```

FC_FAMILY를 추가하면 447이라는 숫자가 나오고 FC_INDEX를 추가하면 449라는
숫자가 나오는데 같은 파일이 중복되어 나타나는 것이었다.

## 결론

1. 실제 글꼴 파일의 숫자는 387개였다.
2. FC_FAMILY 를 선택하면 하나의 글꼴안에 있는 여러개의 Family가 따로 나일되어서 447이 된다.
```
Noto Sans CJK HK : /usr/share/fonts/opentype/noto/NotoSansCJK-Black.ttc
Noto Sans CJK TC : /usr/share/fonts/opentype/noto/NotoSansCJK-Black.ttc
Noto Sans CJK KR : /usr/share/fonts/opentype/noto/NotoSansCJK-Black.ttc
Noto Sans CJK JP : /usr/share/fonts/opentype/noto/NotoSansCJK-Black.ttc
Noto Sans CJK SC : /usr/share/fonts/opentype/noto/NotoSansCJK-Black.ttc
...
D2Coding ligature : /usr/share/fonts/truetype/naver-d2coding/D2Coding-Ver1.3.2-20180524-all.ttc
D2Coding : /usr/share/fonts/truetype/naver-d2coding/D2Coding-Ver1.3.2-20180524-all.ttc
```
2. FC_INDEX의 의미는 아직 잘 모르겠지만 D2Coding 글꼴에만 해당한다.

## DLangUI FontManager

다음과 같이 글꼴을 나열할 수 있다.

```d
import dlangui.graphics.fonts;

mixin APP_ENTRY_POINT;

/// entry point for dlangui based application
extern (C) int UIAppMain(string[] args) 
{
    FontFaceProps[] faces = FontManager.instance.getFaces();
    writeln("Total fonts: ", faces.length);
    for (auto i = 0; i < faces.length; ++i) {
        writeln(i, " ", faces[i].face, " ", faces[i].family);
    }
    
    return 0;
}
```

```
Total fonts: 148
0 Ubuntu Mono MonoSpace
1 KacstTitle SansSerif
2 Abyssinica SIL SansSerif
3 DejaVu Sans Mono MonoSpace
4 KacstOffice SansSerif
...
```

## Windows의 경우

Windows 8이 설치된 PC에 DLangUI 예제를 돌려보았다.
`ui.log` 파일을 보면 다음과 같이 286개의 골꼴을 볼 수 있다.
그런데 이 문자열이 현재 locale에 따른 EUC-KR 문자라
D의 string 함수로 utf8 변환하면서 exception이 났다.
win32의 관련 함수를 호출하도록 patch 하니 글꼴 이를을
제데로 볼 수 있었고 또 이 글꼴을 사용할 수가 있었다.

```
DlangUI log file
...
2020-07-12 17:20:07.832 I  Win32FontManager.initialize()
2020-07-12 17:20:07.832 D  registerFont(MonoSpace,SimSun-ExtB)
2020-07-12 17:20:07.832 D  registerFont(Serif,KodchiangUPC)
...
2020-07-12 17:20:07.833 D  registerFont(Serif,맑은 고딕)
2020-07-12 17:20:07.834 D  registerFont(Serif,바탕)
2020-07-12 17:20:07.834 D  registerFont(MonoSpace,바탕체)
2020-07-12 17:20:07.834 D  registerFont(Serif,궁서)
2020-07-12 17:20:07.834 D  registerFont(MonoSpace,궁서체)
...
2020-07-12 17:20:07.839 D  registerFont(Serif,함초롬바탕)
2020-07-12 17:20:07.839 D  registerFont(Serif,함초롬바탕 확장)
2020-07-12 17:20:07.839 D  registerFont(Serif,함초롬돋움)
2020-07-12 17:20:07.839 D  registerFont(Serif,함초롬돋움 확장)
2020-07-12 17:20:07.839 D  registerFont(Serif,함초롬바탕 확장B)
2020-07-12 17:20:07.839 D  registerFont(SansSerif,NewJumja)
2020-07-12 17:20:07.839 D  registerFont(Serif,문체부 훈민정음체)
2020-07-12 17:20:07.839 D  registerFont(Serif,나눔고딕)
...
2020-07-12 17:20:07.845 I  Found 285 font faces
```

### Korean Face Name on Windows

![Korean Font Face Name](/screenshot/dfontview/20200716_gungseo_win.png)