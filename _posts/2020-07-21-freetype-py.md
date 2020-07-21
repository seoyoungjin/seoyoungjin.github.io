---
title: FontManager Debug
date:  2020-07-21 15:00:00 +0900
last_modified_at:   2020-07-21 15:00:00 +0900
categories:
  - dlangui
  - freetype
  - python
---

몇 가지 글꼴로 글자를 그리다가 오류를 만났다.

## Noto Color Emoji 글꼴

Noto Color Emoji 글꼴의 경우 `FT_Set_Pixel_Sizes` 함수에서
에러 23번이 나서 글꼴을 가져오지 못하는데 그 글꼴을 사용하기 때문에 죽는다.
디버깅할때 C 보다는 python 코드로 여러번 시도해 보는게 도움이 되었다.

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

NotoMono 글꼴로 같은 code를 실행하면 다음과 같다.

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

Noto Color Emoji 글꼴은 fixed size 글꼴이서서 확대 촉소가 되지 않는데
글꼴 선택 목록에 잘못 나타난 것 같다.

### Noto Color Emoji with Gtk FontManager

FontManager 에서 Noto Color Emoji 글꼴을 볼수 있다.
글꼴 확대 축소도 되는것 같은데 이 프로그램에서는 왜 되는걸까?

![Noto Color Emoji with FontManager](/screenshot/noto_color_emoji.png)

## 고정폭 글꼴 계산 오류



## 기타

1. 고정폭 글꼴을 알아내는데 휴리스틱한 방법을 사용한다. 글꼴 정보의 값을 사용하는 것은?