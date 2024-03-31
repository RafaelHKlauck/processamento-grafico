## Lista de Exercícios 1 – Processamento Gráfico

<b>1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?</b>

A GLSL (OpenGL Shading Language) é uma linguagem de programação de alto nível baseada em C, utilizada para escrever shaders em OpenGL. Os dois tipos de shaders obrigatórios no pipeline programável da versão atual são o Vertex Shader e o Fragment Shader. O Vertex Shader processa vértices e o Fragment Shader processa fragmentos.

<b>2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?</b>

Primitivas gráficas são formas geométricas básicas que podem ser desenhadas na tela, como pontos, linhas e triângulos. Os vértices são armazenados em buffers de vértices (VBOs) na OpenGL.
Alguns exemplos de primitivas gráficas são:

- GL_POINTS: desenha um ponto para cada vértice.
- GL_LINES: desenha uma linha entre cada par de vértices.
- GL_LINE_STRIP: desenha uma linha entre cada par de vértices, conectando-os em ordem.
- GL_LINE_LOOP: desenha uma linha entre cada par de vértices, conectando-os em ordem, e uma linha entre o último e o primeiro vértice.
- GL_TRIANGLES: desenha um triângulo para cada grupo de três vértices.
- GL_TRIANGLE_STRIP: desenha um triângulo para cada grupo de três vértices, conectando-os em ordem.
- GL_TRIANGLE_FAN: desenha um triângulo para cada grupo de três vértices, conectando-os em ordem, e um triângulo entre o primeiro vértice e os dois últimos.

<b>3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).</b>

- VBO (Vertex Buffer Object): objeto que armazena dados de vértices na GPU, que incluem posições, cores, normais e coordenadas de textura.
- VAO (Vertex Array Object): é um objeto que armazena as configurações dos VBOs, como o layout dos vértices. Define como os dados armezenados nos VBOs são interpretados.
- EBO (Element Buffer Object): similar ao VBO, porém armazena índices que definem a ordem dos vértices em elementos primitivos, como triângulos ou linhas.

O VAO armazena os VBOs e EBOs, e os VBOs armazenam os vértices. O EBO armazena os índices dos vértices, que são usados para desenhar as primitivas gráficas.

<b>Os demais exercícios 5,6,7,8 e 9 estão dentro da pasta results com imagens dos resultados obtidos</b>
