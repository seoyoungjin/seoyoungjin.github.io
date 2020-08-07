---
title: Font fallback
date:  2020-08-06 12:00:00 +0900
last_modified_at: 2020-08-06 12:00:00 +0900
categories:
  - font fallback
---

선택한 글꼴 아닌 다른 글꼴로 대체(fallbak)하는 경우는 거의 다음 두가지 경우이다.

- 선택한 글꼴시 시스템에 설치되지 않았다.
- 선택한 글꼴에 빈 부분이 있어서 그리고자 하는 글자가 없다.

Windows에서는 XAML로 하는것 같고 Linux에서는 FontConfig에서 하는데
dfontview에서 이걸 해석할 수 있을것 같지는 않다. TT

```
<?xml version="1.0"?>
<!DOCTYPE fontconfig SYSTEM "fonts.dtd">
<fontconfig>
<alias>
   <family>serif</family>
   <prefer>
     <family>Your favorite Latin Serif font name</family>
     <family>Your Chinese font name</family>
   </prefer>
 </alias>
</fontconfig>
```

fc-match
---------

```
$ fc-match monospace
DejaVuSansMono.ttf: "DejaVu Sans Mono" "Book"
```

```
$ fc-match -s monospace:charset=1F4A9
```

Arch Linux fontconfig
---------------------

여러 가지 경우를 fallback sequence를 정의하고 있는데 보기만 해도 두통이 올것 같다.
그리고 안스럽기도 하다. TT

[Arch Linux Font configuration](https://wiki.archlinux.org/index.php/Font_configuration)

