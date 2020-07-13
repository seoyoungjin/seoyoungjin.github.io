---
layout: post
title:  "DLangUI FontManager"
date:   2020-07-13 11:50:00 +0900
categories: dlangui
---

### System and Tools Version

- Ubuntu 20.04 LTS
- DKabgUI 0.9.182 

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
나열해 보았다. FontManager의 결과가 다른 것을 알 수 있다.

```c
{% include_relative fontlist.c %}
```

다음과 같이 총 75개의 경로와 416개의 글꼴이 나열되었다.
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
/usr/share/fonts/opentype/noto
/usr/share/fonts/opentype/urw-base35
/usr/share/fonts/truetype/Gargi
/usr/share/fonts/truetype/Gubbi
/usr/share/fonts/truetype/Nakula
/usr/share/fonts/truetype/Navilu
/usr/share/fonts/truetype/Sahadeva
/usr/share/fonts/truetype/Sarai
/usr/share/fonts/truetype/abyssinica
/usr/share/fonts/truetype/arphic
/usr/share/fonts/truetype/dejavu
/usr/share/fonts/truetype/droid
/usr/share/fonts/truetype/fonts-beng-extra
/usr/share/fonts/truetype/fonts-deva-extra
/usr/share/fonts/truetype/fonts-gujr-extra
/usr/share/fonts/truetype/fonts-guru-extra
/usr/share/fonts/truetype/fonts-kalapi
/usr/share/fonts/truetype/fonts-orya-extra
/usr/share/fonts/truetype/fonts-telu-extra
/usr/share/fonts/truetype/fonts-yrsa-rasa
/usr/share/fonts/truetype/freefont
/usr/share/fonts/truetype/kacst
/usr/share/fonts/truetype/kacst-one
/usr/share/fonts/truetype/lao
/usr/share/fonts/truetype/lato
/usr/share/fonts/truetype/liberation
/usr/share/fonts/truetype/liberation2
/usr/share/fonts/truetype/lohit-assamese
/usr/share/fonts/truetype/lohit-bengali
/usr/share/fonts/truetype/lohit-devanagari
/usr/share/fonts/truetype/lohit-gujarati
/usr/share/fonts/truetype/lohit-kannada
/usr/share/fonts/truetype/lohit-malayalam
/usr/share/fonts/truetype/lohit-oriya
/usr/share/fonts/truetype/lohit-punjabi
/usr/share/fonts/truetype/lohit-tamil
/usr/share/fonts/truetype/lohit-tamil-classical
/usr/share/fonts/truetype/lohit-telugu
/usr/share/fonts/truetype/malayalam
/usr/share/fonts/truetype/naver-d2coding
/usr/share/fonts/truetype/noto
/usr/share/fonts/truetype/openoffice
/usr/share/fonts/truetype/padauk
/usr/share/fonts/truetype/pagul
/usr/share/fonts/truetype/samyak
/usr/share/fonts/truetype/samyak-fonts
/usr/share/fonts/truetype/sinhala
/usr/share/fonts/truetype/tibetan-machine
/usr/share/fonts/truetype/tlwg
/usr/share/fonts/truetype/ttf-khmeros-core
/usr/share/fonts/truetype/ubuntu
/usr/share/fonts/type1/gsfonts
/usr/share/fonts/type1/urw-base35
/usr/share/fonts/X11/encodings/large
```

```
Total fonts: 416
Family: Samyak Malayalam
...
```

## python-fontconfig 

C로 예제를 만들다가 문뜩 어리석은 짓을 하고 있었다는 생각이 들었다.
`python-fontconfig`라는 좋은 툴이 있었다.
글꼴 숫자들이 다르게 나오는 것은 typeface와 lang 그리고 아마도
글꼴의 형식이나 아니면 중복된 글꼴 때문이라고 짐작한다.

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

## DLangUI FontManager

다음 코드를 실행하니 이번에는 148개의 Face가 나온다. TT

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

### Korean Text Buttun on Windows

![Korean Text Button](/image/dlangui_hello_ko_win.png)