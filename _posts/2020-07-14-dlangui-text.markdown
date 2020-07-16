---
layout: post
title:  DLangUI Text Rendering
date:   2020-07-14 17:00:00 +0900
categories: dlangui
---

### System and Tools Version

- Ubuntu 20.04 LTS
- DLabgUI 0.9.182 

## Draw CJK Text Problem

D2Coding 글꼴 `안녕하세요`라는 문장을  출력하면 글자가 뭉개져서 나타
Arial Unicode MS 글꼴로 촐력하면 제데로 나타났다.

`drawText()` 함수를 찾아보니 다음과 같이 텍스트를 그리고 있다.

```
    bool isFixed = ...;
    foreach (ch; text) {
        if (isFixed) {
            x += fixedCharWidth;
            widths[i] = x;
        }
        else {
            Glyph glyph = getCharGlyph(ch, true);
            int kerningDelta = useKerning && prevChar ? getKerningOffset(ch, prevChar) : 0;
            width = ((glyph.widthScaled + kerningDelta + 63) >> 6);
            x += width;
            widths[i] = x;
        }
    }
```

두 글꼴에 대해 "안녕하세요" 문자열의 각각의 글자들은 다음과 같은 width를 가지고 있었다.
글꼴이 뭉개지는 원인은 고정폭 글꼴인 경우 CJK 문자에 대해서도 영어와 같은 간격으로
그리고 있기 때문이었다. 
- D2Cdoing

```
- isFixed = true
- fixedCharWidth = 13
- 0 13 26 39 52
```

- Arial Unicode MS

```
- isFixed = false
- fixedCharWidth = 21
- 0 25 50 75 100

```

### 해결책

1. 아시안 글자가 들어있는 글꼴은 fixed 글꼴로 처리하지 않는다.
2. fixed 글꼴로 처리하더라도 글자의 범위에 따라 CJK 글꼴은 두배로(?) 처리한다.

이런건 20세기때 문제 같은데...

## Local Face name in Windows

Windows 에서 글꼴 이름을 나열하면 `맑은 고딕`과 같이 한글 이름이 나타난다.
Linux에서도 이렇게 되는지 모르겠지만 문제는 글꼴 이름이 완성형으로 와사
D 언어에서 utf8 로 변환하는 중에 exception이 난다는 점이다.

`EnumFontFamiliesExA` 계열 함수를 써서 글꼴 이름을 구하는데 `EnumFontFamiliesExW`
계여 함수를 쓰면 될것 같은데 D 언어와 Windows에 모두 초보라 쉽지가 않다. 

## Ligature in `drawText()`

위의 `measureText` 알고리즘을 보면 글꼴에 따라 kerning은 적당히 처리하는것 같은데
ligature는 처리할 수가 없다. D2Coding 글꼴이 ligature를 지원한다고 하는데
조금 더 살펴보자.

이마 HarfBuzz나 Raqm 갈은 라이브러리를 사용해야 할것 같다.

![ D2Cdding Ligature ](/image/20200714_d2ligature.png)
