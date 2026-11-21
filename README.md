# nó a nó

Do emaranhado de jumpers ao case impresso. Uma página para organizar projetos com ESP32: entender as ligações, montar (com ou sem solda) e gerar um case 3D sob medida.

## Etapas

0. **Começar por uma ideia**: você descreve o que quer fazer (“tenho um ESP e uma tela, quero animações”); a IA escolhe as peças, priorizando o seu material, monta os nós, escreve o código e diz o que falta comprar.
1. **Montagem**: placa, módulos, código e fotos da protoboard. A IA lê o código (fonte principal) e confere pelas fotos.
2. **Nós**: o mapa das ligações. Cada nó é um fio só, mesmo quando chega em vários pinos. Forma pelo tipo (● energia, ■ GND, ◆ sinal), cor de fio sugerida, e um marcador onde o nó se divide em 3 ou mais pontos.
3. **Bancada 3D**: a montagem modelada em 3D, em três vistas: protoboard (com trilhos de alimentação e a checagem de quando a placa é larga demais), jumpers soltos e placa perfurada (com as trilhas por baixo e o botão de virar a placa).
4. **Ligar**, em três modos:
   - *Sem solda*: jumpers em feixe, como num case de acrílico.
   - *Fios soldados*: ordem de solda em corrente ou emenda em Y.
   - *Na placa perfurada*: posiciona as peças nos furos, traça as trilhas e mostra o mapa de solda furo a furo, com vista de baixo espelhada.
5. **Case 3D**: caixa calculada a partir das medidas das peças (lado a lado, em coluna ou sanduíche). Exporta `.stl` do gabarito de teste, da base e da tampa.

6. **Exportar**: um .zip com placa (`.kicad_pcb`, com as trilhas da placa perfurada em B.Cu), netlist (`.net`) e footprints para o KiCad; lista de compras (`.csv`); STL do case; o código Arduino com um `pinos.h` gerado a partir dos nós; o diagrama em SVG e um LEIA-ME.

Também tem **Minhas peças** (peças com medidas e pinagem próprias), **Meu material** (o que tenho em casa) e um chat sobre a montagem.

## Onde roda

É um único `index.html`, sem build.

- **Dentro do Claude** (artifact): IA, salvamento na conta e download funcionam.
- **Fora do Claude** (GitHub Pages, Render, abrir o arquivo): diagrama, bancada 3D, placa perfurada, case 3D, `.stl` e o pacote de exportação funcionam; os projetos ficam salvos só no navegador. Para a IA (começar por uma ideia, análise e chat) funcionar fora do Claude, clique em **IA: configurar** e cole a sua chave da API da Anthropic (console.anthropic.com). Ela fica guardada só no seu navegador e o uso é cobrado na sua conta da API.

## Avisos

- As medidas das peças vêm aproximadas: meça as suas antes de imprimir.
- A pinagem física das placas precisa ser conferida com a serigrafia antes de soldar.
- O case sai com cantos retos e guias de encaixe (sem parafusos).

## Exemplos

- `exemplos/jogo-pelourinho.json`: ESP32-WROOM-32 (DevKit V1, 30 pinos) + TFT 1,8" + 2 botões 6×6, com protoboard e placa perfurada 5×10 (todas as trilhas por baixo). Código de teste em `exemplos/jogo-teste-esp32/`. No site: **importar** → escolha o arquivo (ou escolha o exemplo no seletor de projetos). A versão antiga para ESP32-S3 ficou em `exemplos/jogo-pelourinho-s3.json`.
- `exemplos/knob-cha-cozinha.json` e a pasta `exemplos/knob-cha-cozinha/` (código + `pinos.h`): ESP32-S3 + TFT + 2 botões com um joystick fazendo o papel de knob, e 4 cenas animadas (chá, pepino, batata, cenoura). Prévia no navegador em `knob/index.html`.
