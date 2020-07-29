---
title: DLangUI Font and Text Rendering Problem
date:  2020-07-21 15:00:00 +0900
last_modified_at: 2020-07-21 20:00:00 +0900
categories:
  - dlangui
  - font
  - freetype
---

글자를 그리다가 만난 문제들을 분석했다.

### System and Tools Version

- Ubuntu 20.04 LTS
- DLabgUI 0.9.182 
- Windows 8

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

![ D2Cdding Ligature ](/screenshot/20200714_d2ligature.png)

## Noto Color Emoji 글꼴

Noto Color Emoji 글꼴의 경우 `FT_Set_Pixel_Sizes` 함수에서
에러 23번이 나서 글꼴을 가져오지 못하는데 그 글꼴을 사용하기 때문에 죽는다.
Noto Color Emoji 글꼴은 fixed size 글꼴이서서 확대 촉소가 되지 않는데
글꼴 선택 목록에 잘못 나타난 것 같다.

디버깅은 python 코드로 여러번 시도해 보는게 도움이 되었다.

```py
>>> import freetype
>>> face = freetype.Face("/usr/share/fonts/truetype/noto/NotoColorEmoji.ttf")
>>> face.is_scalable
False
>>> face.set_char_size( 48*64 )
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/usr/local/lib/python3.8/dist-packages/freetype/__init__.py", line 1206, in set_char_size
    if error: raise FT_Exception( error)
freetype.ft_errors.FT_Exception: FT_Exception:  (invalid pixel size)
>>> face.available_sizes[0].width
136
>>> face.available_sizes[0].height
128
>>> face.available_sizes[0].size
6976
>>> face.set_char_size( 6976 )
>>> face.load_char('S')
>>> bitmap = face.glyph.bitmap
>>> print(bitmap.buffer)
[]
```

같은 code를 NotoMono 글꼴로 실행하면 다음과 같다.

```py
>>> import freetype
>>> face = freetype.Face("/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf")
>>> face.is_scalable
True
>>> face.set_char_size( 48*64 )
>>> face.load_char('S')
>>> bitmap = face.glyph.bitmap
>>> print(bitmap.buffer)
[0, 0, 0, 0, 0, 0, 35, 118, 169, 214, 233, 248, 2
...
```

### Noto Color Emoji with Gtk FontManager

FontManager 에서 Noto Color Emoji 글꼴을 볼수 있다.
글꼴 확대 축소도 되는것 같은데 이 프로그램에서는 왜 되는걸까?

![Noto Color Emoji with FontManager](/screenshot/noto_color_emoji.png)

## 고정폭 여백 넓이 계산 오류

MathJax_Vector, MathJax_Vector-Bold 글꼴에서 tabSize가 0이 다음 코드에서
프로그램이 죽는다.

```D
 248         int tabWidth = spWidth * tabSize; // width of full tab in pixels
 249         tabOffset = tabOffset % tabWidth;
```

원인을 찾아보나 `Font::measureText()` 함수에서 fixedCharWidth, spaceWidth 등으로
tabSize를 계산하는데 MathJax_Vector 글꼴의 경우 다음 조건으로 tabSize 가 0이 된다.

```D
charWidth('i') = 0
charWidth('M') = 0
charWidth(' ') = 0
```

## 기타

1. 고정폭 글꼴을 알아내는데 휴리스틱한 방법을 사용한다. 글꼴 정보의 값을 사용하는 것은?
