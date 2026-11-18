# nó a nó

Do emaranhado de jumpers ao case impresso. Uma página para organizar projetos com ESP32: entender as ligações, montar (com ou sem solda) e gerar um case 3D sob medida.

## Etapas

1. **Montagem**: placa, módulos, código e fotos da protoboard. A IA lê o código (fonte principal) e confere pelas fotos.
2. **Nós**: o mapa das ligações. Cada nó é um fio só, mesmo quando chega em vários pinos. Forma pelo tipo (● energia, ■ GND, ◆ sinal), cor de fio sugerida, e um marcador onde o nó se divide em 3 ou mais pontos.
3. **Ligar**, em três modos:
   - *Sem solda*: jumpers em feixe, como num case de acrílico.
   - *Fios soldados*: ordem de solda em corrente ou emenda em Y.
   - *Na placa perfurada*: posiciona as peças nos furos, traça as trilhas e mostra o mapa de solda furo a furo, com vista de baixo espelhada.
4. **Case 3D**: caixa calculada a partir das medidas das peças (lado a lado, em coluna ou sanduíche). Exporta `.stl` do gabarito de teste, da base e da tampa.

Também tem **Minhas peças** (peças com medidas e pinagem próprias), **Meu material** (o que tenho em casa) e um chat sobre a montagem.

## Onde roda

É um único `index.html`, sem build.

- **Dentro do Claude** (artifact): IA, salvamento na conta e download funcionam.
- **Fora do Claude** (GitHub Pages, Render, abrir o arquivo): diagrama, placa perfurada, case 3D e `.stl` funcionam; os projetos ficam salvos só no navegador. A análise com IA e o chat não funcionam fora do Claude.

## Avisos

- As medidas das peças vêm aproximadas: meça as suas antes de imprimir.
- A pinagem física das placas precisa ser conferida com a serigrafia antes de soldar.
- O case sai com cantos retos e guias de encaixe (sem parafusos).
