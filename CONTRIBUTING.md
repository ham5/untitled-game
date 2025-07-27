# Guia de Contribuição

Este documento explica como colaborar no desenvolvimento do jogo usando Git e GitHub, garantindo organização e qualidade no projeto.

---

## Fluxo de Branches

 - **`main`**: código estável e pronto para entrega. Não faça commits diretos aqui.  
 - **`develop`**: integração das funcionalidades. Todas as features devem ser mescladas aqui antes do `main`.  
 - **`feature/nome-da-feature`**: branches individuais para desenvolver funcionalidades específicas.

---

## Como contribuir

1. Atualize sua branch develop local:
   ```bash
   git checkout develop
   git pull origin develop

2. Crie uma branch para sua funcionalidade:
   ```bash
   git checkout -b feature/nome-da-feature

3. Faça commits claros e frequentes:
   ```bash
   git add .
   git commit -m "feat: descrição clara da funcionalidade"

4. Envie sua branch para o repositório remoto:
   ```bash
   git push -u origin feature/nome-da-feature

5. Abra um Pull Request para a branch develop no GitHub.

6. Aguarde revisão de pelo menos um colega antes do merge.

## Convenção de mensagens de commit

 - feat: nova funcionalidade
 - fix: correção de bugs
 - docs: documentação
 - chore: tarefas gerais (configuração, limpeza, etc.)
 - refactor: mudanças no código que não alteram comportamento

## Divisão de tarefas

Cada membro é responsável por uma branch/feature, por exemplo:

| Membro | Branch | Responsabilidade |
--- | :---: | ---
| Membro 1 | feature/menu-e-navegacao | Menu principal e navegação
| Membro 2 | feature/personagem-e-movimentacao | Movimentação do personagem
| Membro 3 | feature/fisica-e-colisao | Colisões e física
| Membro 4 | feature/inimigos-e-ia | Inimigos e inteligência
| Membro 5 | feature/interface-e-hud | Interface e HUD
| Membro 6 | feature/audio-e-efeitos | Áudio e efeitos sonoros

## Boas práticas

 - Atualize sempre a branch develop antes de iniciar uma feature.
 - Faça commits pequenos e descritivos.
 - Revise seu código antes de abrir PR.
 - Participe das revisões dos colegas.

Obrigado por colaborar!
Vamos juntos construir um ótimo jogo! 🎮