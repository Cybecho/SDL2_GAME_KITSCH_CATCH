# SDL2_GAME_KITSCH_CATCH
2024 게임프로그래밍 수업 2팀 '키치캐치대작전' 프로젝트

```프로젝트 세팅```

# 작업 디렉토리 설정
<aside>
💡 디버깅 - 작업 디렉토리 - $(TargetDir) 로 설정해줍시다. 
  
  
  -> 시작 기준이 exe가 존재하는 디렉토리를 기준으로 파일을 탐색합니다

```
$(TargetDir)
```

![image](https://github.com/Cybecho/SDL2_GAME_KITSCH_CATCH/assets/42949995/07b5af6d-6520-44fa-97b7-a521bb90237a)

</aside>

# 파일 위치

<aside>
💡 dll 파일 아래 디렉토리에 포함시켜주세요

```latex
.\x64\Releas
```

</aside>

# 속성

<aside>

  ### 💡 구성속성 - 디버깅 - 환경

```latex
./SDL2.dll
./SDL2_mixer.dll
./SDL2_image.dll
./SDL2_ttf.dll
```


![Untitled](https://github.com/Cybecho/Windows_Setting_dotfiles/assets/42949995/aedbf789-5a25-40d2-8f10-6766c22bc959)


</aside>

<aside>
  
  ### 💡 C/C++ - 일반 - 추가 포함 디렉토리

```latex
../include
../include/SDL2
```

![Untitled 1](https://github.com/Cybecho/Windows_Setting_dotfiles/assets/42949995/fec514ff-ef05-46be-93bb-279ce37b6eec)


</aside>

<aside>
  
### 💡 속성 - C/C++ - 명령줄 - 추가옵션

```latex
/utf-8
```

![스크린샷 2024-05-01 231852](https://github.com/Cybecho/Windows_Setting_dotfiles/assets/42949995/4132f2f1-a441-43f3-9943-3af5b48b6d52)

</aside>

<aside>
  
  ### 💡 링커 - 일반 - 추가 라이브 디렉터리

```latex
../lib/x64
```

![Untitled 2](https://github.com/Cybecho/Windows_Setting_dotfiles/assets/42949995/5f35275c-2750-49d5-8314-55f1bfc12f76)


</aside>

<aside>
  
  ### 💡 링커 - 입력 - 추가 종속성

```latex
../lib/x64/SDL2.lib
../lib/x64/SDL2main.lib
../lib/x64/SDL2_mixer.lib
../lib/x64/SDL2_image.lib
../lib/x64/SDL2_ttf.lib
```

![Untitled 3](https://github.com/Cybecho/Windows_Setting_dotfiles/assets/42949995/bd0dc698-de0f-4f49-97be-a17349ba83f2)

</aside>

-
