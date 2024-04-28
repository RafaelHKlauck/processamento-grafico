# Flappy Bird - Rafael Hansen Klauck

## Como rodar o jogo

Para rodar o jogo, é necessário compilar o código e executar o arquivo main.exe. Caso esteja sendo utilizado o Visual Studio Code(utilizado para o desenvolvimento), basta pressionar `Ctrl + Shift + B` para compilar o código e depois executar o arquivo main.exe. Porém, é necessário configurar o arquivo tasks.json da seguinte forma:

```json
{
  "tasks": [
    {
      "type": "cppbuild",
      "label": "C/C++: clang++ build active file",
      "command": "/usr/bin/clang++",
      "args": [
        "-std=c++17",
        "-fdiagnostics-color=always",
        "-Wall",
        "-g",
        "-I${workspaceFolder}/include",
        "-L${workspaceFolder}/lib",
        "${workspaceFolder}/trabalhoGA/glad.c",
        "${workspaceFolder}/trabalhoGA/main.cpp",
        "${workspaceFolder}/trabalhoGA/Sprite.cpp",
        "${workspaceFolder}/trabalhoGA/Game.cpp",
        "${workspaceFolder}/trabalhoGA/Bird.cpp",
        "${workspaceFolder}/trabalhoGA/Pipe.cpp",
        "${workspaceFolder}/include/imgui/imgui_demo.cpp",
        "${workspaceFolder}/include/imgui/imgui_draw.cpp",
        "${workspaceFolder}/include/imgui/imgui_impl_glfw.cpp",
        "${workspaceFolder}/include/imgui/imgui_impl_opengl3.cpp",
        "${workspaceFolder}/include/imgui/imgui_tables.cpp",
        "${workspaceFolder}/include/imgui/imgui_widgets.cpp",
        "${workspaceFolder}/include/imgui/imgui.cpp",
        "${workspaceFolder}/lib/libglfw.3.dylib",
        "-o",
        "${workspaceFolder}/trabalhoGA/main.exe",
        "-framework",
        "OpenGL",
        "-Wno-deprecated",
        "-Wl,-rpath,${workspaceFolder}/lib"
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "problemMatcher": ["$gcc"],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "detail": "Task generated by Debugger."
    }
  ],
  "version": "2.0.0"
}
```

## Como jogar

Para jogar, basta pressionar a barra de espaço para fazer o pássaro subir. O objetivo é passar pelos canos sem encostar neles. Caso o pássaro encoste em algum cano ou no chão, o jogo acaba. A pontuação é contabilizada pela quantidade de canos que o pássaro passa.

Quando o jogo acaba, é possível reiniciar o jogo pressionando a tecla enter.