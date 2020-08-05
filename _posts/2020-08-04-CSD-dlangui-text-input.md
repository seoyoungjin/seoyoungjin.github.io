---
title: CSD and DLangUI Text Input
date:  2020-08-24 12:00:00 +0900
last_modified_at: 2020-08-24 12:00:00 +0900
categories:
  - CSD
  - dlangui
  - text input
---

## CSD (Client side descoration)

Gnome 일부 어플리케이션에서 윈도우 타이틀에 메뉴와 아이콘이 표시된걸 보았다.
이 영역은 윈도우 매니저가 그리는 영역인데 어떻게 어플리케이션 메뉴가 나오는지 궁금했다.
글자는 그렇다 치고 아이콘까지 표시되는건 그리 간단해 보이지 않았다.

조사해 보니  Client side Decoration이라고 용어가 있다.

- https://en.wikipedia.org/wiki/Client-side_decoration

- GtkHeaderBar 위젯

![CSD 예제](http://www.kcjengr.com/assets/posts/2017-09-29-gtk-headerbar/light-theme.png)

## Text Input on Windows

윈도우에서 텍스트를 엽력할 경우 화면 왼쪽 위 모서리에 글자를 조합중인 창이 뜨고
리턴키를 누르면 글자가 위젯으로 전달된다.
요즘은 Text Services Framework이라고 한다는데 다음 자료를 참고하자.

- https://stackoverflow.com/questions/434048/how-do-you-use-ime/1249990

`EdittorWidgetBase::OnKeyEvent` log

KeyDown and Text event comes with one key down

```
2020-08-05 11:25:28.412 D  onKeyEvent KeyDown 65 flags 0
2020-08-05 11:25:28.412 D  onKeyEvent Text 0 flags 0
2020-08-05 11:25:31.253 D  onKeyEvent KeyDown 8 flags 0
2020-08-05 11:25:32.846 D  onKeyEvent KeyDown 229 flags 0
2020-08-05 11:25:33.815 D  onKeyEvent KeyDown 229 flags 0
2020-08-05 11:25:35.269 D  onKeyEvent KeyDown 229 flags 0
2020-08-05 11:25:37.780 D  onKeyEvent KeyDown 229 flags 0
2020-08-05 11:25:37.780 D  onKeyEvent Text 0 flags 0
2020-08-05 11:25:37.796 D  onKeyEvent KeyDown 13 flags 0
```

## Text Input on Linux

Linux에서는 Windows와 다르게 위젯의 keyEvent로는 조합중의
keyEvent나 조합 중의 한글도 오지 않고 이상한 글자가 찍한다.
DLangUI core나 xcb 백엔드에서 뭔가 잘못하고 있다. TT

```
2020-08-04 13:35:37.652 D  keyEvent code = 75 action = KeyDown
2020-08-04 13:35:43.175 D  keyEvent code = 160 action = KeyDown
2020-08-04 13:35:51.529 D  keyEvent code = 160 action = KeyDown
2020-08-04 13:35:52.687 D  keyEvent code = 65 action = KeyDown
```
